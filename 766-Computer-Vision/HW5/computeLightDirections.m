function light_dirs_5x3 = computeLightDirections(center, radius, img_cell)
    light_dirs_5x3 = zeros(5, 3);

    for i=1:size(img_cell, 1)
        img = img_cell{i};
        [mx, I] = max(img,[],'all');
        [y, x] = ind2sub(size(img), I);
        xx = x - center(1);
        yy = y - center(2);
        zz = sqrt(radius^2 - xx^2 - yy^2);
        dir = [xx yy zz] / norm([xx yy zz]) .* double(mx);
        light_dirs_5x3(i, :) = dir;
    end