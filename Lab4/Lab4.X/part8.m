% Jose Santiago (jlsantia@ucsc.edu)
% ECE 167 Lab 4
% Part 8

%store tumble data into matrix
M_T = readmatrix('Lab3_part8_tumble1.csv');

%store bias recorded in Lab3
b_Ao = [158, -48, -21];
b_Ho = [14231, -9444, -32568];

%store Accelerometer and Magnetometer data
A = [M_T(:,1), M_T(:,2), M_T(:,3)]; %master body measurements
H = [M_T(:,5), M_T(:,6), M_T(:,7)]; %slave body measurements

%Convert to engineering units
A_EU = (A-b_Ao)/16384; %g
H_EU = (H-b_Ho)*0.15; %microTesla

%Run CallibrateEllipsoidData3D and CorrectEllipsoidData3D
[Atilde_A, Btilde_A] = CalibrateEllipsoidData3D(A_EU(:,1), A_EU(:,2), A_EU(:,3), 20, 0);
[Atilde_H, Btilde_H] = CalibrateEllipsoidData3D(H_EU(:,1), H_EU(:,2), H_EU(:,3), 20, 0);
[X_AC, Y_AC, Z_AC] = CorrectEllipsoidData3D(A_EU(:,1), A_EU(:,2), A_EU(:,3), Atilde_A, Btilde_A);
[X_HC, Y_HC, Z_HC] = CorrectEllipsoidData3D(H_EU(:,1), H_EU(:,2), H_EU(:,3), Atilde_H, Btilde_H);

%Corrected Accelerometer and Magnetometer data
AC = [X_AC; Y_AC; Z_AC];
HC = [X_HC; Y_HC; Z_AC];

%Define accelInertial and magInertial
aI = [0; 0; 1]; %inertial accelerometer
%aI = [0; 0; -1]; 
mI = [23559.5; 4906.2; 39753.2]/1000; %inertial M in microTesla

Rmishato= eye(3); %identity matrix as initial

%Call AlignMasterSlave function
[Rmis, Pbody] = AlignMasterSlave(AC, HC, aI, mI, Rmishato);

yaw_mis = (atan2(Rmis(1,2),Rmis(1,1)))*(180/pi)
pitch_mis = (asin(-1*Rmis(1,3)))*(180/pi)
roll_mis = (atan2(Rmis(2,3),Rmis(3,3)))*(180/pi)

yaw_body = (atan2(Pbody(1,2),Pbody(1,1)))*(180/pi)
pitch_body = (asin(-1*Pbody(1,3)))*(180/pi)
roll_body = (atan2(Pbody(2,3),Pbody(3,3)))*(180/pi)
