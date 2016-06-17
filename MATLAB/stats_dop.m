% Returns sats for PDOP, HDOP, VDOP
function [rmean, rmedian, rvar] = get_stats_HDOP_fSATS(data, index)
	for i = 1:12
		select=data((i==data(:,10)),:);
		select=select(:,index);
		rmean(i)=mean(select);
		rmedian(i)=median(select);
		rvar(i)=var(select);
	end
end