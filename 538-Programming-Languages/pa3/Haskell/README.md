## Overview

In this assignment you will work towards building an interpreter and compiler
for a small calculator language. 

You should only need to modify the parts of the files which say:

```haskell
error "TBD: ..."
```

with suitable Haskell code.

**NOTE:** Start early, to avoid any unexpected shocks late in the process.

## Assignment Testing and Evaluation

Most of the points, will be awarded automatically, by **evaluating your
functions against a given test suite**.

[Test.hs](/test/Test.hs) contains a very small suite of tests which gives you
a flavor of of these tests.  When you run

```shell
$ cabal test
```

Your last lines should have

```
All N tests passed (...)
OVERALL SCORE = ... / ...
```

**OR**

```
K out of N tests failed
OVERALL SCORE = ... / ...
```

**If your output does not have one of the above outputs your submission will receive a 0.**

If, for some problem, you cannot get the code to compile, leave it as is with
the `error ...` with your partial solution enclosed below as a comment.

The other lines will give you a readout for each test.
You are encouraged to try to understand the testing code, but you will not be
graded on this.


## Making progress

The code itself has more lines of documentation than actual code. If you are
feeling stuck with any part of the assignment, re-read the paragraphs above
each function definition and see if that helps. If it doesn't, drop by OH to 
chat.