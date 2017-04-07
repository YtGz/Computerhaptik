load data.txt;
[n,p] = size(data);
t = 1:n;
t = t*0.0005;
data(:,2) = data(:,2)/1000;
plot(t,data(:,2)),
xlabel('Time'), ylabel('Force')
title('Spring');
