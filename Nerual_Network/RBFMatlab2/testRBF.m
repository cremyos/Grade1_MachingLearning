clear;
load sample_ex6.mat;  %load data
[M,N]=size(data);
hidden_layers=10;
theta=0.001;
eta=0.00001;
wkj=-1/(hidden_layers^0.5)+2/(hidden_layers^0.5)*rand(1,hidden_layers);
wji=-1/(M^0.5)+2/(M^0.5)*rand(hidden_layers,M);
%input data normalization
[norm_data,norm_dataps]=mapminmax(data);
%10-fold crossing validation
sub_N=N/10;
rates=zeros(1,10);
for i=1:10
    norm_testdata=data(:,1:sub_N);  %set the first part as testdata 
    norm_traindata=data(:,sub_N+1:N);   %set the next nine part as traindata
    rates(1,i)=Batch_BP_Neural_Network(norm_traindata,norm_testdata,hidden_layers,wji,wkj,theta,eta);
    data=[norm_traindata,norm_testdata];
end
disp('the accuracy of ten validation:')
disp(rates);disp('the average accuracy is:')
ave_rate=sum(rates)/10;
disp(ave_rate);