%����Ƶת��Ϊ����ͼƬ
clear
clc

file_name = 'G:\ѧ���о�\DJI_0076.mov';        %��Ƶ�����ļ���  % win7���޷�������û�ж�Ӧ�ı������

obj = VideoReader(file_name);     %��ȡ��Ƶ�ļ�

numFrames = obj.NumberOfFrames;   %��Ƶ�ܵ�֡�� 
for k = 1: numFrames
    frame = read(obj,k);
    %imshow(frame);                
    %gray_frame = rgb2gray(frame); %��ÿһ֡Ϊ��ɫͼƬ��ת��Ϊ�Ҷ�ͼ
    imshow(frame);                %��ʾÿһ֡ͼƬ
    %����ÿһ֡ͼƬ
    imwrite(gray_frame,strcat('G:\ѧ���о�\DJI_0076\',num2str(k),'.jpg'),'jpg');
end

