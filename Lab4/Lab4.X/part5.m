% Jose Santiago (jlsantia@ucsc.edu)
% ECE 167 Lab 4
% Part 5

%Create Trajectory Data
[Acc, Mag, wGyro, Eul] = CreateTrajectoryData(1/50,0);

%create npts to create w/ IntegrateOpenLoop
npts = length(Eul);
E_out = zeros(npts,3);
Eo = deg2rad(Eul(1,:));

%Initialize DCM
Ro = eul2dcm(Eo); %initial attitude from Euler
%Ro = eye(3);

g = deg2rad(wGyro/131);

Rm = Ro; %Rminus input for IntegrateOpenLoop

%call IntegrateOpenLoop npts times
for i = 1:npts,
    %Calculate yaw pitch and roll
    %{
    roll = atan2(Rm(2,3),Rm(3,3));
    pitch = atan2((-1)*Rm(1,3),sqrt((Rm(1,1))^2+(Rm(1,2))^2));
    yaw = atan2(sin(roll)*Rm(3,1)-cos(roll)*Rm(2,1),cos(roll)*Rm(2,2)-sin(roll)*Rm(3,2));
    %}
    
    yaw = atan2(Rm(1,2),Rm(1,1));
    pitch = asin(-1*Rm(1,3));
    roll = atan2(Rm(2,3),Rm(3,3));
    
    
    %Perform open loop integration
    Rp = IntegrateOpenLoop(Rm, transpose(g(i,:)), 1/50);
    Rm = Rp;
     
    %Store extracted Euler angles from DCM as degrees
    E_out(i,:) = rad2deg([yaw pitch roll]);
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
