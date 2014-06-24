function [ q_new ] = new_node( q_near,q_rand,step )
%NEW_NODE : Creates new configuration in the direction of q_near
config_diff=q_rand-q_near;
q_new=zeros(size(q_rand));
for i=1:size(q_rand)
   if(config_diff(i)<0)
       q_new(i)=q_near(i)-step(i);
   elseif(config_diff(i)>0)
       q_new(i)=q_near(i)+step(i);
   else
       q_new(i)=q_near(i);
   end
end
end

