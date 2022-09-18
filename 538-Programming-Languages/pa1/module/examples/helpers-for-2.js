const for_ = require('../helpers.js').for_;

for_(0, _ => true, i => i+1, i => {
  console.log(`yay: ${i}`);
});

// is computationally equivalent to:

for(let i = 0; true; i++) {
  console.log(`yay: ${i}`);
}

// The key difference, however, is that our for_ construct will blow the stack.
// In fact, you should see a 'RangeError: Maximum call stack size exceeded'
// exception being raised by the underlying runtime system. This about why this
// is appending. We will explore this in the scope and storage management
// lecture.
