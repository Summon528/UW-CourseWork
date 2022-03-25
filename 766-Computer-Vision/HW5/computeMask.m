function mask = computeMask(img_cell)
    mask = img_cell{1} ~= 0;
    for i=2:size(img_cell, 1)
        mask = bitor(mask, img_cell{i} ~= 0);
    end