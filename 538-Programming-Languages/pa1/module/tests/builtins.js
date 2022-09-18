const builtIns = require('../builtins.js');
const _        = require('../helpers.js');
const assert   = require('assert');

const isBuiltIn  = builtIns.isBuiltIn;
const getBuiltIn = builtIns.getBuiltIn;

// some things are built ins, others are not
assert(isBuiltIn('fs'));
assert(isBuiltIn('util'));
assert(!isBuiltIn('w00t'));

// make sure that getBuiltIn throws for non builtins
assert.throws(() => getBuiltIn('w00t'));

// The loaded built-in is the same as that returned by require
assert(getBuiltIn('fs') === require('fs'));
// For sanity, let's make sure fs and fs are not the same:
assert(getBuiltIn('fs') !== require('util'));

// Same as above, but for util not fs
assert(getBuiltIn('util') === require('util'));
assert(getBuiltIn('util') !== require('fs'));
