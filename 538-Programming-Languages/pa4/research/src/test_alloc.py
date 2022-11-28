from alloc import malloc, free

myPointer = malloc(10)  # Get 10 bits of space
myPointer.set(True)  # Set first bit to True
for i in range(0, 10, 2):
    myPointer.set(True, i)  # Set every other bit to True

assert myPointer.deref() == True
assert myPointer.deref(1) == False
assert myPointer.deref(2) == True
assert free(myPointer)
