function [flag]=exists(T,q_new)
temp=size(T);
no_of_configs=temp(2);
flag=0;
for i =1:no_of_configs
    if(sum(T(:,i)==q_new)==no_of_configs)
%         disp('found');
%         T(:,i)
%         q_new
        flag=1;
        return;
    end
end