%%
%% Add SIFT binary to path
addpath('siftDemoV4');
%%

%%
%% load images and match files for the first example
%%

% I1 = rgb2gray(imread('FMatrixImagePairs/library1.jpg'));
% I2 = rgb2gray(imread('FMatrixImagePairs/library2.jpg'));

% I1 = rgb2gray(imread('FMatrixImagePairs/house1.jpg'));
% I2 = rgb2gray(imread('FMatrixImagePairs/house2.jpg'));

I1 = rgb2gray(imread('FMatrixImagePairs/Himalaya1.jpg'));
I2 = rgb2gray(imread('FMatrixImagePairs/Himalaya2.jpg'));

[~, D1, F1] = sift(I1);
[~, D2, F2] = sift(I2);
match_idx = match(F1,D1,F2,D2);


N = size(match_idx,1);

x1 = F1(match_idx(:,1),1:2);
x2 = F2(match_idx(:,2),1:2);

%%
%% display two images side-by-side with matches
%% this code is to help you visualize the matches, you don't need
%% to use it to produce the results for the assignment
%%
disp('Debug 0');
imshow([I1 I2]); hold on;
plot(x1(:,1), x1(:,2), '+r');
plot(x2(:,1)+size(I1,2), x2(:,2), '+r');
line([x1(:,1) x2(:,1) + size(I1,2)]', [x1(:,2) x2(:,2)]', 'Color', 'r');
disp('Debug 1');
%uiwait;

disp('Debug 2');
x1(:,3) = 1;
x2(:,3) = 1;



% First, fit fundamental matrix to the matches
[F e1 e2] = fundmatrix(x1',x2'); %This is given
%F = my_fmatrix_ransac(x1',x2'); % YOU WRITE THIS
disp('Calling ransac to find the Fundamental matrix');
F_ransac = ransacfitfundmatrix(x1',x2', 0.01); % YOU WRITE THIS




%%
%% display second image with epipolar lines reprojected 
%% from the first image
%%
L = x1 * F; % transform points from 
% the first image to get epipolar lines in the second image

% find points on epipolar lines L closest to matches(:,3:4)
L = L ./ repmat(sqrt(L(:,1).^2 + L(:,2).^2), 1, 3); % rescale the line
pt_line_dist = sum(L .* x2,2);
closest_pt = x2(:,1:2) - L(:,1:2) .* repmat(pt_line_dist, 1, 2);

% find endpoints of segment on epipolar line (for display purposes)
pt1 = closest_pt - [L(:,2) -L(:,1)] * 10; % offset from the closest point is 10 pixels
pt2 = closest_pt + [L(:,2) -L(:,1)] * 10;

% display points and segments of corresponding epipolar lines
% Do this for both F (with & without RANSAC)
clf;
subplot(1,2,1), imshow(I2); hold on;
plot(x2(:,1), x2(:,2), '+r');
line([x2(:,1) closest_pt(:,1)]', [x2(:,2) closest_pt(:,2)]', 'Color', 'r');
line([pt1(:,1) pt2(:,1)]', [pt1(:,2) pt2(:,2)]', 'Color', 'g');


%%
%% display second image with epipolar lines reprojected 
%% from the first image
%%
L_ransac = x1 * F_ransac; % transform points from 
% the first image to get epipolar lines in the second image

% find points on epipolar lines L closest to matches(:,3:4)
L_ransac = L_ransac ./ repmat(sqrt(L_ransac(:,1).^2 + L_ransac(:,2).^2), 1, 3); % rescale the line
pt_line_dist_ransac = sum(L_ransac .* x2,2);
closest_pt_ransac = x2(:,1:2) - L_ransac(:,1:2) .* repmat(pt_line_dist_ransac, 1, 2);

% find endpoints of segment on epipolar line (for display purposes)
pt1_ransac = closest_pt_ransac - [L_ransac(:,2) -L_ransac(:,1)] * 10; % offset from the closest point is 10 pixels
pt2_ransac = closest_pt_ransac + [L_ransac(:,2) -L_ransac(:,1)] * 10;

% display points and segments of corresponding epipolar lines
% Do this for both F_ransac (with & without RANSAC)

subplot(1,2,2), imshow(I2); hold on;
plot(x2(:,1), x2(:,2), '+r');
line([x2(:,1) closest_pt_ransac(:,1)]', [x2(:,2) closest_pt_ransac(:,2)]', 'Color', 'r');
line([pt1_ransac(:,1) pt2_ransac(:,1)]', [pt1_ransac(:,2) pt2_ransac(:,2)]', 'Color', 'g');
