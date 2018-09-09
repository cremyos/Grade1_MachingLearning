% BP算法手写识别问题
img = imread(ldpath);
% 得到灰度图像
logitimg = img(:,:,2);  

% 获取二值图
logitimg(find(logitimg<200)) = 0;
logitimg(find(logitimg<200)) = 1;

% 提取图像区域
bw2 = edu_imgcrop(logitimg);
% 归一化提取特征
charvec = edu_imgresize(bw2);

P = out(:, 1:10*(numberoftrainingdata - 1));