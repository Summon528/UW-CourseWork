function refocusApp(rgb_stack, depth_map)
    mode = 1;
    idx = 1;
    [h, w] = size(depth_map);
    fh = figure();
    imshow(rgb_stack{idx})
    points = [570 71; 643 617; 534 265; 218 306];
    pidx = 1;
    while 1
        if mode == 1
            pause(1)
            if pidx > size(points, 1)
                break
            end
            x = points(pidx, 1);
            y = points(pidx, 2);
            pidx = pidx + 1;
        else
            [x, y] = ginput(1);
        end

        disp([x, y])
        if x < 1 || x > w || y < 1 || y > h
            break
        end
        next_idx = depth_map(round(y), round(x));
        if next_idx >= idx
            diff = 1;
        else
            diff = -1;
        end
        while idx ~= next_idx
            idx = idx + diff;
            hold on;
            imshow(rgb_stack{idx})
            plot(x, y, 'r+', 'MarkerSize', 30, 'LineWidth', 2);
            drawnow;
        end
    end
    close(fh)

    