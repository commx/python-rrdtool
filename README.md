[![Build Status](https://travis-ci.org/commx/python-rrdtool.svg?branch=master)](https://travis-ci.org/commx/python-rrdtool)

# python-rrdtool

Python bindings for [RRDtool](https://oss.oetiker.ch/rrdtool) for Python 2 and 3.

The bindings are based on the code of the original Python 2 bindings module for rrdtool by Hye-Shik Chang and are now shipped with the RRDtool distribution. This project is maintained separately to provide a more pythonic way to install those bindings via [PyPI](https://pypi.org).

## Features

* Native extension (written in C) for performance reasons.
* Uses library functions as exposed by librrd.
* Works with Python 2.6, 2.7, 3.3 and any later version.


## Installation

The most convenient way to install (on POSIX-like systems) is to use pip:

    pip install rrdtool

**Note:** Unless binary versions are available for your target system, the command above requires rrdtool development files (headers, libraries, dependencies) to be installed, otherwise building the module will fail.

In case you'd like to build the module on your own (regardless of whether binary versions are available for your system), you can obtain a copy of the source code and run `python setup.py install` in its destination folder to build the module.

## Usage

```python
import rrdtool

# Create Round Robin Database
rrdtool.create('test.rrd', '--start', 'now', '--step', '300', 'RRA:AVERAGE:0.5:1:1200', 'DS:temp:GAUGE:600:-273:5000')

# Feed updates to the RRD
rrdtool.update('test.rrd', 'N:32')
```

## Documentation

You can find the latest documentation for this project at https://pythonhosted.org/rrdtool.

## License

[GNU Lesser General Public License version 2.1](https://opensource.org/licenses/LGPL-2.1).
