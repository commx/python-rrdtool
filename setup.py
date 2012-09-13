from distutils.core import setup, Extension
from glob import glob

sources = glob('*.c')
module = Extension('rrdtool', sources=sources, libraries=['rrd'])

setup(name='rrdtool',
      version='1.0',
      description='rrdtool bindings for Python 3',
      ext_modules=[module])

