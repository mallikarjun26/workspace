function [ q_new, bSuccessful, posVector ] = new_node( q_near, q_rand, step, obstacle )
    %NEW_NODE : Creates new configuration in the direction of q_near
    bSuccessful = true;
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
    
    posVector = getPositions(q_new);

    %Checking if the q_new is lies in the obstacle area. If yes, will return the same thing.
    if(isColidingObstacle(q_new, obstacle))
        q_new = q_near;
        bSuccessful = false;
    end
    
end

