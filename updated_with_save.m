clear; 
clc;
% --- GLOBAL VARIABLES ---
global selectedSensor; 
global fullDataBuffer; %make global
btnRight = uicontrol('Parent', fig, 'Style', 'pushbutton', 'String', 'Rotate Right', ...
    'Units', 'normalized', 'Position', [0.26 0.02 0.1 0.05], ...
    'Callback', {@rotateRight, hAxMap}); 
btnRight = uicontrol('Parent', fig, 'Style', 'pushbutton', 'String', 'Save Current Data', ...
    'Units', 'normalized', 'Position', [0.37 0.02 0.1 0.05], ...
    'Callback', @saveDataCSV); 
% --- BUTTON: SAVE DATA ---
function saveDataCSV(~, ~)
    global fullDataBuffer;
    
    currentData = fullDataBuffer; 
    numSamples = size(fullDataBuffer,1); 
    % 1. Remove DC Offset for ALL 25 sensors at once
      dataNoDC = currentData - mean(currentData);
                
    % 2. Compute FFT for all columns simultaneously
    Y_all = fft(dataNoDC);
    
    % 3. Convert to Single-Sided Magnitude
    P2_all = abs(Y_all / numSamples);
    P1_all = P2_all(1:numSamples/2+1, :);
    P1_all(2:end-1, :) = 2 * P1_all(2:end-1, :);
    %find number of rows needed (sampleWindow - P1_all size) 
    magSize = size(P1_all);
    rowsToFill = numSamples - magSize; 
    % fill the 25 columns with NaN to pad the data out 
    fillerBlock = NaN(rowsToFill, 25);
    paddedMag = [P1_all; fillerBlock];
    % combined Data output 
    combinedData = zeros(numSamples, 50); 
    labels = cell(1, 50)
    for i = 1:25 
        combinedData(:, 2*i-1) = currentData(:, i); %time domain 
        combinedData(:, 2*i) = paddedMag(:,i); %freq domain 
        labels{2*i-1} = sprintf('Sen%d_Time', i);
        labels{2*i} = sprintf('Sen%d_Mag', i);
    end 
    %SAVE TO CSV
    tableToSave = array2table(combinedData, 'VariableNames',labels); 
    filename = ['SensorData_', char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')), 'csv']; 
    writetable(tableToSave, filename); 
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