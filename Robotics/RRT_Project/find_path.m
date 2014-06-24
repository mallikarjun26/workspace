function [path] = find_path(T,links_c)
    path=[];
    temp=size(T);
    index_row=temp(1); %row number where indexing is stored for each configurations parent
    index=temp(2);
    while(index~=0)
        path=[T(1:links_c,index) path];
        index=T(index_row,index);
    end
end

