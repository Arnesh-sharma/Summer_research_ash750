clear; 
clc; 

T = readtable("Sensor_1_adj_data.xlsx"); 
dataLen = 4; 

sensorIDs = unique(T.Sensor, 'stable'); 

for i = 1:length(sensorIDs)

    sensReading = sensorIDs(i);
    id = (T.Sensor == sensReading);

    sensTime = T.Time(id); 
    sensADC = T.ADC_readings(id); 
    sensFrq = T.Frequency(id); 
    sensMag = T.Magnitude(id); 


    figure; 

    t = tiledlayout(2,1); 
    title(t, sprintf("Sensor %d time signal and FFT", sensReading)); 

    ax1 = nexttile; 
    plot(ax1, sensTime, sensADC); 
    title(ax1, sprintf("Sensor %d time signal", sensReading)); 

    ax2 = nexttile; 
    plot(ax2, sensFrq, sensMag); 
    title(ax2, sprintf("Sensor %d FFT", sensReading));
    xlim([0, 1024]); 


end 



