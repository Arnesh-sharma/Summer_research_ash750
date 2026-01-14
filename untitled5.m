clear; 
clc;
global selectedSensor; 
selectedSensor = 1; 

% --- CONFIGURATION ---
port = "COM8"; 
baudRate = 115200; 
packet_size = 52; 
elements_per_pack = 26; 

% --- CRITICAL TUNING ---
Fs = 2055;             % <--- SET THIS to your actual Sample Rate (Hz)!!
sampleWindow = 1024;  % FFT works best with powers of 2 (512, 1024, 2048)
gridSize = [5, 5]; 
targetFPS = 30;
drawInterval = 1 / targetFPS; 

% --- SETUP BUFFERS ---
fullDataBuffer = zeros(sampleWindow, 25); 

% --- SETUP DASHBOARD FIGURE ---
fig = figure('Name', 'Sensor Frequency Analysis', 'Color', 'white');
set(fig, 'Units', 'normalized', 'Position', [0.1 0.1 0.8 0.7]); % Make window big

% 1. HEATMAP (Left Side - Tall)
subplot(2, 2, [1 3]); % Span rows 1 and 3 (the whole left side)
hMap = imagesc(zeros(gridSize)); 
colormap('jet'); 
colorbar; 
clim([0, 1000]); 
title('P2P Intensity (Click sensors to see FFT and Signal)'); 
axis square; 
set(hMap, 'ButtonDownFcn', @mapClicked);

% 2. TIME DOMAIN PLOT (Top Right)
subplot(2, 2, 2);
hLineTime = plot(zeros(sampleWindow, 1), 'LineWidth', 1.5, 'Color', '#0072BD'); 
title('Time Domain'); 
grid on; 
xlim([1, sampleWindow]);
ylabel('Amplitude');

% 3. FREQUENCY DOMAIN PLOT (Bottom Right - NEW!)
subplot(2, 2, 4);
% Create initial X-axis (Frequency range is 0 to Fs/2)
f_axis = Fs*(0:(sampleWindow/2))/sampleWindow;
hLineFFT = plot(f_axis, zeros(size(f_axis)), 'LineWidth', 1.5, 'Color', '#D95319'); 
title('Frequency Domain (FFT)'); 
grid on; 
xlabel('Frequency (Hz)'); 
ylabel('Magnitude'); 
xlim([0, Fs/2]); % We can only see up to half the sample rate (Nyquist)

% --- SERIAL CONNECT ---
disp(['Connecting to ' port '...']); 
s = serialport(port, baudRate); 
flush(s); 
lastDrawTime = tic; 

while true
    if s.NumBytesAvailable >= packet_size
        % --- BULK READ (Same as before) ---
        numPacketsToRead = floor(s.NumBytesAvailable / packet_size);
        rawStream = read(s, numPacketsToRead * elements_per_pack, "uint16");
        
        % Process
        matrixStream = reshape(rawStream, elements_per_pack, numPacketsToRead)';
        sensorDataOnly = matrixStream(:, 2:26); 
        
        % Update Buffer
        k = size(sensorDataOnly, 1); 
        if k < sampleWindow
            fullDataBuffer = [fullDataBuffer(k+1:end, :); sensorDataOnly];
        else
            fullDataBuffer = sensorDataOnly(end-sampleWindow+1:end, :);
        end
        
        % --- VISUAL UPDATE LOOP ---
        if toc(lastDrawTime) > drawInterval
            
            % A. Heatmap Calculations (P2P)
            p2pValues = max(fullDataBuffer) - min(fullDataBuffer);
            
            % B. Get Data for Selected Sensor
            timeSignal = fullDataBuffer(:, selectedSensor);
            
            % C. FFT CALCULATION
            % 1. Remove DC Offset (Gravity/Bias)
            sigNoDC = timeSignal - mean(timeSignal); 
            
            % 2. Compute FFT
            Y = fft(sigNoDC);
            
            % 3. Compute Single-Sided Spectrum (Magnitude)
            P2 = abs(Y/sampleWindow);    % Two-sided spectrum
            P1 = P2(1:sampleWindow/2+1); % Single-sided
            P1(2:end-1) = 2*P1(2:end-1); % Scale amplitude
            
            % --- UPDATE PLOTS ---
            % 1. Heatmap
            set(hMap, 'CData', fliplr(reshape(p2pValues, 5, 5)'));
            
            % 2. Time Plot
            set(hLineTime, 'YData', timeSignal);
            title(subplot(2, 2, 2), ['Sensor ' num2str(selectedSensor) ' (Time)']);
            
            % 3. FFT Plot
            set(hLineFFT, 'YData', P1);
            
            % Add a Peak Finder marker on the FFT (Optional Cool Feature)
            [peakMag, peakIdx] = max(P1);
            peakFreq = f_axis(peakIdx);
            title(subplot(2, 2, 4), ['Dominant Freq: ' num2str(peakFreq, '%.1f') ' Hz']);

            drawnow limitrate; 
            lastDrawTime = tic; 
        end
    end
end

function mapClicked(src, event)
    global selectedSensor;
    coords = round(event.IntersectionPoint);
    col = coords(1); row = coords(2);
    if row >= 1 && row <= 5 && col >= 1 && col <= 5
        selectedSensor = (row - 1) * 5 + col;
    end
end