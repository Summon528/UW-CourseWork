const { List } = require('immutable')
const assert = require('assert')

/*
  In this exercise you will implement quicksort in JavaScript, using high-order
  functions. We have provided some scaffolding and broke the problem into
  several different tasks.
*/

/* TASK 1 (10pts):
  Your first task is to write the definition of fold_left. This function is
  sometimes called reduce; you may remember it from its brief mention in class
  or its popularization by MapReduce.
 
  This function takes in 3 arguments as follows:
 
  1. f    - This is the folding function
  2. base - This is the base value of the accumulator
  3. ls   - This is the list which is to be folded over
 
  The goal of fold_left is to fold a list into a single value.
 
  It starts from the beginning (left-side) of the list and applies the folding
  function with the base value and the 1st element of the list.  The value
  returned by applying this function is then used as the new base value for
  the folding function and the next element of the list is used as the other
  parameter. This is done until the list is exhausted, the final accumulated
  value being the return value.
 
  fold_left is implemented recursively, applying the folding function on the
  accumulator and the first element each time, and then recursively passing the
  new accumulator and the remaining list. The accumulated result is returned
  when the list is empty.
 
  For example, let's take a look at a function that uses fold_left.
*/

const sumList = (ls) => fold_left((acc, x) => acc + x, 0, ls);

/*
  The sumList function takes a list (`ls`) and returns the sum of the elements
  in the list.
  
  The inline folding function takes the current accumulator (`acc`) and next
  element in the list (`x`) and simply returns their sum.
  The base (initial accumulator) value when summing a list starts as 0.

  The correct behavior of this function, when expanded into call stacks, looks
  like this:

    fold_left((+), 0, List([1,2,3]))
      fold_left((+), 1, List([2,3]))
        fold_left((+), 3, List([3]))
          fold_left((+), 6, List([]))
            6
          6
        6
      6
    6


  Note: The list that's passed into fold_left (and all subsequent functions) is
  of type immutable.List. You will learn more about the benefits of immutability
  in future lectures. For this assignment, you should look at the following
  docs:

  https://facebook.github.io/immutable-js/docs/#/List

  Some useful List functions for this exercise in particular are:

  size()
  first()
  last()
  push()
  pop()
  unshift()
  shift()
  get()
  slice()
  concat()

  You can read more about them in the link above.

  Implement fold_left below following the above description and example.
*/

const fold_left = function (f, base, ls) {
  if (ls.size == 0) {
    return base;
  }

  // Write the recursive fold_left call
  return /** <FILL-IN> **/ undefined; /** </FILL-IN> **/
};

/* TASK 2 (10pts):
  A good way to think about fold is that it _folds_ a list into a single value.
  This single value can, of course, also be anything -- even a list!
  To emphasize this, your next task is to write an implementation of the map
  function we saw in class using fold_left.

  map takes in 2 arguments:

  1. g  - A function that is to be mapped over a list.
  2. ls - The list that is to be mapped over.

  Here's an example of how map is supposed to work:

  map(x => x + 1, List([1, 2, 3])) ------> List([2,3,4])
  map(x => x * 2, List([1, 2, 3])) ------> List([2,4,6])

  You can normally do this using a loop construct, but your constraint here is
  to define map using fold_left only.

  Hint: the base value could be an empty list (List([])).
*/

const map = (g, ls) => /** <FILL-IN> **/ undefined; /** </FILL-IN> **/


/* TASK 3 (7.5pts):
  In a similar fashion to map, your next task is to define the filter function
  using fold_left.

  filter takes in 2 arguments:
  
  1. g  - This is a function that decides whether an element in the list is
          permitted or not. If the function returns true, then the element is
          permitted, otherwise it's not -- it's filtered out.

  2. ls - This is the list that passes through filter function g.

  Some examples of filter are:

  filter(x => x <= 2, List([1,2,3,4])) ------> List([1,2])
  filter(x => x % 2 == 0, List([1,2,3,4])) ------> List([2,4])

  Again, your constraint is to use fold_left only (no loops!).
*/

const filter = (g, ls) => /** <FILL-IN> **/ undefined; /** </FILL-IN> **/


/* TASK 4 (7.5pts):
  Similar to filter we have another function called partition. This function
  partitions a list into 2 separate partitions based on a predicate function.

  Specifically, partition takes 2 arguments:

  1. g  - This function decides which partition an element of the list falls into
          If the function returns true, then it goes into the left partition,
          otherwise it goes into the right partition.
  
  2. ls - This is the list that is to be partitioned.

  Some examples of partition are:

  partition((x) => x <= 2, List([1,2,3,4])) ------> List([List([1,2]), List([3,4])])
  partition((x) => x % 2 == 0, List([1,2,3,4])) ------> List([List([2,4]), List([1,3])])

  For this task, your constraint is to write this function using filter. You
  are not allowed to use other looping constructs, even fold_left.
*/

const partition = (g, ls) => /** <FILL-IN> **/ undefined; /** </FILL-IN> **/


/* TASK 5 (15pts):
  Now that we have all the building blocks, our final task is to implement a
  naive version of quicksort.
  
  Here's an explanation of how quicksort works:

  Given a list to sort, pick an element x from the list as a pivot. Now the
  remainder of the list is divided into two partitions, one partition with all
  the elements <= x, the other partition with all the elements > x. Now
  recursively call the quicksort algorithm on each partition and then combine
  the two partitions again (don't forget to put the pivot back in as well!).

  The base case for the recursion would be when there's only 1 or 0 elements
  in a partition, in which case the partition is returned as is. (The inductive
  case is described above.)

  Quicksort looks pretty scary in most languages, but with the help of
  functional programming, it should only be a few lines of code.

  For simplicity, pick the first element of the list as the pivot and the
  remaining list as the list to be partitioned.
*/


const quicksort = function(ls) {
  if(ls.size <= 1) {
    return ls;
  }

  /** <FILL-IN> **/
  return undefined;
  /** </FILL-IN> **/
};


/*
  Here are some asserts to check whether your implementations are correct. You
  may wish to come up with additional tests and examples.
*/

assert(sumList(List([1,2,3,4])) == 10);
assert(map((x) => x * 2, List([1,2,3,4])).equals(List([2,4,6,8])));
assert(filter((x) => x <= 2, List([1,2,3,4])).equals(List([1,2])));
assert(filter((x) => x % 2 == 0, List([1,2,3,4])).equals(List([2,4])));
assert(partition((x) => x <= 2, List([1,2,3,4])).equals(List([List([1,2]), List([3,4])])));
assert(partition((x) => x % 2 == 0, List([1,2,3,4])).equals(List([List([2,4]), List([1,3])])));
assert(quicksort(List([4,7,3,6,8,7,1,2,2,1,5])).equals(List([1,1,2,2,3,4,5,6,7,7,8])));

exports.fold_left = fold_left;
exports.map       = map;
exports.filter    = filter;
exports.partition = partition;
exports.quicksort = quicksort;
