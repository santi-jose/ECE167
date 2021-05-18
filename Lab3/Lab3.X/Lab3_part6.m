%Jose Santiago (jlantia@ucsc.edu)
%Lab 3 part 6

%10 second gyroscope raw data
g_10s = readmatrix('Lab3_part6_gyro_10sec.csv');
g_X_10s = g_10s(:,1);
g_Y_10s = g_10s(:,2);
g_Z_10s = g_10s(:,3);

%convert 10second readings to dps
g_X_dps_10s = g_X_10s/131;
g_Y_dps_10s = g_Y_10s/131;
g_Z_dps_10s = g_Z_10s/131;

%estimate bias
b_g_X_10s = mean(g_X_dps_10s);
b_g_Y_10s = mean(g_Y_dps_10s);
b_g_Z_10s = mean(g_Z_dps_10s);

%store data for 180 degree rotatioms
%X rotation
g_X_dps_180p = readmatrix('Lab3_part6_gyro_X_180.csv');
g_X_dps_180p = g_X_dps_180p(:,1);
g_X_dps_180n = readmatrix('Lab3_part6_gyro_X_-180.csv');
g_X_dps_180n = g_X_dps_180n(:,1);

%Y rotation
g_Y_dps_180p = readmatrix('Lab3_part6_gyro_Y_180.csv');
g_Y_dps_180p = g_Y_dps_180p(:,2);
g_Y_dps_180n = readmatrix('Lab3_part6_gyro_Y_-180.csv');
g_Y_dps_180n = g_Y_dps_180n(:,2);

%Z rotation
g_Z_dps_180p = readmatrix('Lab3_part6_gyro_Z_180.csv');
g_Z_dps_180p = g_Z_dps_180p(:,3);
g_Z_dps_180n = readmatrix('Lab3_part6_gyro_Z_-180.csv');
g_Z_dps_180n = g_Z_dps_180n(:,3);

%integrate the dps to get angular position
%X degrees
g_X_d_p = cumtrapz(g_X_dps_180p - b_g_X_10s);
g_X_d_n = cumtrapz(g_X_dps_180n - b_g_X_10s);
Max_X = max(g_X_d_n);
Min_X = abs(min(g_X_d_p));

%Y degrees
g_Y_d_p = cumtrapz(g_Y_dps_180p - b_g_Y_10s);
g_Y_d_n = cumtrapz(g_Y_dps_180n - b_g_Y_10s);
Max_Y = max(g_Y_d_n);
Min_Y = abs(min(g_Y_d_p));

%Z degrees
g_Z_d_p = cumtrapz(g_Z_dps_180p - b_g_Z_10s);
g_Z_d_n = cumtrapz(g_Z_dps_180n - b_g_Z_10s);
Max_Z = max(g_Z_d_p);
Min_Z = abs(min(g_Z_d_n));

%calculate scale factors
sf_X = (Max_X + Min_X)/2;
sf_Y = (Max_Y + Min_Y)/2;
sf_Z = (Max_Z + Min_Z)/2;

%calculate bias
b_X = sf_X + Min_X;
b_Y = sf_Y + Min_Y;
b_Z = sf_Z + Min_Z;

%callibrate degrees to +/- 180
%X degrees callibration
C_g_X_d_p = (g_X_d_p*180)/Min_X;
C_g_X_d_n = (g_X_d_n*180)/Max_X;
%C_g_X_d_p = ((g_X_d_p - b_X)*180)/sf_X;
%C_g_X_d_n = ((g_X_d_n - b_X)*180)/sf_X;

%Y degrees callibration
C_g_Y_d_p = (g_Y_d_p*180)/Min_Y;
C_g_Y_d_n = (g_Y_d_n*180)/Max_Y;
%C_g_Y_d_p = ((g_Y_d_p - b_Y)*180)/sf_Y;
%C_g_Y_d_n = ((g_Y_d_n - b_Y)*180)/sf_Y;

%Z degrees callibration
C_g_Z_d_p = (g_Z_d_p*180)/Max_Z;
C_g_Z_d_n = (g_Z_d_n*180)/Min_Z;
%C_g_Z_d_p = ((g_Z_d_p - b_Z)*180)/sf_Z;
%C_g_Z_d_n = ((g_Z_d_n - b_Z)*180)/sf_Z;

figure(1)
subplot(3,1,1)
plot(C_g_X_d_p,'linewidth',2)
hold on
plot(C_g_X_d_n,'linewidth',2)
axis([0 4500 -200 200])
ylabel('Degrees')
title('X axis rotation')
legend('Negative Rotation','Positive Rotation')
hold off

subplot(3,1,2)
plot(C_g_Y_d_p,'linewidth',2)
hold on
plot(C_g_Y_d_n,'linewidth',2)
axis([0 5500 -200 200])
ylabel('Degrees')
title('Y axis rotation')
legend('Negative Rotation','Positive Rotation')
hold off

subplot(3,1,3)
plot(C_g_Z_d_n,'linewidth',2)
hold on
plot(C_g_Z_d_p,'linewidth',2)
axis([0 6350 -200 200])
ylabel('Degrees')
title('Z axis rotation')
legend('Negative Rotation','Positive Rotation')
hold off
