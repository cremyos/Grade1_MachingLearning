% BP�㷨��дʶ������
img = imread(ldpath);
% �õ��Ҷ�ͼ��
logitimg = img(:,:,2);  

% ��ȡ��ֵͼ
logitimg(find(logitimg<200)) = 0;
logitimg(find(logitimg<200)) = 1;

% ��ȡͼ������
bw2 = edu_imgcrop(logitimg);
% ��һ����ȡ����
charvec = edu_imgresize(bw2);

P = out(:, 1:10*(numberoftrainingdata - 1));