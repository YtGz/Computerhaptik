load b0_2__w1_2_v2.txt;
[n,p] = size(b0_2__w1_2_v2);
t = 1:n;
t = t*0.05*0.01;
b0_2__w1_2_v2(:,2) = b0_2__w1_2_v2(:,2)*100;
plot(t,b0_2__w1_2_v2),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('5 Textures (b = 0.2, w = 1.2mm)'),
legend('Position [mm]','Force [J/cm]');
saveas(gcf, '5_textures_b0_2__w1_2_v2', 'svg');

load b0_2__w1_2.txt;
[n,p] = size(b0_2__w1_2);
t = 1:n;
t = t*0.05*0.01;
b0_2__w1_2(:,2) = b0_2__w1_2(:,2)*100;
plot(t,b0_2__w1_2),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('5 Textures (b = 0.2, w = 1.2mm)'),
legend('Position [mm]','Force [J/cm]');
saveas(gcf, '5_textures_b0_2__w1_2', 'svg');

load b0_03__w1_2.txt;
[n,p] = size(b0_03__w1_2);
t = 1:n;
t = t*0.05*0.01;
b0_03__w1_2(:,2) = b0_03__w1_2(:,2)*100;
plot(t,b0_03__w1_2),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('5 Textures (b = 0.03, w = 1.2mm)'),
legend('Position [mm]','Force [J/cm]');
saveas(gcf, '5_textures_b0_03__w1_2', 'svg');

load b0_05__w0_8.txt;
[n,p] = size(b0_05__w0_8);
t = 1:n;
t = t*0.05*0.01;
b0_05__w0_8(:,2) = b0_05__w0_8(:,2)*100;
plot(t,b0_05__w0_8),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('5 Textures (b = 0.05, w = 0.8mm)'),
legend('Position [mm]','Force [J/cm]');
saveas(gcf, '5_textures_b0_05__w0_8', 'svg');

load b0_7__w0_8.txt;
[n,p] = size(b0_7__w0_8);
t = 1:n;
t = t*0.05*0.01;
b0_7__w0_8(:,2) = b0_7__w0_8(:,2)*100;
plot(t,b0_7__w0_8),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('5 Textures (b = 0.7, w = 0.8mm)'),
legend('Position [mm]','Force [J/cm]');
saveas(gcf, '5_textures_b0_7__w0_8', 'svg');

load b0_7__w0_8_v2.txt;
[n,p] = size(b0_7__w0_8_v2);
t = 1:n;
t = t*0.05*0.01;
b0_7__w0_8_v2(:,2) = b0_7__w0_8_v2(:,2)*100;
plot(t,b0_7__w0_8_v2),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('5 Textures (b = 0.7, w = 0.8mm)'),
legend('Position [mm]','Force [J/cm]');
saveas(gcf, '5_textures_b0_7__w0_8_v2', 'svg');

load b0_7__w2.txt;
[n,p] = size(b0_7__w2);
t = 1:n;
t = t*0.05*0.01;
b0_7__w2(:,2) = b0_7__w2(:,2)*100;
plot(t,b0_7__w2),
xlabel('Time [s]'), ylabel('Force [J/cm] // Position [mm]')
title('5 Textures (b = 0.7, w = 2mm)'),
legend('Position [mm]','Force [J/cm]');
saveas(gcf, '5_textures_b0_7__w2', 'svg');