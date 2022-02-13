function [db, out_img] = compute2DProperties(orig_img, labeled_img)
    labels_with_bg = unique(labeled_img);
    labels = labels_with_bg(2:end);
    item_cnt = size(labels, 1);
    area = zeros(item_cnt, 1);
    row_idx_sum = zeros(item_cnt, 1);
    col_idx_sum = zeros(item_cnt, 1);

    [rows, cols] = size(labeled_img);
    for i = 1 : rows
        for j = 1 : cols
            label = labeled_img(i, j);
            if labeled_img(i, j) == 0
                continue
            end
            area(label) = area(label) + 1;
            row_idx_sum(label) = row_idx_sum(label) + i;
            col_idx_sum(label) = col_idx_sum(label) + j;
        end
    end

    %%
    db = zeros(6, item_cnt);
    db(1,:) = labels;
    db(2,:) = row_idx_sum ./ area;
    db(3,:) = col_idx_sum ./ area;

    %%
    a = zeros(item_cnt, 1);
    b = zeros(item_cnt, 1);
    c = zeros(item_cnt, 1);
    for i = 1 : rows
        for j = 1 : cols
            label = labeled_img(i, j);
            if labeled_img(i, j) == 0
                continue
            end
            a(label) = a(label) + (j - db(3, label))^2;
            b(label) = b(label) + (j - db(3, label)) * (i - db(2, label));
            c(label) = c(label) + (i - db(2, label))^2;
        end
    end

    %%
    b = b * 2;
    theta = atan2(b, a - c)/ 2;
    emin = a .* sin(theta).^2 - b .* sin(theta) .* cos(theta) + c .* cos(theta).^2;
    theta2 = theta + pi / 2;
    emax = a .* sin(theta2).^2 - b .* sin(theta2) .* cos(theta2) + c .* cos(theta2).^2;
    db(4,:) = emin;
    db(5,:) = theta;
    db(6,:) = emin./emax;
    db(7,:) = emin./area;

    %%
    fh1 = figure();
    imshow(orig_img);
    for i = 1 : item_cnt
        hold on;
        x = db(3, i);
        y = db(2, i);
        plot(x, y, 'r+', 'MarkerSize', 20, 'LineWidth', 2);
        L = 50;
        x2=x+(L*cos(theta(i)));
        y2=y+(L*sin(theta(i)));
        plot([x x2],[y y2], 'LineWidth', 2)
    end
    out_img = saveAnnotatedImg(fh1);
end

%%
function annotated_img = saveAnnotatedImg(fh)
figure(fh); % Shift the focus back to the figure fh

% The figure needs to be undocked
set(fh, 'WindowStyle', 'normal');

% The following two lines just to make the figure true size to the
% displayed image. The reason will become clear later.
img = getimage(fh);
truesize(fh, [size(img, 1), size(img, 2)]);

% getframe does a screen capture of the figure window, as a result, the
% displayed figure has to be in true size. 
frame = getframe(fh);
pause(0.5); 
% Because getframe tries to perform a screen capture. it somehow 
% has some platform depend issues. we should calling
% getframe twice in a row and adding a pause afterwards make getframe work
% as expected. This is just a walkaround. 
annotated_img = frame.cdata;
end
