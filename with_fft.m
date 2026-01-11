clear; 
clc; 

% --- SETUP ---
port = "COM8"; 
baudRate = 115200; 
packet_size = 52; 
elements_per_pack = 26; 

% CONFIGURATION
sampleWindow = 1000; % Must be an EVEN number for FFT logic to be simple

disp(['Connecting to ' port '...']); 
s = serialport(port,baudRate); 
flush(s); 

% --- PART 1: CALIBRATION (Calculating Time Axis) ---
disp('Aligning and Calibrating Sampling Rate...');

% 1. Align to the start of a packet (find 0xFFFF)
while (read(s, 1, "uint16") ~= 65535); end 

% 2. Read a batch of packets to measure speed
calibrationPackets = 1000; 
calibDataCount = calibrationPackets * elements_per_pack;

tic; 
read(s, calibDataCount, "uint16"); 
timeTaken = toc; 

% 3. Calculate Fs (Sampling Frequency)
Fs = calibrationPackets / timeTaken * elements_per_pack; 
% NOTE: multiplied by elements_per_pack because 'calibrationPackets' 
% is the number of USB packets, but 'Fs' is samples per second.
% Wait, calibrationPackets * 26 samples? 
% Let's double check logic: 
% We read (100 * 26) samples.
% Fs = (Total Samples Read) / timeTaken
Fs = (calibrationPackets * elements_per_pack) / timeTaken;

dt = 1/Fs; 
fprintf('Calibration Complete.\nEstimated Sampling Rate: %.2f Hz\n', Fs);

% --- PLOT SETUP ---
databuffer = zeros(sampleWindow,1);

figure('Name', 'Live Sensor Data', 'Color', 'w');

% Top Plot: Time Domain
subplot(2,1,1);
hPlot = plot(nan, nan, 'LineWidth', 1.5);
title(sprintf('Sensor 9 Time Domain (Fs = %.1f Hz)', Fs)); 
xlabel('Time (s)'); 
ylabel('ADC value');
ylim([0, 4096]);
grid on;

% Bottom Plot: FFT (Frequency Domain)
subplot(2,1,2);
hFFT = plot(nan, nan, 'r', 'LineWidth', 1.5);
title('FFT Frequency Spectrum'); 
xlabel('Frequency (Hz)'); 
ylabel('Magnitude');
grid on;
xlim([0, Fs/2]); 

% Pre-calculate X-axis for plots
f_axis = Fs*(0:(sampleWindow/2))/sampleWindow;
timeAxis = (0:sampleWindow-1) * dt; 

% --- MAIN LOOP ---
lastPlotTime = tic; 
disp('Starting Live Capture...');

while true
    
    bytesAvailable = s.NumBytesAvailable; 
    
    if bytesAvailable >= packet_size
        numPacket = floor(bytesAvailable/packet_size); 
        
        % Read data
        allData = read(s, numPacket*elements_per_pack, "uint16"); 
        
        % Extract Channel 9
        newPoints = allData(9:elements_per_pack:end); 
        
        % --- FIX IS HERE: ROBUST BUFFER UPDATE ---
        % 1. Concatenate old buffer and new data
        tempBuffer = [databuffer; newPoints(:)];
        
        % 2. Force-trim to exactly 'sampleWindow' size (keep most recent)
        if length(tempBuffer) > sampleWindow
            databuffer = tempBuffer(end-sampleWindow+1:end);
        else
            databuffer = tempBuffer;
        end
        % -----------------------------------------
        
        % Update Display (limit to 20fps)
        if toc(lastPlotTime) > 0.05
            
            % Update Time Plot
            set(hPlot, 'XData', timeAxis, 'YData', databuffer);
            
            % Update FFT
            data_AC = databuffer - mean(databuffer); 
            Y = fft(data_AC);
            L = length(databuffer); % This is now GUARANTEED to be 1000
            P2 = abs(Y/L);
            P1 = P2(1:L/2+1);      
            P1(2:end-1) = 2*P1(2:end-1);
            
            set(hFFT, 'XData', f_axis, 'YData', P1);
            
            drawnow;
            lastPlotTime = tic; 
        end
    end
end