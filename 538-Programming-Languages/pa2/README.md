## Due Dates:

Early turn-in: October 16, 11:00 PM
Hard turn-in: October 23, 11:00 PM

## Getting started

For this assignment, you'll need both Node and Haskell installed.  The Haskell
assignment README contains some information on getting Haskell set up.

## Assignments

The programming assignment consists of 2 parts:

In the first part, you will be implementing a lazy-evaluation system in
JavaScript (lazy.js).  For this assignment, we have removed the solutions and
included an opening and closing comment `/** <FILL-IN> **/` where you are
expected to fill in your answers.  You may not add code anywhere outside these
comment blocks.

In the second part, you will be doing some problems collected from
a review of CS 538 material. For these problems, fill in lines that say

```Haskell
error "TBD"
```

### Testing your solution

You can run the code by calling Node:

For example:

```bash
node lazy.js
```

For Haskell, you'll want to run the test suite that comes with the
assignment. You can do that as follows:

```bash
stack test
```
(Assuming you have Haskell/Stack installed correctly.)

#### No-nos

In all cases we are exporting the answers (e.g., `exports.fold_left = ...`),
please do not remove any of this code. If we can't test your code, you lose
points.

#### Grading

We have included the points-worth of each question in the comments.

#### Submission

You must submit the assignment via gradescope. 
Submit your assignment by uploading the following files only:

- lazy.js
- Warmup.hs

Unlike our first assignment, the autograder will not look for your files or
otherwise fix your bugs. You may only upload these files and you must ensure
they can be loaded.
