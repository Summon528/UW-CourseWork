# Binary Search Trees

In this assignment, you will design key-value maps based on binary search trees
in Rust. If you need a refresher about how these datastructures work:

https://en.wikipedia.org/wiki/Binary_search_tree

In particular, all of your operations should maintain the central BST invariant:
from any node, all of the keys in the left subtree are less than the key of the
parent, and all of the keys in the right subtree are greater than the key of the
parent. Furthermore, all keys in a BST must be unique---no repetitions.

There are many variants on BSTs that achieve better balancing, like red-black
trees, AVL trees, B-Trees, etc. We won't worry about that too much---our goal is
just a basic BST. We'll already have our hands full with the Rust compiler.

## TreeMap functions

Implement the `TreeMap` methods in `bst.rs` without changing the function types.
You may use recursion in all functions (but see the extensions section).

## Iterators for TreeMap

In the second part of the assignment, you will design three kinds of iterators
for `TreeMap`s:

1. Consuming iterator (typically created with `.into_iter()`)
2. Borrowing iterator (typically created with `.iter()`)
3. Mutable iterator (typically created with `.iter_mut()`)

The first iterator consumes the whole `TreeMap`, returning owned data. The second
iterator "borrows" `TreeMap`, returning immutable references to data ("borrowed
values"). The third iterator returns *mutable* references to data, so that the
client can update the `TreeMap` in place. To make sure that the client doesn't
mess up the BST property, we will give out *immutable* references to keys and
*mutable* references to values for this last iterator. The other iterators
should give (key, value) pairs.

If you look for these functions for Rust's BTreeMap's, you'll see that the
standard library implements fancier iterators. For instance, the standard
library iterators are double-ended so you can get elements from the front or
elements from the back. We'll just implement regular iterators.

## Don't panic

Some standard functions in Rust may panic. The function `unwrap()` is one main
example. Avoid using these functions unless you really know that they will not
panic, for instance if you have already checked that the Option is not None.
There are usually better ways of getting values out from Option, like:
* pattern matching;
* [if-let](https://doc.rust-lang.org/rust-by-example/flow_control/if_let.html); and
* [while-let](https://doc.rust-lang.org/rust-by-example/flow_control/while_let.html).

## Debugging

For convenience, we've added the Debug trait bound everywhere so you can print
out values. A real version of the library would not have these bounds (since
`TreeMap`s should work on all types, not just ones with the Debug trait). You're
free to strip the Debug traits when you're done---we won't be testing them.

Rust has a `dbg!` macro that is quite nice. See examples
[here](https://doc.rust-lang.org/std/macro.dbg.html). Or, you can use regular
`println!` or `eprintln!` with a special formatting token:

```
println!("What is this value: {:?}", thing_with_debug);
```

We also have some basic tests, which you can run with `cargo test`. Keep in mind
that two of the required tests (`test_borrow_iter` and `test_mut_iter`) are
commented out at the bottom of the file. When your borrowing/mutable iterators
are ready, you should uncomment these tests.

## Tips

There's no way around it: you're going to meet the borrow checker. After working
at it, you'll get a better idea of what the errors mean as you improve your
mental model of how Rust "works", how the compiler reasons about your program.

You'll want to read the docs carefully (and examples), especially for:

* `Option`: https://doc.rust-lang.org/std/option/index.html

You'll be using the `take` function a lot. The `std::mem::swap` and
`std::mem::replace` functions may also be helpful.

* `Box`: https://doc.rust-lang.org/std/boxed/index.html

`Box` is a basic example of a *smart pointer*, something that looks and feels
like a regular reference, but with some special features. The basic idea is that
a `Box` points to some data located on the heap. Unlike standard references, the
`Box` owns the data it points to. We use Boxes to define inductive
datastructures like `TreeMap`, where the total size of the data is not known at
compile time.

For now, just treat `Box<T>` like a regular reference `&T` or `&mut T`, depending on
whether the Box is mutable or not. To make a new Box holding something, use the
constructor `Box::new(the_thing)`. If you have a Box `b`, you can dereference it
to get the thing that it's pointing at by doing `*b`. Just like for references,
you can call a method of the boxed thing via `b.foo()`.

These may look like magic, but as we will discuss this behavior and syntax is
enabled by Rust traits (`Deref`). Rust also has some "magic" for Box, special
syntax for this type. For instance, `*b` gives ownership of the boxed thing
whereas usually `*p` only gets a reference to the data.

* `BTreeMap`: https://doc.rust-lang.org/std/collections/struct.BTreeMap.html

Our API is based off of this collection, though the stdlib implementation is
much fancier (and much faster). Incidentally, the collections module has great
documentation:

https://doc.rust-lang.org/std/collections/index.html

* Indexing traits

These are the standard Rust traits for allowing indexing, things like `map[42]`.

https://doc.rust-lang.org/std/ops/trait.Index.html
https://doc.rust-lang.org/std/ops/trait.IndexMut.html

* `Iterator` trait: https://doc.rust-lang.org/std/iter/trait.Iterator.html

You need two ingredients to implement this trait: a type of items to return (an
"associated type") and a `next` function, which returns the next item and
updates the iterator state.

* Iterator conversion traits: FromIterator and IntoIterator 

https://doc.rust-lang.org/std/iter/trait.FromIterator.html
https://doc.rust-lang.org/std/iter/trait.IntoIterator.html

These are the traits for converting to/from iterators (e.g., `.collect()`).

These are the standard traits for working with range-like things, for instance
stuff created by `(1..10)` or `('a'..'z')`. Note that there is no requirement
that the range elements are ordered (i.e., implement `Ord`), but we are working
with ranges of keys and keys must be ordered.

Parts of this assignment are inspired by Learning Rust with Entirely Too Many
Linked Lists (TMLL):

https://cglab.ca/~abeinges/blah/too-many-lists/book/README.html

The material in 2-3 cover similar ground to what we are doing, but for boring
linked lists instead of amazingly cool BSTs. Keep in mind that TMLL was written
a few years ago, and the Rust language has advanced quite a bit in that time.
Many of the problems in that tutorial are no longer problems in Rust. On the
downside, some of the examples in TMLL no longer work with the current version
of Rust.

## Extension (5 points maximum)

This assignment has a few optional extensions. Note: the amount of work needed
to do these extensions is almost certainly not worth it for the extra point
values. You should only do these extensions if you're actually interested.

### Range iterators (2 extra points)

The standard `BTreeMap`s support range iterators for iterating over a range of
keys. Take a look at the standard library, and implement the following:

1. Borrowing range iterator (called with `.range(a..b)`)
2. Mutable range iterator (called with `.range_mut(a..b)`)

Some tips:

* You'll want to read up on ranges and bounds:

https://doc.rust-lang.org/std/ops/trait.RangeBounds.html
https://doc.rust-lang.org/std/ops/enum.Bound.html

* You will need to adapt the iterator structs.

* The standard range iterators implement a ton of other traits. Just implement
  the plain `Iterator` trait.

* Your range iterators should take ownership of the range.

* Do not iterate over items unnecessarily. For instance, do not just use a
  regular iterator, skip until you get into the range, and then continue from
  there.

### Avoiding recursion (2 extra points)

Recursion is not encouraged in Rust. For medium-sized data structures, recursive
operations will quickly blow up the stack. Implement all operations using loops
instead of recursion.

**HINT**: The `remove` operation is particular tricky to implement without
recursion, since it is easy to run into limitations of the borrow checker (even
if your code is sound). Here are some general tips:

* Try to borrow mutably as little as possible: when comparing, for instance, you
  can use `Option::as_ref()` to take a immutable borrow.
* Your logic will be simpler if you check whether the key is in the tree first,
  before doing anything---if not, you can simply return, and if yes, you can use
  `unwrap()` later without panicking.

### Custom Drop trait (1 extra point)

The built-in `Drop` trait behaves poorly because it drops `TreeMap` recursively.  If
the `TreeMap` too deep, this will blow up the call stack when dropping (see TMLL
2.7 for details).

Implement a custom `Drop` trait to drop the `TreeMap` in a loop rather than
recursing. The default `Drop` implementation for the `TreeMap`/`Node` types drops
according to a *pre-order* traversal: drop root, then drop left, then drop
right. Your implementation should maintain this order.

If you attempt this extension, you can uncomment the test `test_drop_big`. But
keep in mind that this test will first build a very deep tree, so if you haven't
implemented `insert` without recursion, the test will run out of stack space
even before the tree is dropped. You can do this extension with or without
doing the other extensions, though.

## Disclaimer

TMLL contains a very good discussion of writing your own linked list in Rust,
and the same conclusions carry over to writing BSTs. Essentially: there is no
good reason to do this in practice. You're going to be way better off just using
Rust's excellent standard libraries and collections, and if you're really doing
something weird you will almost always use unsafe Rust to write pointer-based
datastructures. The purpose of this assignment is to give you hands-on
experience in one shot with many core aspects Rust: ownership, borrowing,
references, pointers, iterators, traits, lifetimes, etc. If you ever need a map
collection in the future, you almost certainly don't want to make your own.
(This goes in essentially every language, not just Rust.)

## Submission

Submit your lib.rs file alone to Gradescope to run your code against the autograder
tests.