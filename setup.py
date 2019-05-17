import os.path
import sys

try:
    from setuptools import setup, Extension
except ImportError:
    sys.exit('The setup requires setuptools.')

from distutils.errors import CompileError, LinkError

# absolute path to package dir
package_dir = os.path.abspath(os.path.dirname(__file__))

# package version
package_version = '0.1.15'


def check_extensions():
    macros = [
        ('PACKAGE_VERSION', '"%s"' % package_version),
    ]

    # for compatibility reasons, don't define WITH_FETCH_CB, so binary packages
    # will still work with older rrdtool versions
    if not os.environ.get('OLDCOMPAT'):
        macros.append(('WITH_FETCH_CB', 1))

    try:
        return compile_extensions(macros)
    except LinkError:
        sys.stderr.write('Warning: Seems like your rrdtool version does not support fetch callbacks, recompiling.')

        # try to recompile without WITH_FETCH_CB
        macros = [macro for macro in macros if macro[0] != 'WITH_FETCH_CB']

        try:
            return compile_extensions(macros, compat=True)
        except LinkError:
            sys.exit('Error: Unable to link binary module. Do you have the rrdtool header and libraries installed?')


def compile_extensions(macros, compat=False):
    """
    Compiler subroutine to test whether some functions are available
    on the target system. Since the rrdtool headers shipped with most
    packages do not disclose any versioning information, we cannot test
    whether a given function is available that way. Instead, use this to
    manually try to compile code and see if it works.

    Taken from http://stackoverflow.com/questions/28843765/setup-py-check-if-non-python-library-dependency-exists.
    """
    import distutils.sysconfig
    import distutils.ccompiler
    import tempfile
    import shutil

    from textwrap import dedent

    # common vars
    libraries = ['rrd']
    include_dirs = [package_dir, '/usr/local/include']
    library_dirs = ['/usr/local/lib']
    compiler_args = dict(
        libraries=libraries,
        include_dirs=include_dirs,
        library_dirs=library_dirs,
        define_macros=macros)
    exts = [Extension('rrdtool', sources=['rrdtoolmodule.c'], **compiler_args)]

    if compat:
        return exts

    # in non-compat mode, try to link to check if the new symbols are present in librrd
    c_code = dedent('''
        #include <rrd.h>
        #include "rrdtoolmodule.h"

        int main(int argc, char *argv[]) {
            rrd_fetch_cb_register(NULL);  /* exists in rrdtool >= 1.5.0 */
            return 0;
        }
    ''')

    tmp_dir = tempfile.mkdtemp(prefix='tmp_python_rrdtool')
    bin_file_name = os.path.join(tmp_dir, 'test_rrdtool')
    file_name = bin_file_name + '.c'

    with open(file_name, 'w') as fp:
        fp.write(c_code)

    # try to compile it
    compiler = distutils.ccompiler.new_compiler()
    assert isinstance(compiler, distutils.ccompiler.CCompiler)
    for s in include_dirs:
        compiler.add_include_dir(s)
    for s in library_dirs:
        compiler.add_library_dir(s)
    for s in libraries:
        compiler.add_library(s)
    for s in macros:
        compiler.define_macro(*s)
    distutils.sysconfig.customize_compiler(compiler)

    try:
        compiler.link_executable(
            compiler.compile([file_name]),
            bin_file_name,
            libraries=libraries)
    except CompileError:
        sys.exit('Error: Unable to compile the binary module. Do you have the rrdtool header and libraries installed?')
    except LinkError:
        raise
    else:
        return exts  # seems to be available, compile in regular way
    finally:
        shutil.rmtree(tmp_dir)


def main():
    with open('README.md') as readme:
        long_description = readme.read()

    kwargs = dict(
        name='rrdtool',
        version=package_version,
        description='Python bindings for rrdtool',
        long_description=long_description,
        long_description_content_type='text/markdown',
        keywords=['rrdtool'],
        author='Christian Kroeger',
        author_email='commx@commx.ws',
        url='https://github.com/commx/python-rrdtool',
        classifiers=[
            'Development Status :: 5 - Production/Stable',
            'License :: OSI Approved :: GNU Lesser General Public License v2 (LGPLv2)',
            'Operating System :: POSIX',
            'Programming Language :: C',
            'Programming Language :: Python :: 2',
            'Programming Language :: Python :: 2.6',
            'Programming Language :: Python :: 2.7',
            'Programming Language :: Python :: 3',
            'Programming Language :: Python :: 3.3',
            'Programming Language :: Python :: 3.4',
            'Programming Language :: Python :: 3.5',
            'Programming Language :: Python :: 3.6',
            'Programming Language :: Python :: 3.7',
        ],
        ext_modules=check_extensions(),
        test_suite='tests',
        package_data={
            '': ['LICENSE']
        },
        python_requires='>=2.6, !=3.0.*, !=3.1.*, !=3.2.*, <4'
    )

    setup(**kwargs)


if __name__ == '__main__':
    main()
