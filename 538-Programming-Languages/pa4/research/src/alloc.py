"""The Allocator

alloc.py contains the methods and implementations for handling direct memory
allocation. In this program, "memory" will be treated as a single array
each of which should contain either a 0 (False) or 1 (True).

In our implementation we get some benefits of abstracting memory, in that
our own memory manager will not be using the same memory as the one we are
implementing.

Memory is _bit_ addressable for our purposes.

You may edit any part of this file, so long as you keep the api of malloc and free.
"""

# "Memory" contains 1 Mb (megabit) of space
# Note: _memory actually contains 8MB of memory because Python.
_memory = [False for _ in range(2**20)]


def TODO(err="Code not implemented"):
    raise Exception(err)


class Ptr:
    loc: int

    def __init__(self, loc):
        self.loc = loc

    def addr(self) -> int:
        return self.loc

    def deref(self, offset: int = 0) -> bool:
        """Returns the value in memory at this location + offset."""
        TODO()

    def set(self, val: bool, offset: int = 0) -> None:
        """Set the value in memory at this location + offset."""
        TODO()


def malloc(n: int) -> Ptr:
    """
    Allocate memory in _memory for external usage.

    Params:
        n: int - The number of bits to allocate

    Returns:
        Ptr - A pointer to the location in memory with `n` bits available

    Raises:
        Exception("Out Of Memory") - if more bits are requested than free
        Exception("Bad Input") - if user provides invalid input to malloc

    Hint: You might have to create some helper functions to handle the
    datastructure of your memory.
    """
    TODO()


def free(p: Ptr) -> bool:
    """
    Free a pointer, release the memory at its location back to the allocator.

    Params:
        p: Ptr - An active pointer

    Returns:
        bool - True if the method succeeds, False if it fails

    Raises:
        free should never raise an exception
    """
    TODO()


def used() -> int:
    """Returns the amount of space used in _memory."""
    TODO()


def remaining() -> int:
    """Returns the amount of space still available in _memory."""
    TODO()
