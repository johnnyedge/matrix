The quickest way to get started is to simply type `./cmakefile` within the source directory.

`git` is required (which you should already have); however `cmake` is also be required. The
software builds into a directory called `build` within the source directory. By default, running
`./cmakefile` will download `googletest` from the Internet, build it, build and link the unit
tests, and run the unit tests.

The `cmakefile` script is just a simple wrapper around the cmake binary to hide commonly used
command line parameters. In general, running `./cmakefile` is sufficient; however, you can
also use the `--target <target>` argument to it. Valid targets are:

* `unittest`: builds but does not execute the unit tests
* `run_tests`: builds and executes the unit tests
* `install`: installs the library on your system (some fiddling with the `CMAKE_INSTALL_PREFIX`
             may be necessary)
* `doxygen`: builds documentation in html format (and attempts to display it in your browser)
             using the `doxygen` program (which will need to be installed/available on your
             system.

By default, the `run_tests` target is executed.

The library is very simple at the moment and, as it is templated, exists only in headers. No
archive/shared object is generated nor installed.

Finally, the unit tests can be controlled by setting environmental variables when executing the
`run_tests` target. See https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#running-test-programs-advanced-options
