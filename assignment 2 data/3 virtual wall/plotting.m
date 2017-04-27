load k0_3.txt;
[n,p] = size(k0_3);
t = 1:n;
t = t*0.05*0.01;
k0_3(:,2) = k0_3(:,2)*10;
plot(t,k0_3),
xlabel('Time [s]'), ylabel('Force [J/dm] // Position [mm]')
title('3 Virtual Wall (k = 0.3)'),
legend('Position [mm]', 'Force [J/cm]');
saveas(gcf, '3_virtual_wall_k0_3', 'svg');
