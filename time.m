
myData = {'L0N.CSV', 'L1N.CSV','L2N.CSV', 'L3N.CSV',... 
    'L4N.CSV', 'L5N.CSV'}; 

figure; 
for i = 1:6
filename = sprintf(myData{i});
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
legend ( 'LOOP 0', 'LOOP 1', 'LOOP 2', 'LOOP 3', 'LOOP 4', 'LOOP 5'); 


title('Time domain response of sensors with no perspex and source 20mm away and exciter (5Vpp 10K Hz) ')

for i = 1:6
filename = sprintf(myData{i});
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
legend ('LOOP 0', 'LOOP 1', 'LOOP 2', 'LOOP 3', 'LOOP 4', 'LOOP 5'); 
% xlim([0,1000])

title('Bode  response of sensors with no perspex and source 20mm away')
