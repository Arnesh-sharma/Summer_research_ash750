clear; 
clc; 

% --- CONFIGURATION ---
port = "COM8"; 
baudRate = 115200; 
packet_size = 52; % Bytes (26 uint16s * 2 bytes)
elements_per_pack = 26; 

% --- HEATMAP SETUP ---
% Define the grid dimensions
gridSize = [5, 5]; 

% Initialize Figure
figure;
% Create the initial image with zeros (5x5)
% 'CDataMapping' scaled ensures colors map to values properly
hMap = imagesc(zeros(gridSize)); 

% Visual Styling
colormap('jet');    % 'jet', 'parula', 'hot', 'turbo' are good options
colorbar;           % Show the color scale
clim([0, 4096]);    % <--- IMPORTANT: Lock the color scale (0 to Max ADC)
                    % If you don't do this, the colors will flicker as data changes.

title('Real-Time Sensor Heatmap (5x5)');
axis square;        % Make pixels square
xticks(1:5);        % Label X axis 1-5
yticks(1:5);        % Label Y axis 1-5
grid on;            % Add grid lines to separate boxes
pixelMap = [1, 2, 3, 4, 5, 6, 7, 8, 22, 23, 24, 25, 9, 10, 11, 12, 13, 14, 15, 21, 20, 19, 18, 17, 16 ]; 

% --- SERIAL SETUP ---
disp(['Connecting to ' port '...']); 
s = serialport(port, baudRate); 
flush(s); 
disp('Reading now...'); 

while true
    % 1. Wait for header (Sync)
    while (read(s, 1, "uint16") ~= 65535)   
    end 
    
    % 2. Check for enough data
    if s.NumBytesAvailable >= packet_size
        
        % 3. Read ONE full packet (26 elements)
        % We use the sync byte we just read + the rest of the packet
        % Note: Since we already read the header (1 uint16), we read 25 more.
        % OR: simple logic -> read the remaining packet
        packetData = read(s, elements_per_pack - 1, "uint16");
        
        % 4. Extract Sensor Data
        % Assuming the packetData is now [S1, S2, ... S25]
        % (Adjust this if your packet structure is different)
        sensorData_1D = packetData(1:25); 
        
        % 5. Reshape to 5x5
        % reshape(data, 5, 5) creates a 5x5 matrix
        % The ' at the end transposes it so it fills Row-by-Row
        sorted_data = sensorData_1D(pixelMap);
        sensorGrid = reshape(sorted_data, 5, 5)'; 
        
        
        % 6. Update the Heatmap
        % We only update the 'CData' property. This is very fast.
        set(hMap, 'CData', sensorGrid);
        
        % Optional: Add text numbers on top of the blocks? 
        % (Doing this in a loop can be slow, try the map first)

        drawnow limitrate; 
    end
end
