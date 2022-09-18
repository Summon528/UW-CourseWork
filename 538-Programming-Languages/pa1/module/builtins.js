const { List } = require('immutable');
const assert   = require('assert');
const _        = require('./helpers.js');

/*
 * This module exports an API for checking if something is a built in library
 * and getting the module if it is.  For this exercise, we're only going to
 * expose some of the core modules (below). If you are curious, you can see the
 * list of all the modules and documentation here:
 * https://nodejs.org/dist/latest-v8.x/docs/api/
 *
 * The API exposed by this module consists of two functions:
 *
 * - isBuiltIn: takes the name of a module and returns true if the module is a
 *   built-in, and false otherwise.
 *
 * - getBuiltIn: takes the name of a built-in module and returns the module
 *   object. A module is only loaded once, so this module always returns the
 *   same object for a corresponding name. (If a bad name -- a name that
 *   doesn't correspond to a built-in module -- is given, this function throws
 *   an exception.)
 */

const names = List([
  'assert',
  'buffer',
  'console',
  'events',
  'fs',
  'os',
  'path',
  'process',
  'readline',
  'stream',
  'timers',
  'util'
]);

/* We are going to use Node.js' require() to load the built-in modules.  Since
 * require()ing modules ahead of time is not cheap (it amounts to reading
 * files, etc. as you will see in this problem), we are going to lazily
 * require() built-ins modules (and always return the same loaded object once
 * it has been required).  Specifically, we are only going to actually
 * require() a module when getBuiltIn() is actually called.
 *
 * Our public API is built around a local cache object.  This cache object is a
 * mapping from names to special functions (called *thunks*). Each function,
 * when called by getBuiltIn(), will return the corresponding module object. In
 * doing so, these functions may need to call require() to actually load the
 * built-in module.
 *
 * We will initialize this cache below with all the built-in modules.
 */
const cache = {};
exports._cache = cache;

/* QUESTION 3 (5pts):
 *
 * To get you started, implement the simple isBuiltIn function. This function,
 * as describe above, should return true if name corresponds to a built-in
 * module and false otherwise. You may assume that the cache has already been
 * initialized with the built-in modules, but don't need to.
 *
 * We have included a test at the end of this file and in tests/builtins.js
 * that you can use to test your implementation.
 */
function isBuiltIn(name) {
  /** <FILL-IN> **/
  /** </FILL-IN> **/
}
exports.isBuiltIn = isBuiltIn;

/* The getBuiltIn function, as also described above, returns the module object
 * corresponding to the built-in `name`. 
 *
 * This function assumes that the cache has been properly initialized (your
 * task). Make sure to read the code and understand it.
 */
function getBuiltIn(name) {
  assert(isBuiltIn(name));
  // Get the function (thunk) in the cache that corresponds to name.
  const thunk = cache[name];
  // Calling the function should return the module object, loading it via
  // require() if necessary.
  return thunk();
}
exports.getBuiltIn = getBuiltIn;

/* QUESTION 4 (15pts):
 *
 * We will initialize the cache by iterating over all the built-in module
 * names.  Specifically, for each built-in module name `name` we will set
 * `cache[name]` to a function which should return the corresponding module
 * object. If the module has not already been loaded with require(), this
 * function should do so. Importantly, however, it should only call require()
 * once for any name. So, for example, the incorrect (but very close) solution
 * is:
 *
 *   cache[name] = () => require(name);
 *
 * The problem with this solution is that it calls require() every time
 * getBuiltIn() is called. Nevertheless, if you're having trouble with this,
 * you may wish to use this (incorrect solution) while you're working on the
 * rest of the problem and get back to it later.
 *
 * See tests/builtins.js for some functionality tests.
 */
_.each(names, name => {
  cache[name] = /** <FILL-IN> **/ () => require(name); /** </FILL-IN> **/
});

/* Simple isBuitlIn sanity-check test. You are encouraged to write additional
 * tests.
 * */
_.each(names, name => {
  assert(isBuiltIn(name));
});
