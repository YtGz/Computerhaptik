load Spring_k0_03.txt;
[n,p] = size(Spring_k0_03);
t = 1:n;
t = t*0.0005;
Spring_k0_03(:,2) = Spring_k0_03(:,2)*10;
plot(t,Spring_k0_03),
xlabel('Time'), ylabel('Force')
title('Spring');
