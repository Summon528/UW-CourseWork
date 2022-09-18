const for_ = require('../helpers.js').for_;

for_(0,
     i => i < 10,
     i => i+1,
     i => {
       console.log(`yay: ${i}`);
});

// is computationally equivalent to:

for(let i = 0; i < 10; i++) {
  console.log(`yay: ${i}`);
}

// which is equivalent to:

{
  let i = 0;
  for_(null,
      _ => i < 10,
      _ => i++,
      _ => {
        console.log(`yay: ${i}`);
  });
}
