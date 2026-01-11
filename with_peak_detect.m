clear; 
clc; 

% --- SETUP ---
port = "COM8";       
baudRate = 115200; 
packet_size = 52;    
elements_per_pack = 26; 

% --- CONFIGURATION ---
Fs = 2500;           % Fixed Sample Rate
dt = 1/Fs;           % Time per sample
sampleWindow = 1000; 

% THRESHOLD FOR HIGH PEAKS ONLY
% We still need to know what counts as a "High" peak to start the timer.
% Adjust this if your signal is very small (e.g. set to 1500).
thresh_high_start = 2000; 

disp(['Connecting to ' port '...']); 
s = serialport(port,baudRate); 
flush(s); 

% --- PLOT SETUP ---
databuffer = zeros(sampleWindow,1);
figure('Name', 'Robust Wave Detector', 'Color', 'w');

hPlot = plot(nan, nan, 'b', 'LineWidth', 1.5); hold on;
hHighs = plot(nan, nan, 'ro', 'MarkerFaceColor', 'r'); % Red = Highs
hLows  = plot(nan, nan, 'go', 'MarkerFaceColor', 'g'); % Green = Lows

% Info Box
dim = [0.15 0.75 0.3 0.1]; 
hText = annotation('textbox', dim, 'String', 'Waiting...', ...
    'FitBoxToText', 'on', 'BackgroundColor', 'white', ...
    'FontSize', 12, 'EdgeColor', 'black', 'FontWeight', 'bold');

title(sprintf('Logic: High -> Min -> High (Fs: %d Hz)', Fs)); 
xlabel('Time (s)'); 
ylabel('ADC Value');
ylim([0, 4096]); 
yline(thresh_high_start, 'r--', 'High Thresh');
grid on;

timeAxis = (0:sampleWindow-1) * dt; 

% --- MAIN LOOP ---
lastPlotTime = tic; 
disp('Starting Live Plot...');

while true
    
    bytesAvailable = s.NumBytesAvailable; 
    
    if bytesAvailable >= packet_size
        numPacket = floor(bytesAvailable/packet_size); 
        allData = read(s, numPacket*elements_per_pack, "uint16"); 
        newPoints = allData(9:elements_per_pack:end); 
        
        % Robust Buffer Update
        tempBuffer = [databuffer; newPoints(:)];
        if length(tempBuffer) > sampleWindow
            databuffer = tempBuffer(end-sampleWindow+1:end);
        else
            databuffer = tempBuffer;
        end
        
        % Update Display (Limit to ~20 FPS)
        if toc(lastPlotTime) > 0.05
            
            % --- ROBUST SEQUENCE LOGIC ---
            
            % 1. Find all potential High Peaks to start from
            [pks_h, locs_h] = findpeaks(databuffer, 'MinPeakHeight', thresh_high_start, ...
                                                    'MinPeakDistance', 20);
            
            freq = 0; period = 0;
            P1_idx = []; P1_val = [];
            Low_idx = []; Low_val = [];
            P2_idx = []; P2_val = [];
            found_sequence = false;
            
            if ~isempty(locs_h)
                % Look backwards from the most recent High Peak to find a full wave
                for i = length(locs_h)-1 : -1 : 1
                    
                    curr_H_idx = locs_h(i);
                    
                    % 2. Find the MINIMUM value (Valley) strictly after this High Peak
                    %    We search from this peak to the end of the buffer.
                    search_range = databuffer(curr_H_idx:end);
                    
                    % Find the absolute lowest point in this range
                    [min_val, min_rel_idx] = min(search_range);
                    abs_min_idx = curr_H_idx + min_rel_idx - 1;
                    
                    % 3. Find the NEXT High Peak strictly after that Low Point
                    %    We search from the Low Point to the end of the buffer.
                    range_after_low = databuffer(abs_min_idx:end);
                    
                    % Note: We look for ANY peak > threshold here.
                    [next_pks, next_locs] = findpeaks(range_after_low, ...
                                                      'MinPeakHeight', thresh_high_start, ...
                                                      'MinPeakDistance', 20);
                    
                    if ~isempty(next_locs)
                        % FOUND A VALID SEQUENCE!
                        
                        % Point 1 (Start High)
                        P1_idx = curr_H_idx;
                        P1_val = databuffer(curr_H_idx);
                        
                        % Point 2 (The Low Valley)
                        Low_idx = abs_min_idx;
                        Low_val = min_val;
                        
                        % Point 3 (Stop High)
                        % Correct the index offset
                        P2_idx = abs_min_idx + next_locs(1) - 1; 
                        P2_val = next_pks(1);
                        
                        % Calculate
                        samples_diff = P2_idx - P1_idx;
                        period = samples_diff * dt;
                        freq = 1 / period;
                        
                        found_sequence = true;
                        break; % Done! We found the most recent wave.
                    end
                end
            end
            
            % --- UPDATE PLOT ---
            set(hPlot, 'XData', timeAxis, 'YData', databuffer);
            
            if found_sequence
                msg = sprintf('Freq: %.2f Hz\nPeriod: %.2f ms\nLow Val: %d', ...
                              freq, period*1000, round(Low_val));
                set(hText, 'String', msg, 'Color', 'black', 'EdgeColor', 'black');
                
                % Mark the 3 Critical Points
                set(hHighs, 'XData', timeAxis([P1_idx, P2_idx]), 'YData', [P1_val, P2_val]);
                set(hLows, 'XData', timeAxis(Low_idx), 'YData', Low_val);
            else
                set(hText, 'String', 'Searching...', 'Color', 'red', 'EdgeColor', 'red');
                set(hHighs, 'XData', [], 'YData', []);
                set(hLows, 'XData', [], 'YData', []);
            end
            
            drawnow;
            lastPlotTime = tic; 
        end
    end
end