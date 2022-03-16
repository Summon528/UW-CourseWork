function dest_pts_nx2 = applyHomography(H_3x3, src_pts_nx2)
    dest_pts_nx2 = [];
    for i=1:size(src_pts_nx2, 1)
        tmp = [src_pts_nx2(i, 1); src_pts_nx2(i, 2); 1];
        tmp = H_3x3 * tmp;
        tmp = [tmp(1)/tmp(3) tmp(2)/tmp(3)];
        dest_pts_nx2 = [dest_pts_nx2;tmp];
    end