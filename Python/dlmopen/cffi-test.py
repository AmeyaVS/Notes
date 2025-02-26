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

	handle = libc.dlmopen(LM_ID_NEWLM, b'libpython3.so', ffi.RTLD_DEEPBIND | ffi.RTLD_LOCAL | ffi.RTLD_LAZY)
	#handle = libc.dlopen('libpython2.7.so', ffi.RTLD_DEEPBIND | ffi.RTLD_LOCAL | ffi.RTLD_LAZY)

	error = libc.dlerror()
	if error: 
		print('error: {}'.format(ffi.string(error)))
		exit(1)

	init = ffi.cast('void (*)(void)', libc.dlsym(handle, b'Py_Initialize'))
	error = libc.dlerror()
	if error: 
		print('error: {}'.format(ffi.string(error)))
		exit(1)

	fini = ffi.cast('void (*)(void)', libc.dlsym(handle, b'Py_Finalize'))
	error = libc.dlerror()
	if error: 
		print('error: {}'.format(ffi.string(error)))
		exit(1)

	run  = ffi.cast('void (*)(const char*)', libc.dlsym(handle, b'PyRun_SimpleString'))
	error = libc.dlerror()
	if error: 
		print('error: {}'.format(ffi.string(error)))
		exit(1)

	init()
	cmd = dedent('''
		from sys import version_info
		from os import getpid
		from os import getcwd
		import sys
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
		print('Before: {}'.format('test_module' in modules))
		print(getcwd())
		sys.path.append(getcwd())
		from test_module import fact
		print('After: {}'.format('test_module' in modules))
		print('\\n')
		print(fact(5))
		#execfile('cffi-test.py')
		exec(open('cffi-test.py').read())

		''')
	run(cmd.encode('utf-8'))
	fini()
