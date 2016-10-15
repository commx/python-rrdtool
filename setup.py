try:
    from setuptools import setup
    from setuptools.extension import Extension
except ImportError:
    from distutils.core import setup, Extension


def main():
    module = Extension('rrdtool',
                       sources=['rrdtoolmodule.c'],
                       include_dirs=['/usr/local/include'],
                       library_dirs=['/usr/local/lib'],
                       libraries=['rrd'])

    kwargs = dict(
        name='rrdtool',
        version='0.1.6',
        description='Python bindings for rrdtool',
        keywords=['rrdtool'],
        author='Christian Kroeger, Hye-Shik Chang',
        author_email='commx@commx.ws',
        license='LGPL',
        url='https://github.com/commx/python-rrdtool',
        ext_modules=[module]
    )

    setup(**kwargs)


if __name__ == '__main__':
    main()
