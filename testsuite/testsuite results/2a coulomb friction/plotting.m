load b0_7.txt;
[n,p] = size(b0_7);
t = 1:n;
t = t*0.0125;
b0_7(:,3) = b0_7(:,3)*10;
plot(t,b0_7),
xlabel('Time'), ylabel('Force')
title('Spring');
