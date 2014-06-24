function [index,q_near] = nearest_node( T,q_rand )
%NEAREST_NODE : finds the closest configuration in the currently explored
%tree to the given random configuration
temp=size(T);
no_of_nodes=temp(2);
q_near=T(:,1);
index=1;
dist_low=norm(q_rand-T(:,1));
for i=2:no_of_nodes
    dist_c=norm(q_rand-T(:,i));
    if(dist_c<dist_low)
        q_near=T(:,i);
        dist_low=dist_c;
        index=i;
    end
end
end

