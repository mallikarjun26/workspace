function [noOfNodes] = rrt (step, goldenAngle)

% addpath 'C:\Users\anirudh\Documents\MATLAB\matgraph';
links_c=3;
q_init=[45 0 270]'; %initial configuration
q_f=[90 50 -120]'; %final configuration
obstacle = [0 2.5 6 2]';
q_c=q_init; %current configuration
tolerance=[5 5 10]'; % tolerance allowed from final configration to stop exploration
%step=[5 5 5]';
T=[q_init;0]; %Tree of explored nodes
%p_init=getPositions(q_init);
%P=[p_init;0];

i=0;

while(sum(abs(q_c-q_f)>tolerance)>0)
    %abs(q_c-q_f)
    i=i+1;
    if(mod(i,goldenAngle)==0)
        q_rand=q_f;
    else
        q_rand=[rand*360 rand*360 rand*400]';
    end
    %q_rand=[rand*360 rand*360]';
    [index,q_near]=nearest_node(T(1:links_c,:),q_rand);
    [q_new, bSuccessful, posVector]=new_node(q_near, q_rand, step, obstacle);
    
    if(bSuccessful)
        q_c=q_new;
        if(not(exists(T(1:links_c,:),q_new)))
            
            T=[T [q_new;index]];
            %P =[P [posVector;index]];
        end
    end
    %plot3(q_new(1),q_new(2),q_new(3))
    %     i=i+1;
    %     if(i>10)
    %         break
    %     end
    %     T=[T q_new];
end
%%plot the robot given a configuration
[path, routeWorkSpace]=find_path(T,links_c);

noOfNodes = size(path,2);
%disp(['For Step Size:' num2str(step) ' Number of nodes:' num2str(size(path,2))]);

clf;
figure(1)


%%plot the robot configurations in 3d space as an rrt
%%
subplot(1,2,1);
plot(T(1,:),T(2,:),'+');
plot3(T(1,:),T(2,:),T(3,:),'+');
hold on;

%  for i=1:size(path,2)
%     plot3( path(1,i),path(2,i),path(3,i),'*g');
%     pause(0.5);
%  end

plot3(path(1,:),path(2,:),path(3,:),'*g');
plot3(q_f(1),q_f(2),q_f(3),'+r');

%%


% A = getHTMatrices(q_init);
% H20 = (A(:,:,1) * A(:,:,2));
% H30 = (A(:,:,1) * A(:,:,2) * A(:,:,3));
% pt0 = [0 0 0]';
% pt1 = A(1:3,4,1);
% pt2 = H20(1:3,4);
% pt3 = H30(1:3,4);
% orgs = [pt0 pt1 pt2 pt3];

%sphere(3, [4, 4 ,4]);
%sphere(obstacle(4,1), obstacle(1:3,1));

% [x y z]=sphere(obstacle(4));
% surf(x+obstacle(1),y+obstacle(2),x+obstacle(3));
%hold;
%P(:,:,:) = find(P(:,:,:));

subplot(1,2,2);
pts = plot_robot(routeWorkSpace,path, obstacle);

% %plot3(path(1),path(2),path(3),'+b');
% axis([0 360 0 360 0 400]);

end
