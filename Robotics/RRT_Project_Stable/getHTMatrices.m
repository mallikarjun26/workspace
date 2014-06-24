%function [A] = getHTMatrices(theta,d,a,alpha)
function [A] = getHTMatrices(theta)

%disp('getting HT matrices for 3-link RRR manipulator...')
n=3;
%theta=[90 45 30];
d=[8 0 0];
a=[0 6 6];
alpha=[90 0 -90];

for i = 1:n
    A(:,:,i) =[cosd(theta(i))       -sind(theta(i))*cosd(alpha(i))      sind(theta(i))*sind(alpha(i))         a(i)*cosd(theta(i));
               sind(theta(i))        cosd(theta(i))*cosd(alpha(i))      -cosd(theta(i))*sind(alpha(i))        a(i)*sind(theta(i));
                    0                       sind(alpha(i))                   cosd(alpha(i))                       d(i);
                    0                            0                               0                                1          ];
    
end

%disp('Done : ')
end