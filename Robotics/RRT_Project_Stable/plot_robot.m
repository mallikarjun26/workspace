function [pts] = plot_robot( routeWorkSpace, path, obstacle )
    %plot3(orgs(1,:),orgs(2,:),orgs(3,:));
    disp(size(routeWorkSpace,2))
    [x,y,z] = sphere();
    r = obstacle(4,1);
    surf( r*x+(obstacle(1,1)), r*y+(obstacle(2,1)), r*z+(obstacle(3,1)));
    
    for i=1:size(routeWorkSpace,2)
%         clf;
        P1=routeWorkSpace(1:3,i)';
        P2=routeWorkSpace(4:6,i)';
        P3=routeWorkSpace(7:9,i)';
        P4=routeWorkSpace(10:12,i)';
        pts=[P1;P2;P3;P4]; 
        if(i==size(routeWorkSpace,2))
            line(pts(1:2,1), pts(1:2,2), pts(1:2,3), 'Color',[1,0.4,0.6]);       
            line(pts(2:3,1), pts(2:3,2), pts(2:3,3), 'Color',[1,0.4,0.6]);       
            line(pts(3:4,1), pts(3:4,2), pts(3:4,3), 'Color',[1,0.4,0.6]);
            %disp(P4);
            %disp(path(:,size(path,2)));
        else
            line(pts(1:2,1), pts(1:2,2), pts(1:2,3), 'Color',[1,0.1,0.3]);       
            line(pts(2:3,1), pts(2:3,2), pts(2:3,3), 'Color',[0.2,0.1,0.6]);       
            line(pts(3:4,1), pts(3:4,2), pts(3:4,3), 'Color',[0.4,0.1,0.9]);
%             disp(P1);
%             disp(P2);
%             disp(P3);
%             disp(P4);
            %pause(.5)
        end
        %disp(i)
        hold off;
        pause(.1)
    end
    
   
    
    
end

