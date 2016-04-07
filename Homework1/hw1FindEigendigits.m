function [m V] = hw1FindEigendigits(A, k)
% [m V] = hw1FindEigendigits(A) takes take an (x by k) matrix A 
%			x is the total number of pixels in an image 
%			k is the number of training images 
%			Returns 
%			vector m of length x containing the mean column vector of A 
%			V : x by k matrix that contains k eigenvectors of the covariance matrix of A (after the mean has been subtracted)

%Normalize the matrix X before getting eigenvalues/vectors
meanMu = mean(A');
A_norm = bsxfun(@minus, A', meanMu);
A_norm = A_norm'*1.0;
m = meanMu';
%Compute Eigen Vectors. The function returns the top k eigen vectors. 
[U mu] = eigs(A_norm'*A_norm, k);
%[U mu] = eig(A_norm'*A_norm);
V = A_norm*U;
