from distutils.core import setup, Extension

def main():
    module = Extension('rrdtool', sources=['rrdtool-py3k.c'],
                       libraries=['rrd'])

    kwargs = dict(
        name='python-rrdtool',
        version='0.1.0',
        description='rrdtool bindings for Python 3',
        keywords=['rrdtool'],
        author='Christian Jurk, Hye-Shik Chang',
        author_email='commx@commx.ws',
        ext_modules=[module],
        py_modules=['RRDtool']
    )

    setup(**kwargs)

if __name__ == '__main__':
    main()
