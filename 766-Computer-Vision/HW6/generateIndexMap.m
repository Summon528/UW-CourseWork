function index_map = generateIndexMap(gray_stack, w_size)
    cnt = size(gray_stack, 1);
    avg_focus_im = cell(cnt, 1);
    for i=1:cnt
        avg_focus_im{i} = gen_avg_focus_im(gray_stack{i}, w_size);
    end
    [~, index_map] = max(cat(3, avg_focus_im{:}), [], 3);
end


function avg_focus_im = gen_avg_focus_im(im, hK)
    im = pad_img(im,1);
    [h, w] = size(im);
    focus_im = zeros(h, w, 'int32');
    ker = [1,-2,1];
    for i=2:h-1
        for j=2:w-1
            focus_im(i, j) = abs(im(i-1, j)*ker(1) + im(i, j)*ker(2)  + im(i+1, j)*ker(3)) + ...
                             abs(im(i, j-1)*ker(1) + im(i, j)*ker(2)  + im(i, j+1)*ker(3));
        end
    end
    focus_im = focus_im(2:end-1, 2:end-1);

    K = hK + hK + 1;
    focus_im = pad_img(focus_im, hK);
    [h, w] = size(focus_im);
    avg_focus_im = zeros(h, w, 'int32');
    for i=2+hK:h-hK
        for j=2+hK:w-hK
            for ii=-hK:hK
                for jj=-hK:hK
                    avg_focus_im(i, j) = avg_focus_im(i, j) + focus_im(ii+i, jj+j);
                end
            end
            avg_focus_im(i, j) = avg_focus_im(i, j) / K / K;
        end
    end
    avg_focus_im = avg_focus_im(1+hK:end-hK, 1+hK:end-hK);
end


function im = pad_img(src, cnt)
    im = src;
    im = [im(:,1:1+cnt-1), im,  im(:, end-(cnt-1):end)];
    im = [im(1:1+cnt-1,:); im;  im(end-(cnt-1):end, :)];
    im = cast(im, 'int32');
end
