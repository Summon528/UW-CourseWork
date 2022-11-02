# Rust warmup

Welcome to Rust! This quick warmup is designed to check that your Rust tools are
working correctly. As the first step, you'll need to install Rust. Follow the
[instructions](https://www.rust-lang.org/learn/get-started) on the website.

## Cargo

The Rust package manager is called Cargo, (the Cabal of Haskell).
This tool plays an important role, doing stuff like:

* Automatically tracking and downloading dependencies ("crates")
* Running the compiler `rustc`
* Running source-code tools
* Generating documentation
* Running tests

Cargo will be your main interface to running Rust---we don't recommend you to
manually run the Rust compiler (though you certainly can, if you want). Here are
the key commands for Cargo:

* `cargo new <path>`: Create a new Cargo project.
* `cargo check`: Run the type and borrow checkers. This is much, much faster
  than building the program---you should use this.
* `cargo build`: Build an executable/library.
* `cargo run`: Run the executable.
* `cargo clean`: Clean up temporary files.
* `cargo package`: Compress the project into a single `.crate` file.

Besides the first command, these commands will only work when you are inside a
Cargo project. A Cargo project is controlled by the `Cargo.toml` text file; 
take a look if you're curious.

Other useful commands that we mostly won't use:

* `cargo bench`: Run benchmarks.
* `cargo doc`: Generate documentation.
* `cargo test`: Run tests.

As the first step in this warmup, run `cargo check`. Fix all errors, then do a
`cargo build` and run the program with `cargo run`.

## Clippy

Clippy is a linter, much like Hlint for Haskell. This tool will check your code
and give automatic suggestions about how to make your program more idiomatic
(more "Rust-y"). You should run this tool on everything you submit, and continue
fixing errors until it's happy.

As the second step in this warmup, run `cargo clippy` and follow the
suggestions. If the command is not working (and you've already installed
clippy), try it in the top level of the project (the directory with the
`Cargo.toml`).

## Rustfmt

In Rust, it's possible to give a fairly uniform formatting style for source
code. (This is very difficult in Haskell, which is much more free-form.) The
rustfmt tool will automatically format your source code to follow a consistent
style. It's possible to customize this style, but for our purposes the default
style will do just fine.

As the final step in this warmup, run `cargo fmt`. Take a look at what the
source code looks like after running it.

# Submission

Once you've carried out all three steps, run `cargo package` in the project root
directory. This will create a single `.crate` file under `targets/package`.
