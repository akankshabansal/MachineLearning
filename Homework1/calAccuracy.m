function accuracy = calAccuracy(myLabels, testLabels)
%Compute the accuracy of the assigned labels

total = size(myLabels, 1);
match = 0;
for i=1:total,
	if myLabels(i) == testLabels(1,i),
		match = match + 1;
	end;
end;

accuracy = double(double(match*100.0)/(double(total)*1.0));

end