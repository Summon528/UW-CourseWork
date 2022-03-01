% img_list = {'hough_1', 'hough_2', 'hough_3'};


% %[0.06, 0.1, 0.02]
% img = imread([img_list{3} '.png']);
% imshow(img)
% edge_img = edge(img, 'canny', 0.02);
% imshow(edge_img)



img = imread("edge_hough_1.png");
imshow(img);
img = edge(im2gray(img), 'canny');
%%

sz = size(img);

theta_num_bins = 180;
diag = sqrt(sz(1) * sz(1) + sz(2) * sz(2));
rho_num_bins = 300;
theta_interval = pi/theta_num_bins;
rho_interval = diag * 2 / rho_num_bins;
thetas = [0:theta_interval:pi];
rhos = [-diag:rho_interval:diag];
thetas(end) = [];
rhos(end) = [];

%%


%%
accu = zeros(rho_num_bins, theta_num_bins);
[ys, xs] = find(img);
for i = 1:size(ys, 1)
    rhos_calc = ys(i) * cos(thetas) - xs(i) * sin(thetas) + diag;
    rhos_idx = get_rho_idx(rhos_calc, rho_interval, rho_num_bins);
    for j = 1:theta_num_bins
        accu(rhos_idx(j), j) = accu(rhos_idx(j), j) + 1;
    end
end


%%
mx = max(accu, [], 'all');
accu = min(255, round(accu .* (255 ./ mx)));
fh = figure();
imshow(accu, [0 255]);
colormap(gca,hot);


%%
orig_img = img;
hough_img = accu;
[rows, cols] = find(accu >= 130);
line_thetas = thetas(cols);
line_rhos = rhos(rows);



%%

figure()

imshow(orig_img);
axis on;
hold on

for i=1:size(rows,1)
    if line_thetas(i) > pi / 2
        y = [0 sz(1)];
        x = (y * cos(line_thetas(i)) - line_rhos(i)) * csc(line_thetas(i));
    else
        x = [0 sz(2)];
        y = (x * sin(line_thetas(i)) + line_rhos(i)) * sec(line_thetas(i));
    end
    line(x, y)
end 


%%


orig_img = img;
hough_img = accu;
[rows, cols] = find(accu >= 130);
figure()
imshow(orig_img)
axis on
hold on
[ys, xs] = find(orig_img);
for k = 1:size(rows, 1)
    mn = [inf, inf];
    mx = [0 0];
    disp(k)
    for i = 1:size(ys, 1)
        rhos_calc = ys(i) * cos(thetas) - xs(i) * sin(thetas) + diag;
        rhos_idx = get_rho_idx(rhos_calc, rho_interval, rho_num_bins);
        for j = 1:theta_num_bins
            if rhos_idx(j) == rows(k) && j == cols(k)
                p = [xs(i) ys(i)];
                if p(1) > mx(1) || p(1) == mx(1) && p(2) > mx(2)
                    mx = p;
                end
                if p(1) < mn(1) || p(1) == mn(1) && p(2) < mn(2)
                    mn = p;
                end
            end
        end
    end
    line([mn(1) mx(1)], [mn(2) mx(2)]);
end


%%
accu = zeros(rho_num_bins, theta_num_bins);
[ys, xs] = find(img);
for i = 1:size(ys, 1)
    rhos_calc = ys(i) * cos(thetas) - xs(i) * sin(thetas) + diag;
    rhos_idx = get_rho_idx(rhos_calc, rho_interval, rho_num_bins);
    for j = 1:theta_num_bins
        accu(rhos_idx(j), j) = accu(rhos_idx(j), j) + 1;
    end
end




%%
function idx = get_rho_idx(rho, rho_interval, bin_cnt)
    idx = min(bin_cnt, max(1, ceil(rho/rho_interval)));
end

