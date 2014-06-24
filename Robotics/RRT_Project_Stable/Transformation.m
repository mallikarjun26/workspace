function [T] = Transformation(dhvalues)
load(dhvalues);
dimensions=size(dhtable);
T=eye(4);
for i=1:dimensions(1)
Ai=HTMatrix(dhtable(i,:));
T=T*Ai;
end
end

function [A] = HTMatrix(values)
theta=values(1);
d=values(2);
a=values(3);
alpha=values(4);
A=[cos(theta) -sin(theta)*cos(alpha) sin(theta)*sin(alpha) a*cos(theta);sin(theta) cos(theta)*cos(alpha) -cos(theta)*sin(alpha) a*sin(theta);0 sin(alpha) cos(alpha) d;0 0 0 1];
end

