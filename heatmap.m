clear; 
clc; 


%setup 
port = "COM8"; 
baudRate = 115200; 
packet_size = 52; 
elements_per_pack = 26; 
gridSize = [5,5]; 

%init figure
figure; 

%make the heat map and init with zeros 
heatMap = imagesc(zeros(gridSize));
colormap('jet'); 
colorbar; 
clim([0, 4096]); 

title('5x5 Cap Sensor Array'); 
axis square; 
xticks(1:5);
yticks(1:5);
grid on; 

%connection process
disp(['Connecting to ' port '...']); 
s = serialport(port,baudRate); 
flush(s); 
disp('Reading now...'); 


while true


    while (read(s, 1, "uint16") ~= 65535)
        %wait for flag bit otherwise do nothing do nothing
    end 


    if s.NumBytesAvailable >= packet_size
        % check if we have enough data then read packet
        packetData = read(s, elements_per_pack-1, "uint16"); 

        %get the sensor data and store it 
        sensorData = packetData(1:25); 

        %fit in 5x5 grid and adjust data also 
        sensorGrid = reshape(sensorData, 5, 5)'; 


        %update heat map
        set(heatMap, 'CData', sensorGrid); 

        drawnow limitrate; 




    end 
end 

