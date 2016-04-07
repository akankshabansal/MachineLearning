clear; close all; clc

% U: Source Matrix 
% A: Mixing Matrix 
% X: Mixed Matrix 
 
%Load Test data 
load('icaTest.mat'); 
% U: 3x40   A(3x3)
%Load Data 
load('sounds.mat'); 

U=sounds(1:3,:);	% Take only 3 signals

plot(0,0);
hold on;
xlabel('Time');
offSet=-1;
label='U';
%Print Original Source
offSet = addtoPlot(U, offSet, label); 

%Generate A 
sizeSrc = size(U,1);
A=rand(sizeSrc, sizeSrc);

%Determine X 
X = A*U;
label='X';
%Print Mixed Signal
offSet = addtoPlot(X, offSet, label); 

eta = 0.01;
iterCount=1000;
annealRate = eta; 
annealSize = 1000;

%Initialize W with random values 
%W = rand(size(A)) / 10 ;
W = rand(size(A)) * 0.01;

for i=0:iterCount,
	% Y: Estimate of source Signal 
    Y = W*X;			
    % Gradient descent - shift by delta
	deltaW = gradient(eta, Y, W);	
	% update W
    W = W + deltaW;			
    %convergence using annealing 
    %eta = annealRate / (1 + (i/annealSize)); % annealing - learning rate
    
    % For Convergence using norm of delat W
    r = norm(deltaW);
    if (r < 0.0000000000001)
       disp(i);
       disp(deltaW);
       break;
    end    
end;

% Generate Final estimate of source Signal 
Y = W*X;		
%Normalization
Y = (Y - min(min(Y))) ./ (max(max(Y)) - min(min(Y)));
%Y2=Y;
Y2 = Y .* 2.0;

% Print the Estimates Signal
label='Y';
offSet = addtoPlot(Y2, offSet, label); 
ofile='icaCorrelations3_';
str_iter= num2str(iterCount);
fileName = 'finalImage';
fileName = strcat(fileName,str_iter)
fileName = strcat(fileName,'.png');
print (fileName,'-dpng' );
hold off;

% Calculate the Correlation to match the Signals
ofile = strcat(ofile,str_iter);
ofile = strcat(ofile,'.txt');
corrMat = correlations(U,Y,ofile);
%Play the output Sound
%sound(Y2(1,:));
