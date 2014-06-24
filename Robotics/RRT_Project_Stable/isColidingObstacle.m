function bColiding = isColidingObstacle(q_new, obstacle)
    bColiding = false;
    
    theta = q_new';
    A = getHTMatrices(theta);
    
    H20 = (A(:,:,1) * A(:,:,2));
    H30 = (A(:,:,1) * A(:,:,2) * A(:,:,3));
    
    pt0 = [0 0 0]';
    pt1 = A(1:3,4,1);
    pt2 = H20(1:3,4);
    pt3 = H30(1:3,4);
    
    setOfPoints = [];
    vector_pt0_pt1 = pt1 - pt0;
    vector_pt0_pt1_length = (sqrt(vector_pt0_pt1(1)^2 + vector_pt0_pt1(2)^2 + vector_pt0_pt1(3)^2 )); 
    vector_pt0_pt1 = vector_pt0_pt1 / vector_pt0_pt1_length;
    
    vector_pt1_pt2 = pt2 - pt1;
    vector_pt1_pt2_length = (sqrt(vector_pt1_pt2(1)^2 + vector_pt1_pt2(2)^2 + vector_pt1_pt2(3)^2 ));
    vector_pt1_pt2 = vector_pt1_pt2 / vector_pt1_pt2_length;
    
    vector_pt2_pt3 = pt3 - pt2;
    vector_pt2_pt3_length = (sqrt(vector_pt2_pt3(1)^2 + vector_pt2_pt3(2)^2 + vector_pt2_pt3(3)^2 ));
    vector_pt2_pt3 = vector_pt2_pt3 / vector_pt2_pt3_length;
    
    for i=1:5
       setOfPoints = [setOfPoints pt0 + ( ((vector_pt0_pt1_length/4)*i) * vector_pt0_pt1) ];
       setOfPoints = [setOfPoints pt1 + ( ((vector_pt1_pt2_length/4)*i) * vector_pt1_pt2) ];
       setOfPoints = [setOfPoints pt2 + ( ((vector_pt2_pt3_length/4)*i) * vector_pt2_pt3) ];
    end
    
    noOfPoints = size(setOfPoints);
    noOfPoints = noOfPoints(2);
%     disp('noOfPoints:');
%     disp(noOfPoints);
    
    for i=1:noOfPoints
        distance_temp = sqrt( (setOfPoints(1,i) - obstacle(1,1))^2 + (setOfPoints(2,i) - obstacle(2,1))^2 + (setOfPoints(3,i) - obstacle(3,1))^2);
        if( distance_temp <= obstacle(4,1) )
           bColiding = true;
           %disp('Cool : I like it!!!! ')
           break;
        end
    end
    
end
