function hough_img = generateHoughAccumulator(img, theta_num_bins, rho_num_bins)
    sz = size(img);
    diag = sqrt(sz(1) * sz(1) + sz(2) * sz(2));
    theta_interval = pi/theta_num_bins;
    rho_interval = diag * 2 / rho_num_bins;
    thetas = [0:theta_interval:pi];
    thetas(end) = [];

    hough_img = zeros(rho_num_bins, theta_num_bins);
    [ys, xs] = find(img);
    for i = 1:size(ys, 1)
        rhos_calc = ys(i) * cos(thetas) - xs(i) * sin(thetas) + diag;
        rhos_idx = get_rho_idx(rhos_calc, rho_interval, rho_num_bins);
        for j = 1:theta_num_bins
            hough_img(rhos_idx(j), j) = hough_img(rhos_idx(j), j) + 1;
        end
    end


    mx = max(hough_img, [], 'all');
    hough_img = min(255, round(hough_img .* (255 ./ mx)));
    fh = figure();
    imshow(hough_img, [0 255]);
    colormap(gca,hot);
    pause(1)
    close(fh)
end

%%
function idx = get_rho_idx(rho, rho_interval, bin_cnt)
    idx = min(bin_cnt, max(1, ceil(rho/rho_interval)));
end




