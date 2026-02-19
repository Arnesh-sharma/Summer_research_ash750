clear; 
clc; 

figure; 
hold on; 

for i = 0:2:8
filename = sprintf('S1%dMMP.csv', i);
 fprintf('reading %s :)\n', filename); 
%filename = sprintf('S%d2P.csv', i);
%filename = sprintf('S%d4P.csv', i);
%filename = sprintf('S%d8PCN.csv', i);

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
legend ('sen0', 'sen1', 'sen2', 'sen3', 'sen4', 'sen5'); 

if contains(filename, "NP")
    title('Bode response of sensors with no perspex and source 20mm away')
elseif contains(filename, "2P")
    title('Bode response of sensors with 2mm perspex and source 20mm away')
elseif contains(filename, "4P")
    title('Bode response of sensors with 4mm perspex and source 20mm away')
elseif contains(filename, "8P")
    title('Bode response of sensors with 8mm perspex and source 20mm away')
elseif contains(filename, "MMP")
     title('Bode response of Sensor 1 with different perspex thicknesses and source 20mm away')
     legend ('0mm', '2mm', '4mm', '8mm'); 
     xlim ([10,2000]); 
end