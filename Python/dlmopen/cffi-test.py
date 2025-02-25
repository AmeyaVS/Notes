#!/usr/bin/python

# Refer: https://gist.github.com/dutc/eba9b2f7980f400f6287

from cffi import FFI
from textwrap import dedent
from sys import exit

if __name__ == '__main__':
	ffi = FFI()
	ffi.cdef('''
		void *dlmopen (long int __nsid, const char*, int);
		void *dlopen (const char*, int);
		void *dlsym(void *handle, const char *symbol);
		char *dlerror(void);
	''')
	LM_ID_NEWLM = -1

	libc = ffi.dlopen('libdl.so.2')

	# clear error
	libc.dlerror()

	handle = libc.dlmopen(LM_ID_NEWLM, 'libpython2.7.so', ffi.RTLD_DEEPBIND | ffi.RTLD_LOCAL | ffi.RTLD_LAZY)
	#handle = libc.dlopen('libpython2.7.so', ffi.RTLD_DEEPBIND | ffi.RTLD_LOCAL | ffi.RTLD_LAZY)

	error = libc.dlerror()
	if error: 
		print('error: {}'.format(ffi.string(error)))
		exit(1)

	init = ffi.cast('void (*)(void)', libc.dlsym(handle, 'Py_Initialize'))
	error = libc.dlerror()
	if error: 
		print('error: {}'.format(ffi.string(error)))
		exit(1)

	fini = ffi.cast('void (*)(void)', libc.dlsym(handle, 'Py_Finalize'))
	error = libc.dlerror()
	if error: 
		print('error: {}'.format(ffi.string(error)))
		exit(1)

	run  = ffi.cast('void (*)(const char*)', libc.dlsym(handle, 'PyRun_SimpleString'))
	error = libc.dlerror()
	if error: 
		print('error: {}'.format(ffi.string(error)))
		exit(1)

	init()
	cmd = dedent('''
		from sys import version_info
		from os import getpid
		print('Version info: {}'.format(version_info))
		print('Process ID: {}'.format(getpid()))
		# two quick&dirty tests to check if we're looking at separate interpreterss
		# (locale is not thread-safe)
		from locale import getlocale, setlocale, LC_ALL
		print('Before: {}'.format(getlocale()))
		setlocale(LC_ALL, '')
		print('After: {}'.format(getlocale()))
		# interpreter-level state
		from sys import modules
		print('Before: {}'.format('itertools' in modules))
		from itertools import chain
		print('After: {}'.format('itertools' in modules))
		print('\\n')
		execfile('cffi-test.py')''')
	run(cmd)
	fini()
