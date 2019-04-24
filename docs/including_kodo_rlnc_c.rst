.. _including_kodo_rlnc_c:

Including kodo-rlnc-c in Your Application
=========================================

This guide shows how to include kodo-rlnc-c in your application.

First of all, you need to build kodo-rlnc-c following the
:ref:`quick_start_kodo_rlnc_c` guide. If you want to cross-compile for your
target platform (e.g. Android, iOS, Raspberry Pi), please follow the
`Cross-compilation <http://docs.steinwurf.com/cross_compile.html>`_ guide.

In principle, you can use the library with any build system. Basically,
you can choose between the shared library and the static library.

Shared Library
--------------

In many cases, it is easier to include kodo-rlnc-c as a *shared* library in
your application. With the following command, you can copy the compiled
shared library to the target folder specified by the ``install_path`` option.
In this example, we will create the ``shared_test`` folder for this purpose::

    python waf install --install_shared_libs --install_path="./shared_test"

The kodo-rlnc-c shared library is called ``libkodo_rlnc_c.so`` on Linux,
``kodo_rlnc_c.dll`` on Windows and ``libkodo_rlnc_c.dylib`` on Mac OSX.
You can link with this shared library using your own build system.
You also need to include the ``encoder.h`` or ``decoder.h`` header files
that are installed to the ``include`` folder within the specified
``install_path``.

Now we copy an existing kodo-rlnc-c example (encode_decode_simple) to the
``shared_test`` folder and we compile it to a binary called ``myapp``::

    cp examples/encode_decode_simple/encode_decode_simple.c shared_test/myapp.c
    cd shared_test

The following command demonstrates the necessary flags for the gcc/g++ compiler
(other compilers require similar settings)::

    gcc myapp.c -o myapp -I./include -L. -Wl,-Bdynamic -lkodo_rlnc_c -lstdc++ \
    -Wl,-rpath .

In practice, you should set the ``-I`` and ``-L`` flags to the path where you
installed the shared library.

Now you should be able to run the new binary::

    ./myapp

If you dynamically link your application with the shared library, then you
have to copy the shared library to a folder where your system can find it
when you execute your application. On Windows, you typically place the DLL
in the same folder as your executable. On Unix systems, you can set the
``rpath`` of your executable or you can adjust ``LD_LIBRARY_PATH`` to include
the path where you installed the shared library.

Static Library
--------------

After building kodo-rlnc-c, you can install the static libraries to your target
folder with the following command (the ``install_path`` option specifies
the target folder which will be ``static_test`` in this example)::

    python waf install --install_static_libs --install_path="./static_test"

The kodo-rlnc-c static library is called ``libkodo_rlnc_c_static.a`` on Linux
and Mac and ``kodo_rlnc_c_static.lib`` on Windows. The install command also
installs the static libraries from the kodo-rlnc-c dependencies (you will need
the ``kodo_rlnc``, ``fifi`` and ``cpuid`` libraries as well).

You can link with these static libraries using your own build system. Of course,
you will need to include ``encoder.h`` or ``decoder.h`` in your code (which
are installed to the ``include`` folder within the specified ``install_path``).

Now we copy an existing kodo-rlnc-c example (encode_decode_simple) to the
``static_test`` folder and we compile it to a binary called ``myapp``::

    cp examples/encode_decode_simple/encode_decode_simple.c static_test/myapp.c
    cd static_test

The following command demonstrates the necessary flags for the gcc/g++ compiler
(other compilers require similar settings)::

    gcc myapp.c -o myapp -I./include -Wl,-Bstatic -L. -lkodo_rlnc_c_static \
    -lkodo_rlnc -lfifi -lcpuid -Wl,-Bdynamic -lm -lstdc++

In practice, you should set the ``-I`` and ``-L`` flags to the path where you
installed the static libraries.

Now you should be able to run the new binary (note that this binary will
be quite large, since it includes all the static libraries)::

    ./myapp

It is important to note that you need to link with the C++ standard library
(by using ``-lstdc++`` above), because the kodo-rlnc-c library actually wraps a
C++ library (kodo) that uses the C++ standard library. However, you can omit
this flag if you link your application with g++ instead of gcc (g++
automatically includes the stdc++ library)::

    g++ myapp.c -o myapp -I./include -Wl,-Bstatic -L. -lkodo_rlnc_c_static \
    -lkodo_rlnc -lfifi -lcpuid -Wl,-Bdynamic

Using CMake as a Build System
-----------------------------

It is also possible to integrate kodo-rlnc-c with the CMake build system.

The following steps will show how to compile a simple kodo-rlnc example using
CMake.

#. As a starting point, we assume that you completed the kodo-rlnc-c
   :ref:`quick_start_kodo_rlnc_c` section and you can successfully execute our
   unit tests with this command::

    python waf --run_tests

#. Then we will copy the static libraries to the ``~/cmake_test/kodo_build``
   folder (these static libraries must be linked to your application)::

    python waf install --install_path=~/cmake_test/kodo_build --install_static_libs --install_relative

   This command also installs the necessary header files into the ``include``
   subfolder of the ``kodo_build`` folder.

#. Now we copy an existing kodo-rlnc-c example (encode_decode_simple) to the
   ``cmake_test`` folder ::

    cp examples/encode_decode_simple/encode_decode_simple.c ~/cmake_test/

#. Then we go to the ``cmake_test`` folder and create a ``CMakeLists.txt``
   file (you can use any text editor for this)::

    cd ~/cmake_test
    nano CMakeLists.txt

   Add the following lines to this text file and save it::

    cmake_minimum_required(VERSION 2.8)

    project(kodo_test)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm -lstdc++")

    include_directories(${PROJECT_SOURCE_DIR}/kodo_build/include)

    set(KODO_LIBS kodo_rlnc_c_static kodo_rlnc fifi cpuid)
    link_directories(${PROJECT_SOURCE_DIR}/kodo_build)

    add_executable(kodo_test encode_decode_simple.c)
    target_link_libraries(kodo_test ${KODO_LIBS})

   Since CMake will call a C compiler (gcc) to compile our pure C example,
   it is important to add some flags to CMAKE_EXE_LINKER_FLAGS to link with
   libstdc++ (the C++ standard library) and libm. You might need different
   flags if you use a different compiler.

   We also add the ``kodo_build/include`` folder to the include path and
   define a list of the static libraries that we need to link to our executable.

#. Now we can call CMake to generate the build files and invoke make to
   build the ``kodo_test`` executable that we defined in ``CMakeLists.txt``::

    cmake .
    make

#. If everything went fine, then you can run the new executable::

    ./kodo_test
