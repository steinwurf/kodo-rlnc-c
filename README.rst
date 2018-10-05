This kodo-rlnc-c contains high-level C bindings for the Kodo RLNC erasure coding
library. The bindings provide access to the basic functionality provided by
Kodo RLNC, such as encoding and decoding data. The ``examples`` folder provides
sample applications that demonstrate the usage of the C API.

The kodo-rlnc-c repository: https://github.com/steinwurf/kodo-rlnc-c

.. image:: http://buildbot.steinwurf.dk/svgstatus?project=kodo-rlnc-c
    :target: http://buildbot.steinwurf.dk/powerconsole?project=kodo-rlnc-c

If you have any questions or suggestions about this library, please contact
us at our developer mailing list (hosted at Google Groups):

* http://groups.google.com/group/steinwurf-dev

License
-------

To obtain a valid Kodo license **you must fill out the license request** form_.

Kodo is available under a research- and education-friendly license, see the
details in the LICENSE.rst file.

.. _form: http://steinwurf.com/license/

Documentation
-------------

Please read our general documentation here to get started:
http://docs.steinwurf.com

The kodo-rlnc documentation is located here:
http://docs.steinwurf.com/kodo-rlnc-c/master/index.html

Quick Start
-----------

If you already installed a C++14 compiler, git and python on your system,
then you can clone this repository to a suitable folder::

    git clone git@github.com:steinwurf/kodo-rlnc-c.git

Configure and build the project::

    cd kodo-rlnc-c
    python waf configure
    python waf build

Run the unit tests::

  python waf --run_tests
