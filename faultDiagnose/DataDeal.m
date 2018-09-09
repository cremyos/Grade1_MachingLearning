clear;
clc;

% 原始数据
GunZhu_data = importdata('FaultDiagnoseData\gunzhu.txt');
WaiQuan_data = importdata('FaultDiagnoseData\waiquan.txt');
Neiquan_data = importdata('FaultDiagnoseData\neiquan.txt');
Zhengchang_data = importdata('FaultDiagnoseData\zhengchang.txt');


% 绘制滚珠原始图像
figure(1)

subplot(2, 2, 1)
plot(GunZhu_data)
title('滚珠故障原始')

subplot(2, 2, 2)
plot(WaiQuan_data)
title('外圈故障原始')

subplot(2, 2, 3)
plot(Neiquan_data)
title('内圈故障原始')

subplot(2, 2, 4)
plot(Zhengchang_data)
title('正常数据原始')



% 求数据均值
[lineGZ, rowGZ] = size(GunZhu_data);
meansGZ = mean(GunZhu_data);
meansGZVec = meansGZ * ones(lineGZ, 1);

[lineWQ, rowWQ] = size(WaiQuan_data);
meansWQ = mean(WaiQuan_data);
meansWQVec = meansWQ * ones(lineWQ, 1);

[lineNQ, rowNQ] = size(Neiquan_data);
meansNQ = mean(Neiquan_data);
meansNQVec = meansNQ * ones(lineNQ, 1);

[lineZC, rowZC] = size(Zhengchang_data);
meansZC = mean(Zhengchang_data);
meansZCVec = meansZC * ones(lineZC, 1);

var_data = var(GunZhu_data)


% 绘制滚珠去均值图像
figure(2)

% 去均值化的数据
NewGunZhu_data = GunZhu_data - meansGZVec;
NewWaiQuan_data = WaiQuan_data - meansWQVec;
NewNeiQuan_data = Neiquan_data - meansNQVec;
NewZhengChang_data = Zhengchang_data - meansZCVec;

subplot(2, 2, 1)
plot(NewGunZhu_data)
title('滚珠故障去均值图像')

subplot(2, 2, 2)
plot(NewWaiQuan_data)
title('外圈故障去均值图像')

subplot(2, 2, 3)
plot(NewNeiQuan_data)
title('内圈故障去均值图像')

subplot(2, 2, 4)
plot(NewZhengChang_data)
title('正常数据去均值图像')

% 对信号进行小波包分解，得到节点的小波包系数。然后对每个节点系数进行重构。
wpt = wavelet_packetdecomposition_reconstruct(NewGunZhu_data, 4, 'db1')
E1 = wavelet_energy_spectrum(wpt, 4)

wpt2 = wavelet_packetdecomposition_reconstruct(NewWaiQuan_data, 4, 'db1')
E2 = wavelet_energy_spectrum(wpt2, 4)

wpt3 = wavelet_packetdecomposition_reconstruct(NewNeiQuan_data, 4, 'db1')
E3 = wavelet_energy_spectrum(wpt3, 4)

wpt4 = wavelet_packetdecomposition_reconstruct(NewZhengChang_data, 4, 'db1')
E4 = wavelet_energy_spectrum(wpt4, 4)








