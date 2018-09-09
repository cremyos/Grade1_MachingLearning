clear;
clc;

% ԭʼ����
GunZhu_data = importdata('FaultDiagnoseData\gunzhu.txt');
WaiQuan_data = importdata('FaultDiagnoseData\waiquan.txt');
Neiquan_data = importdata('FaultDiagnoseData\neiquan.txt');
Zhengchang_data = importdata('FaultDiagnoseData\zhengchang.txt');


% ���ƹ���ԭʼͼ��
figure(1)

subplot(2, 2, 1)
plot(GunZhu_data)
title('�������ԭʼ')

subplot(2, 2, 2)
plot(WaiQuan_data)
title('��Ȧ����ԭʼ')

subplot(2, 2, 3)
plot(Neiquan_data)
title('��Ȧ����ԭʼ')

subplot(2, 2, 4)
plot(Zhengchang_data)
title('��������ԭʼ')



% �����ݾ�ֵ
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


% ���ƹ���ȥ��ֵͼ��
figure(2)

% ȥ��ֵ��������
NewGunZhu_data = GunZhu_data - meansGZVec;
NewWaiQuan_data = WaiQuan_data - meansWQVec;
NewNeiQuan_data = Neiquan_data - meansNQVec;
NewZhengChang_data = Zhengchang_data - meansZCVec;

subplot(2, 2, 1)
plot(NewGunZhu_data)
title('�������ȥ��ֵͼ��')

subplot(2, 2, 2)
plot(NewWaiQuan_data)
title('��Ȧ����ȥ��ֵͼ��')

subplot(2, 2, 3)
plot(NewNeiQuan_data)
title('��Ȧ����ȥ��ֵͼ��')

subplot(2, 2, 4)
plot(NewZhengChang_data)
title('��������ȥ��ֵͼ��')

% ���źŽ���С�����ֽ⣬�õ��ڵ��С����ϵ����Ȼ���ÿ���ڵ�ϵ�������ع���
wpt = wavelet_packetdecomposition_reconstruct(NewGunZhu_data, 4, 'db1')
E1 = wavelet_energy_spectrum(wpt, 4)

wpt2 = wavelet_packetdecomposition_reconstruct(NewWaiQuan_data, 4, 'db1')
E2 = wavelet_energy_spectrum(wpt2, 4)

wpt3 = wavelet_packetdecomposition_reconstruct(NewNeiQuan_data, 4, 'db1')
E3 = wavelet_energy_spectrum(wpt3, 4)

wpt4 = wavelet_packetdecomposition_reconstruct(NewZhengChang_data, 4, 'db1')
E4 = wavelet_energy_spectrum(wpt4, 4)








