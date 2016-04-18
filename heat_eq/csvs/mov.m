clear;

beg=0.00;
inc=0.02;
en=1.50

for t=beg:inc:en
    A(int32(t/inc + 1), :, :)=csvread(strcat('./term', num2str(t, '%1.2f')))
end
x=A(1, :, 1)
for i=1:int32(en/inc + 1)
    y(i, :)=A(i, :, 2)
end

obj=VideoWriter('heat.avi');
obj.FrameRate=25
%obj.FrameCount=31
open(obj)

for k=1:int32(en/inc + 1)
    plot(x, y(k, :));
    axis([0 1 0 1.01E+9])
    frame=getframe;
    writeVideo(obj, frame)
end
obj
close(obj)