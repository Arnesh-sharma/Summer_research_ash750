clear; 
clc; 
global selectedSensor; 
selectedSensor = 1; 

%setup 
port = "COM8"; 
baudRate = 115200; 
packet_size = 52; 
elements_per_pack = 26; 
gridSize = [5,5]; 

%tuning for FFT and heatmap 
Fs = 2055; 
sampleWindow = 1024; 
targetFps = 30; 
drawTime = 1/targetFps; 

%make data storage buffer 
dataBuffer = zeros(sampleWindow, 25); 

%setup figure 
fig = figure('Name', 'Sensor Frequency Analysis', 'Color', 'White');
set(fig, 'Units', 'normalized', 'Position', [0.1 0.1 0.8 0.7]);

%setup heatmap
%will sit on the left side of the figure, has a 5x5 matrix setup
subplot(2,2, [1,3]);
heatMap = imagesc(zeros(gridSize));
colormap('jet'); 
colorbar; 
clim([0, 4096]); 
title('Sensor array (signal mag intensity)'); 
axis square; 
xticks(1:5);
yticks(1:5); 
%var to save x&y label if updated
heatMapXY = gca; 
%set init labels
updatedAxisLabels(heatMapXY, 0); 
%enable clicking 
set(heatMap, 'ButtonDownFunc', @mapClicked); 

%The rotation buttons - did not know we could do this
btnLeft = uicontrol('Parent', fig, 'Style', 'pushbutton', 'String', 'Rotate Left', ...
    'Units', 'normalized', 'Position', [0.15 0.02 0.1 0.05], ...
    'Callback', {@rotateLeft, hAxMap});

btnRight = uicontrol('Parent', fig, 'Style', 'pushbutton', 'String', 'Rotate Right', ...
    'Units', 'normalized', 'Position', [0.26 0.02 0.1 0.05], ...
    'Callback', {@rotateRight, hAxMap});


%time domain plot 
subplot(2,2,2);
timePlot = plot(zeros(sampleWindow, 1), 'LineWidth', 1.5); 
title ('Time domain Signal (signal from PSoC)'); 
xlim([0, sampleWindow]); 
ylable('ADC count'); 


%fft plot 
subplot(2,2,4);
fAxis = Fs*(0:(sampleWindow/2))/sampleWindow; 
fftPlot = plot(fAxis, zeros(size(fAxis)), 'LineWidth',1.5); 
title('FFT'); 
grid on; 
xlim([0, Fs/2]); 
xlabel('Frequency (Hz)'); 
ylabel('Magnitude (dB)'); 


%connect to serial port 
disp(['Connecting to ' port '...']); 
try
    s = serialport("COM8", baudRate); 
    flush(s); 
catch
    error('Connection failed try again'); 
end

lastDraw = tic; 








