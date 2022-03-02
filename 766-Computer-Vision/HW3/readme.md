# Homework 3

## Walkthrough 1

I selected my threshold value based on trial and error.

## Challenge 1a

Again, select threshold based on trial and error.

## Challenge 1b

Initially, I chose `theta_num_bins = 180` and `rho_num_bins = 300` but it produced bad result especially on `hough_3.png`. Increasing the resolution to `theta_num_bins = 400` and `rho_num_bins = 400` yields good results. (I vote for a single bin only)

## Challenge 1c

I'm not using anything fancy and use thresholds to find peaks. The threshold values are picked by trial and error for each image.

## Challenge 1d

To find the endpoints of a line, I run the algorithm in (1b) again, however, this time the algorithm collects the points that vote for the peaks found in (1c). After that, we select the min and the max of all those points and draw a line in between.
