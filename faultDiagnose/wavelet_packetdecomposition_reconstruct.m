function wpt= wavelet_packetdecomposition_reconstruct( x,n,wpname )
%% 对信号进行小波包分解，得到节点的小波包系数。然后对每个节点系数进行重构。 
% Decompose x at depth n with wpname wavelet packets.using Shannon entropy.
%   
%  x-input signal 
%  n-the number of decomposition layers
%  wpname-a particular wavelet.type:string.
%

%%
wpt=wpdec(x,n,wpname);
% wpt=wpdec(x,n,'shannon');
% Plot wavelet packet tree (binary tree)
plot(wpt)
%% wavelet packet coefficients.default：use the front 4.
cfs0=wpcoef(wpt,[n 0]);
cfs1=wpcoef(wpt,[n 1]);
cfs2=wpcoef(wpt,[n 2]);
cfs3=wpcoef(wpt,[n 3]);
figure;
subplot(5,1,1);
plot(x);
title('原始信号');
subplot(5,1,2);
plot(cfs0);
title(['结点 ',num2str(n) '  1',' 系数'])
subplot(5,1,3);
plot(cfs1);
title(['结点 ',num2str(n) '  2',' 系数'])
subplot(5,1,4);
plot(cfs2);
title(['结点 ',num2str(n) '  3',' 系数'])
subplot(5,1,5);
plot(cfs3);
title(['结点 ',num2str(n) '  4',' 系数'])

%% reconstruct wavelet packet coefficients.
rex0=wprcoef(wpt,[n 0]);
rex1=wprcoef(wpt,[n 1]);
rex2=wprcoef(wpt,[n 2]);
rex3=wprcoef(wpt,[n 3]);
figure;
subplot(5,1,1);
plot(x);
title('原始信号');
subplot(5,1,2);
plot(rex0);
title(['重构结点 ',num2str(n) '  1',' 系数'])
subplot(5,1,3);
plot(rex1);
title(['重构结点 ',num2str(n) '  2',' 系数'])
subplot(5,1,4);
plot(rex2);
title(['重构结点 ',num2str(n) '  3',' 系数'])
subplot(5,1,5);
plot(rex3);
title(['重构结点 ',num2str(n) '  4',' 系数'])


end

