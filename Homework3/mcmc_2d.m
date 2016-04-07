clearvars;
close all;
clc;

%Generate the Expected Data Distribution
noOfDistributions = 2;
mu = [-25 20];
sd = [10 0; 0 10 ];
noOfObsv = 5000;
P = mvnrnd(mu,sd, noOfObsv);
totalSamples = 5000;

% Create the array to store the collected samples. 
generatedSample = zeros(totalSamples,2);
generatedSample(1,:) = [(0.3 * -25) + (0.7 * 20),(0.3 * -25) + (0.7 * 20)];
PosteriorOld = mvnpdf(generatedSample(1,:), mu, sd) ; 

t=2;
while t <= totalSamples+1
    
    %Proposal Distribution : 
    %q(x) = Gaussian(mu^(t-1),sd)
    %Generate a sample from the proposal distribution to see how accurate it is
    generatedProposal = zeros(1,2);
    generatedProposal = mvnrnd(generatedSample(t-1,:),sd,1);
    
    %Data Likelihood for the porposed value....
    PosteriorNew = mvnpdf(generatedProposal, mu, sd); 
    
    %Acceptance probability
    A = min([1, PosteriorNew/PosteriorOld]);
    
    %disp(strcat('New:',num2str(A)));
    %Acceptance Check.. 
    if rand() <= A
       generatedSample(t,:) =  generatedProposal;
       t = t + 1;
       PosteriorOld = PosteriorNew;
       disp(generatedProposal);
    
    end
end

fh = figure;
hist3(P,[25 25]);
saveas(fh, 'Original-2d', 'jpg');

fh = figure;
hist3(generatedSample,[25 25]);

%plot(h,'r');
saveas(fh, 'GeneratedSamples-2d', 'jpg');
