function E = wavelet_energy_spectrum( wpt,n )
%% 计算每一层每一个节点的能量
%  wpt-wavelet packet tree
%  n-第n层能量
% 
%%
% 求第n层第i个节点的系数
E(1:2^n )=0;
for i=1:2^n 
E(i) = norm(wpcoef(wpt,[n,i-1]),2); %求第i个节点的范数平方，其实也就是平方和
end
%求每个节点的概率
E_total=sum(E); 
for i=1:2^n
p_node(i)= 100*E(i)/E_total;
end
% E = wenergy(wpt); only get the last layer
figure;
x=1:2^n;
bar(x,p_node);
title(['第',num2str(n),'层']);
axis([0 2^n 0 100]);
xlabel('结点');
ylabel('能量百分比/%');
for j=1:2^n
text(x(j),p_node(i),num2str(p_node(j),'%0.2f'),...
    'HorizontalAlignment','center',...
    'VerticalAlignment','bottom')
end

end

