clear; 
clc;
% --- GLOBAL VARIABLES ---
global selectedSensor; 
global rotationState; % 0=0deg, 1=90deg, 2=180deg, 3=270deg

selectedSensor = 1; 
rotationState = 0;    % Start in default orientation

% --- CONFIGS ---
port = "COM13";        
baudRate = 115200; 
packet_size = 52;     % 26 uint16s * 2 bytes
elements_per_pack = 26; 

% --- SIGNAL TUNING ---
Fs = 2055;             
sampleWindow = 1024;   
gridSize = [5, 5]; 
targetFPS = 30;       % Refresh rate limit
drawInterval = 1 / targetFPS; 

% --- SETUP BUFFERS ---
fullDataBuffer = zeros(sampleWindow, 25); 

% --- SETUP DASHBOARD FIGURE ---
fig = figure('Name', 'Real-Time Sensor Dashboard', 'Color', 'white');
set(fig, 'Units', 'normalized', 'Position', [0.1 0.1 0.8 0.7]); % Large window

% ---------------------------------------------------------
% 1. HEATMAP (Left Side)
% ---------------------------------------------------------
subplot(2, 2, [1 3]); % Span left column
hMap = imagesc(zeros(gridSize)); 
colormap('jet'); 
colorbar; 
clim([0, 100]);      % Adjust color sensitivity here
title('Signal Strength (P2P)'); 
axis square; 
xticks(1:5); yticks(1:5);
hAxMap = gca;         % Save handle to axes to update labels later
updateAxisLabels(hAxMap, 0); % Set initial labels
set(hMap, 'ButtonDownFcn', @mapClicked); % Enable clicking

% ---------------------------------------------------------
% 2. ROTATION BUTTONS (Bottom Left)
btnLeft = uicontrol('Parent', fig, 'Style', 'pushbutton', 'String', '↺ Rotate Left', ...
    'Units', 'normalized', 'Position', [0.15 0.02 0.1 0.05], ...
    'Callback', {@rotateLeft, hAxMap}); 

btnRight = uicontrol('Parent', fig, 'Style', 'pushbutton', 'String', 'Rotate Right ↻', ...
    'Units', 'normalized', 'Position', [0.26 0.02 0.1 0.05], ...
    'Callback', {@rotateRight, hAxMap});

% ---------------------------------------------------------
% 3. TIME PLOT (Top Right)
% ---------------------------------------------------------
subplot(2, 2, 2);
hLineTime = plot(zeros(sampleWindow, 1), 'LineWidth', 1.5, 'Color', '#0072BD'); 
title('Time Domain (Raw Signal)'); 
grid on; 
xlim([1, sampleWindow]);
ylabel('Amplitude (ADC)');

% ---------------------------------------------------------
% 4. FFT PLOT (Bottom Right)
% ---------------------------------------------------------
subplot(2, 2, 4);
% Create frequency axis: 0 to Fs/2
f_axis = Fs*(0:(sampleWindow/2))/sampleWindow;
hLineFFT = plot(f_axis, zeros(size(f_axis)), 'LineWidth', 1.5, 'Color', '#D95319'); 
title('Frequency Domain (FFT)'); 
grid on; 
xlim([0, Fs/2]); 
xlabel('Frequency (Hz)'); ylabel('Magnitude');

% --- CONNECT TO SERIAL ---
disp(['Connecting to ' port '...']); 
try
    s = serialport(port, baudRate); 
    flush(s); 
catch
    error('Connection Failed. Check COM Port and close other apps.');
end

lastDrawTime = tic; 

% --- MAIN LOOP ---
while ishandle(fig) % Runs until you close the window
    if s.NumBytesAvailable >= packet_size
        
        % --- A. FAST BULK READ ---
        numPacketsToRead = floor(s.NumBytesAvailable / packet_size);
        rawStream = read(s, numPacketsToRead * elements_per_pack, "uint16");
        
        % Reshape to [Packets x 26]
        matrixStream = reshape(rawStream, elements_per_pack, numPacketsToRead)';
        sensorDataOnly = matrixStream(:, 2:26) % Drop sync byte (index 1)
        
        % --- B. UPDATE HISTORY BUFFER ---
        k = size(sensorDataOnly, 1); 
        if k < sampleWindow
            fullDataBuffer = [fullDataBuffer(k+1:end, :); sensorDataOnly];
        else
            fullDataBuffer = sensorDataOnly(end-sampleWindow+1:end, :);
        end
        
      % --- C. VISUALIZATION (Throttled to 30 FPS) ---
        if toc(lastDrawTime) > drawInterval
            
            try
                % =========================================================
                % NEW CALCULATION: MAX FFT MAGNITUDE FOR HEATMAP
                % =========================================================
                
                % 1. Remove DC Offset for ALL 25 sensors at once
                dataNoDC = fullDataBuffer - mean(fullDataBuffer);
                
                % 2. Compute FFT for all columns simultaneously
                Y_all = fft(dataNoDC);
                
                % 3. Convert to Single-Sided Magnitude (Standard FFT Math)
                P2_all = abs(Y_all / sampleWindow);
                P1_all = P2_all(1:sampleWindow/2+1, :);
                P1_all(2:end-1, :) = 2 * P1_all(2:end-1, :);
                
                % 4. Extract the Maximum Magnitude for each sensor
                % max(matrix, [], 1) finds the max value in each column
                heatmapData = max(P1_all, [], 1);
                
                % =========================================================
                % GRID ORIENTATION & DRAWING
                % =========================================================
                
                % A. Base: Reshape to 5x5, Flip Right-to-Left (PCB Layout)
                baseGrid = fliplr(reshape(heatmapData, 5, 5)');
                
                % B. Apply User Rotation
                finalGrid = rot90(baseGrid, rotationState);
                
                set(hMap, 'CData', finalGrid);
                
                % =========================================================
                % UPDATE INDIVIDUAL PLOTS (For Selected Sensor)
                % =========================================================
                
                % 1. Get data for the clicked sensor
                timeSignal = fullDataBuffer(:, selectedSensor);
                
                % 2. Update Time Plot
                set(hLineTime, 'YData', timeSignal);
                title(subplot(2, 2, 2), ['Sensor ' num2str(selectedSensor) ' (Time)']);
                
                % 3. Update FFT Plot
                P1_selected = P1_all(:, selectedSensor);
                
                set(hLineFFT, 'YData', P1_selected);
                
                % Find Peak Frequency for Title
                [maxMag, peakIdx] = max(P1_selected);
                peakFreq = f_axis(peakIdx);
                title(subplot(2, 2, 4), ['Dominant Freq: ' num2str(peakFreq, '%.1f') ' Hz | Mag: ' num2str(maxMag, '%.1f')]);
                
                drawnow limitrate; 
                lastDrawTime = tic; 
                
            catch
                disp('Dashboard closed.');
                break; 
            end
        end
    end 
end

% ---------------------------------------------------------
% HELPER FUNCTIONS
% ---------------------------------------------------------

% --- BUTTON: ROTATE LEFT ---
function rotateLeft(~, ~, axHandle)
    global rotationState;
    rotationState = rotationState + 1; % 90 deg CCW
    if rotationState > 3, rotationState = 0; end

    % Update axis labels directly on the handle provided
    updateAxisLabels(axHandle, rotationState);
end

% --- BUTTON: ROTATE RIGHT ---
function rotateRight(~, ~, axHandle)
    global rotationState;
    rotationState = rotationState - 1; % 90 deg CW
    if rotationState < 0, rotationState = 3; end

    % Update axis labels directly on the handle provided
    updateAxisLabels(axHandle, rotationState);
end

% --- CLICK HANDLER (SMART MAPPING) ---
function mapClicked(src, event)
    global selectedSensor;
    global rotationState;
    
    coords = round(event.IntersectionPoint);
    col = coords(1); 
    row = coords(2);
    
    if row >= 1 && row <= 5 && col >= 1 && col <= 5
        % Generate the "Reference Map" identical to the visual map
        % 1. Create indices 1-25
        indexMap = 1:25; 
        % 2. Apply initial Right-to-Left Flip (PCB Layout)
        baseMap = fliplr(reshape(indexMap, 5, 5)');
        % 3. Apply User Rotation
        rotatedMap = rot90(baseMap, rotationState);
        
        % 4. Select the sensor at the clicked location
        selectedSensor = rotatedMap(row, col);
        
        fprintf('Selected Sensor: %d\n', selectedSensor);
    end
end

% --- AXIS LABEL UPDATER ---
function updateAxisLabels(axHandle, rotState)
    lblAsc  = {'1','2','3','4','5'};
    lblDesc = {'5','4','3','2','1'};
    
    % Logic to keep 5-1 orientation consistent with rotation
    switch rotState
        case 0 % Normal (Right-Left PCB)
            xticklabels(axHandle, lblDesc); yticklabels(axHandle, lblAsc);
            xlabel(axHandle, 'Columns (5->1)'); ylabel(axHandle, 'Rows (1->5)');
        case 1 % 90 CCW
            xticklabels(axHandle, lblAsc);  yticklabels(axHandle, lblDesc);
            xlabel(axHandle, 'Rows (1->5)'); ylabel(axHandle, 'Columns (1->5)');
        case 2 % 180
            xticklabels(axHandle, lblAsc);  yticklabels(axHandle, lblDesc);
            xlabel(axHandle, 'Columns (1->5)'); ylabel(axHandle, 'Rows (5->1)');
        case 3 % 270 CCW
            xticklabels(axHandle, lblDesc); yticklabels(axHandle, lblAsc);
            xlabel(axHandle, 'Rows (5->1)'); ylabel(axHandle, 'Columns (5->1)');
    end
end