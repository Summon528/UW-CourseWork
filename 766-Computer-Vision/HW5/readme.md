# HW5

## Challenge 1b

The normal vector $[N_x\;N_y\;N_z]$ is calculated using the following formula, where ($x$, $y}$) is the position of the brightest pixel and ($Center_x$, $Center_y$) is the centroid of the sphere's projection.

$$
\begin{cases}
N_x = x - Center_x \\
N_y = y - Center_y \\
N_z = \sqrt{Radius^2 - (N_x)^2 - (N_y)^2}
\end{cases}
$$

Recall the irradiance formula:
$$ E = \frac{Jcos\theta}{r^2} $$
When $\theta=0$, the irradiance is at its maximum. Therefore, it is safe to assume that the normal vector of the brightest location and the light source are parallel.

## Challenge 1d

I use all five images for this part since there are some outliers when using just three.
