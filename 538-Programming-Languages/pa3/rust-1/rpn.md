# Reverse Polish Notation Calculator

Because it's important to write a calculator in every language, we will produce
a small Reverse Polish Notation calculator in Rust. Here is an intro on RPN:

https://en.wikipedia.org/wiki/Reverse_Polish_notation

The rough idea is that a RPN calculator is a *stack language*: we push things
onto a stack as we parse them, and when we parse operations we pop off the
required number of arguments, apply the operation, and push the result back on
to the stack.

We've supplied much of the code to help you get started. There are two main
files (and two main tasks): `rpn.rs` and `parser.rs`.

## parser.rs

This file contains the parser and main read-eval-print loop. We'll be relying on
standard libraries for parsing. You will write the main loop of the evaluator,
which will take in a token and update the stack/apply the indicated operation.
You shouldn't need a lot of code here---the hard work is in the other file.

## rpn.rs

In this file, you will implement the main data structure for our calculator and
give the evaluation functions. The main data structure is a `Stack`; you are
free to implement the functions however you like, but a `Vec` might be a good
choice... (note that Vectors have `push` and `pop` operations).

You will also implement the evaluation function: given a `Stack` and an
operation, perform the indicated operation on the `Stack`. Your calculator
should support (at least) the following operations:

* Add (`+`): Add two numbers together. Sample input: `3 5 +` should lead to 8.
* Eq (`=`): Check if two numbers or two booleans are equal. Sample input: `3 5
  =` should lead to false.
* Neg (`~`): Negate a boolean. Sample input: `false ~` should lead to true.
* Swap (`<->`): Swap the top two elements of the stack. Sample input: `0 1 <->
  [...]` should lead to `1 0 [...]`.
* Rand (`#`): Produce a random integer from 0 to the top element of the stack
  (which must be non-negative).  Sample input: `5 # [...]` should lead to a
  uniform integer in {0, 1, 2, 3, 4}, followed by `[...]`.
* Cond (`?`): If-then-else. Looks at the top three elements of the stack, and
  does an if-then-else. Sample input: `true 1 2 ?` should lead to 1, and `false
  1 2 ?` should lead to 2.
* Quit (`quit`): Quit the calculator. (You can just return `Error::Quit`.)

Your calculator should also accept numbers and boolean constants: `true` and `false`.

To implement the Rand operation, we will use the `rand` crate. Take a quick
look at the basic examples:

https://docs.rs/rand/0.6.5/rand/

We've already added the `rand` dependency to the Cargo.toml file; take a look if
you're curious.

## Working with Result

You will work with Rust's `Result` type in both of the files. Here is a recap of
Rust's "question mark" syntax---you will be using this a lot. Suppose `foo()`
returns something of type `Result<T>`. Then, we can write:

```
let x = foo()?;
```

If `foo()` returns `Ok(ret)`, `ret` will be assigned to `x`. Otherwise if
`foo()` returns `Err(e)`, the assignment is not performed and `Err(e)` is
returned from the enclosing function.

This syntax helps when working with multiple operations that may fail, like:

```
let x = foo()?;
let y = bar(x)?;
let z = baz(x, y)?;
...
```

The first one to fail will abort the whole function while returning the error.
(This behavior should remind you of do-notation for `OrErr` or `Either String`,
which we saw in previous HWs.)
