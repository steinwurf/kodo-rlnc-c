#!/usr/bin/env python
# encoding: utf-8

import os
import sys
import json
import shutil
import subprocess

project_name = 'kodo-rlnc-c'


def run_command(args):
    print("Running: {}".format(args))
    sys.stdout.flush()
    subprocess.check_call(args)


def get_tool_options(properties):
    options = []
    if 'tool_options' in properties:
        # Make sure that the values are correctly comma separated
        for key, value in properties['tool_options'].items():
            if value is None:
                options += ['--{0}'.format(key)]
            else:
                options += ['--{0}={1}'.format(key, value)]

    return options


def configure(properties):
    command = [sys.executable, 'waf']

    if properties.get('build_distclean'):
        command += ['distclean']

    command += ['configure', '--git_protocol=git@']

    if 'waf_resolve_path' in properties:
        command += ['--resolve_path=' + properties['waf_resolve_path']]

    if 'dependency_project' in properties:
        command += ['--{0}_checkout={1}'.format(
            properties['dependency_project'],
            properties['dependency_checkout'])]

    command += ["--cxx_mkspec={}".format(properties['cxx_mkspec'])]
    command += get_tool_options(properties)

    run_command(command)


def build(properties):
    command = [sys.executable, 'waf', 'build', '-v']
    run_command(command)


def run_tests(properties):
    command = [sys.executable, 'waf', '-v', '--run_tests']
    run_cmd = None

    if properties.get('valgrind_run'):
        run_cmd = 'valgrind --error-exitcode=1 %s'

    if run_cmd:
        command += ["--run_cmd={}".format(run_cmd)]

    command += get_tool_options(properties)

    run_command(command)


def install(properties):
    command = [sys.executable, 'waf', '-v', 'install']

    if 'install_path' in properties:
        install_path = properties['install_path']
        command += ['--install_path={0}'.format(install_path)]
        # Make sure that the previous install folder is removed
        if os.path.isdir(install_path):
            shutil.rmtree(install_path)
    if properties.get('install_relative'):
        command += ['--install_relative']
    command += ['--install_static_libs']
    run_command(command)

    # The following compilation test is only executed when the native g++ can
    # be used (cross-compilers should be ignored since they built the static
    # libs for a different platform)
    mkspec = properties.get('cxx_mkspec', '')
    if not mkspec.startswith('cxx_gxx'):
        return

    # After installing the headers and static libs, we test the standalone
    # compilation of an example without invoking waf
    example_path = os.path.join(install_path, 'temp_example')
    # Create a temporary folder for the example and copy the source file
    if not os.path.isdir(example_path):
        os.makedirs(example_path)
    example_source = os.path.join(
        'examples', 'encode_decode_simple', 'encode_decode_simple.c')
    shutil.copy(example_source, example_path)
    # Invoke gcc with a minimal set of flags to compile the C example
    os.chdir(example_path)
    command = ['gcc', 'encode_decode_simple.c', '-o', 'encode_decode_simple']
    command += ['-I../include', '-Wl,-Bstatic', '-L..']
    command += ['-lkodo_rlnc_c_static', '-lkodo_rlnc', '-lfifi', '-lcpuid']
    command += ['-Wl,-Bdynamic', '-lm', '-lstdc++']
    run_command(command)
    # Run the example
    run_command(['./encode_decode_simple'])


def coverage_settings(options):
    options['required_line_coverage'] = 90.0


def main():
    argv = sys.argv

    if len(argv) != 3:
        print("Usage: {} <command> <properties>".format(argv[0]))
        sys.exit(0)

    cmd = argv[1]
    properties = json.loads(argv[2])

    if cmd == 'configure':
        configure(properties)
    elif cmd == 'build':
        build(properties)
    elif cmd == 'run_tests':
        run_tests(properties)
    elif cmd == 'install':
        install(properties)
    else:
        print("Unknown command: {}".format(cmd))


if __name__ == '__main__':
    main()
