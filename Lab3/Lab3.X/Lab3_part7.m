%Jose Santiago (jlantia@ucsc.edu)
%Lab 3 part 7

%Call functions

%store simulated data
[Anoise,Hnoise,Adist,Bdist] = CreateTumbleData(1500);

%store accelerometer x y and z data
A_X = Anoise(:,1);
A_Y = Anoise(:,2);
A_Z = Anoise(:,3);

%convert to Engineering units
EU_A_X = A_X/16348;
EU_A_Y = A_Y/16348;
EU_A_Z = A_Z/16348;

%calculate means (AKA bias)
M_A_X = mean(A_X);
M_A_Y = mean(A_Y);
M_A_Z = mean(A_Z);

%calculate max and minimums
max_A_X = max(A_X);
min_A_X = min(A_X);

max_A_Y = max(A_Y);
min_A_Y = min(A_Y);

max_A_Z = max(A_Z);
min_A_Z = min(A_Z);

%calculate scale factors
sf_A_X = (max_A_X + abs(min_A_X))/2; 
sf_A_Y = (max_A_Y + abs(min_A_Y))/2;
sf_A_Z = (max_A_Z + abs(min_A_Z))/2;

%callibrate acceleration
C_A_X = ((A_X - M_A_X)/sf_A_X);
C_A_Y = ((A_Y - M_A_Y)/sf_A_Y);
C_A_Z = ((A_Z - M_A_Z)/sf_A_Z);

%Calculate norm of acceleration data
N_C_A = sqrt(C_A_X.^2 + C_A_Y.^2 + C_A_Z.^2);
N_A = sqrt(A_X.^2 + A_Y.^2 + A_Z.^2);
SD_A = std(N_C_A);

%store magnetometer x y and z data
H_X = Hnoise(:,1);
H_Y = Hnoise(:,2);
H_Z = Hnoise(:,3);

%convert to Engineering units (nanoTesla)
EU_H_X = H_X*150; 
EU_H_Y = H_Y*150;
EU_H_Z = H_Z*150;

%calculate means (AKA bias)
M_H_X = mean(H_X);
M_H_Y = mean(H_Y);
M_H_Z = mean(H_Z);

%calculate max and minimums (aka +/- scale factor)
max_H_X = max(H_X);
min_H_X = min(H_X);

max_H_Y = max(H_Y);
min_H_Y = min(H_Y);

max_H_Z = max(H_Z);
min_H_Z = min(H_Z);

%calculate scale factors
sf_H_X = (max_H_X + abs(min_H_X))/2; 
sf_H_Y = (max_H_Y + abs(min_H_Y))/2;
sf_H_Z = (max_H_Z + abs(min_H_Z))/2;

%callibrate magnetometer
C_H_X = ((H_X - M_H_X)/sf_H_X);
C_H_Y = ((H_Y - M_H_Y)/sf_H_Y);
C_H_Z = ((H_Z - M_H_Z)/sf_H_Z);

%Calculate norm of magnetometer data
N_C_H = sqrt(C_H_X.^2 + C_H_Y.^2 + C_H_Z.^2);
N_H = sqrt(H_X.^2 + H_Y.^2 + H_Z.^2);
SD = std(N_C_H);

%LS calibration 

%Accelerometer
[Atilde_A, Btilde_A] = CalibrateEllipsoidData3D(EU_A_X, EU_A_Y, EU_A_Z, 1000, 0);
[Xcorr_A, Ycorr_A, Zcorr_A] = CorrectEllipsoidData3D(EU_A_X, EU_A_Y, EU_A_Z, Atilde_A, Btilde_A);

%Norm of LS calibrated data
N_LS_A = sqrt(Xcorr_A.^2 + Ycorr_A.^2 + Zcorr_A.^2);

%Norm of non LS calibrated data
N_EU_A = sqrt(EU_A_X.^2 + EU_A_Y.^2 + EU_A_Z.^2);

%Magnetometer
[Atilde_H, Btilde_H] = CalibrateEllipsoidData3D(EU_H_X, EU_H_Y, EU_H_Z, 1000, 0);
[Xcorr_H, Ycorr_H, Zcorr_H] = CorrectEllipsoidData3D(EU_H_X, EU_H_Y, EU_H_Z, Atilde_H, Btilde_H);

%Norm of LS calibrated data
N_LS_H = sqrt(Xcorr_H.^2 + Ycorr_H.^2 + Zcorr_H.^2);

%Norm of non LS calibrated data
N_EU_H = sqrt(EU_H_X.^2 + EU_H_Y.^2 + EU_H_Z.^2);

%{
%plot acceleration data
figure(1)
subplot(3,1,1)
plot(C_A_X,'r.')
axis([0 1000 -1.5 1.5])
ylabel('g')
title('X')

subplot(3,1,2)
plot(C_A_Y,'r.')
axis([0 1000 -1.5 1.5])
ylabel('g')
title('Y')

subplot(3,1,3)
plot(C_A_Z,'r.')
axis([0 1000 -1.5 1.5])
ylabel('g')
title('Z')

sgtitle('Acceleration: Callibrated')

figure(2)
subplot(3,1,1)
plot(A_X,'.')
hold on
yline(M_A_X,'linewidth',2)
hold off
title('X')

subplot(3,1,2)
plot(A_Y, '.')
hold on
yline(M_A_Y,'linewidth',2)
hold off
title('Y')

subplot(3,1,3)
plot(A_Z, '.')
hold on
yline(M_A_Z,'linewidth',2)
hold off
title('Z')

sgtitle('Acceleration: Raw')

%plot magnetometer data
figure(3)
subplot(3,1,1)
plot(C_H_X,'r.')
axis([0 1000 -1.5 1.5])
ylabel('Magnetic Field')
title('X')

subplot(3,1,2)
plot(C_H_Y,'r.')
axis([0 1000 -1.5 1.5])
ylabel('Magnetic Field')
title('Y')

subplot(3,1,3)
plot(C_H_Z,'r.')
axis([0 1000 -1.5 1.5])
ylabel('Magnetic Field')
title('Z')

sgtitle('Magnetometer: Callibrated')

figure(4)
subplot(3,1,1)
plot(H_X,'.')
hold on
yline(M_H_X,'linewidth',2)
hold off
title('X')

subplot(3,1,2)
plot(H_Y, '.')
hold on
yline(M_H_Y,'linewidth',2)
hold off
title('Y')

subplot(3,1,3)
plot(H_Z, '.')
hold on
yline(M_H_Z,'linewidth',2)
hold off
title('Z')

sgtitle('Magnetometer: Raw')

%plot3 for 3D
%}

%plot the norm of callibrated magnetometer data
figure(1)
subplot(2,1,1)
plot(N_C_H,'.')
title('Norm of Magnetometer Data: Post-Naive Calibration')

subplot(2,1,2)
plot(N_H,'.')
title('Norm of Magnetometer Data: Pre-Naive Calibration')

figure(2)
histfit(N_C_H);
title('Normal Distribution of Norm of Magnetometer: Post-Naive Calibration')

figure(3)
histfit(N_H);
title('Normal Distribution of Norm of Magnetometer: Pre-Naive Calibration')

figure(4)
subplot(2,1,1)
plot(N_C_A,'.')
title('Norm of Acceleration Data: Post-Naive Calibration')

subplot(2,1,2)
plot(N_A,'.')
title('Norm of Acceleration Data: Pre-Naive Calibration')

figure(5)
histfit(N_C_A);
title('Normal Distribution of Norm of Acceleration: Post- NaiveCalibration')

figure(6)
histfit(N_A);
title('Normal Distribution of Norm of Acceleration: Pre-Naive Calibration')

figure(7)
plot3(C_H_X, C_H_Y, C_H_Z,'.')
xlabel('X')
ylabel('Y')
zlabel('Z')
title('3-D Magnetometer Data: Naive Calibration')

figure(8)
plot3(C_A_X, C_A_Y, C_A_Z, '.')
xlabel('X')
ylabel('Y')
zlabel('Z')
title('3-D Accelerometer Data: Naive Calibration')

%LS calibration
%Acceleration LS calibration
figure(9)
plot3(Xcorr_A, Ycorr_A, Zcorr_A,'.');
xlabel('X')
ylabel('Y')
zlabel('Z')
title('3-D Accelerometer Data: LS Calibration')

figure(10)
subplot(2,1,1)
plot(N_EU_A,'.')
title('Norm of Acceleration Data: Pre-LS Calibration')

subplot(2,1,2)
plot(N_LS_A,'.')
title('Norm of Acceleration Data: Post-LS Calibration')

figure(11)
histfit(N_EU_A);
title('Normal Distribution of Norm of Acceleration: Pre-LS Calibration')

figure(12)
histfit(N_LS_A);
title('Normal Distribution of Norm of Acceleration: Post-LS Calibration')

%Magnetometer LS calibration
figure(13)
plot3(Xcorr_H, Ycorr_H, Zcorr_H,'.');
xlabel('X')
ylabel('Y')
zlabel('Z')
title('3-D Magnetometer Data: LS Calibration')

figure(14)
subplot(2,1,1)
plot(N_EU_H,'.')
title('Norm of Magnetometer Data: Pre-LS Calibration')

subplot(2,1,2)
plot(N_LS_H,'.')
title('Norm of Magnetometer Data: Post-LS Calibration')

figure(15)
histfit(N_EU_H);
title('Normal Distribution of Norm of Magnetometer: Pre-LS Calibration')

figure(16)
histfit(N_LS_H);
title('Normal Distribution of Norm of Magnetometer: Post-LS Calibration')
