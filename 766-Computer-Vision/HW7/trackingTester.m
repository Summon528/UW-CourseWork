function trackingTester(data_params, tracking_params)
    minx = tracking_params.rect(1);
    miny = tracking_params.rect(2);
    w = tracking_params.rect(3);
    h = tracking_params.rect(4);
    sr = tracking_params.search_half_window_size;
    bin = tracking_params.bin_n;
    
    mkdir(data_params.out_dir)
    img1 = imread(fullfile(data_params.data_dir, data_params.genFname(1)));
    template = img1(miny:miny+h, minx:minx+w, :);
    [X, MAP] = rgb2ind(template, bin);
    temphis = histcounts(X(:));
    
    for id=data_params.frame_ids
        img2 = imread(fullfile(data_params.data_dir, data_params.genFname(id)));
        mindiff = inf;
        for i=-sr:sr
            for j=-sr:sr
                target = img2(miny+i:miny+h+i, minx+j:minx+w+j, :);
                X = rgb2ind(target, MAP);
                targethis = histcounts(X(:), bin);
                diff = sum(abs(temphis - targethis));
                if diff < mindiff
                    mindiff = diff;
                    loc = [miny+i minx+j];
                end
            end
        end
        result = drawBox(img2, [loc(2), loc(1), w, h], [255 0 0], 3);
        imwrite(result, fullfile(data_params.out_dir, data_params.genFname(id)));
        minx = loc(2);
        miny = loc(1);
    end


