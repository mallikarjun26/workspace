addpath 'C:\Users\anirudh\Documents\MATLAB\matgraph';
q_init=[0 0 0]'; %initial configuration
q_f=[10 30 20]'; %final configuration
q_c=q_init; %current configuration
tolerance=[5 5 10]'; % tolerance allowed from final configration to stop exploration
step=[10 10 25]';
T=[q_init]; %Tree of explored nodes
rrt_g=graph();
while(sum(abs(q_c-q_f)>tolerance)>0)
    %abs(q_c-q_f)
    q_rand=[rand*360 rand*360 rand*400]';
    %q_rand=[rand*360 rand*360]';
    q_near=nearest_node(T,q_rand);
    q_new=new_node(q_near,q_rand,step);
    q_c=q_new;
    if(not(exists(T,q_new)))
        T=[T q_new];
    end
    %plot3(q_new(1),q_new(2),q_new(3))
%     i=i+1;
%     if(i>10)
%         break
%     end
%     T=[T q_new];
end
%plot(T(1,:),T(2,:),'+');
plot3(T(1,:),T(2,:),T(3,:),'+');
hold on;
plot3(q_f(1),q_f(2),q_f(3),'+r');
axis([0 360 0 360 0 400]);