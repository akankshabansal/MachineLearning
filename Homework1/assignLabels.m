function [O Oset] = assignLabels(labels, ind)
%Assign test sample's label based on majority from the k-nearest
O=zeros(size(ind,1),1);
Oset=zeros(size(ind));
for i=1:size(ind,1),
	for j=1:size(ind,2),
		Oset(i,j) = labels(1, ind(i,j));
	end;
	%Counting Occurences of unique element and pick maximum
	[u, temp, j] = unique(Oset(i,:));		
    %Get index of max frequency of unique elements
	[max_freq max_ind] = max([accumarray(j', 1)]);	
	if (max_freq > 1)
		O(i) = u'*(max_ind);			
    else
		O(i) = Oset(i,1);			
	end;
end;
end