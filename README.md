rrdtool-py3k
============

Python 3 bindings for rrdtool with a native C extension and an object-oriented way to work with Round Robin Databases.

The bindings are based on the code of the original Python 2 bindings for rrdtool by Hye-Shik Chang.

Installation
------------

In order to build the native C extension (which is an required step), you'll need librrd and its headers installed. Having rrdtool installed should be enough on most distributions.

**How to Install? **

1. Download a copy of the repository.
2. Run `python setup.py install` to build an install the native C extension as well as the RRD module.

Usage
-----

You can either use the low-level `rrdtool` module (which offers almost the same functions as the old Python 2 bindings for rrdtool provided), or the `RRDtool` module, which represents a object-oriented interface to rrdtool.

Unlike the Python 2 binding, this binding is able to create graphs entirely in-memory, which makes it ideal for generating a large amount of graphs without having high I/O. This feature is currently available on POSIX platforms only, because I wasn't able to find a portable way to redirect stdout to a memory allocated buffer (which is required for that). To use this feature, specify "-" as the output filename in graphs (low-level extension), or `None` in the high-level module.

### Using the low-level `rrdtool` module

```python
import rrdtool

# Create Round Robin Database
rrdtool.create('test.rrd', '--start', 'now', '--step', '300', 'RRA:AVERAGE:0.5:1:1200', 'DS:temp:GAUGE:600:-273:5000')

# Feed updates to the RRD
rrdtool.update('test.rrd', 'N:32')
```

### Using the high-level `RRDtool` module

```python
import RRDtool

# Create a Round Robin Database
rrd = RRDtool.create('test.rrd', '--start', 'now', '--step', '300', 'RRA:AVERAGE:0.5:1:1200', 'DS:temp:GAUGE:600:-273:5000')

# Update the RRD
rrd.update([(None, 32)])

# Create a graph from it
rrd.graph('test.png', '--end', 'now', '--start', 'end-5minutes', '--width', '400', 'DEF:ds0a=test.rrd:temp:AVERAGE', 'LINE1:ds0a#0000FF:"temperature\l"')

# Same, but keep data in memory.
data = rrd.graph(None, '--end', 'now', '--start', 'end-5minutes', '--width', '400', 'DEF:ds0a=test.rrd:temp:AVERAGE', 'LINE1:ds0a#0000FF:"temperature\l"')

# You can also use file-like objects
from io import BytesIO
rrd.graph(io, ...)
```

Author
------

Christian Jurk <commx@commx.ws>

This binding was created because I am currently porting some existing Python 2 code to Python 3 and try to help the community by contributing a updated binding extension. Hope someone can benefit from it.

If you encounter any bugs (which I expected at time of writing this), please submit them in the issue tracker here on the project page on Github. Thank you.