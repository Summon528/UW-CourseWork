# Scheduling
- Add `tickets` and `tick` field to `struct proc`
- Implement `settickets` to set current process's `tickets`
- Implement `getpinfo` that will fill out a `pstat` by interating all the processes
- Modify `scheduler()` so that a process will run `tickets` time before finding next process to run and increment `tick` when running
- Modify `allocproc()` and `fork()` so that `tickets` will init properly

# Memory
- Add `-Ttext 0x1000` to `Makefile` so the code will load to the first page
- Modify `exec()` `copyuvm()` to skip the first page