% Converts an array of WGS84 LLA to an array of ECEF coordinates
function [X] = LLA2ECEF(lat, long, alt)
	a=6378137.0;
	b=6356752.3142;
	e=0.08181919084;
	lat=pi*lat/180;
	long=pi*long/180;
	N=a./sqrt(1-(e*sin(lat)).^2);
	xy=(N+alt).*cos(lat);
	x=xy.*cos(long);
	y=xy.*sin(long);
	z=(N*(1-e^2)+alt).*sin(lat);
    X=[x,y,z];
end