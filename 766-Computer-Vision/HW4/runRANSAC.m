function [inliers_id, H] = runRANSAC(Xs, Xd, ransac_n, eps)
    inliers_id = [];
    H = [];
    cnt = size(Xs, 1);
    for i=1:ransac_n
        idxs = randperm(cnt, 4);
        H_3x3 = computeHomography(Xs(idxs, :), Xd(idxs, :));
        xd_calc = applyHomography(H_3x3, Xs);
        dist = (xd_calc(:, 1) - Xd(:, 1)).^2 + (xd_calc(:, 2) - Xd(:, 2)).^2;
        tmp = find(dist < eps);
        if size(tmp, 1) > size(inliers_id, 1)
            inliers_id = tmp;
            H = H_3x3;
        end
    end
    


