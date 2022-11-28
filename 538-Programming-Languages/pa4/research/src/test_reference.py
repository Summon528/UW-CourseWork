from alloc import used
from reference import Object, track_references, sizeof, assign

# How to create a new class with "Object"
class Cat(Object):
    def __init__(self):
        super().__init__(self)


# How to create a new function with "@track_references"
@track_references
def myFunction():
    x = Cat()
    normal_return = "Calculate a value here"
    # Note: locals() is returned so that @track_references can clean up any objects
    # in this scope for us.
    return normal_return, locals()


# How to assign with Objects

x = Cat()
y = Cat()
assert x.reference_count == 1
assert used() == 2 * sizeof(x)

assign(y, x)  # y = x
assert x.reference_count == 2
assert used() == sizeof(x)
