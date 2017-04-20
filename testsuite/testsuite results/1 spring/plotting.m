load k1_3.txt;
[n,p] = size(k1_3);
t = 1:n;
t = t*0.0125;
k1_3(:,2) = k1_3(:,2)*10;
plot(t,k1_3),
xlabel('Time'), ylabel('Force')
title('Spring');
