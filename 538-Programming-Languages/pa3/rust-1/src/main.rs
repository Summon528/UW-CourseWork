#![allow(dead_code)]
#![forbid(unsafe_code)]

mod exercise1;
mod exercise2;
mod exercise3;
mod exercise4;

mod parser;
mod rpn;

fn main() {
    if let Err(err) = parser::rpn_repl() {
        if let rpn::Error::Quit = err {
          println!("Finished.");
        } else{
          println!("Error: {:?}", err);
        }
    }
}
