import sys
from alloc import free, malloc, TODO


class Object:
    reference_count: int = 0

    def __init__(self, *args, **kwargs):
        self.__create__()

    def __create__(self):
        """
        Allocate memory for this object.

        Note: you will not actually set any memory values, this is just a toy example.
        A real object allocator would allocate enough memory for the object, and begin
        using that memory to store the object's data.
        Here we will only allocate the memory.
        """
        TODO()

    def __increase_count__(self):
        """Used to update reference count when an object points to this object."""
        TODO()

    def __decrease_count__(self):
        """Decrement count, then check if this object should be freed."""
        TODO()


def sizeof(obj: Object) -> int:
    """Returns the size of an object in bits."""
    return sys.getsizeof(obj) * 8


def assign(left, right) -> None:
    """
    Assign the variable on the `left` the value on the `right`.
    Mimic the structure: `left = right` (i.e. `assign(left, right)`)

    In addition to setting the variable `left`, we must account for the new
    reference to the value `right`, and the reduced reference in the previous
    value of `left`.

    Params:
        left: Object - The variable
        right: Object - The value

    Returns:
        None

    Raises:
        Exception("Type Error") - if the types of left and right are not compatible.
    """
    TODO()


def track_references(function):
    """Decorate a function to track parameters as they enter and leave a scope."""

    def _track_references(*args, **kwargs):
        # TODO("Capture all parameters and increment reference count of any objects")
        ret_value, variables = function(*args, **kwargs)
        # TODO("Decrement reference count of any objects before leaving scope")
        # variables contains the list of local variables in the previous scope
        return ret_value

    return _track_references
