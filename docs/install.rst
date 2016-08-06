.. _install:

System requirements
===================

Before installing, please make sure that some dependencies are installed - otherwise building the module will fail.

* Python 2.6+, 3.3+
* Python header and library files
* librrd header and library files

Debian/Ubuntu
-------------

If you want to build the bindings for *Python 2.x*, use::

   $ sudo apt-get install librrd-dev libpython-dev

For *Python 3.x* (may be available on newer OS versions only), use::

   $ sudo apt-get install librrd-dev libpython3-dev

RHEL/CentOS
-----------

If you want to build the bindings for *Python 2.x*, use::

   $ sudo yum install rrdtool-devel python-devel

For *Python 3.x* (may be available on newer OS versions only), use::

   $ sudo yum install rrdtool-devel python34-devel

Please note that *python34-devel* is correct for CentOS 7.x, but may be different for older/newer releases.

Installation
============

The simplest way to install the bindings is using `pip <https://pip.pypa.io>`_::

   $ pip install rrdtool

Source Code
-----------

In case you'd like to build the module on your own, or for your specific Python installation, you can clone the `GitHub repository <https://github.com/commx/python-rrdtool>`_ and install it::

   $ git clone https://github.com/commx/python-rrdtool.git
   $ cd python-rrdtool
   $ python setup.py install