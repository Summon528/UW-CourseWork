function cropped_line_img = lineSegmentFinder(orig_img, hough_img, hough_threshold, edge_img)
    sz = size(orig_img);
    diag = sqrt(sz(1) * sz(1) + sz(2) * sz(2));
    theta_num_bins = size(hough_img, 2);
    rho_num_bins = size(hough_img, 1);
    theta_interval = pi/theta_num_bins;
    rho_interval = diag * 2 / rho_num_bins;
    thetas = [0:theta_interval:pi];
    thetas(end) = [];

    mxs = zeros(rho_num_bins, theta_num_bins, 2);
    mns = zeros(rho_num_bins, theta_num_bins, 2);
    mns(:,:,:) = inf;
    [rows, cols] = find(hough_img >= hough_threshold);
    selected = zeros(rho_num_bins, theta_num_bins);
    selected(rows, cols) = 1;
    
    axis on
    hold on
    [ys, xs] = find(edge_img);
    
    for i = 1:size(ys, 1)
        rhos_calc = ys(i) * cos(thetas) - xs(i) * sin(thetas) + diag;
        rhos_idx = get_rho_idx(rhos_calc, rho_interval, rho_num_bins);
        for j = 1:theta_num_bins
            idx = [rhos_idx(j), j];
            if selected(idx(1), idx(2)) == 1
                p = [xs(i) ys(i)];
                if p(1) > mxs(idx(1), idx(2), 1) || p(1) == mxs(idx(1), idx(2), 1) && p(2) > mxs(idx(1), idx(2), 2)
                    mxs(idx(1), idx(2), :) = p;
                end
                if p(1) < mns(idx(1), idx(2), 1) || p(1) == mns(idx(1), idx(2), 1) && p(2) < mns(idx(1), idx(2), 2)
                    mns(idx(1), idx(2), :) = p;
                end
            end
        end
    end
    
    fh = figure();
    imshow(orig_img)
    for k = 1:size(rows, 1)
        hold on
        line([mns(rows(k), cols(k), 1) mxs(rows(k), cols(k), 1)], ...
             [mns(rows(k), cols(k), 2) mxs(rows(k), cols(k), 2)]);
    end

    cropped_line_img = saveAnnotatedImg(fh);
    pause(1);
    close(fh);
end


function annotated_img = saveAnnotatedImg(fh)
figure(fh); % Shift the focus back to the figure fh

% The figure needs to be undocked
set(fh, 'WindowStyle', 'normal');

% The following two lines just to make the figure true size to the
% displayed image. The reason will become clear later.
img = getimage(fh);
truesize(fh, [size(img, 1), size(img, 2)]);

% getframe does a screen capture of the figure window, as a result, the
% displayed figure has to be in true size. 
frame = getframe(fh);
frame = getframe(fh);
pause(0.5); 
% Because getframe tries to perform a screen capture. it somehow 
% has some platform depend issues. we should calling
% getframe twice in a row and adding a pause afterwards make getframe work
% as expected. This is just a walkaround. 
annotated_img = frame.cdata;
end


function idx = get_rho_idx(rho, rho_interval, bin_cnt)
    idx = min(bin_cnt, max(1, ceil(rho/rho_interval)));
end
