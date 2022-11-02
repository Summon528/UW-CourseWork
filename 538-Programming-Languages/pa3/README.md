## Due Dates

Early turn-in: November 13, 11:00 PM
Hard turn-in: November 20, 11:00 PM

## Assignments

The programming assignment consists of 2 parts:

- Haskell: you will build a simple arithmetic language in Haskell, 
  and then compile this language into a simple stack based instruction language.
- Rust: you will build roughly the same program, as an intro to Rust.


Each section has a README.md associated with it. Read those before getting started.
The `rust-warmup` is optional/not-graded but highly recommended to get started.
It only takes 5 minutes.


## Grading

We have included the points-worth of each question in the comments. As before, you'll
be able to submit to Gradescope to see your current scoring.

## Submission

You must submit the assignment via gradescope. Submit your assignment by uploading the following files only:

- Calculator.hs
- parser.rs
- rpn.rs

Additionally, run `cargo package` in the rust project root directory. 
This will create a single `.crate` file under `targets/package`.
Upload this `crate` file.

Unlike the first assignment, the autograder will not look for your files or
otherwise fix your bugs. You may only upload these files and you must ensure
they can be loaded.
