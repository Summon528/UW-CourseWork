function line_detected_img = lineFinder(orig_img, hough_img, hough_threshold)
    sz = size(orig_img);
    diag = sqrt(sz(1) * sz(1) + sz(2) * sz(2));
    theta_num_bins = size(hough_img, 2);
    rho_num_bins = size(hough_img, 1);
    theta_interval = pi/theta_num_bins;
    rho_interval = diag * 2 / rho_num_bins;
    thetas = [0:theta_interval:pi];
    rhos = [-diag:rho_interval:diag];
    thetas(end) = [];
    rhos(end) = [];

    [rows, cols] = find(hough_img >= hough_threshold);
    line_thetas = thetas(cols);
    line_rhos = rhos(rows);

    fh = figure();
    imshow(orig_img);
    hold on;
    for i=1:size(rows,1)
        if line_thetas(i) > pi / 2
            y = [0 sz(1)];
            x = (y * cos(line_thetas(i)) - line_rhos(i)) * csc(line_thetas(i));
        else
            x = [0 sz(2)];
            y = (x * sin(line_thetas(i)) + line_rhos(i)) * sec(line_thetas(i));
        end
        line(x, y)
    end 
    line_detected_img = saveAnnotatedImg(fh);
    pause(1);
    close(fh)
end


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
frame = getframe(fh);
pause(0.5); 
% Because getframe tries to perform a screen capture. it somehow 
% has some platform depend issues. we should calling
% getframe twice in a row and adding a pause afterwards make getframe work
% as expected. This is just a walkaround. 
annotated_img = frame.cdata;
end