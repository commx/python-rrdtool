from distutils.core import setup, Extension

def main():
    module = Extension('rrdtool',
                       sources=['rrdtoolmodule.c'],
                       include_dirs = ['/opt/local/include'],
                       libraries=['rrd'])

    kwargs = dict(
        name='python-rrdtool',
        version='0.1.1',
        description='rrdtool bindings for Python',
        keywords=['rrdtool'],
        author='Christian Jurk, Hye-Shik Chang',
        author_email='commx@commx.ws',
        ext_modules=[module],
        py_modules=['RRDtool']
    )

    setup(**kwargs)

if __name__ == '__main__':
    main()
