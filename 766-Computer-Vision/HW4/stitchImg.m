function stitched_img = stitchImg(varargin)
    cnt = numel(varargin);
    imgd = varargin{1};
    ransac_n = 100; % Max number of iteractions
    ransac_eps = 10; %Acceptable alignment error 
    for c=2:cnt
        imgs = varargin{c};
        [xs, xd] = genSIFTMatches(imgs, imgd);
        [~, H_3x3] = runRANSAC(xs, xd, 30, ransac_eps);
        % Pad image
        H = H_3x3;
        ws = size(imgs, 2);
        hs = size(imgs, 1);
        wd = size(imgd, 2);
        hd = size(imgd, 1);
        xs = [1 1 ws ws];
        ys = [1 hs 1 hs];
        maxX = 0;
        minX = inf;
        maxY = 0;
        minY = inf;
        for i=1:4
            x = xs(i);
            y = ys(i);
            tmp = [x; y; 1];
            tmp = H * tmp;
            x_r = round(tmp(1)/tmp(3));
            y_r = round(tmp(2)/tmp(3));
            maxX = max(maxX, x_r);
            minX = min(minX, x_r);
            maxY = max(maxY, y_r);
            minY = min(minY, y_r);
        end
        minX = min(0, minX) - 50;
        maxX = max(wd, maxX) + 50;
        minY = min(0, minY) - 50;
        maxY = max(hd, maxY) + 50;
        imgd = [zeros(hd, abs(minX), 3), imgd];
        imgd = [imgd, zeros(hd, maxX  - wd, 3)];
        wd = size(imgd, 2);
        imgd = [zeros(abs(minY), wd, 3); imgd];
        imgd = [imgd; zeros(maxY - hd, wd, 3)];
        [xs, xd] = genSIFTMatches(imgs, imgd);
        [~, H_3x3] = runRANSAC(xs, xd, ransac_n, ransac_eps);
        dest_canvas_width_height= [size(imgd, 2) size(imgd, 1)];
        [mask, dest_img] = backwardWarpImg(imgs, inv(H_3x3), dest_canvas_width_height);
        out_img = blendImagePair(imgd, sum(imgd, 3) > 0, dest_img, mask, "blend");
        imgd = out_img;
    end
    stitched_img  = imgd;