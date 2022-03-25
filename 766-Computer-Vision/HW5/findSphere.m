function [center, radius] = findSphere(img)
    [rows, cols] = size(img);
    bwimg = im2bw(img, 0);
    x_sum = 0;
    y_sum = 0;
    area = 0;
    for x=1:cols
        for y=1:rows
            if bwimg(y, x)
                x_sum = x_sum + x;
                y_sum = y_sum + y;
                area = area + 1;
            end
        end
    end
    center = [x_sum / area, y_sum / area];
    radius =  sqrt(area/pi);