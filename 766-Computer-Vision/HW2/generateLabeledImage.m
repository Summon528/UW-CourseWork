function labeled_img = generateLabeledImage(gray_img, threshold)
    img_bw = im2bw(gray_img, threshold);
    labeled_img = bwlabel(img_bw);