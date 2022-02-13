# Homework 2

## Walkthrough 1

My parameters are listed below, selected based on trial and error.
`threshold = 0.32;`
The number of dilations/erosions (phase 1): `k = 2`
The number of erosions/dilations (phase 2): `k = 11`

## Challenge 1a

I use `threshold = 0.5` for every image.

## Challenge 1b

Other than the six required properties, I added a 7th one which is the minimum moment of inertia divided by area.

## Challenge 1c

The objects are the same if the following criteria are met:

1. the difference between the roundness is <= 0.03
2. the difference between the 7th property is within 10% of the one found in the database
