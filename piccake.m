clc
clear
img=imread('F:\Embedded Development\Software\STM32 Project\Base MKD Keil\Lisaru\image\dx.jpg');   %��ȡ��һ��ͼƬ
i = imresize(img,[1024,1024]);
[Height,Width]=size(i);                 %��ȡԭͼ��ĸ߶ȺͿ��
T1=affine2d([-1 0 0;0 1 0;Width 0 1]);  %����ռ�任�ṹT1������Ϊˮƽ����任����
i=imwarp(i,T1);                         %��ԭͼ��I����ˮƽ����任  
thresh = graythresh(i);                 %�Զ�ȷ����ֵ����ֵ
i2 = im2bw(i,thresh);                   %��ͼ���ֵ��

% figure(1)
% imshow(i)     %��ʾ��ֵ��֮ǰ��ͼƬ
% figure(2)
% imshow(I2)    %��ʾ��ֵ��֮���ͼƬ



i3 =~i2;
figure(3)
PicEdge6=edge(i3,'roberts');
PicEdge7=edge(PicEdge6,'roberts');
imshow(PicEdge7)

[m,n] =find(PicEdge7==1);
m = m*4;
n= n*4;
m = 4096-m;
n= 4096-n;
fid = fopen('y.txt','wt');
fprintf(fid,'%g,',m);       
fclose(fid)
fid = fopen('x.txt','wt');
fprintf(fid,'%g,',n);       
fclose(fid)