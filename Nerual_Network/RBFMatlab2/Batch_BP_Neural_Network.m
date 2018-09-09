function correct_rate=Batch_BP_Neural_Network(train_data,test_data,hidden_layers,Wji,Wkj,theta,eta)
%-------------------------------------------------------------------
%Batch back-propagation neural network function which includes input layer(multiple layers with bias)¡¢
%hidden layer(multiple layers) and output(one layer)
%Inputs:
%train_data     -train data(including samples and its target output)
%test_data      -test data(including samples and its target output)
%hidden_layers  -numbers of hidden layers
%Wji            -weights between input layer and hidden layer
%Wkj            -weights between hidden layer and putput layer
%theta          -threhold of target function
%eta            -learnning rate
%Output:
%correct_rate:  -classification correct rate of the test data
%-------------------------------------------------------------------
[rows,cols]=size(train_data);
train_input=train_data(1:rows-1,:);
train_target=train_data(rows,:);
test_input=test_data(1:rows-1,:);
test_target=test_data(rows,:);
%augmentation the train and test input
train_bias=ones(1,cols);
test_bias=ones(1,size(test_data,2));
train_input=[train_bias;train_input];
test_input=[test_bias;test_input];
%batch bp algorithm
r=0;   %initialize the episode
J=zeros(1,1000); %initialize the error function
while(1)     %outer loop
    r=r+1;m=0;DELTA_Wji=zeros(hidden_layers,rows);DELTA_Wkj=zeros(1,hidden_layers);  %initialization
    while(1)    %inner loop
        m=m+1;
        netj=zeros(1,hidden_layers);  
        yj=zeros(1,hidden_layers);
        for j=1:hidden_layers
            netj(1,j)=sum(train_input(:,m)'.*Wji(j,:));   %sum of product 
            yj(1,j)=3.432/(1+exp(-2*netj(1,j)/3))-1.716;   %activation
        end
        netk=sum(yj(1,:).*Wkj(1,:));  %sum of product,output layer has only one neutron
        zk=netk;   %activation
        J(1,r)=J(1,r)+(train_target(1,m)-zk)^2/2;   %every sample has a error
        for j=1:hidden_layers
            delta_k=(train_target(1,m)-zk);  %the sensitivity of output neutrons
            DELTA_Wkj(1,j)=DELTA_Wkj(1,j)+eta*delta_k*yj(1,j); %update the DELTA_Wkj
        end
        delta_j=zeros(1,hidden_layers);
        for j=1:hidden_layers
            delta_j(1,j)=Wkj(1,j)*delta_k*(2.288*exp(-2*netj(1,j)/3)/(1+exp(-2*netj(1,j)/3)^2)); %the sensitivity of hidden neutrons
            for i=1:rows
                DELTA_Wji(j,i)=DELTA_Wji(j,i)+eta*delta_j(1,j)*train_input(i,m);   %update the DELTA_Wji
            end
        end
        if(m==cols)  %all samples has been trained(one episode)
            break;   %back to outer loop
        end
    end    %end inner loop
    for j=1:hidden_layers
        Wkj(1,j)=Wkj(1,j)+DELTA_Wkj(1,j);   %update Wkj
    end
    for j=1:hidden_layers
        for i=1:rows
            Wji(j,i)=Wji(j,i)+DELTA_Wji(j,i);   %update Wji
        end
    end
    J(1,r)=J(1,r)/cols;
    if((r>=2)&&abs(J(1,r)-J(1,r-1))<theta)  %determine when to stop
        %disp('ok!');disp(r);
        %plot(0:r-1,J(1,1:r));hold on;
        %start to test the model 
        correct=0;
        for i=1:size(test_data,2)
            test_netj=zeros(1,hidden_layers);  
            test_yj=zeros(1,hidden_layers);
            for j=1:hidden_layers
                test_netj(1,j)=sum(test_input(:,i)'.*Wji(j,:));   %sum of product 
                test_yj(1,j)=3.432/(1+exp(-2*test_netj(1,j)/3))-1.716;   %activation
            end
            test_netk=sum(test_yj(1,:).*Wkj(1,:));  %sum of product,output layer has only one neutron
            test_zk=test_netk;   %activation
            if((test_zk>0&&test_target(1,i)==1)||(test_zk<0&&test_target(1,i)==-1))
                correct=correct+1;
            end
        end
        correct_rate=correct/size(test_data,2);
        break;
    end
end 