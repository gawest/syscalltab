## syscalltab
Simple python interface for getting system call numbers, counts and build information. Build information is the kernel release and machine strings.

A build should result in a shared object that can be imported by python 2.6 or 2.7 called syscalltab.so This shared object can be imported on other systems running the same kernel release and same machine architecture as it was built on.

### building
In the top level source directory.
```
this builds the shared object and a binary test program
% make

this builds just the shared object
% make build

this will run some basic tests on the built shared object
% make test
```

### using

After building, in the build/lib.linux directory, should be the *syscalltab.so*
```
>>> import syscalltab as sct
>>> #
>>> # get information on host that it was built on.
>>> # 
>>> sct.buildinfo()
{'release': '4.5.1-1.el7.elrepo.x86_64', 'machine': 'x86_64'}
>>> #
>>> # get the number of system calls for the build system
>>> #
>>> sct.count()
327
>>> #
>>> # get the system call number for 'open', 'gettid' and 'sethostname'
>>> #
>>> sct.number('gettid')
186
>>> sct.name(67)
'shmdt'
```
