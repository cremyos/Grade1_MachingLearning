% ��ȡ����
%[ReadData,ReadLabels] = textread('TrainFile.txt','%f %d');
[ReadData,ReadLabels] = textread('TrainFile.txt');
[ReadDataHang, ReadDataLie] = size(ReadData);
ReadDataHangTrain = round(ReadDataHang * 0.8)

% ��ȡѵ������
TrainData = ReadData(1:ReadDataHangTrain, 1);
TrainLabels = ReadLabels(1:ReadDataHangTrain, 1);

% ��ȡ��������
TestData = ReadData(ReadDataHangTrain:ReadDataHang, 1);
TestLabels = ReadLabels(ReadDataHangTrain:ReadDataHang, 1);


%% training function and predict function
% ѵ�����ݣ�ռ80%
[W1, sigma1, C1] = RBF_training(TrainData, TrainLabels, 4);
y1 = RBF_predict(TrainData, W1, sigma1, C1);
Y_OutPut = round(y1);
% ѵ������׼ȷ�Է���
correctTrain = Y_OutPut - TrainLabels;

% ��������Ԫ�صĸ���
correctTrain_Count = nnz(correctTrain);
[hang, lie] = size(correctTrain);

% ��׼ȷ��
AccuracyTrain = 1 - correctTrain_Count/hang

[PrintHang,PrintLie] = size(TrainData);

%���������ļ���
fidout=fopen('OutPutTrain.txt','w');
% ����������У��Ѵ�������д���ļ�OutPut.txt
%fprintf(fidout, '%f %d || %d\n', TestData, TestLabels, yReal_OutPut);
for i = 1:PrintHang
    fprintf(fidout, '%f || %d\n', TrainData(i, PrintLie), Y_OutPut(i, PrintLie));
end
fclose(fidout);


yTest = RBF_predict(TestData, W1, sigma1, C1);
yReal_OutPut = round(yTest);
% ��������׼ȷ�Է���
correctTest = yReal_OutPut - TestLabels;

% ��������Ԫ�صĸ���
correctTest_Count = nnz(correctTest);
[hang, lie] = size(correctTest);

% ��׼ȷ��
AccuracyTest = 1 - correctTest_Count/hang

[PrintTestHang, PrintTestLie] = size(TestData);

%���������ļ���
fidout=fopen('OutPutTest.txt','w');
% ����������У��Ѵ�������д���ļ�OutPut.txt
%fprintf(fidout, '%f %d || %d\n', TestData, TestLabels, yReal_OutPut);
for i = 1:PrintTestHang
    fprintf(fidout, '%f || %d\n', TestData(i, PrintTestLie), yReal_OutPut(i, PrintTestLie));
end
fclose(fidout);

% ���
%���������ļ���
%fidout=fopen('OutPutWrong3.txt','w');
% ����������У��Ѵ�������д���ļ�OutPut.txt
%fprintf(fidout, '%f %d\n', TrainFile(1:37988, 1), TrainFile(1:37988, 2));
%fprintf(fidout, '%f %d\n', ReadData, ReadLabels);
%fclose(fidout);
