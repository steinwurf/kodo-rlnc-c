#! /usr/bin/env python
# encoding: utf-8

import os

APPNAME = 'kodo-rlnc-c'
VERSION = '3.0.0'

def build(bld):

    CXX = bld.env.get_flat("CXX")
    # Matches both g++ and clang++
    if 'g++' in CXX or 'clang' in CXX:
        # The -fPIC flag is required for all underlying static libraries that
        # will be included in the shared library
        bld.env.append_value('CXXFLAGS', '-fPIC')

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_KODO_RLNC_C_VERSION="{}"'.format(VERSION))

    # Build the kodo-rlnc-c static library
    bld.stlib(
        source=bld.path.ant_glob('src/kodo_rlnc_c/*.cpp'),
        target='kodo_rlnc_c_static',
        name='kodo_rlnc_c_static',
        defines=['KODO_RLNC_C_STATIC'],
        export_defines=['KODO_RLNC_C_STATIC'],
        export_includes='src',
        use=['kodo_rlnc'])

    # Build the kodo-rlnc-c shared library
    bld.shlib(
        source=bld.path.ant_glob('src/kodo_rlnc_c/*.cpp'),
        target='kodo_rlnc_c',
        name='kodo_rlnc_c',
        defines=['KODO_RLNC_C_DLL_EXPORTS'],
        install_path=None,
        export_includes='src',
        use=['kodo_rlnc'])

    if bld.is_toplevel():

        bld.recurse('test')
        bld.recurse('examples/encode_decode_on_the_fly')
        bld.recurse('examples/encode_decode_simple')
#        bld.recurse('examples/sparse_seed')
        bld.recurse('examples/switch_systematic_on_off')
#        bld.recurse('examples/udp_sender_receiver')
#        bld.recurse('examples/use_trace_layers')

        # Install kodo_rlnc_c.h to the 'include' folder
        if bld.has_tool_option('install_path'):
            install_path = bld.get_tool_option('install_path')
            install_path = os.path.abspath(os.path.expanduser(install_path))
            start_dir = bld.path.find_dir('src')
            bld.install_files(
                os.path.join(install_path, 'include'),
                start_dir.ant_glob('**/*.h'),
                cwd=start_dir,
                relative_trick=True)
