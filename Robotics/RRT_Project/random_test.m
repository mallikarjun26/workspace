% temp=size(T);
% n=temp(2);
% a=[];
% for i=1:n
%     if((T(1,i)>=5 && T(1,i)<=15)&&(T(2,i)>=335 && T(2,i)<=3455))
%         a=[a T(:,i)];
%     end
% end
% size(a)


% a1=ceil(360*rand(100000,1));
% temp=size(a1);
% n2=temp(1);
% f1=zeros(360,1);
% for i=1:n2
%     f1(a1(i))=f1(a1(i))+1;
% end
% p=f./10000;
% plot(1:360,p);
% hold;
% axis([0 360 0 0.05]);
% sum(p)

trials=10000;
f1=zeros(360,1);
f2=zeros(360,1);
for i=1:trials
    r1=ceil(360*rand);
    r2=ceil(360*rand);
    f1(r1)=f1(r1)+1;
    f2(r2)=f2(r2)+1;
end
p1=f1./trials;
p2=f2./trials;
plot([1:360]',p1,'b');
hold on;
plot([1:360],p2,'r');
axis([0 360 0 0.05]);

    
