import ctypes

import os

print(__file__)

libpint = ctypes.CDLL(os.path.realpath("build/libpint.so"))

libpint.print.argtypes = [ctypes.c_char_p]

libprint = ctypes.CDLL(os.path.realpath("build/libprint.so"))

libprint.print.argstypes = [ctypes.c_char_p]

libprint2 = ctypes.CDLL(os.path.realpath("build/libprint.so"))

libprint2.print.argstypes = [ctypes.c_char_p]
s1 = b"Hellop"
s2 = b"Helloprint"
s3 = b"Helloprint2"

libpint.print(s1)

libprint.print(s2)
libprint2.print(s3)


libpint.print(s1)
libprint.print(s2)
libprint2.print(s3)
