step = [];
noOfNodesForStepVariation =[];
noOfNodesForGoldenAngleVariation = [];
stepRatio = [0.4 0.6 0.8];
for i = 1:4
    step = [(i+1)*stepRatio(1) (i+1)*stepRatio(2) (i+1)*stepRatio(3)];
    goldenAngle = 10;
    temp_nodes = rrt(step, goldenAngle);
    noOfNodesForStepVariation = [ noOfNodesForStepVariation [step'; ] ];
    disp(['For Step Size:' num2str(step) ' Number of nodes:' temp_nodes]);
end


for i = 1:5
    step = [5*stepRatio(1) 5*stepRatio(2) 5*stepRatio(3)];
    goldenAngle = 10*i;
    noOfNodesForGoldenAngleVariation = [ noOfNodesForGoldenAngleVariation [step'; rrt(step, goldenAngle)] ];
    disp(['For goldenAngleRatio:' num2str(goldenAngle) ' For Step Size:' num2str(step) ' Number of nodes:' temp_nodes]);
end
% 
% noOfNodesForStepVariation
% noOfNodesForGoldenAngleVariation