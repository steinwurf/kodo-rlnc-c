#! /usr/bin/env python
# encoding: utf-8

import os
from waflib.Build import BuildContext
from waflib.extras.wurf.directory import remove_directory

APPNAME = 'kodo-rlnc-c'
VERSION = '4.0.0'


def options(opt):

    opt.add_option(
        '--all_docs', default=False, action='store_true',
        help='Generate all documentation versions using giit.')


def configure(conf):

    if conf.is_toplevel():

        # Make sure we recreate the docs virtualenv on (re-)configure
        venv_path = os.path.join(
            conf.path.abspath(), "build", "virtualenv-docs")

        if os.path.isdir(venv_path):
            remove_directory(venv_path)


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
        bld.recurse('examples/encode_decode_using_coefficients')
        bld.recurse('examples/sparse_seed')
        bld.recurse('examples/switch_systematic_on_off')
        bld.recurse('examples/symbol_status_updater')
        bld.recurse('examples/udp_sender_receiver')
        bld.recurse('examples/uncoded_symbols')
        bld.recurse('examples/use_trace_layers')

        # Install the header files to the 'include' folder
        if bld.has_tool_option('install_path'):
            install_path = bld.get_tool_option('install_path')
            install_path = os.path.abspath(os.path.expanduser(install_path))
            start_dir = bld.path.find_dir('src')
            bld.install_files(
                os.path.join(install_path, 'include'),
                start_dir.ant_glob('**/*.h'),
                cwd=start_dir,
                relative_trick=True)


class DocsContext(BuildContext):
    cmd = 'docs'
    fun = 'docs'


def docs(ctx):

    build_path = os.path.join(ctx.path.abspath(), 'build')
    venv = ctx.create_virtualenv(
        cwd=build_path, name="virtualenv-docs", overwrite=False)

    if not ctx.options.all_docs:
        venv.run('python -m pip install -r docs/requirements.txt',
                 cwd=ctx.path.abspath())
        venv.run('sphinx-build -v -E -a -D release={} -b html '
                 '-d build/doctrees docs build/html'.format(VERSION),
                 cwd=ctx.path.abspath())
    else:
        giit = 'git+https://github.com/steinwurf/giit.git@master'
        venv.run('pip install {}'.format(giit))
        build_path = os.path.join(ctx.path.abspath(), 'build', 'giit')
        venv.run('giit clean . --build_path {}'.format(build_path),
                 cwd=ctx.path.abspath())
        venv.run('giit sphinx . --build_path {}'.format(build_path),
                 cwd=ctx.path.abspath())
        venv.run('giit versjon . --build_path {}'.format(build_path),
                 cwd=ctx.path.abspath())