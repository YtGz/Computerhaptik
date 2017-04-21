load b0_03__w1_2.txt;
[n,p] = size(b0_03__w1_2);
t = 1:n;
t = t*0.0125;
b0_03__w1_2(:,2) = b0_03__w1_2(:,2)*10;
plot(t,b0_03__w1_2),
xlabel('Time'), ylabel('Force')
title('Spring');
