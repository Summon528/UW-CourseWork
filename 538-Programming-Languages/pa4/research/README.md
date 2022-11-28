# Research Path

In this project you will be implementing a memory management system 
according to a relatively simple API.

The first two projects are

## Tasks

[Python] Memory Management
1. Implement malloc + free (alloc.py)
2. Implement reference counting (reference.py)
3. [ Bonus ] Implement liveness garbage collector
4. [ Bonus ] Implement ownership

## Submission

Submit your alloc.py and reference.py files.

Also provide a brief writeup named `RESEARCH.md` answering the following questions:

[ Alloc ]
1. What tradeoffs did you make when implementing malloc/free?
2. How does your system handle (or ignore) use-after-free and double-free errors?
3. Justify any modifications you made to the alloc.py file other than replacing TODOs.
4. Who is responsible for errors in your system? The allocator or the programmer? (For example, using Ptr.set() with an offset that is too large or negative)

[ Reference Counting ]
5. How would you augment this project to allow for cycle detection in references? What constructs would you need?
6. If each object could only have a single owner, (like in Rust), how would that simplify our logic in this project?

[ Performance ]
7. Evaluate the performance of your allocator and reference counting implementation. Then consider an alternative implementation. How would your performance change? In what scenarios is your implementation better than the alternative and visa versa.*

Note, 7 is a vague question on purpose. You will have to define an evaluation technique and justify it.
