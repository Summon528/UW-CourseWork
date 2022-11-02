/*
 * Reverse Polish Notation: parser.rs
 * See `rpn.md` for the overview.
 */

extern crate rand;

use std::io::{self, Write};

use super::rpn;

pub fn rpn_repl() -> rpn::Result<()> {
    let mut stack = rpn::Stack::new();
    let mut input = String::new();

    // Read-eval-print loop
    loop {
        // Clear the input buffer
        input.clear();

        // Prompt the user
        print!("> ");
        io::stdout().flush().map_err(rpn::Error::IO)?;

        // Read a line and evaluate it
        io::stdin().read_line(&mut input).map_err(rpn::Error::IO)?;
        evaluate_line(&mut stack, &input)?;

        // A successful run should end with a stack with a exactly one item: the result
        let res = stack.pop()?;
        if stack.empty() {
            println!("Reply> {:?}", res);
        } else {
            return Err(rpn::Error::Extra);
        }
    }
}

pub fn evaluate_line(stack: &mut rpn::Stack, buf: &str) -> rpn::Result<()> {
    // Trim whitespace and split; this gives an iterator of tokens.
    let tokens = buf.trim().split_whitespace();

    /*
     * Write the main loop processing the tokens. The `parse` method for Strings will be useful for
     * parsing integers. See here for examples:
     *
     * https://doc.rust-lang.org/std/primitive.str.html#method.parse
     */
    for tok in tokens {
        todo!()
    }
    Ok(())
}
