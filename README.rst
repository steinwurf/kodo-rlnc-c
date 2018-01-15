License
-------

A valid Kodo license is required if you wish to use this project.

Please request a license by **filling out the license request** form_.

Kodo is available under a research- and education-friendly license,
you can see the details here_.

If you try to configure without a valid license, then you will get an error!

.. _form: http://steinwurf.com/license/
.. _here: http://steinwurf.com/research-license/

About
-----

This repository contains high-level C bindings for the Kodo RLNC erasure coding
library. The bindings provide access to the basic functionality provided by
Kodo RLNC, such as encoding and decoding data. The examples folder provides
sample applications that demonstrate the usage of the C API.

.. image:: http://buildbot.steinwurf.dk/svgstatus?project=kodo-rlnc-c
    :target: http://buildbot.steinwurf.dk/powerconsole?project=kodo-rlnc-c

If you have any questions or suggestions about this library, please contact
us at our developer mailing list (hosted at Google Groups):

* http://groups.google.com/group/steinwurf-dev

Documentation
-------------

To get started, see the manual here:
http://docs.steinwurf.com

Quick Start
-----------

If you already installed a C++11 compiler, git and python on your system,
then you can clone this repository to a suitable folder::

    git clone git@github.com:steinwurf/kodo-rlnc-c.git

Configure and build the project::

    cd kodo-rlnc-c
    python waf configure
    python waf build

Run the unit tests::

  python waf --run_tests
