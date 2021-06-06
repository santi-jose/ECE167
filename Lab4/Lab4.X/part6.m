% Jose Santiago (jlsantia@ucsc.edu)
% ECE 167 Lab 4
% Part 6

%Create Trajectory Data with noise
[Acc, Mag, wGyro, Eul] = CreateTrajectoryData(1/50,1);
npts = length(Eul);

biasEstimate = [0; 0; 0];
%{
for i = 1:50
    biasEstimate = transpose(deg2rad((wGyro(i,:))/131)) + biasEstimate; %bias 
end
biasEstimate = biasEstimate/50;
%}  

%gyroInput=transpose(deg2rad(wGyro(1,:)/131));
gyroInput = transpose(deg2rad(wGyro/131));

accelInertial=[0;0;1];
%accelReading = transpose(Acc(1,:));
accelReading = transpose(Acc);
%magInertial=[1;0;0];
magInertial = [22770;5329;41510.2]/1000; %NED in nT
%magReading = transpose(Mag(1,:));
magReading = transpose(Mag);

%Kp_a=.2;
%Ki_a=Kp_a/10;
Kp_a = 0.01;
Ki_a = 0;

%Kp_m=.2;
%Ki_m=Kp_m/10;
Kp_m = 0;
Ki_m = 0;

R = [1 0 0; 0 1 0; 0 0 1];
%R = eul2dcm(Eul);
E_out = zeros(npts,3);

for j = 1:npts
    [R,biasEstimate] = IntegrateClosedLoop(R,biasEstimate,gyroInput(:,j),magReading(:,j),accelReading(:,j),magInertial,accelInertial,1/50);
    yaw = atan2(R(1,2),R(1,1));
    pitch = asin(-1*R(1,3));
    roll = atan2(R(2,3),R(3,3));
    E_out(j,:) = rad2deg([yaw pitch roll]);
end

yaw_err = E_out(:,1) - Eul(:,1);
pitch_err = E_out(:,2) - Eul(:,2);
roll_err = E_out(:,3) - Eul(:,3);

figure(1)
subplot(3,1,1)
plot(Eul(:,1))
title('Yaw')
hold on
subplot(3,1,2)
plot(Eul(:,2))
title('Pitch')
hold on
subplot(3,1,3)
plot(Eul(:,3))
title('Roll')
sgtitle('Known Euler')
hold off

figure(2)
subplot(3,1,1)
plot(E_out(:,1))
title('Yaw')
hold on
subplot(3,1,2)
plot(E_out(:,2))
title('Pitch')
hold on
subplot(3,1,3)
plot(E_out(:,3))
title('Roll')
sgtitle('Calculated Euler')
%title('Calculated Euler')
hold off

figure(3)
subplot(3,1,1)
plot(yaw_err)
title('Yaw Error')
subplot(3,1,2)
plot(pitch_err)
title('Pitch Error')
subplot(3,1,3)
plot(roll_err)
title('Roll Error')
sgtitle('Error')

function C=eul2dcm(eul)
%----------------------------------------------------------------
% function C=eul2dcm(eul)
%
%   This functions determines the direction cosine matrix C
%   that transforms a vector in a reference axis system at time k
%   to one the same axis sytem at time k+1.  The input argument to
%   this function is a vector of the Euler angles in the following
%   order: eul = [yaw,pitch,roll]. (i.e., 3-2-1 rotation convention).  
%
%-----------------------------------------------------------------  

ps=eul(1); th=eul(2); ph=eul(3);

C1=[1 0 0; 0 cos(ph) sin(ph); 0 -sin(ph) cos(ph)];
C2=[cos(th) 0 -sin(th); 0 1 0; sin(th) 0 cos(th)];
C3=[cos(ps) sin(ps) 0; -sin(ps) cos(ps) 0; 0 0 1];

C=C1*C2*C3;
end
