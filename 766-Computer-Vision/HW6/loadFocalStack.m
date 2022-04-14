function [rgb_stack, gray_stack] = loadFocalStack(focal_stack_dir)
    rgb_stack = cell(25, 1);
    gray_stack = cell(25, 1);
    for i=1:25
        im = imread(fullfile(focal_stack_dir, "/frame"+i+".jpg"));
        rgb_stack{i} = im;
        gray_stack{i} = rgb2gray(im);
    end