# KV
Simple Key Value Database

See: https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/initial-kv

## Implementation
Data is store in a simple dynamic array, every entry in the array is a `kv_pair` struct.  
Whenever the program start, it will read `kv.txt` and load the data to memory, when it quit it will write memory to the text file.  
The `kv.txt` is stored is the following format
```
key1,value1
key2,value2
key3,value3
```