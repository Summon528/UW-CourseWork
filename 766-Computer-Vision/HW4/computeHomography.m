function H_3x3 = computeHomography(src_pts_nx2, dest_pts_nx2)
    A = [];
    for i=1:4
      tmp = [src_pts_nx2(i, 1) src_pts_nx2(i, 2) 1 0 0 0 -dest_pts_nx2(i ,1)*src_pts_nx2(i, 1) -dest_pts_nx2(i ,1)*src_pts_nx2(i, 2) -dest_pts_nx2(i, 1);
      0 0 0 src_pts_nx2(i, 1) src_pts_nx2(i, 2) 1 -dest_pts_nx2(i ,2)*src_pts_nx2(i, 1) -dest_pts_nx2(i ,2)*src_pts_nx2(i, 2) -dest_pts_nx2(i, 2)];
      A = [A;tmp;];
    end
    [V, D] = eig(A'*A);
    [~, I] = min(diag(D));
    H = V(:, I);
    H_3x3 = [H(1) H(2) H(3);
     H(4) H(5) H(6);
     H(7) H(8) H(9)];