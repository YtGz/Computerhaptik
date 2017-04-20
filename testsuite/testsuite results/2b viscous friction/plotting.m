load b0_03.txt;
[n,p] = size(b0_03);
t = 1:n;
t = t*0.0125;
b0_03(:,3) = b0_03(:,3)*10;
plot(t,b0_03),
xlabel('Time'), ylabel('Force')
title('Spring');
