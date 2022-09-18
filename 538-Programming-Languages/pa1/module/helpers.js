const { List } = require('immutable');
const assert   = require('assert');

/* QUESTION 1 (5pts):
 *
 * You will implement a for-loop like construct, for_, using first-class
 * functions.
 *
 * for_ takes 4 arguments:
 *
 *   - cur: the initial current value
 *
 *   - cond: the function that is used to decide whether or not we should
 *   execute the "loop" body. The cond function expects an argument whose type
 *   is the same as that of cur; the cond function is expected to return a
 *   boolean.
 *
 *   - next: the function that is used to compute the "next" current value.
 *   Like cond, next expects an argument whose type is the same as that of cur;
 *   the next function is expected to return a value of the same type.
 *
 *   - fbody: the function corresponding to the loop body. This function is
 *   called with the cur value as an argument.
 *
 *   See examples/helpers-for-{1,2,3}.js for examples that demonstrate how to
 *   use for_ in place of for-loops. These files also describe the expected
 *   behavior of for_.
 */
function for_(cur, cond, next, fbody) {
  /** <FILL-IN> **/
  /** </FILL-IN> **/
}
exports.for_ = for_;

/* QUESTION 2 (5pts):
 *
 * Using the above for_ construct, you will now implement a construct for
 * iterating over elements of an array. each takes a list and a function f. For
 * each element in the list, the function calls f with the element and the
 * index of the element in the list.
 */
function each(list, f) {
  if (list.size == 0) {
    return;
  }
  /** <FILL-IN> **/
  /** </FILL-IN> **/
}
exports.each = each;

/* Below are some simple functionality tests.
 * You may wish to test your code more extensively.
 */
{
  let nr = 0; // track number of times function was e
  each(List([1, 2, 3, 4, 5]), (el, idx) => {
    nr++;
    assert(el === idx + 1);
  });
  assert(nr === 5);
}

{
  let nr = 0;
  each(List([]), _ => { nr++; });
  assert(nr === 0);
  each(List([1]), _ => { nr++; });
  assert(nr === 1);
}
