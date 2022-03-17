function out_img = blendImagePair(wrapped_imgs, masks, wrapped_imgd, maskd, mode)
    og_class = class(wrapped_imgs);
    wrapped_imgs = cast(wrapped_imgs, "double");
    masks = cast(masks, "double");
    wrapped_imgd = cast(wrapped_imgd, "double");
    maskd = cast(maskd, "double");
    
    [rows, cols] = size(masks);
    out_img = zeros(rows, cols, 3, class(wrapped_imgs));
    if mode == "overlay"
        maskd2 = maskd > 0;
        masks2 = masks - maskd;
        masks2 = masks2 > 0;
    else
        masks2 = bwdist(masks == 0, 'euclidean');
        masks2 = masks2 / max(masks2, [], 'all');
        maskd2 = bwdist(maskd == 0, 'euclidean');
        maskd2 = maskd2 / max(maskd2, [], 'all');
        mask_sum = masks2 + maskd2;
        mask_sum = mask_sum / max(mask_sum, [], 'all');
        mask_sum(mask_sum == 0) = 1;
        masks2 = masks2 ./ mask_sum;
        masks2 = masks2 / max(masks2, [], 'all');
        maskd2 = maskd2 ./ mask_sum;
        maskd2 = maskd2 / max(maskd2, [], 'all');
    end
    for i=1:rows
        for j=1:cols
               out_img(i, j, :) = wrapped_imgd(i, j, :) * maskd2(i, j) ...
                    + wrapped_imgs(i, j, :) * masks2(i, j);
        end
    end
    
    out_img = cast(out_img, og_class);
             
