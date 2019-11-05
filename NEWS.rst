News for kodo-rlnc-c
====================

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

Latest
------
* Minor: Changed resolve step in CMake file.
* Minor: Lowered CMake version requirement.

5.0.0
-----
* Minor: Added cmake.
* Major: The factories were removed from the API, therefore new coder objects
  can be instantiated using krlnc_create_encoder and krlnc_create_decoder.
* Major: The write_payload and write_symbol functions were renamed to
  produce_payload and produce_symbol (including their variants).
* Major: The read_payload and read_symbol functions were renamed to
  consume_payload and consume_symbol (including their variants).
* Major: The uncoded designation was renamed to decoded in all function names
  to improve consistency in the API.
* Major: The trace designation was changed to log in all function names.
* Major: Upgrade to kodo-rlnc 15

4.0.0
-----
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
