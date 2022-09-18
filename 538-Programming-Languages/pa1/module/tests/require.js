const assert   = require('assert');
const require_ = require('../require.js').makeRequire(__dirname);


// require_ should return the same thing as require() for built-ins
assert(require_('fs') === require('fs'));
assert(require_('util') === require('util'));

// load relative modules that circularly load each other:
const a = require_('./require/a.js');
const b = require_('./require/b.js');
assert(a.ok);
assert(a.x === 33);
assert(a.filename == require.resolve('./require/a.js'));
assert(a === a.b.a);
assert(a.b.y === 42);
assert(a.b === b);
assert(b.a === a);

// load a module that redefines module.exports to a function:
const c = require_('./require/c.js');
assert(typeof c === 'function');
assert(c(42) === 44 + 42);

// load a module based on directory that only contains index.js:
const d = require_('./require/d');
assert(d.foo === 'bar');

// load a json object
const e = require_('./require/e.json');
assert(e.name === 'problem set 0');
assert(e.x.y === 1337)
