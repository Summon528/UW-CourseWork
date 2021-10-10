# Shell

Every input will go through the following process  
1. Parse  
    The `parse_input(char *)` function will parse the line and return an `Input_t` object
    ```c
    typedef struct Input {
        char* filename;  // If redirect, this will be the output filename, otherwise it will be null
        char** cmd;      // A NULL terminated list of tokens
    } Input_t;
    ```
2. Run  
    The `run(Input_t* input)` function will check if the command is one of the built-in command. The implementation of `exit` and `cd` is straightforward. For the `path` command, I maintain a dynamic string array `PATH` as well as its length `PATH_CNT`. If the command is a `loop` command, this function will substitute all `$loop` to a number, move the `cmd` pointer by 2 and call itself `$loop` times again.

3. Execute  
    If the command is not a built-in command, it will go through the `void exec(const Input_t* input)`, which will `fork` thep process and call `execv`. 