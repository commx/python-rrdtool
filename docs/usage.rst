.. _usage:

Usage
=====

Once installed, you can start using the bindings inside your Python interpreter::

   import rrdtool

   rrdtool.create(
       "test.rrd",
       "--start", "now",
       "--step", "300",
       "RRA:AVERAGE:0.5:1:1200",
       "DS:temp:GAUGE:600:-273:5000")

   # feed updates to the database
   rrdtool.update("test.rrd", "N:32")

You can retrieve documentation for each of the module functions by using the ``help()`` function on it, for example::

   help(rrdtool.create)

Functions
---------

The function calls are converted to appropriate values and mapped to their librrd counterparts. Optional results are converted to Python representations whenever possible.

.. function:: create(*args)

   Create a round robin database.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: None

.. function:: dump(*args)

   Dump an round robin database to XML.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: None

.. function:: fetch(*args)

   Fetch data from an round robin database.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: tuple

   Example::

      result = rrdtool.fetch("test.rrd", "AVERAGE")
      start, end, step = result[0]
      ds = result[1]
      rows = result[2]

.. function:: first(*args)

   Get the first UNIX timestamp of the first data sample in an RRD.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: int

.. function:: flushcached(*args)

   Flush RRD files from memory.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: None

.. function:: graph(*args)

   Create a graph based on one or more RRDs.
   
   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: tuple

   Example::

      result = rrdtool.graph("graph.png", *args)
      width = result[0]
      height = result[1]

      if result[2] is not None:
          calcpr = result[2]  # list

.. function:: graphv(*args)

   Create a graph based on one or more RRDs and return info dictionary.
   
   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: dict

   Example::

      >>> rrdtool.graphv("-", *args)
      {
          'graph_end': 1470478967,
          'graph_height': 100,
          'graph_left': 51,
          'graph_top': 15,
          'graph_start': 1470392567,
          'graph_width': 400,
          'image': '\x99PNG\r\n…',
          'image_height': 155,
          'image_width': 481,
      }

.. function:: info(*args)

   Extract header information from an RRD file.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: dict

   Example::

      >>> rrdtool.info("test.rrd")
      {
          'rra[0].pdp_per_row': 1,
          'rra[0].rows': 1200,
          'ds[temp].last_ds': '32',
          'ds[temp].type': 'GAUGE',
          'rra[0].cur_row': 673,
          'rrd_version': '0003',
          'ds[temp].unknown_sec': 209,
          'header_size': 584,
          'last_update': 1470441516,
          'step': 300,
          'rra[0].cf': 'AVERAGE',
          'rra[0].cdp_prep[0].unknown_datapoints': 0,
          'rra[0].cdp_prep[0].value': None,
          'ds[temp].minimal_heartbeat': 600,
          'filename': 'test.rrd',
          'ds[temp].index': 0
      }

.. function:: last(*args)

   Get the UNIX timestamp of the most recent data sample in an RRD.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: int

.. function:: lastupdate(*args)

   Returns datetme and value stored for each datum in the most recent update of an RRD.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: dict

   Example::

      >>> rrdtool.lastupdate("test.rrd")
      {
          'date': datetime.datetime(2016, 8, 6, 1, 58, 36),
          'ds': {
              'temp': 32.0
           }
      }

.. function:: lib_version()

   Get the version of librrd this binding was compiled against.

   :rtype: str

   Example::

      >>> rrdtool.lib_version()
      '1.4.8'

.. function:: resize(*args)

   Modify the number of rows in an RRD.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: None

.. function:: tune(*args)

   Modify basic properties of an RRD file.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: None

.. function:: update(*args)

   Store a new set of values into the round robin database.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: None

.. function:: updatev(*args)

   Store a new set of values into the round robin database and return an info dictionary.

   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: dict

.. function:: xport(*args)

   Dictionary representation of data stored in RRDs.
   
   :param args: Arguments
   :type args: tuple
   :raises ProgrammingError: in the event of an programming error
   :raises OperationalError: in the event of an rrdtool error
   :rtype: dict

   Example::

      >>> rrdtool.xport("DEF:a=test.rrd:temp:AVERAGE", *args)
      {
          'meta': {
              'start': 1234567890,
              'end': 1234567890,
              'step': 300,
              'rows': 128,
              'columns': 1,
              'legend': [
                  'item_a'
              ]
          },
          'data': [
              None,
              None,
              1.927492222,
              None,
              None
          ]
      }

.. py:attribute:: __version__

   Returns the version of python-rrdtool.

   :rtype: str
   
   Example::

      >>> rrdtool.__version__
      '0.1.5'

Errors and Exceptions
---------------------

python-rrdtool will raise exceptions in the event of errors.

.. py:exception:: ProgrammingError

   Raised in the event of programming errors (e.g. passing wrong datatypes).

.. py:exception:: OperationalError

   Raised in the event of errors generated by rrdtool itself.

Both exception classes are directly derived from the ``Exception`` class.