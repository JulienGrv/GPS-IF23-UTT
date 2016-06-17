% Clear all
clear;clc;close all;format long;

% Import files
bouchon; % Parcours du bouchon de Troyes à vélo
car; % Parcours en voiture
rooftop; % Satellites tracking for more than 48h on a rooftop

% Scale altitude from cm to m
var_bouchon(:,9) = var_bouchon(:,9)/100;
var_rooftop(:,9) = var_rooftop(:,9)/100;
var_car(:,9) = var_car(:,9)/100;

% Scale speed from 100ths knots to km/h
var_bouchon(:,13) = var_bouchon(:,9)/100*1.852;
var_rooftop(:,13) = var_rooftop(:,9)/100*1.852;
var_car(:,13) = var_car(:,9)/100*1.852;

% Scale HDOP from 100ths to 1
var_bouchon(:,11) = var_bouchon(:,11)/100;
var_rooftop(:,11) = var_rooftop(:,11)/100;
var_car(:,11) = var_car(:,11)/100;

% Remove too high PDOP, HDOP and VDOP
var_bouchon((15<var_bouchon(:,12)),:)=[];
var_car((15<var_car(:,12)),:)=[];
var_rooftop((15<var_rooftop(:,12)),:)=[];
var_bouchon((15<var_bouchon(:,13)),:)=[];
var_car((15<var_car(:,13)),:)=[];
var_rooftop((15<var_rooftop(:,13)),:)=[];
var_bouchon((15<var_bouchon(:,14)),:)=[];
var_car((15<var_car(:,14)),:)=[];
var_rooftop((15<var_rooftop(:,14)),:)=[];

% Plot 'bouchon' trajectory in 2D
figure(1);
plot(var_bouchon(:,8),var_bouchon(:,7),'b');
title('Parcours du bouchon de Troyes (2D)');
xlabel('Latitude (deg)');ylabel('Longitude (deg)');

% "" in 3D
figure(2);
plot3(var_bouchon(:,8),var_bouchon(:,7),var_bouchon(:,9),'b');
title('Parcours du bouchon de Troyes (3D)');
xlabel('Latitude (deg)');ylabel('Longitude (deg)');zlabel('Altitude (m)');

% Plot PDOP, HDOP and VDOP as function of the number of used satellites
%% PDOP
[bmean,bmed,bvar]=stats_dop(var_bouchon,12);
[cmean,cmed,cvar]=stats_dop(var_car,12);
[rmean,rmed,rvar]=stats_dop(var_rooftop,12);
figure(3);
plot(var_bouchon(:,10),var_bouchon(:,12),'.b',var_car(:,10),var_car(:,12),'.g',var_rooftop(:,10),var_rooftop(:,12),'.r',1:12,bmean,'b',1:12,cmean,'g',1:12,rmean,'r',1:12,bmed,'--b',1:12,cmed,'--g',1:12,rmed,'--r');
axis([3 10 1 10])
title('PDOP en fonction du nombre de satellites utilises');
legend('Urbain','Voiture','Ciel degage','Moyenne urbain','Moyenne voiture','Moyenn ciel degage','Mediane urbain','Mediane voiture','Mediane ciel degage');
xlabel('Nb de satellites');ylabel('PDOP');
figure(6);
plot(1:12,bvar,'+b',1:12,cvar,'+g',1:12,rvar,'+r');
hold on;
%% HDOP
[bmean,bmed,bvar]=stats_dop(var_bouchon,13);
[cmean,cmed,cvar]=stats_dop(var_car,13);
[rmean,rmed,rvar]=stats_dop(var_rooftop,13);
figure(4);
plot(var_bouchon(:,10),var_bouchon(:,13),'.b',var_car(:,10),var_car(:,13),'.g',var_rooftop(:,10),var_rooftop(:,13),'.r',1:12,bmean,1:12,cmean,'g',1:12,rmean,'r',1:12,bmed,'--b',1:12,cmed,'--g',1:12,rmed,'--r');
axis([3 10 1 3.5])
title('HDOP en fonction du nombre de satellites utilises');
legend('Urbain','Voiture','Ciel degage','Moyenne urbain','Moyenne voiture','Moyenn ciel degage','Mediane urbain','Mediane voiture','Mediane ciel degage');
xlabel('Nb de satellites');ylabel('HDOP');
figure(6);
plot(1:12,bvar,'*b',1:12,cvar,'*g',1:12,rvar,'*r');
%% VDOP
[bmean,bmed,bvar]=stats_dop(var_bouchon,14);
[cmean,cmed,cvar]=stats_dop(var_car,14);
[rmean,rmed,rvar]=stats_dop(var_rooftop,14);
figure(5);
plot(var_bouchon(:,10),var_bouchon(:,14),'.b',var_car(:,10),var_car(:,14),'.g',var_rooftop(:,10),var_rooftop(:,14),'.r',1:12,bmean,1:12,cmean,'g',1:12,rmean,'r',1:12,bmed,'--b',1:12,cmed,'--g',1:12,rmed,'--r');
axis([3 10 1 8])
title('VDOP en fonction du nombre de satellites utilises');
legend('Urbain','Voiture','Ciel degage','Moyenne urbain','Moyenne voiture','Moyenn ciel degage','Mediane urbain','Mediane voiture','Mediane ciel degage');
xlabel('Nb de satellites');ylabel('VDOP');
figure(6);
plot(1:12,bvar,'ob',1:12,cvar,'og',1:12,rvar,'or');
title('Variance des DOP en fonction du nombre de satellites utilises');
legend('Variance PDOP urbain','Variance PDOP voiture','Variance PDOP ciel degage','Variance HDOP urbain','Variance HDOP voiture','Variance HDOP ciel degage','Variance VDOP urbain','Variance VDOP voiture','Variance VDOP ciel degage');
xlabel('Nb de satellites');ylabel('DOP');
hold off;

% Calculate mean of the GPS receiver positioning
% Convert LLA to ECEF
var_bouchon_ecef=LLA2ECEF(var_bouchon(:,7),var_bouchon(:,8),0);
var_car_ecef=LLA2ECEF(var_car(:,7),var_car(:,8),0);
var_rooftop_ecef=LLA2ECEF(var_rooftop(:,7),var_rooftop(:,8),0);
% Get average point to get theoretical 
var_rooftop_ecef_mean=mean(var_rooftop_ecef);
temp=repmat(var_rooftop_ecef_mean,size(var_rooftop_ecef,1),1);
var_rooftop_ecef(:,4)=sqrt(sum((var_rooftop_ecef-temp).^2,2));
average_distance=mean(var_rooftop_ecef(:,4));
disp(strcat('Distance moyenne par rapport au point théorique = ',num2str(average_distance)));