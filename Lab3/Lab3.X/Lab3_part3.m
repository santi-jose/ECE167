%Jose Santiago (jlsantia@ucsc.edu)
%Lab 3 Part 3

EllipseXYData

%A = [Xmeas.^2 2*Xmeas.^2 2*Xmeas ones(size(Xmeas))];

%R = ones(size(Xmeas));

%Perform least squares on X and Y meas
A = [Xmeas -Ymeas.^2 Ymeas ones(size(Xmeas))];

R = Xmeas.^2;

%Correct Xmeas and Ymeas as Xhat and Yhat
Xhat = 1:size(Xmeas);
Xhat = Xhat(:);
Yhat = 1:size(Ymeas);
Yhat = Yhat(:);

%variables to store norms of pre and post callibrated data
%{
n = 1:size(Xmeas);
n = n(:);
nhat = 1:size(Xmeas);
nhat = n(:);
%}

%{
for i = 1:size(Xmeas)
    Xhat(i) = (Xmeas(i) - 0.3962)/1.1056;
    Yhat(i) = (Ymeas(i) + 0.3388)/2.1445;
end
%}
Xhat = (Xmeas - 0.3962)/1.1056;
Yhat = (Ymeas + 0.3388)/2.1445;

%plot ellipses
figure(1)
plot(Xmeas, Ymeas, '.')
hold on
plot(Xhat, Yhat, '.')
title('Ellipses')
legend('Raw','Corrected')
hold off

%plot norm
figure(2)
plot(sqrt(Xmeas.^2 + Ymeas.^2),'.')
hold on
plot(sqrt(Xhat.^2 + Yhat.^2),'.')
title('Norm')
legend('Raw', 'Corrected')
hold off
    