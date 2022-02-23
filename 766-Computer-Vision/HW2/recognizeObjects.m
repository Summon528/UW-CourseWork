function output_img = recognizeObjects(orig_img, labeled_img, obj_db)
    [db, ] = compute2DProperties(orig_img, labeled_img);
    db_item_cnt = size(obj_db, 2);
    item_cnt = size(db, 2);
    
    fh1 = figure();
    imshow(orig_img)
    for i = 1 : item_cnt
        for j = 1 : db_item_cnt
            %print(abs(db(6, j) - obj_db(6, i)))
            if abs(obj_db(6, j) - db(6, i)) <= 0.03 && abs(obj_db(7, j) - db(7, i)) <= obj_db(7, j) * 0.1
                hold on;
                x = db(3, i);
                y = db(2, i);
                plot(x, y, 'r+', 'MarkerSize', 20, 'LineWidth', 2);
                L = 50;
                x2=x+(L*cosd(db(5, i)));
                y2=y+(L*sind(db(5, i)));
                plot([x x2],[y y2], 'LineWidth', 2)
                %%break
            end
        end
    end
    output_img = saveAnnotatedImg(fh1);
    delete(fh1);
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
pause(0.5); 
% Because getframe tries to perform a screen capture. it somehow 
% has some platform depend issues. we should calling
% getframe twice in a row and adding a pause afterwards make getframe work
% as expected. This is just a walkaround. 
annotated_img = frame.cdata;
end
              