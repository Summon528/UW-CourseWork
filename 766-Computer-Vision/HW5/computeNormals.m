function [normals, albedo_img] = ...
    computeNormals(light_dirs, img_cell, mask)
    cnt = size(img_cell, 1);
    
    [rows, cols] = size(mask);
    albedo_img = zeros(rows, cols);
    normals = zeros(rows, cols, 3);
    normals(:,:,3) = 1;
    for i=1:rows
        for j=1:cols
            if mask(i, j) == 0
                continue
            end
            vals = zeros(cnt, 1);
            for k=1:cnt
                vals(k) = img_cell{k}(i, j);
            end
            I = vals;
            S = light_dirs;
            N = (S' * S)\S' * I;
            albedo_img(i, j) = norm(N);
            normals(i, j, :) = N / norm(N);
        end
    end
    
    albedo_img = albedo_img ./ max(albedo_img, [], 'all');