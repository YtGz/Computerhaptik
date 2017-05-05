load k0_3__A2_4__d1__f181.txt;
[n,p,q,r] = size(k0_3__A2_4__d1__f181);
t = 1:n;
t = t*0.05*0.01;
k0_3__A2_4__d1__f181(:,2) = k0_3__A2_4__d1__f181(:,2)*100;
k0_3__A2_4__d1__f181(:,3) = k0_3__A2_4__d1__f181(:,3)*100;
k0_3__A2_4__d1__f181(:,4) = k0_3__A2_4__d1__f181(:,4)*100;
plot(t,k0_3__A2_4__d1__f181),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('4 Hard Surface (k = 0.03, A = 0.2, d = 2, f = 24Hz)'),
legend('Position [mm]', 'Spring', 'Decaying Sinusoid', 'Combined Force [J/cm]');
saveas(gcf, '4_hard_surface_k0_3__A2_4__d1__f181', 'svg');

load k0_8__A2_4__d2__f242.txt;
[n,p,q,r] = size(k0_8__A2_4__d2__f242);
t = 1:n;
t = t*0.05*0.01;
k0_8__A2_4__d2__f242(:,2) = k0_8__A2_4__d2__f242(:,2)*100;
k0_8__A2_4__d2__f242(:,3) = k0_8__A2_4__d2__f242(:,3)*100;
k0_8__A2_4__d2__f242(:,4) = k0_8__A2_4__d2__f242(:,4)*100;
plot(t,k0_8__A2_4__d2__f242),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('4 Hard Surface (k = 0.8, A = 2.4, d = 2, f = 242Hz)'),
legend('Position [mm]', 'Spring', 'Decaying Sinusoid', 'Combined Force [J/cm]');
saveas(gcf, '4_hard_surface_k0_8__A2_4__d2__f242', 'svg');

load k0_8__A2_4__d2__f242_v2.txt;
[n,p,q,r] = size(k0_8__A2_4__d2__f242_v2);
t = 1:n;
t = t*0.05*0.01;
k0_8__A2_4__d2__f242_v2(:,2) = k0_8__A2_4__d2__f242_v2(:,2)*100;
k0_8__A2_4__d2__f242_v2(:,3) = k0_8__A2_4__d2__f242_v2(:,3)*100;
k0_8__A2_4__d2__f242_v2(:,4) = k0_8__A2_4__d2__f242_v2(:,4)*100;
plot(t,k0_8__A2_4__d2__f242_v2),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('4 Hard Surface (k = 0.8, A = 2.4, d = 2, f = 242Hz)'),
legend('Position [mm]', 'Spring', 'Decaying Sinusoid', 'Combined Force [J/cm]');
saveas(gcf, '4_hard_surface_k0_8__A2_4__d2__f242_v2', 'svg');