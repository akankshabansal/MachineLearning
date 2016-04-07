function [grad] = gradient(eta, Y, W)
% Delta W = eta (I + (1-2Z)Y')W 
%	eta = learning rate
%	Z = sigmoid(Y)
%	W is the predicted matrix

delW = zeros(size(W));

% sigmoid Function 
Z = zeros(size(Y));
Z = 1 ./ (1 + exp(-Y));
Id = eye(size(Y,1));
grad = eta * (Id + (1-2*Z)*Y') * W;

end