function [ p_init ] = getPositions( q_init )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

A = getHTMatrices(q_init);
H20 = (A(:,:,1) * A(:,:,2));
H30 = (A(:,:,1) * A(:,:,2) * A(:,:,3));
pt0 = [0 0 0]';
pt1 = A(1:3,4,1);
pt2 = H20(1:3,4);
pt3 = H30(1:3,4);

p_init(:,:) = [pt0 ;pt1 ;pt2 ;pt3];


end

