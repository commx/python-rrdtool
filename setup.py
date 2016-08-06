try:
    from setuptools import setup
    from setuptools.extension import Extension
except ImportError:
    from distutils.core import setup, Extension


def main():
    module = Extension('rrdtool',
                       sources=['rrdtoolmodule.c'],
                       include_dirs=['/opt/local/include'],
                       libraries=['rrd'])

    kwargs = dict(
        name='rrdtool',
        version='0.1.5',
        description='rrdtool bindings for Python',
        keywords=['rrdtool'],
        author='Christian Jurk, Hye-Shik Chang',
        author_email='commx@commx.ws',
        license='LGPL',
        url='https://github.com/commx/python-rrdtool',
        ext_modules=[module],
    )

    setup(**kwargs)

if __name__ == '__main__':
    main()
