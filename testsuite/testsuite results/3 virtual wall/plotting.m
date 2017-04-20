load k0_03.txt;
[n,p] = size(k0_03);
t = 1:n;
t = t*0.0125;
k0_03(:,2) = k0_03(:,2)*10;
plot(t,k0_03),
xlabel('Time'), ylabel('Force')
title('Spring');
