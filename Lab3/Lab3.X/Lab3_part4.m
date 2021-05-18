%Jose Santiago (jlsantia@ucsc.edu)
%Lab 3 Part 4

%Accelerometer-----------------------------------------------------------

%Read data from logging
%Z axis
a_Z_p = readmatrix('Lab3_part4_a_Z_o+.csv');
M_a_Z_p = mean(a_Z_p);

a_Z_n = readmatrix('Lab3_part4_a_Z_o-.csv');
M_a_Z_n = mean(a_Z_n);

%Y axis
a_Y_p = readmatrix('Lab3_part4_a_Y_o+.csv');
M_a_Y_p = mean(a_Y_p);

a_Y_n = readmatrix('Lab3_part4_a_Y_o-.csv');
M_a_Y_n = mean(a_Y_n);

%X axis
a_X_p = readmatrix('Lab3_part4_a_X_o+.csv');
M_a_X_p = mean(a_X_p);

a_X_n = readmatrix('Lab3_part4_a_X_o-.csv');
M_a_X_n = mean(a_X_n);

%Calculate scale factor
sf_a_Z = (M_a_Z_p + abs(M_a_Z_n))/2;
sf_a_Y = (M_a_Y_p + abs(M_a_Y_n))/2;
sf_a_X = (M_a_X_p + abs(M_a_X_n))/2;

%Calculate bias
b_a_Z = M_a_Z_n + sf_a_Z;
b_a_Y = M_a_Y_n + sf_a_Y;
b_a_X = M_a_X_n + sf_a_X;

%Callibrate data
%Z acceleration
C_a_Z_p = ((a_Z_p - b_a_Z))/sf_a_Z;
M_C_a_Z_p = mean(C_a_Z_p);
C_a_Z_n = ((a_Z_n - b_a_Z))/sf_a_Z;
M_C_a_Z_n = mean(C_a_Z_n);

%Y acceleration
C_a_Y_p = ((a_Y_p - b_a_Y))/sf_a_Y;
M_C_a_Y_p = mean(C_a_Y_p);
C_a_Y_n = ((a_Y_n - b_a_Y))/sf_a_Y;
M_C_a_Y_n = mean(C_a_Y_n);

%X acceleration
C_a_X_p = ((a_X_p - b_a_X))/sf_a_X;
M_C_a_X_p = mean(C_a_X_p);
C_a_X_n = ((a_X_n - b_a_X))/sf_a_X;
M_C_a_X_n = mean(C_a_X_n);

%Magnetometer------------------------------------------------------------
%X axis
m_X_p = readmatrix('Lab3_part4_m_X_o+.csv');
m_X_p = m_X_p(:,1)*150; %convert data to nanoTesla (nT)
M_m_X_p = mean(m_X_p);

m_X_n = readmatrix('Lab3_part4_m_X_o-.csv');
m_X_n = m_X_n(:,1)*150; %convert data to nanoTesla (nT)
M_m_X_n = mean(m_X_n);

%Y axis
m_Y_p = readmatrix('Lab3_part4_m_Y_o+.csv');
m_Y_p = m_Y_p(:,2)*150; %convert data to nanoTesla (nT)
M_m_Y_p = mean(m_Y_p);

m_Y_n = readmatrix('Lab3_part4_m_Y_o-.csv');
m_Y_n = m_Y_n(:,2)*150; %convert data to nanoTesla (nT)
M_m_Y_n = mean(m_Y_n);

%Z axis
m_Z_p = readmatrix('Lab3_part4_m_Z_o+.csv');
m_Z_p = m_Z_p(:,3)*150; %convert data to nanoTesla (nT)
M_m_Z_p = mean(m_Z_p);

m_Z_n = readmatrix('Lab3_part4_m_Z_o-.csv');
m_Z_n = m_Z_n(:,3)*150; %convert data to nanoTesla (nT)
M_m_Z_n = mean(m_Z_n); 

%Calculate scale factor in microTesla (uT)
sf_m_X = (M_m_X_p + abs(M_m_X_n))/2;
sf_m_Y = (M_m_Y_p + abs(M_m_Y_n))/2;
sf_m_Z = (M_m_Z_p + abs(M_m_Z_n))/2;

%Calculate Bias
b_m_X = M_m_X_n + sf_m_X;
b_m_Y = M_m_Y_n + sf_m_Y;
b_m_Z = M_m_Z_n + sf_m_Z;

%Callibrate data
%X axis
C_m_X_p = ((m_X_p - b_m_X));
M_C_m_X_p = mean(C_m_X_p);
C_m_X_n = ((m_X_n - b_m_X));
M_C_m_X_n = mean(C_m_X_n);

%Y axis
C_m_Y_p = ((m_Y_p - b_m_Y));
M_C_m_Y_p = mean(C_m_Y_p);
C_m_Y_n = ((m_Y_n - b_m_Y));
M_C_m_Y_n = mean(C_m_Y_n);

%Z axis
C_m_Z_p = ((m_Z_p - b_m_Z));
M_C_m_Z_p = mean(C_m_Z_p);
C_m_Z_n = ((m_Z_n - b_m_Z));
M_C_m_Z_n = mean(C_m_Z_n);

%Plots-------------------------------------------------------------------

%Acceleration
%Z axis
figure(1)
subplot(3,2,5)
plot(a_Z_p,'.')
hold on
plot(C_a_Z_p,'.')
hold on
%yline(M_C_a_Z_p,'Linewidth',2)
legend('Raw','Callibrated')
title('Acceleration: Positive Z Orientation')
hold off

subplot(3,2,6)
plot(a_Z_n,'.')
hold on
plot(C_a_Z_n,'.')
hold on
%yline(M_C_a_Z_n,'Linewidth',2)
legend('Raw','Callibrated')
title('Acceleration: Negative Z Orientation')
hold off

%Y axis
subplot(3,2,3)
plot(a_Y_p,'.')
hold on
plot(C_a_Y_p,'.')
hold on
%yline(M_C_a_Y_p,'Linewidth',2)
legend('Raw','Callibrated')
title('Acceleration: Positive Y Orientation')
hold off

subplot(3,2,4)
plot(a_Y_n,'.')
hold on
plot(C_a_Y_n,'.')
hold on
%yline(M_C_a_Y_n,'Linewidth',2)
legend('Raw','Callibrated')
title('Acceleration: Negative Y Orientation')
hold off

%X axis
subplot(3,2,1)
plot(a_X_p,'.')
hold on
plot(C_a_X_p,'.')
hold on
%yline(M_C_a_X_p,'Linewidth',2)
legend('Raw','Callibrated')
title('Acceleration: Positive X Orientation')
hold off

subplot(3,2,2)
plot(a_X_n,'.')
hold on
plot(C_a_X_n,'.')
hold on
%yline(M_C_a_X_n,'Linewidth',2)
legend('Raw','Callibrated')
title('Acceleration: Negative X Orientation')
hold off

%Magnetometer
%X axis
figure(2)
subplot(3,2,1)
plot(m_X_p,'.')
hold on
plot(C_m_X_p,'.')
hold on
yline(M_C_m_X_p,'Linewidth',2)
axis([0 700 25000 50000])
legend('Raw','Callibrated','scale factor: 30,685')
title('Magnetometer: Positive X Orientation')
ylabel('nanoTesla')
hold off

subplot(3,2,2)
plot(m_X_n,'.')
hold on
plot(C_m_X_n,'.')
hold on
yline(M_C_m_X_n,'Linewidth',2)
axis([0 700 -35000 -10000])
legend('Raw','Callibrated','scale factor: -30,685')
title('Magnetometer: Negative X Orientation')
ylabel('nanoTesla')
hold off

%Y axis
subplot(3,2,3)
plot(m_Y_p,'.')
hold on
plot(C_m_Y_p,'.')
hold on
yline(M_C_m_Y_p,'Linewidth',2)
axis([0 1000 15000 35000])
legend('Raw','Callibrated','scale factor: 29,701')
title('Magnetometer: Positive Y Orientation')
ylabel('nanoTesla')
hold off

subplot(3,2,4)
plot(m_Y_n,'.')
hold on
plot(C_m_Y_n,'.')
hold on
yline(M_C_m_Y_n,'Linewidth',2)
axis([0 1000 -45000 -25000])
legend('Raw','Callibrated','scale factor: -29,701')
title('Magnetometer: Negative Y Orientation')
ylabel('nanoTesla')
hold off

%Z axis
subplot(3,2,5)
plot(m_Z_p,'.')
hold on
plot(C_m_Z_p,'.')
hold on
yline(M_C_m_Z_p,'Linewidth',2)
axis([0 1500 0 50000])
legend('Raw','Callibrated','scale factor: 42,580')
title('Magnetometer: Positive Z Orientation')
ylabel('nanoTesla')
hold off

subplot(3,2,6)
plot(m_Z_n,'.')
hold on
plot(C_m_Z_n,'.')
hold on
yline(M_C_m_Z_n,'Linewidth',2)
axis([0 1500 -80000 -35000])
legend('Raw','Callibrated','scale factor: -42,580')
title('Magnetometer: Negative Z Orientation')
ylabel('nanoTesla')
hold off

