# Pzip

- First, the main thread divides each file into chunks where each chuck has the size of WORKSIZE
- The main thread then submit all the jobs to a queue
- The main thread create child threads
- For every child thread, it identifies the leading and ending character and produce the zipped string representation of the middle part
- e.g. `aabbbbcccddd` will produce `front = 'a', flen = 2, str = "4b3c", back = 'd', blen = 3`
- Finally, each thread will wait for its turn to output and handle the overlapping part between the current chuck and the previous chunk

## Runtime
```sh
[tsengcy@royal-30] (4)$ time ~cs537-1/tests/p3a/test-pzip.sh
test 1: passed
test 2: passed
test 3: passed
test 4: passed
test 5: passed
test 6: passed
test 7: passed
test 8: passed
test 9: passed
test 10: passed
test 11: passed
test 12: passed
test 13: passed
test 14: passed
test 15: passed
test 16: passed
test 17: passed

real    0m8.799s
user    0m7.966s
sys     0m2.936s
```