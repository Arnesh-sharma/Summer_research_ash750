clear; 
clc; 
%global vars
global selectedSensor;
global timeSig; 
global FFTSig; 
global mode; 
global P1All; 

%Config 
PORT = "COM13"; %port connected to PSoC 
BAUDRATE = 115200; 
BYTES = 52; %total bytes in packet 
VALS = 26; %total values per packet 
SAMPLE_WINDOW = 1024; 
SENSORS = 25;
targetFPS = 30; 
drawInterval = 1/targetFPS; 
Sensor = 1; 
Fs = 2055; 

%pre allocate buffer
buffer = zeros(SAMPLE_WINDOW, SENSORS); 
%open serial port 
s = serialport(PORT, BAUDRATE); 
disp('Connected to:'); 
disp(PORT); 
disp('\n');
%flush the old data
flush(s); 
%wait for the data to come
disp('waiting for data...');

while s.NumBytesAvailable < 52
    %busy waiting
end 

%got enough bytes 
disp('Bytes avaiable (YAY):');
disp(s.NumBytesAvailable); 
%read a packet of 26 2Byte data 
raw = read(s, VALS, "uint16"); 
disp('Raw packet (26 values hopefully please): ')
disp(raw); 

%plots 
 
fig = figure('Name','Live Sensor data', 'Color', 'white');
%plot(t, buffer(:, Sensor), 'LineWidth', 1.2, 'Color', 'blue'); 
%time domain plot 
ax1 = subplot(2, 2, 2); 
t = (0: SAMPLE_WINDOW -1)/ Fs;
hline = plot(t, zeros(1024, 1), 'Color','blue', 'LineWidth',1.2); 
xlabel('time (s)')
ylabel('ADC count'); 
title('Sensor %d time domain signal', Sensor); 
grid on; 
xlim([0, max(t)]); 

%Magnitude plot
ax2 = subplot(2,2,4); 
f_axis = Fs * (0:(SAMPLE_WINDOW)/2)/SAMPLE_WINDOW; 
hFFT = plot(f_axis, zeros(size(f_axis)), 'Color', 'red', 'LineWidth', 1.2); 
xlabel('Frequency Hz'); 
ylabel('Magnitude dB'); 
grid on; 
xlim([0, Fs/2]); 

%heatmap
ax3 = subplot(2,2,[1 3]); 
hMap = imagesc(zeros(5, 5)); 
colormap('jet'); 
colorbar; 
clim([0 300]); 
axis square; 
title('Heat map of 5 x 5 Sensors'); 
set(hMap, 'ButtonDownFcn', @mapClicked);
%Bluk read the data 

%save button
ax = gca; 
saveButton = uicontrol(fig, 'Style', 'pushbutton', 'String', 'Save Data', ...
    'Callback', @(src,event) plotButtonPushed(ax, t, timeSig, f_axis, FFTSig, buffer, mode, P1All));

dropdown = uicontrol('Parent', fig, ...
    'Style', 'popupmenu', ...
    'String', {'Single', 'Adjacent', 'All'}, ...
    'Position', [50 100 200 30], ...
    'Callback', @(src, event) modeSelect(src));




lastDrawTime = tic; 
while ishandle(fig)
    if s.NumBytesAvailable >= BYTES
        %check how many packets are here 
        numPackets = floor(s.NumBytesAvailable/26); %nearest integer value of number of packets available
        %fprintf('Reading %d packets \n', numPackets); 
        rawStreamData = read(s, numPackets * VALS, "uint16"); %read all avaiable packets 26 values each
        %Reshape into the matrix goes column by column so by making 
        %matrix 26 x numPackets, then each column is one packet 
        matrix = reshape(rawStreamData, VALS, numPackets)';
        sensors = matrix(:,2:26);  %remove the sync bit row
        newRows = double(sensors); %for the 25 sensors only
        buffer = updateBuffer(buffer, newRows); 
        
        if toc(lastDrawTime) > drawInterval
                %FFT plot
                %remove DC offset
                bufferDC = buffer - mean(buffer, 1); 
        
                %apply hann window to remove any spectral leakage
                window = hann(SAMPLE_WINDOW); 
                bufWind = bufferDC .* window; %needs to be element wise multi
        
                %run FFT
                yAll = fft(bufWind); 
        
                %GEt the nums for the single sided mag plot 
                P2All = abs(yAll/SAMPLE_WINDOW); 
                P1All = P2All(1 : SAMPLE_WINDOW/2 +1, :); 
                P1All(2: end-1,:) = 2 * P1All(2 : end-1, :); 
        
                %heatmap 
                heatmapData = max(P1All, [], 1); 
                %disp(size(heatmapData)); 
               % grid5 = reshape(heatmapData, 5, 5)'; %from 1x25 to 5x5 
                gridflipped = fliplr(reshape(heatmapData, 5, 5)'); 
                set(hMap, 'Cdata', gridflipped); 
        
               
                %time plot
                timeSig = buffer(:, selectedSensor); 
                set(hline, 'Ydata', timeSig); 
                title(subplot(2, 2, 2), ['Sensor ' num2str(selectedSensor) ' (Time)']);
        
                %FFT plot
                FFTSig = P1All(:, selectedSensor); 
                set(hFFT, 'Ydata', FFTSig); 
        
                %find peak freq
                [maxMag, peakIdx] = max(FFTSig); 
                peakFreq = f_axis(peakIdx); 
                title(subplot(2, 2, 4), ['Dominant Freq: ' num2str(peakFreq, '%.1f') ' Hz | Mag: ' num2str(maxMag, '%.1f')]);
        
                drawnow limitrate ;   %force it to draw at 20fps
                lastDrawTime = tic; 
        end
    end 
end 
%clear the serial line
clear s; 
function buf = updateBuffer(buf, newRows)
    %inputs = buffer (1024 x 25), newRows (k x 25)
    %output = updated buffer with newRows appended at the bottom, so drop
    %the oldest 
    k = size(newRows, 1); % the number of new rows 
    if k >= size(buf,1)
        %there are more newRows than the window size - take the last 1024
        buf = newRows(end-1023:end, :); 
    else
        %shift the old data up (new to the bottom) 
        buf = [buf(k+1:end, :); newRows]; 
    end 
end 
%Map clicker
function mapClicked(src, event)
    global selectedSensor;
    
    coords = round(event.IntersectionPoint);
    col = coords(1); 
    row = coords(2);
    
    if row >= 1 && row <= 5 && col >= 1 && col <= 5
        % Generate the "Reference Map" identical to the visual map
        % 1. Create indices 1-25
        indexMap = 1:25; 
        % 2. Apply initial Right-to-Left Flip (PCB Layout)
        baseMap = fliplr(reshape(indexMap, 5, 5)');
        
        % 3. Select the sensor at the clicked location
        selectedSensor = baseMap(row, col);
        
        fprintf('Selected Sensor: %d\n', selectedSensor);
    end
end

function plotButtonPushed(ax, t, adcSig, f, mag, buf, mode, P1All)
    global selectedSensor; 
    global mode; 
    global P1All; 
    selectedMode = mode;  
    Time = t'; 
    %disp(size(Time));
    ADC = adcSig; 
    Freq = f'; 
   % disp(size(Freq));
    Mag = mag; 
    Mean =  mean(buf, 1);
    selectedMean = Mean(:, selectedSensor); 

    maxLen = max(length(ADC), length(Mag));
   % disp(size(selectedMean));


    %need to pad the fft readings 
    mag_padded = [Mag; nan(maxLen - length(Mag), 1)]; 
    freq_padded = [Freq; nan(maxLen - length(Freq), 1)]; 
    mean_padded = [selectedMean;nan(maxLen - length(selectedMean), 1)]; 

    % disp(size(Time)); 
    % disp(size(ADC)); 
    % disp(size(mag_padded));
    % disp(size(freq_padded)); 
    % disp(size(mean_padded)); 

    if selectedMode == 1
        %single mode
        clear myData;
        T = table(Time, ADC, freq_padded, mag_padded, mean_padded, 'VariableNames', {'Time', 'ADC readings', 'Frequency', 'Magnitude', 'DC offset'}); 
        filename = sprintf('Sensor_%d_data.xlsx', selectedSensor); 
        writetable(T, filename); 
        
    elseif selectedMode == 2 
        %adjacent mode
        clear myData;
        myData = cell(1, 8);    
        possibleCells = []; 
        possibleCells(end+1) = selectedSensor; 
        %check cardinal directions so like up, down, left, and right
         if(selectedSensor - 5) > 0 
             possibleCells(end+1) = selectedSensor - 5; 

         end 

        
        if(selectedSensor - 1) > 0 
             possibleCells(end+1) = selectedSensor - 1; 
           
        end 

        if(selectedSensor +5) < 26
             possibleCells(end+1) = selectedSensor + 5; 
           
        end

        if(selectedSensor +1) < 26
             possibleCells(end+1) = selectedSensor + 1; 
           
        end
        
        %check diagonal directions 
        if(selectedSensor - 6) > 0 
             possibleCells(end+1) = selectedSensor - 6; 
           
        end

        if(selectedSensor -4) > 0 
             possibleCells(end+1) = selectedSensor - 4; 
           
        end 

        if(selectedSensor +4) < 0 
             possibleCells(end+1) = selectedSensor + 4; 
            
        end

        if(selectedSensor +6) < 26
             possibleCells(end+1) = selectedSensor +6; 
           
        end 
        
  
        %save selected sensor
        for i = 1:length(possibleCells)
            ADC = buf(:, possibleCells(i)); 
            mag = P1All(:, possibleCells(i)); 
            sensor_mean = Mean(:, possibleCells(i)); 
            maxLenAll = max(length(ADC), length(mag));
            mag_padded = [mag; nan(maxLenAll - length(mag), 1)];
            mean_padded = [sensor_mean;nan(maxLenAll - length(sensor_mean), 1)];
            SensorNumber = repmat(possibleCells(i), maxLenAll, 1);

            T = table(SensorNumber,Time, ADC, freq_padded, mag_padded, mean_padded, 'VariableNames', {'Sensor','Time', 'ADC_readings', 'Frequency', 'Magnitude', 'DC_offset'});
            myData{i} = T;
        end

       

        finalTable = vertcat(myData{:});
        filename = sprintf('Sensor_%d_adj_data_len_%d.xlsx', selectedSensor, length(possibleCells)); 
        writetable(finalTable, filename); 




    elseif selectedMode == 3
        %get all sensor data 
        clear myData;
        myData = cell(1, 25); 
        for i = 1:25
            ADC = buf(:, i); 
            mag = P1All(:, i); 
            sensor_mean = Mean(:, i); 
            maxLenAll = max(length(ADC), length(mag));
            mag_padded = [mag; nan(maxLenAll - length(mag), 1)];
            mean_padded = [sensor_mean;nan(maxLenAll - length(sensor_mean), 1)]; 
            SensorNumber = repmat(i, maxLenAll, 1);
            T = table(SensorNumber,Time, ADC, freq_padded, mag_padded, mean_padded, 'VariableNames', {'Sensor','Time', 'ADC_readings', 'Frequency', 'Magnitude', 'DC_offset'});
            myData{i} = T; 
        end
        finalTable = vertcat(myData{:});
        filename = sprintf('Sensor_ALL_data.xlsx'); 
        writetable(finalTable, filename); 

        

    else
        fprintf("Somethings gone wrong :("); 
    end 
end

function modeSelect(src)
    global mode; 
    mode = src.Value; 
    modeString = src.String; 
    selectedMode = modeString{mode};  

    fprintf('Selected %s mode', selectedMode); 
end

