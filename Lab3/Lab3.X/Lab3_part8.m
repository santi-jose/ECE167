%Jose Santiago (jlantia@ucsc.edu)
%Lab 3 part 8

%store tumble data nito matrix
M_T = readmatrix('Lab3_part8_tumble1.csv');

%Extract data from tumble matrix for Acceleration
A_X = M_T(:,1);
A_Y = M_T(:,2);
A_Z = M_T(:,3);

%store bias from part 4
b_A_X = 158;
b_A_Y = -48;
b_A_Z = -21;

%store scale factors from part 4
sf_A_X = 16435; 
sf_A_Y = 16439;
sf_A_Z = 16493;

%callibrate acceleration
C_A_X = ((A_X - b_A_X)/sf_A_X);
C_A_Y = ((A_Y - b_A_Y)/sf_A_Y);
C_A_Z = ((A_Z - b_A_Z)/sf_A_Z);

%take mean and standard deviation of norm of calibrated data
C_N_A = sqrt(C_A_X.^2 + C_A_Y.^2 + C_A_Z.^2);
C_m_N_A = mean(C_N_A);
SD_C_N_A = std(C_N_A);

%Convert data to engineering units
%A_X = A_X/16384;
%A_Y = A_Y/16384;
%A_Z = A_Z/16384;

%take the mean and standard deviations of norm of the raw data
N_A = sqrt(A_X.^2 + A_Y.^2 + A_Z.^2);
m_N_A = mean(N_A);
SD_N_A = std(N_A);

%Extract data from tumble matrix for Magnetometer
H_X = M_T(:,5);
H_Y = M_T(:,6);
H_Z = M_T(:,7);

%store bias from part 4
b_H_X = 14231;
b_H_Y = -9444;
b_H_Z = -32568;

%store scale factors from part 4
sf_H_X = 30685; 
sf_H_Y = 29701;
sf_H_Z = 42580;

%Calibrate  Magnetometer
C_H_X = ((H_X - b_H_X)/sf_H_X);
C_H_Y = ((H_Y - b_H_Y)/sf_H_Y);
C_H_Z = ((H_Z - b_H_Z)/sf_H_Z);

%take mean and standard deviation of norm of calibrated data
C_N_H = sqrt(C_H_X.^2 + C_H_Y.^2 + C_H_Z.^2);
C_m_N_H = mean(C_N_H);
SD_C_N_H = std(C_N_H);

%Plot Acceleration stuff
figure(1)
%subplot(1,2,1)
plot3(A_X, A_Y, A_Z, '.')
xlabel('X Acceleration (g)')
ylabel('Y Acceleration (g)')
zlabel('Z Acceleration (g)')
title('Acceleration Pre-Calibration')

figure(2)
%subplot(1,2,2)
plot3(C_A_X, C_A_Y, C_A_Z, '.')
xlabel('X Acceleration (g)')
ylabel('Y Acceleration (g)')
zlabel('Z Acceleration (g)')
title('Acceleration Post-Calibration')

figure(3)
plot(N_A, '.')
title('Acceleration Norm Pre-Calibration')

figure(4)
plot(C_N_A, '.')
title('Acceleration Norm Post-Calibration')

figure(5)
histfit(N_A)
title('Acceleration Normal Distribution: Pre-Calibration')

figure(6)
histfit(C_N_A)
title('Acceleration Normal Distribution: Post Calibration')

%Plot magnetometer stuff
figure(7)
%subplot(1,2,1)
plot3(H_X, H_Y, H_Z, '.')
xlabel('X Magnetometer')
ylabel('Y Magnetometer')
zlabel('Z Magnetometer')
title('Magnetometer Pre-Calibration')

figure(8)
%subplot(1,2,2)
plot3(C_A_X, C_A_Y, C_A_Z, '.')
xlabel('X Magnetometer')
ylabel('Y Magnetometer')
zlabel('Z Magnetometer')
title('Magnetometer Post-Calibration')

figure(9)
plot(N_H, '.')
title('Magnetometer Norm Pre-Calibration')

figure(10)
plot(C_N_H, '.')
title('Magnetometer Norm Post-Calibration')

figure(11)
histfit(N_H)
title('Magnetometer Normal Distribution: Pre-Calibration')

figure(12)
histfit(C_N_H)
title('Magnetometer Normal Distribution: Post Calibration')
