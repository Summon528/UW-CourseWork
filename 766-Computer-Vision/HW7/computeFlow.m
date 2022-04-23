function result = computeFlow(img1, img2, win_radius, template_radius, grid_MN)
sr = win_radius;
tr = template_radius;
[h, w] = size(img1);

m = grid_MN(1);
n = grid_MN(2);
X = [];
Y = [];
U = [];
V = [];
for a=1:(h-1)/m:h
    for b=1:(w-1)/n:w
        i = floor(a);
        j = floor(b);
        template = img1(max(1, i-tr):min(h, i+tr), max(1, j-tr):min(w ,j+tr));
        sti = max(1, i-sr);
        stj = max(1, j-sr);
        searchwin = img2(sti:min(h, i+sr), stj:min(w, j+sr));
        c = normxcorr2(template, searchwin);
        [ypeak,xpeak] = find(c==max(c(size(template, 1):size(searchwin, 1), size(template, 2):size(searchwin, 2)), [], 'all'));
        vector = [ypeak+sti-1 xpeak+stj-1] - [min(h, i+tr) min(w ,j+tr)];
        X = [X j];
        Y = [Y i];
        U = [U vector(2)];
        V = [V vector(1)];
    end
end

fh = figure;
imshow(img1);
hold on;
quiver(X,Y,U,V, "off")
axis equal
result = saveAnnotatedImg(fh);
pause(1);
close(fh);