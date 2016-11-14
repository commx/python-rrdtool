try:
    from setuptools import setup
    from setuptools.extension import Extension
except ImportError:
    from distutils.core import setup, Extension

import os.path

# absolute path to package dir
package_dir = os.path.abspath(os.path.dirname(__file__))

def main():
    module = Extension('rrdtool',
                       sources=['rrdtoolmodule.c'],
                       include_dirs=['/usr/local/include', package_dir],
                       library_dirs=['/usr/local/lib'],
                       libraries=['rrd'])

    kwargs = dict(
        name='rrdtool',
        version='0.1.9',
        description='Python bindings for rrdtool',
        keywords=['rrdtool'],
        author='Christian Kroeger, Hye-Shik Chang',
        author_email='commx@commx.ws',
        license='LGPL',
        url='https://github.com/commx/python-rrdtool',
        ext_modules=[module],
        test_suite='tests'
    )

    setup(**kwargs)


if __name__ == '__main__':
    main()
