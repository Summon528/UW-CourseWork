{-# OPTIONS_GHC -fwarn-incomplete-patterns #-}

module Calculator where

-- In this lab, we will get our hands dirty with Haskell by writing a
-- simple arithmetic language, and then compiling this language into a
-- simple stack based instruction language.
--
-- This is NOT a heavy coding lab: the solution for each given problem
-- should only be a few lines of code.  However, this is not an easy
-- lab: you'll have to think, and idiomatic code will end up using
-- nearly all of Haskell's features: types, algebraic data types,
-- pattern matching, and recursion.  We will be grading
-- your code as much for parsimony as we will be for correctness.
--
-- The recommended way to run this code is to load it into ghci:
--
--      ghci Calculator.hs
--
-- You will then be able to run most functions directly in the resulting
-- REPL.  If you make modifications to this file, type ':r' to reload
-- your changes in the repl.  Note that you will lose any local bindings
-- when you reload, so if you want some to persist put it in this file.

-- Note: We did not provide the parameters as part of the functions
-- that you have to write. You're given the type signature for each
-- problem so it is up to you to figure out what parameters each function
-- will take.

import Control.Monad

-------------------------------------------------------------------

--                  The expression language

-------------------------------------------------------------------

-- The expression language we'd like to define is very simple:
--
--      b ::= + | - | *
--      e ::= e b e | n
--
-- You may recall seeing this language in one of the homework readings.
--
-- Here's a data type for this language.  Before you continue, make sure
-- you can understand the correspondence between the grammar and the
-- data types we've written above.

data BinOp = Plus | Minus | Times
  deriving (Show, Eq)

data Expr
  = Op BinOp Expr Expr
  | Lit Int
  deriving (Show, Eq)

-- Here are some handy utility functions for constructing example
-- expressions.

plus, minus, times :: Expr -> Expr -> Expr
plus = Op Plus
minus = Op Minus
times = Op Times

example :: Expr
example = minus (plus (Lit 2) (Lit 2)) (Lit 3)

-- Enough of syntax, what is the meaning of a binary operator?  A simple
-- answer is that it simply represents the corresponding Haskell
-- (metalanguage) operator.  Here is a simple (higher order!) function
-- which interprets the meaning of a binary operator.

interpBinOp :: BinOp -> (Int -> Int -> Int)
interpBinOp Plus = (+) -- Recall, this is Haskell notation to take an infix
interpBinOp Minus = (-) -- operator and turn it into a normal non-infix "symbol"
interpBinOp Times = (*)

-- We can write an interpreter for these expressions, which takes an
-- arithmetic expression and evaluates it to an integer.

interp :: Expr -> Int
-- <FILL-IN>
interp = question "[5 pts] COMPLETE THE DEFINITION"

-- </FILL-IN>

-- You might like to use interpBinOp; also, feel free to change the top-level
-- declaration as you like, e.g. to add pattern matching on arguments.

-------------------------------------------------------------------

--                  A simple optimizer

-------------------------------------------------------------------

-- Now we are going to write a simple optimizer for our expression
-- language.  Technically, an optimizer is a bit useless for the
-- particular language we have here, as we can always evaluate an
-- expression to a number. However, it is not difficult to imagine a
-- variant of our arithmetic language that has variables as well, in
-- which case an optimizer would be useful.
--
-- We'll write a very simple optimizer which optimizes expressions of
-- the form "e + 0" or "0 + e" to "e".  To start off, write a PATTERN
-- for this function which matches expressions which look like "e + 0"
-- or "0 + e" and optimizes them to "e". We DO NOT want you to attempt
-- to optimize nested occurrences of this expression.
--
-- A real world peephole optimizer will include more algebraic
-- identities than just e + 0 or 0 + e. If you like, you can add some
-- more; however, for the purposes of this lab, an implementation of
-- simplifyZero that ONLY optimizes additions by zero is acceptable.

simplifyZero :: Expr -> Expr
-- <FILL-IN>
simplifyZero = question "[7 pts] COMPLETE THE DEFINITION"

-- </FILL-IN>

-- You have probably been testing your programs correctness by running them
-- on simple examples.
--
-- Here are three properties we might want our optimizer to uphold:
--
--      1. (Correctness) If we run the unoptimized and optimized
--      versions, we should get the same result.
--
--      2. (Idempotency) Running the optimizer twice is equivalent to
--      running it once.
--
--      3. (Optimality) After running the optimizer, there should be
--      no more instances of the pattern that we looked for.
--
-- At this point, it should be clear that the property fails
-- because simplifyZero will only optimize an expression like 2 + 0, and
-- not a more complex one like 2 + (2 + 0), since the addition of zero
-- occurs in a nested expression; however, our property wants all nested
-- occurrences to be simplified.  How should we fix this? We could
-- rewrite this function to recursively look into subexpressions and
-- attempt to optimize them too, but a better strategy is to define a
-- HIGHER ORDER function which does this for us.
--
-- 'peephole' is a higher order function that takes an optimizer
-- (like simplifyZero) and recursively applies it to every
-- subexpression of an expression.  If 'simplifyZero' is an optimizer
-- which only looks at the top-most expression, 'peephole simplifyZero'
-- is an optimizer which looks at all sub-expressions.

peephole :: (Expr -> Expr) -> Expr -> Expr
-- <FILL-IN>
peephole = question "[8 pts] COMPLETE THE DEFINITION"

-- </FILL-IN>

-------------------------------------------------------------------

--                  The instruction language

-------------------------------------------------------------------

-- Let's talk about the simple instruction language that you will be
-- compiling your expressions to.  This language is defined by the
-- following grammar:
--
--      b ::= + | - | *
--      i ::= OP b | PUSH n
--      is ::= i; is | {- empty -}          -- this is a list

data Instr
  = IOp BinOp
  | IPush Int
  deriving (Show, Eq)

-- What is the meaning of an instruction?  We are defining a stack
-- machine, so instructions do one of two things:
--
--  1. They "PUSH" a literal value onto the stack, or
--  2. They pop two values off the stack, do an "OP"eration on them,
--  and then pushes the result onto the stack.  Note that this could
--  FAIL if there are not enough values on the stack (a stack underflow
--  error).
--
-- For example, consider the following program:
--
--      PUSH 4; PUSH 1; OP -; PUSH 3; OP *
--
-- After running the first two instructions, we have a stack consisting
-- of [1, 4].  Running the minus OP consumes these two values and
-- we have a new stack [3]. Push 3 to get [3, 3], and then multiplication
-- gives us a final stack of [9].
--
-- We'll represent stacks as just lists of integers, where the head of
-- the list is the top of the stack.  Recall that in Haskell, lists are
-- pure data structures which cannot be mutated; we will "mutate" the
-- stack by returning a *new* stack from a function which represents the
-- updated stack.

type Stack = [Int]

-- We can interpret an instruction as taking a stack, performing
-- some operation on it, and returning the new stack after the
-- operation, but possibly failing.  Please implement 'step', which
-- does this.

step :: Instr -> Stack -> Maybe Stack
-- <FILL-IN>
step = question "[8 pts] COMPLETE THE DEFINITION"

-- </FILL-IN>

-- We should also tie this together, and write a function that
-- takes a stream of instructions and an initial stack, and executes
-- the instructions one by one until there are no more left.

-- Hint: Look up what foldM is and try to use it

run :: [Instr] -> Stack -> Maybe Stack
-- <FILL-IN>
run = question "[12 pts] COMPLETE THE DEFINITION"

-- </FILL-IN>

-------------------------------------------------------------------

--                  A simple compiler

-------------------------------------------------------------------

-- Finally, we are ready to implement a compiler for our expression
-- language.  This compiler takes an AST, and compiles it into a
-- sequence of instructions which, when evaluated on an empty
-- stack, results in a stack with one element that is equal to
-- the result of interpreting the expression directly. (You'll get
-- no points for cheekily emitting a single PUSH instruction!)
--
-- (Hint: This is a recursive function, and it can be implemented in
-- two lines.)

compile :: Expr -> [Instr]
-- <FILL-IN>
compile = question "[10 pts] COMPLETE THE DEFINITION"

-- </FILL-IN>

-- Your compiler is correct if running the instructions produced
-- by the compiler on an empty stack gives you the same result as
-- directly evaluating the expression.

-- Try compiling and running the example or come up with your own
-- examples to see if your calculator works correctly.

-------------------------------------------------------------------

--                  Utility code

-------------------------------------------------------------------

-- This section contains some utility code for the rest of the lab.
-- You may find it useful if you want to crib some syntax, or if
-- you're just curious what's going on behind the scenes.  Reading
-- this part is OPTIONAL.

-- We've also provided for you a simple decompiler which takes
-- a stream of instructions and translates it back into an expression
-- that would generate it if possible.

decompile :: [Instr] -> Maybe Expr
decompile is =
  case decompile' is [] of -- decompile' is defined below
    Just [e] -> Just e -- section of this lab
    _ -> Nothing

-- Our decompiler is implemented as a bit of abstract interpretation:
-- instead of maintaining a stack of integers, we maintain a stack
-- of expressions, and build them up as we execute instructions.

decompile' :: [Instr] -> [Expr] -> Maybe [Expr]
decompile' [] stk = Just stk
decompile' (i : is) stk =
  case decompileInstr i stk of
    Nothing -> Nothing
    Just stk' -> decompile' is stk'

decompileInstr :: Instr -> [Expr] -> Maybe [Expr]
decompileInstr (IPush n) stk = Just (Lit n : stk)
decompileInstr (IOp b) (y : x : rest) = Just (Op b x y : rest)
decompileInstr _ _ = Nothing

-- Helper pedantic function (ignore)
question = error
