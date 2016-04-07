clearvars;
close all;
clc;

%Generate the Expected Data Distribution
noOfDistributions = 2;
mu = [-25 20];
multiplier = [0.3 0.7];
sd = 10;
noOfObsv = 5000;
componentObserved = randsample(1:noOfDistributions, noOfObsv, true, multiplier);

P = zeros(1,noOfObsv);
for i = 1:noOfObsv
    component = componentObserved(i);
    muObs = mu(component);
    P(i) = normrnd(muObs,sd);
end

fh = figure;
histfit(P',50,'kernel');
saveas(fh, 'Original-1', 'jpg');


% No of Samples to be collected 
totalSamples = 5000;

% Create the array to store the collected samples. 
generatedSample(1) = [(0.3 * -25) + (0.7 * 20)]; %can be any random values

PosteriorOld = (multiplier(1) * normpdf(generatedSample(1), mu(1), sd)) + (multiplier(2) * normpdf(generatedSample(1), mu(2), sd)); 
%disp(strcat('Old:',num2str(PosteriorOld)));
t=2;
while t <= totalSamples+1
    
    %Proposal Distribution : 
    %q(x) = Gaussian(mu^(t-1),sd)
    %Generate a sample from the proposal distribution to see how accurate it is
    generatedProposal = normrnd(generatedSample(t-1),20);
    
    %Data Likelihood for the porposed value....
    PosteriorNew = (multiplier(1) * normpdf(generatedProposal, mu(1), sd)) + (multiplier(2) * normpdf(generatedProposal, mu(2), sd)); 
    
    %Acceptance probability
    A = min([1, PosteriorNew/PosteriorOld]);
    
    %disp(strcat('New:',num2str(A)));
    %Acceptance Check.. 
    if rand() <= A
       generatedSample(t) =  generatedProposal;
       t = t + 1;
       PosteriorOld = PosteriorNew;
    end
end

fh = figure;
hold on;
%saveas(fh, 'Original-2d', 'jpg');
histfit(P',50,'kernel');

hLines = findobj('Type','Line')
set(hLines(1),'Color','r')
g=findobj(gca,'Type','patch');
set(g(1),'FaceColor',[1 1 1],'EdgeColor','w')
hist(generatedSample,50);

%plot(h,'r');
saveas(fh, 'GeneratedSamples_20', 'jpg');