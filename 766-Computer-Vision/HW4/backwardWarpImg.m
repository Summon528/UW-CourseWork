function [mask, result_img] = backwardWarpImg(src_img, resultToSrc_H,...
    dest_canvas_width_height)
    H = resultToSrc_H;
    w_d = dest_canvas_width_height(1);
    h_d = dest_canvas_width_height(2);
    result_img = zeros(h_d, w_d, 3);
    mask = zeros(h_d, w_d);
    w_s = size(src_img, 2);
    h_s = size(src_img, 1);
    for x=1:w_d
        for y=1:h_d
            tmp = [x; y; 1];
            tmp = H * tmp;
            x_s = round(tmp(1)/tmp(3));
            y_s = round(tmp(2)/tmp(3));
            if x_s < 1 || x_s > w_s || y_s < 1 || y_s > h_s
                continue
            end
            result_img(y, x, :) = src_img(y_s, x_s, :);
            mask(y, x) = 1;
        end
    end