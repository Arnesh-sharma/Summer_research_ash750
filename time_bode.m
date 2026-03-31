clear; 
clc; 

figure; 
hold on; 

for i = 2:4
filename = sprintf("S%d5V1KB.CSV", i);
 fprintf('reading %s :)\n', filename); 

 if isfile(filename)
    data = readmatrix(filename); 

    freq = data(:,2);
    mag = data(:,3); 


    h = semilogx(freq, mag, 'LineWidth', 1.5); 
    grid on; 
    hold on; 
else
    fprintf('No file detected put files in same directory plz :)\n')

 end 
end 

xlabel('Frequnecy (Hz)'); 
ylabel('Magnitude (dB)'); 
legend ('sen2', 'sen3', 'sen4'); 

title('Bode response of sensors with no perspex and source 20mm away and exciter over sensor 2')


figure; 
for i = 2:4
filename = sprintf("S%d5V1KN.CSV", i);
 fprintf('reading %s :)\n', filename); 

 if isfile(filename)
    data = readmatrix(filename); 

     x = data(:,1);
    y = data(:,2); 


    plot(x,y);
    grid on; 
    hold on; 
else
    fprintf('No file detected put files in same directory plz :)\n')

 end 
end 
xlabel('time (s)'); 
ylabel('Volts (V)'); 
legend ( 'sen2', 'sen3', 'sen4'); 
xlim([-0.0121, 0.0121]);

title('Time domain response of sensors with no perspex and source 20mm away and exciter (5Vpp 1K Hz) over sensor 2')

