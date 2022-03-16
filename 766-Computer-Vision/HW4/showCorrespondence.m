function result_img = ...
    showCorrespondence(orig_img, warped_img, src_pts_nx2, dest_pts_nx2)
    fh = figure();
    w = size(orig_img, 2);
    imshow([orig_img warped_img]);
    hold on;
    axis on;
    for i=1:size(src_pts_nx2, 1)
        plot([src_pts_nx2(i, 1) dest_pts_nx2(i, 1) + w], [src_pts_nx2(i, 2) dest_pts_nx2(i, 2)]);
    end
    result_img = saveAnnotatedImg(fh);
    pause(1);
    close(fh);
