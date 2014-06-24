addpath 'C:\Users\anirudh\Documents\MATLAB\matgraph';
links_c=3;
q_init=[45 0 270]'; %initial configuration
q_f=[60 90 120]'; %final configuration
q_c=q_init; %current configuration
tolerance=[5 5 10]'; % tolerance allowed from final configration to stop exploration
step=[5 5 5]';
T=[q_init;0]; %Tree of explored nodes
i=0;
while(sum(abs(q_c-q_f)>tolerance)>0)
    %abs(q_c-q_f)
    i=i+1;
    if(mod(i,100)==0)
        q_rand=q_f;
    else
        q_rand=[rand*360 rand*360 rand*400]';
    end
    %q_rand=[rand*360 rand*360]';
    [index,q_near]=nearest_node(T(1:links_c,:),q_rand);
    q_new=new_node(q_near,q_rand,step);
    q_c=q_new;
    if(not(exists(T(1:links_c,:),q_new)))
        T=[T [q_new;index]];
    end
    %plot3(q_new(1),q_new(2),q_new(3))
%     i=i+1;
%     if(i>10)
%         break
%     end
%     T=[T q_new];
end
path=find_path(T,links_c)
%plot(T(1,:),T(2,:),'+');
plot3(T(1,:),T(2,:),T(3,:),'+');
hold on;
plot3(q_f(1),q_f(2),q_f(3),'+r');
axis([0 360 0 360 0 400]);