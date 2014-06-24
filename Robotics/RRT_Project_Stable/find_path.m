function [path, routeWorkSpace] = find_path(T,links_c, P)
    i=1;
    
    routeWorkSpace=[];
    path=[];
    temp=size(T);
    index_row=temp(1); %row number where indexing is stored for each configurations parent
    index=temp(2);
    while(index~=0)
        path=[T(1:links_c,index) path];
        %disp(index);
        temp = getPositions(T(1:links_c,index));
        routeWorkSpace = [temp routeWorkSpace];
        index=T(index_row,index);
        i=i+1;
    end
end

