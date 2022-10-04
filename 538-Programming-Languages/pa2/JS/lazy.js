"use strict";
// In this lab, you will use JavaScript's first class functions and
// closures to implement Haskell-style lazy evaluation (call-by-need).
//
// You can run this file by running the following command:
//
//      node lazy.js
//
// In the beginning, you should get an exception on the enumFrom test.

// ----------------------------------------------------------------- //
//                          Lazy Evaluation
// ----------------------------------------------------------------- //

// There are two basic properties about lazy evaluation.
//
//  1. When an expression is 'delay'ed, it is not evaluated immediately;
//  instead we return a *thunk* which evaluates the expression when
//  it is 'force'd (i.e., when we need it).
//
//  2. Once a delayed expression is forced, it is never evaluated
//  again: the result is cached and returned directly if the thunk
//  is forced again.
//
// Here is a simple example of creating a thunk and then forcing it
// twice.  Notice that the expression we pass to delay has to be written
// in a function() { ... } expression, since otherwise JavaScript will
// eagerly evaluate it!

console.log("== TEST: Memoization ==");

let t = delay(function() { console.log("Call me once"); return 2; });
console.log("1st force");
console.log(force(t));
console.log("2nd force");
console.log(force(t));

// Question 1 (5pts):
// A simple implementation which doesn't work is to have 'delay' simply
// return the function it is given, and 'force' call this function.
// This fulfills property (1), but not property (2).  WITHOUT changing
// 'force', fix delay so that the computed expression is only evaluated
// once.  Hint: represent the thunk as a CLOSURE. 

function delay(f) {
  let ran = false;
  let ret;
  return () => {
    if (ran) {
      return ret;
    } else {
      ran = true;
      return ret = f();
    }
  }
}

function force(t) {
  return t();
}

// ----------------------------------------------------------------- //
//                          Streams
// ----------------------------------------------------------------- //

// With delay and force, we can implement infinite streams of
// numbers.  A stream is simply a thunk that, when forced, gives
// you an object containing 'head', the head element of the
// stream, and 'tail', a thunk which can be forced to give
// you the rest of the stream.  Here are two example functions:

// 'repeat' accepts an element n, and creates a stream of that
// element repeated, e.g., [n, n, ...].

function repeat(n) {
  return delay(function() {
    return {
      head: n,
      tail: repeat(n)
    };
  });
}

// 'take' accepts a number n and a stream, and returns a list (actually, a
// JavaScript array) of the first n elements of the stream.  If you want to see
// what a stream looks like, we suggest you 'take' some elements and print them
// to the console.

function take(n, thunk_xs) {
  let r = [];
  for (let i = 0; i < n; i++) {
    let xs = force(thunk_xs);
    r.push(xs.head);
    thunk_xs = xs.tail;
  }
  return r;
}

// Here's a simple example of these two functions working together.
// You can try changing the number of elements which are taken and
// observe that the list is as long as you want.

console.log("== TEST: repeat ==");
console.log(take(5, repeat(2)));

// ----------------------------------------------------------------- //
//                          Exercises
// ----------------------------------------------------------------- //

// Please implement the following functions.  You should follow
// the naming convention that thunk_xs represents a thunk which must be
// forced before it can be used, whereas xs is an actual object; you
// will probably find it helpful to follow this convention.
// Additionally, the function 'repeat' and others like it returns a
// THUNK (the delayed function, however, should return a real object!).

// Question 2 (5pts):
// enumFrom accepts a number n, and creates an stream of the
// numbers [n, n+1, n+2, ...]

function enumFrom(n) {
  return delay(() => ({
    head: n,
    tail: enumFrom(n + 1)
  }));
}

console.log("== TEST: enumFrom ==");
console.log(take(5, enumFrom(2)));

// Question 3 (5pts):
// map accepts a function f and an stream [x_0, x_1, ....]
// and returns a new stream [f(x_0), f(x_1), ...]

function map(f, thunk_xs) {
  return delay(() => {
    const { head, tail } = force(thunk_xs);
    return {
      head: f(head),
      tail: map(f, tail)
    }
  });
}

console.log("== TEST: map ==");
let xs = map(function(x) { console.log("Multiplying " + x); return x * 2 }, enumFrom(4));
console.log("Taking... (this should happen before any Multiplying)");
console.log(take(5, xs));

// Question 4 (10pts):
// zipWith accepts a function f and two streams [x_0, x_1, ...]
// and [y_0, y_1, ...], and returns a new stream
// [f(x_0,y_0), f(x_1,y_1), ...]

function zipWith(f, thunk_xs, thunk_ys) {
  return delay(() => {
    const x = force(thunk_xs);
    const y = force(thunk_ys);
    return {
      head: f(x.head, y.head),
      tail: zipWith(f, x.tail, y.tail)
    }
  });
}

console.log("== TEST: zipWith ==");
console.log(take(5, zipWith(function(x, y) { return x > y },
  enumFrom(8),
  map(function(x) { return x * x }, enumFrom(1)))));

// Question 5 (5pts):
// tail
//      Accepts a stream [x_0, x_1, x_2, ...],
//      Returns a new stream [x_1, x_2, ...].
//
function tail(thunk_xs) {
  // Why?
  return delay(() => ({
    head: force(force(thunk_xs).tail).head,
    tail: force(force(thunk_xs).tail).tail
  }));
}

console.log("== TEST: tail ==");
console.log(take(5, tail(enumFrom(2))));

// Question 6 (5pts):
// cons
//      Accepts an element x and a stream [y_0, y_1, ...],
//      Returns new stream [x, y_0, y_1, ... ].
function cons(head, thunk_tail) {
  return delay(() => ({
    head,
    tail: thunk_tail
  }));
}

console.log("== TEST: cons ==");
console.log(take(5, cons(1, enumFrom(2))));

exports.delay = delay;
exports.force = force;
exports.repeat = repeat;
exports.take = take;
exports.enumFrom = enumFrom;
exports.map = map;
exports.zipWith = zipWith;
exports.cons = cons;
exports.tail = tail;
