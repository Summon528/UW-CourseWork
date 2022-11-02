/*
 * Reverse Polish Notation: rpn.rs
 * See `rpn.md` for the overview.
 */

use std::io;

// Stacks will work with Items, which either either integers or booleans
#[derive(Eq, PartialEq, Ord, PartialOrd, Debug)]
pub enum Item {
    Int(i32),
    Bool(bool),
}

// List of possible errors
#[derive(Debug)]
pub enum Error {
    Empty,         // Tried to pop empty stack
    Extra,         // Stack ended with extra elements
    Type,          // Type mismatch
    Syntax,        // Syntax error, didn't recognize op
    IO(io::Error), // Some kind of IO error
    Quit,          // User quitting
}

// Base operations supported by calculator, see rpn.md
#[derive(Debug)]
pub enum Op {
    Add,
    Eq,
    Neg,
    Swap,
    Rand,
    Cond,
    Quit,
}

// We'll define a result type for our calculator: either a valid value, or a calculator Error
pub type Result<T> = std::result::Result<T, Error>;

// Define a type for Stacks
#[derive(Debug)]
pub struct Stack(Vec<Item>);

// Implement the following functions on Stacks
impl Stack {
    // Make a new Stack
    pub fn new() -> Self {
        todo!()
    }

    // Check if a Stack is empty
    pub fn empty(&self) -> bool {
        todo!()
    }

    // Push an item onto a stack (should never error)
    pub fn push(&mut self, item: Item) -> Result<()> {
        todo!()
    }

    // Pop an item off the Stack; may result in Empty error
    pub fn pop(&mut self) -> Result<Item> {
        todo!()
    }

    /*
     * Main evaluation function: apply an operation to a Stack
     *
     * Complete each of the cases. 
     *
     * Hint: You'll probably want to use the "question-mark" syntax quite a bit; see `rpn.md`.
     */
    pub fn eval(&mut self, op: Op) -> Result<()> {
        todo!()
    }
}
