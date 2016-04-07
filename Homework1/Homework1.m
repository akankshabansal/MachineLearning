%load the test and train data
load('digits.mat');
% Training the Model 
trainImages = double(trainImages);
%Array to Store the accuracy accross various tests. 
acc_arr=zeros(7,1);

for t=6:7,
    %Create a training Images Set 
    trainSize = 2^t;
    %No Of Eigen Vectors to be used
    k=16;
    trainSet = trainImages(:, :, 1, 1:trainSize);
    %Display Some images from the Train Set 
    s1 = 'TrainSet_';
    s2 = num2str(i);
    s = strcat(s1,s2)
    s1='.png';
    s = strcat(s,s1)
    h1Display(trainSet,s);
    A = reshape(trainSet,28*28, trainSize); 
    %Find the Eigen vectors And Eigen values
    [mean eigen_Vec] = hw1FindEigendigits(A, k);
    Vreshaped = reshape(eigen_Vec,28,28,k);
    %Display Eigen Vectors 
    s1 = 'Eigen_Vectors_';
    s2 = num2str(t);
    s = strcat(s1,s2)
    s1='.png';
    s = strcat(s,s1)
    h1Display(Vreshaped(:,:,1:16), s);
    
    %Normalize the Eigen Vectors 
    for i=1:size(eigen_Vec,2),
        Eigen_norm(:,i) = eigen_Vec(:,i)/norm(eigen_Vec(:,i));
    end;

    %Project the Training Images to the Eigen Vector Space
    trainEigenProj = eigenProjection(Eigen_norm, A, mean);
    trainRec = reconstruct(trainEigenProj, Eigen_norm, k);
    trainReshaped = reshape(trainRec,28,28,trainSize);
    %Display Some images from train set into Eigen Space 
    s1 = 'TrainSetEigen_';
    s2 = num2str(t);
    s = strcat(s1,s2)
    s1='.png';
    s = strcat(s,s1)
    h1Display(trainReshaped(:,:,1:16),s);

    %Test Images captured from the Good test Images Set. 
    testSize=1000;
    testImages=double(testImages);
    selectedTest = testImages(:, :, 1, 1:testSize);
    TestLabelSet = testLabels(1, 1:testSize);

    n=3;
    %Display Test set images 
    s1 = 'TestSet_';
    s2 = num2str(t);
    s = strcat(s1,s2)
    s1='.png';
    s = strcat(s,s1)
    h1Display(selectedTest(:,:,1:16),s);

    testSet = reshape(selectedTest,28*28,testSize);
    testEigenProj = eigenProjection(Eigen_norm, testSet, mean);
    testRec = reconstruct(testEigenProj, Eigen_norm, k);
    testReshaped = reshape(testRec,28,28,testSize);
    s1 = 'TestSetEigen_';
    s2 = num2str(t);
    s = strcat(s1,s2)
    s1='.png';
    s = strcat(s,s1)
    h1Display(testReshaped(:,:,1:16),s);

    %%====K-nearest neighbours for Good Test Images =============
    [ind,d]=knnsearch(trainRec',testRec','k',n,'distance','minkowski','p',5);
    [myTestLabels fullLabs] = assignLabels(trainLabels, ind);
    accuracy = calAccuracy(myTestLabels, TestLabelSet);
    display(accuracy);

    %Test Images captured from the Not So Good test Images Set. 
    selectedTest = testImages(:, :, 1, 5000+(1:testSize));
    TestLabelSet = testLabels(1, 5000+(1:testSize));
    s1 = 'TestSet_b_';
    s2 = num2str(t);
    s = strcat(s1,s2)
    s1='.png';
    s = strcat(s,s1)
    h1Display(selectedTest(:,:,1:16),s);

    testSet = reshape(selectedTest,28*28,testSize);
    testEigenProj = eigenProjection(Eigen_norm, testSet, mean);
    testRec = reconstruct(testEigenProj, Eigen_norm, k);
    testReshaped = reshape(testRec,28,28,testSize);
    %Display the Eigen Projection of the Not so good Test images. 
    s1 = 'TestSetEigen_b_';
    s2 = num2str(t);
    s = strcat(s1,s2)
    s1='.png';
    s = strcat(s,s1)
    h1Display(testReshaped(:,:,1:16),s);

    %%==============Find k-nearest neighbours
    [ind,d]=knnsearch(trainRec',testRec','k',n,'distance','minkowski','p',5);

    [myTestLabels fullLabs] = assignLabels(trainLabels, ind);
    accuracy = calAccuracy(myTestLabels, TestLabelSet);
    
    display(accuracy);
    acc_arr(t)= accuracy;
end;
