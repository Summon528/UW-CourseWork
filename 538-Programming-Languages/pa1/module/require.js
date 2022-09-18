const path = require('path');
const fs   = require('fs');
/* In this assignment you will get a chance to explore a number of JavaScript
 * language features and Node.js APIs by implementing a simple module system.
 * Specifically, you will implement a require()-like function that will load
 * Node.js modules. See
 * https://nodejs.org/dist/latest-v6.x/docs/api/modules.html#modules_all_together
 * for an explanation of Node.js' actual algorithm. We will only follow this in
 * spirit.
 *
 * For example, for this exercise we will only handle modules that are:
 *
 * - built-in modules (e.g., 'fs' or 'util')
 * - modules that named by relative/absolute file paths
 *
 * We will not support loading modules such as 'immutable' that are stored in
 * node_module directories. We will, however, explore the loading of different
 * _kinds_ of modules: JavaScript modules and JSON modules.
 *
 * This module is the entry point. However, to get this to work you will first
 * need to define some helper utilities. We recommend solving this problem in
 * the following order:
 *
 * - QUESTION 1 and 2 from ./helpers.js
 * - QUESTION 3 and 4 from ./builtins.js
 * - QUESTION 5 and 6 from ./require-json.js
 *
 * Once you've addressed these questions, you should be able to run the tests
 * in tests/require.js. These tests test the module system implementation. You
 * may, however, wish to implement additional tests and modify/comment-out some
 * tests from our sample set while you're working.
 */

/* To get started, we will define a few regular expressions to check if a
 * module file path starts with './', '../', or '/'. If you have never played
 * with regular expressions, see
 * https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp
 *
 * For this assignment, you don't need to understand anything about regular
 * expressions. (Though, in practice they often come in handy and are written
 * more cleverly than we did below.)
 */
const dot    = new RegExp('^./');
const dotdot = new RegExp('^../');
const slash  = new RegExp('^/');

/* We are going to load our built-ins module that that handles require()ing
 * built-in modules. */
const builtIns = require('./builtins.js');

/* Since modules can load files with relative paths (e.g.,
 * require('./some-file.js')), we need to make sure that the require function
 * knows what directory the relative paths are to. Suppose we have several
 * modules, structured according to this directory tree:
 *
 *    /dir1/file1.js
 *    /dir1/file2.js
 *    /dir1/dir2/file1.js
 *    /dir1/dir2/file2.js
 *
 * If the module code in /dir1/dir2/file1.js calls require('./file2.js'),
 * the function should load the module at /dir1/dir2/file1.js. Similarly,
 * require('./file2.js') in file /dir1/file1.js should load the module at
 * /dir1/file1.js.
 *
 * We will leverage closures to ensure that require() knows about relative
 * paths. Specifically, we will use makeRequire to create our require
 * functions, with the directory name bound as dirname:
 */
function makeRequire(dirname) {
  /* Return the require() function that module code will execute. Note, that we
   * use the dirname passed to makeRequire within the function body.
   *
   * The returned require() function is straight forward. It simply checks the
   * name of the module that we're trying to load and calls the corresponding
   * loader. The function knows how to resolve relative paths (step 2a) by
   * assuming dirname to be the containing dir of the module that will use the
   * returned require() function.
   */
  return function (name) {
    // 1. If name corresponds to a core library such as 'fs', just return the
    // built-in module:
    if (builtIns.isBuiltIn(name)) {
      return builtIns.getBuiltIn(name);
    }
    // 2a. If the name starts with './', '../', let's load the file/directory
    // after appending the name relative to dirname:
    else if (dot.test(name) || dotdot.test(name)) {
      return loadFile(path.join(dirname, name));
    }
    // 2b. If the name starts with '/', let's load the file/directory:
    else if (slash.test(name)) {
      return loadFile(name);
    }
    // 3. Not a built-in or path file, fail:
    else {
      throw new Error(`Not sure how to load ${name}`);
    }
  };
}
exports.makeRequire = makeRequire;

/* The heavy lifting of require() is done by loadFile(). This function reads
 * source files, evaluates them, etc. Since it's costly to always read files
 * from the filesystem, we will use a cache (like Node.js' module system) to
 * map module names to the corresponding module objects.
 *
 * This cache will be tried first when loading files. We will not cache
 * built-in modules since that is handled by the builtIn library.
 */
const cache = {};
exports._cache = cache;

/* When loading a module, we need to know what to do depending on the file
 * extension. For example, for '.js' files we will want to properly load the
 * code (with eval). In this assignment, we will also handle '.json' files
 * (much like Node.js' require). In general we will want to expose an interface
 * that allows users to add their own custom loaders. For example, after this
 * assignment you should be able to load '.coffee' files (by calling the
 * CoffeeScript to JavaScript compiler in the loader).
 *
 * loadFile() below handles all the loading for require(). It does so by first
 * resolving the filename of the file to load (using Node.js' resolution
 * algorithm). Then, it checks the cache to see if the module has already been
 * loaded. If it has, it simply returns the cache exports object. If it has
 * not, it will call the default file loader (`exports.loader`).
 *
 * exports.loader is a function that is called with the resolved name of the
 * module to load and the nextLoader. The default loader defined below handles
 * '.js' files by calling loadJSFile(). All other file types are passed on to
 * nextLoader.
 *
 * Note that loadFile() calls the exports.loader function with a nextLoader
 * that indicates failure -- this is the last loader that notifies the user of
 * require() that there was no viable loader to handle the file.
 *
 * We are deliberately defining the default loader as a property on the
 * `exports` object.  This allows user code (your code) to extend how the
 * module system handles files.
 *
 * For example, in the ./require-json.js you will define a loader the handles
 * '.json' files.
 */
exports.loader = (resolvedName, nextLoader) => {
  if (path.extname(resolvedName) === '.js') {
    return loadJSFile(resolvedName);
  } else {
    return nextLoader(resolvedName);
  }
};

function loadFile(name) {
  // Resolve the name using Node.js' resolution strategy. Even when name
  // actually points to a directory, _resolveFilename will resolve the
  // proper javascript file to load.
  const resolvedName = require('module')._resolveFilename(name);

  // Check to see if this module is already loaded, if it is return the cached
  // module exports.
  if (resolvedName in cache) {
    return cache[resolvedName].exports;
  }

  // Get the loader to try. By default the loader is the function define above
  // that handles '.js' files.  As we will see, the loader will be redefined
  // (mutated) by user code to add support for handling '.json' files.
  const defaultLoader = exports.loader;

  // Call the loader and provide a fall-back loader that throws an exception.
  // In practice we would allow users to also add their own fall-back loader.
  return defaultLoader(resolvedName, () => {
    throw new Error(`Failed to load ${resolvedName}`);
  });
}

/* This function function loads the javaScript module named by the absolute
 * path `resolvedName`. The function wraps the module source to properly expose
 * the variables it expects (e.g., exports) and evaluates it. 
 *
 * You will not need to modify this function. You don't even need to completely
 * understand what this function is doing for this assignment. You will
 * hopefully completely understand it after the lambda calculus lecture.
 *
 * For this assignment, it may be informative to read this source when
 * implementing the '.json' handlers.
 */
function loadJSFile(resolvedName) {
  // Read the file contents
  const code = fs.readFileSync(resolvedName, 'utf8');
  // Create the module object (much as we did in class):
  const mod = { 
    exports : {} // this will be populate by the module code
  };
  // Add module object to the cache before loading the module code; this
  // ensures that we can load modules that depend on eachother.
  cache[resolvedName] = mod;

  // Create variables that are "global" to the module:
  const filename = resolvedName; // name of the module
  const dirname  = path.dirname(filename); // containing directory
  const require_ = makeRequire(dirname);
  // Wrap the code in a function expression and eval it. Foreshadowing to the
  // lambda calculus lecture: this capture the "global" (free) variables of the
  // module.
  const moduleFunc = 
    eval(`(function (exports, require, module, __filename, __dirname) {${code}});`);
  // Call the function expression with:
  // - `this` set to mod.exports
  // - `exports` set to mod.exports
  // - `require` set to require_
  // - `__filename` set to filename
  // - `__dirname` set to dirname
  //
  // This, for example, ensures that when the code sets a property on the
  // exports object, a new exports object is in scope. Similarly, it ensures
  // that __filename corresponds to the module filename. And so on.
  // These variables shadow the one in this (require.js) file.
  //
  // Again, foreshadowing the lambda calculus lecture: exports, require,
  // module, __filename, __dirname are free variables in the open term, code.
  // By wrapping the open term with a lambda (the function (..) {}) we are
  // capturing these variables and producing a closed term (via eval).

  // Actually run the module code:
  moduleFunc.call(mod.exports, mod.exports, require_, mod, filename, dirname);

  // In a more realistic scenario we would check if this threw an exception and
  // remove the module from the cache since the loading failed.

  // Return the exports object:
  return mod.exports;
}

/* Extend the default loader with support for JSON. You can comment out the
 * line below if you are just testing out .js files. */
require('./require-json.js');
