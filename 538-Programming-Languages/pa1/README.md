## Due Dates

Early turn-in: September 25, 11:59 PM
Hard turn-in: October 1, 11:59 PM

## Background information

For this assignment, we will use immutable lists. You should look at the
following docs to learn more about them:

https://facebook.github.io/immutable-js/docs/#/List

You don't need to understand this library in detail, but do need to understand
how to create lists and manipulate them. We are using this library instead of
JavaScript's built-in arrays because they are conceptually simpler. You will
not mutate lists, all functions on lists that return lists will return new
lists.

## Getting started

You will need to install the immutable library (see above). To do this you
should run `npm install` in the top-level assignment directory. This will
install this dependency.

## Assignments

The programming assignment consists of 3 different sub problems:

- Quicksort (`quick.js`): implement quicksort in a functional programming
  style. This problem asks that you implement various helper functions
  that do most of the heavy lifting (`fold_left`, `map`, etc.) for the actual
  quicksort implementation.

  To solve this problem: read and modify `quick.js` as instructed there.

- Extra credit (`extra.js`): various short exercises.  The point of the short
  exercises is to get you more familiar with immutable lists and high-order
  functions. We strongly recommend that you do these exercises, if only to test
  some of the things you implemented for the quicksort problem.

  This problem is optional, but any extra credits can be used as points towards
  mistakes you make in the other 2 problems.

  To solve this problem: read and modify `extra.js` as instructed there.

- Module system (`module` directory): implement various parts of a module
  system. In this problem you will implement various components that will be
  together used to build a module `require()`-function that can be used to load
  JavaScript Node.js modules.

  To solve this problem: read the problem description and instructions given
  `module/require.js`.

In all three cases, we have removed the solutions and included a comment `/**
<FILL-IN> **/` where you are expected to fill in your answers. (You'll want to
remove the `undefined` values appropriately.) You should not need to add code
anywhere outside these comment blocks.

### Testing your solution

We have included simple tests in the files themselves and/or (in the module
system problem, specifically) in separate problems. In the module system
problem, we have also included some examples in the `examples` subdirectory.
You can run the code by with the Node.js CLI tool:

For example:

```bash
node quick.js
```

Or:

```bash
node module/tests/require.js
```

If you don't get an assertion error, you are likely set. You may, of course,
want to add more tests.

#### No-nos

- In all cases we are exporting the answers (e.g., `exports.fold_left = ...`),
  please do not remove any of this code. If we can't test your code, you lose
  points.

- We will be loading your modules with `require()`. Please make sure that your
  modules can be loaded -- test them by loading them and checking to make sure
  `require()` doesn't fail (by throwing an exception). This is especially
  important if you don't solve everything. We will not be fixing your files to
  ensure they can be loaded.

#### Grading

You can see how many points each question is worth in the source
files themselves. The quicksort and module assignments are each
worth 50 points.  Each extra credit question is worth 2 points.
The maximum score of the assignment is 100.

#### Submission

You must submit the assignment via gradescope. The class
entry code is **MVWN3X**.  Submit your assignment by
uploading the following files:

- quick.js
- extra.js
- module/builtins.js
- module/helpers.js
- module/require-json.js


**NOTE:** Upload only these .js files, not a zip file containing your whole
project directry, not fewer files, not more files.
