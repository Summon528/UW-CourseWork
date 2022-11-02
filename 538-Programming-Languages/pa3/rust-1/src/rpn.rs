/*
 * Reverse Polish Notation: rpn.rs
 * See `rpn.md` for the overview.
 */

use rand::{thread_rng, Rng};
use std::io;

// Stacks will work with Items, which either either integers or booleans
#[derive(Eq, PartialEq, Ord, PartialOrd, Debug)]
pub enum Item {
    Int(i32),
    Bool(bool),
}

impl Item {
    pub fn int(self) -> Result<i32> {
        match self {
            Item::Int(t) => Ok(t),
            _ => Err(Error::Type),
        }
    }

    pub fn bool(self) -> Result<bool> {
        match self {
            Item::Bool(t) => Ok(t),
            _ => Err(Error::Type),
        }
    }
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
        Self(Vec::new())
    }

    // Check if a Stack is empty
    pub fn empty(&self) -> bool {
        self.0.is_empty()
    }

    // Push an item onto a stack (should never error)
    pub fn push(&mut self, item: Item) -> Result<()> {
        self.0.push(item);
        Ok(())
    }

    // Pop an item off the Stack; may result in Empty error
    pub fn pop(&mut self) -> Result<Item> {
        match self.0.pop() {
            Some(val) => Ok(val),
            None => Err(Error::Empty),
        }
    }

    /*
     * Main evaluation function: apply an operation to a Stack
     *
     * Complete each of the cases.
     *
     * Hint: You'll probably want to use the "question-mark" syntax quite a bit; see `rpn.md`.
     */
    pub fn eval(&mut self, op: Op) -> Result<()> {
        match op {
            Op::Add => {
                let b = self.pop()?.int()?;
                let a = self.pop()?.int()?;
                let result = Item::Int(a + b);
                self.push(result)
            }
            Op::Eq => {
                let b = self.pop()?;
                let a = self.pop()?;
                let result = Item::Bool(a == b);
                self.push(result)
            }
            Op::Neg => {
                let a = self.pop()?.bool()?;
                let result = Item::Bool(!a);
                self.push(result)
            }
            Op::Swap => {
                let b = self.pop()?;
                let a = self.pop()?;
                self.push(b)?;
                self.push(a)
            }
            Op::Rand => {
                let a = self.pop()?.int()?;
                let mut rng = thread_rng();
                let result = Item::Int(rng.gen_range(0, a));
                self.push(result)
            }
            Op::Cond => {
                let b = self.pop()?;
                let a = self.pop()?;
                let cond = self.pop()?.bool()?;
                self.push(if cond { a } else { b })
            }
            Op::Quit => Err(Error::Quit),
        }
    }
}
