python-rrdtool
==============

Python bindings for [RRDtool](http://oss.oetiker.ch/rrdtool) with a native C extension.

Supported Python versions: 2.6+, 3.3+.

The bindings are based on the code of the original Python 2 bindings for rrdtool by Hye-Shik Chang, which are currently shipped as official bindings with rrdtool.

Installation
------------

The easy way:

    # pip install rrdtool

**Note:** This requires rrdtool and it's development files (headers, libraries, dependencies) to be installed.

In case you'd like to build the module on your own, you can obtain a copy of the repository and run `python setup.py install` in it's destination folder to build the native C extension.

Usage
-----

```python
import rrdtool

# Create Round Robin Database
rrdtool.create('test.rrd', '--start', 'now', '--step', '300', 'RRA:AVERAGE:0.5:1:1200', 'DS:temp:GAUGE:600:-273:5000')

# Feed updates to the RRD
rrdtool.update('test.rrd', 'N:32')
```

Changes
-------


## 0.1.2

*Released 2015-08-10*

* Added xport command
* Added lastupdate command
* Removed high-level interface (RRDtool module)
* Improved documentation of functions
* Code cleanup by using convenience macros
* On Python 2.x, str objects are returned instead of unicode objects
* Merged some changes from upstream
* License changed to LGPL 2.1 to match upstream licensing

## 0.1.1

*Released 2013-12-19*

* Added support for Python 2.7 (other 2.x versions might also work, but its not tested)
* Added dump command
* Fixed some issues regarding generating graphs with `graphv` on Python 3.3

*Please note:* The `graph` method in the object-oriented RRD class will now return a dict by default (as returned by graphv). Only if the `output_file` parameter is None, the actual graph image bytes are returned. Python 3.3 will return a bytes object whereas Python 2.x will return a str object.

## 0.1.0

*Released 2012-09-17*

* Initial release.
* Support for Python 3.x added
* Updated documentation strings (`__doc__`) for each of the rrdtool functions

Author
------

Christian Jurk <commx@commx.ws>

This binding was created because I am currently porting some existing Python 2 code to Python 3 and try to help the community by contributing a updated binding extension. Hope someone can benefit from it.

If you encounter any bugs (which I expected at time of writing this), please submit them in the issue tracker here on the project page on Github. Thank you.
