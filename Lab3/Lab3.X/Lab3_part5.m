%Jose Santiago (jlantia@ucsc.edu)
%Lab 3 part 5

%10 second gyroscope raw data
g_10s = readmatrix('Lab3_part5_gyro_10sec.csv');
g_X_10s = g_10s(:,1);
g_Y_10s = g_10s(:,2);
g_Z_10s = g_10s(:,3);

%gyroscope dps 10s
g_X_dps_10s = g_X_10s/131;
g_Y_dps_10s = g_Y_10s/131;
g_Z_dps_10s = g_Z_10s/131;

%estimate bias
b_g_X_10s = mean(g_X_dps_10s);
b_g_Y_10s = mean(g_Y_dps_10s);
b_g_Z_10s = mean(g_Z_dps_10s);

%1 hour gyroscope raw data
g_1h = readmatrix('Lab3_part5_gyro_1hour.csv');
g_X_1h = g_1h(:,1);
g_Y_1h = g_1h(:,2);
g_Z_1h = g_1h(:,3);

%gyroscope dps 1h
g_X_dps_1h = g_X_1h/131;
g_Y_dps_1h = g_Y_1h/131;
g_Z_dps_1h = g_Z_1h/131;

%subtract bias from 1h data
C_g_X_dps_1h = g_X_dps_1h - b_g_X_10s;
C_g_Y_dps_1h = g_Y_dps_1h - b_g_Y_10s;
C_g_Z_dps_1h = g_Z_dps_1h - b_g_Z_10s;

%angular rate
g_X_d = cumtrapz(C_g_X_dps_1h);
g_Y_d = cumtrapz(C_g_Y_dps_1h);
g_Z_d = cumtrapz(C_g_Z_dps_1h);

%plot bias over time
figure(1)

%X dps
subplot(3,1,1)
plot(C_g_X_dps_1h)
ylabel('dps')
title('X dps in 1 hour')
hold on
%Y dps
subplot(3,1,2)
plot(C_g_Y_dps_1h)
ylabel('dps')
title('Y dps in 1 hour')
hold on
%Z dps
subplot(3,1,3)
plot(C_g_Z_dps_1h)
ylabel('dps')
title('Z dps in 1 hour')
hold off

%plot drift of degrees in 1 hour
figure(2)

%X degrees
subplot(3,1,1)
plot(g_X_d,'linewidth',2)
ylabel('degrees')
title('Angular X position in 1 hour')
hold on
%Y degrees
subplot(3,1,2)
plot(g_Y_d,'linewidth',2)
ylabel('degrees')
title('Angular Y position in 1 hour')
hold on
%Z degrees
subplot(3,1,3)
plot(g_Z_d,'linewidth',2)
ylabel('degrees')
title('Angular Z position in 1 hour')
hold on



