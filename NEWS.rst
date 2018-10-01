News for kodo-rlnc-c
====================

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

Latest
------
* tbd
* Minor: Added some examples.
* Major: Separated the kodo_rlnc_c.h header file to encoder.h and decoder.h
  in order to mirror the kodo-rlnc headers.
* Major: Changed the opaque pointer types to allow the compilation of pure
  C programs.

3.0.0
-----
* Major: Upgrade to kodo-rlnc 13

2.0.0
-----
* Minor: Added support for binary16 finite field.
* Patch: Fix defines so that the library now works on Windows.
* Major: Switch from using kodo-rlnc's mix-in layer stacks to using the
  wrappers.
* Major: Upgraded to kodo-rlnc 11.
* Major: Changed c "namespace" from ``kodo_rlnc`` to ``krlnc``.
* Major: Changed ``construct`` and ``destruct`` to ``new`` and ``delete``.

1.0.0
-----
* Major: Initial release.
