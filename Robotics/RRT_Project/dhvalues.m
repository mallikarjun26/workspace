% %Stanford manipulator
% disp('setting DH values for stanford manipulator...')
% n=6;
% jtype=[1 1 0 1 1 1];
% %symbolic variables of DH table
% theta=sym('t',[1,n]);
% d=sym('d',[1,n]);
% a=sym('a',[1,n]);
% alpha=sym('alpha',[1,n]);
% %replacing symbolic variables with known values in DH table
% theta=[theta(1) theta(2) 0 theta(4) theta(5) theta(6)];
% d=[0 d(2) d(3) 0 0 d(6)];
% a=[0 0 0 0 0 0];
% alpha=[-90 90 0 -90 90 0];
% % cosalpha=cosd(alpha);
% % sinallpha=sind(alpha);
% disp('Done')
% %end stanford manipulator


%3-link RRP(spherical) manipulator
disp('setting DH values for 3-link RRP (spherical) manipulator...')
n=3;
jtype=[1 1 0];
%symbolic variables of DH table
theta=sym('t',[1,n]);
d=sym('d',[1,n]);
a=sym('a',[1,n]);
alpha=sym('alpha',[1,n]);
%replacing symbolic variables with known values in DH table
theta=[theta(1) theta(2) 0];
d=[0 0 d(3)];
a=[300 300 0];
alpha=[0 180 0];
disp('Done')
%end 3-link RRP manipulator

% %2-link planar manipulator
% disp('setting DH values for 2-link planar manipulator...')
% n=2;
% jtype=[0 0];
% %symbolic variables of DH table
% theta=sym('t',[1,n]);
% d=sym('d',[1,n]);
% a=sym('a',[1,n]);
% alpha=sym('alpha',[1,n]);
% %replacing symbolic variables with known values in DH table
% theta=[-90 -90];
% d=[d(1) d(2)];
% a=[0 0];
% alpha=[-90 0];
% disp('Done')
% %end 2-link planar manipulator


