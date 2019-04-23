import ctypes

# Load the shared library into c types.
libc = ctypes.CDLL("./build/libclib.so")

# Another way to load the library
#libname = os.path.abspath(
#    os.path.join(os.path.dirname(__file__), "libclib1.so"))

#libc = ctypes.CDLL(libname)

# Call the C function from the library
counter = libc.simple_function()
print(counter)
counter = libc.simple_function()
print(counter)
counter = libc.simple_function()
print(counter)

print("Calling C function which tries to modify Python string")
original_string = "starting string"
print("Before:", original_string)

# This call does not change value, even though it tries!
libc.add_one_to_string(original_string)

print("After: ", original_string)

# The ctypes string buffer IS mutable, however.
print("Calling C function with mutable buffer this time")

# Need to encode the original to get bytes for string_buffer
mutable_string = ctypes.create_string_buffer(str.encode(original_string))

print("Before:", mutable_string.value)
libc.add_one_to_string(mutable_string)  # Works!
print("After: ", mutable_string.value)

# Specifying Function Signatures in ctypes
#alloc_func = libc.alloc_C_string
#alloc_func.restype = ctypes.POINTER(ctypes.c_char)

#free_func = libc.free_C_string
#free_func.argtypes = [ctypes.POINTER(ctypes.c_char), ]

alloc_func = libc.alloc_C_string

# This is a ctypes.POINTER object which holds the address of the data
alloc_func.restype = ctypes.POINTER(ctypes.c_char)

print("Allocating and freeing memory in C")
c_string_address = alloc_func()

# Wow we have the POINTER object.
# We should convert that to something we can use
# on the Python side
phrase = ctypes.c_char_p.from_buffer(c_string_address)

print("Bytes in Python {0}".format(phrase.value))

free_func = libc.free_C_string
free_func.argtypes = [ctypes.POINTER(ctypes.c_char), ]
free_func(c_string_address)

