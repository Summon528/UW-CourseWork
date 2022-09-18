const for_ = require('../helpers.js').for_;

const str = "hello world!";

// From http://stackoverflow.com/questions/14696261/declaring-two-variables-in-a-for-loop

{
  let i, c;
  for(i = 0, 
      c = str.charAt(0);
      i < str.length;
      ++i, c = str.charAt(i)){
    console.log(`char at ${i} is '${c}'`);
  }
}

// is more cleanly expressed as:
  
for_({i: 0, c: str.charAt(0) },
      cur => cur.i < str.length,
      cur => ({ i: cur.i + 1, c : str.charAt(cur.i + 1) }),
      cur => {
    console.log(`char at ${cur.i} is '${cur.c}'`);
});

// One of the big reasons our for_ code is cleaner come from the fact that we
// are not using the comma operator:
// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Comma_Operator
// After the lambda calculus lecture, in the Haskell lecture, we will see how
// you can make for_ even cleaner, to for example let you thread the i and c independently.

