function [next] = addtoPlot(data, offest, label)
% Plot sound signal data and add it to previous plot

srcCount = size(data,1);
col = 'rgbcmyk';
offest = offest+1;

for i=1:srcCount,
	plot(data(i,:) + offest+(i*1.5), sprintf('%s',col(i)));
	text(pi, offest+(i*1.5)+0.6, sprintf('%s-%d',label,i));
end;
 
next = offest + (srcCount*1.5);
plot([0, size(data,2)],[next+1.5,next+1.5], 'k');

end