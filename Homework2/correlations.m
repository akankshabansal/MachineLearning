function [corrMat] = correlations(orgSignal, recSignal, filename)
% 1. Calculate correlation between Source and recovered Signals
% 2. Output the corerelation into a file. 
% 3. Find the accuracy of the output

numSrc = size(orgSignal,1);
corrMat = zeros(numSrc,numSrc);

for i=1:numSrc,
	for j=1:numSrc,
		corrMat(i,j) = corr2(orgSignal(i,:), recSignal(j,:));
	end;
end;

fileID = fopen(filename,'w');
fprintf(fileID,'\n ');
for i=1:numSrc,
	fprintf(fileID,'\t Y-%d',i);
end;

for i=1:numSrc,
	fprintf(fileID,'\n X-%d',i);
	for j=1:numSrc,
		fprintf(fileID,' %f ',corrMat(i,j));
	end;
end;
fprintf(fileID,'\n');

AvgCorr =0;
for i=1:numSrc,
	[value, match] = max(corrMat(i,:));
	fprintf(fileID,'X - %d matches Y - %d with %f\n',i,match, value);
    AvgCorr = value + AvgCorr;
end;

accuracy = AvgCorr /numSrc * 100;

fprintf(fileID,'Acuuracy = %f\% \n',accuracy);
end

