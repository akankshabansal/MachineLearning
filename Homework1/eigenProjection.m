function Z = eigenProjection(eigen, A, mean)
%project (28*28) x Size data to dimensions defined by Eigen Values 

Z = zeros(size(eigen,2), size(A,2));
A_norm = bsxfun(@minus, A, mean);
Z = eigen' * A_norm;

end