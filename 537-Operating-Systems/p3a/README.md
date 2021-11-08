# Pzip

- First, the main thread divides each file into chunks where each chuck has the size of WORKSIZE
- The main thread then submit all the jobs to a queue
- The main thread create child threads
- For every child thread, it identifies the leading and ending character and produce the zipped string representation of the middle part
- e.g. `aabbbbcccddd` will produce `front = 'a', flen = 2, str = "4b3c", back = 'd', blen = 3`
- Finally, each thread will wait for its turn to output and handle the overlapping part between the current chuck and the previous chunk