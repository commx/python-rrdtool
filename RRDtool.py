#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  python-rrdtool, rrdtool bindings for Python.
#  Based on the rrdtool Python bindings for Python 2 from
#  Hye-Shik Chang <perky@fallin.lv>.
#
#  Copyright 2012 Christian Jurk <commx@commx.ws>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as 
#  published by the Free Software Foundation; either version 3 of the 
#  License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#
#

import io
import os
import rrdtool

from datetime import datetime
from time import mktime

def create(filename, *args):
	"Create a Round Robin Database and return a RRD object on success."
	rrdtool.create(filename, *args)

	if not os.access(filename, os.F_OK):
		raise rrdtool.OperationalError('RRD file was not created')

	return RRD(filename)

class RRD:
	"""An object-based interface to the rrdtool module."""

	def __init__(self, filename, check_type=True):
		"Initialize the class instance with a filename."

		if not os.access(filename, os.F_OK | os.R_OK):
			raise rrdtool.OperationalError('RRD {!s} cannot be opened.' \
			  .format(filename))

		# Use rrdinfo to test whether the file is a valid RRD file
		if check_type is True:
			rrdtool.info(filename)

		self.readonly = not os.access(filename, os.W_OK)
		self.filename = filename

	def graph(self, output_file, *args):
		"""
		Generate a graph based on the arguments passed to this function.

		If output_file is None, "-" will be used as the output filename.
		In that case, rrdtool returns the image bytes within its info dict.
		"""
		outfile = '-' if output_file is None else output_file

		# when writing to a file-like object, use output buffering
		if isinstance(output_file, io.IOBase):
			outfile = '-'

		info = rrdtool.graphv(outfile, *args)

		if isinstance(info, dict) and 'image' in info:
			if isinstance(output_file, io.IOBase):
				output_file.write(info['image'])
			elif output_file is None:
				return info['image']

		return info

	def info(self):
		return rrdtool.info(self.filename)

	def update(self, values, *args):
		vl = []

		if self.readonly:
			raise rrdtool.OperationalError('RRD file is read-only: {!s}' \
			  .format(self.filename))
		elif not isinstance(values, (list, tuple)):
			raise rrdtool.ProgrammingError('The values parameter must be a ' \
			  'list or tuple')
		else:
			for row in values:
				if isinstance(row, str):
					vl.append(row)
				elif isinstance(row, (list, tuple)):
					if len(row) < 2:
						raise rrdtool.ProgrammingError('Value {!r} has too ' \
						  'few elements in sequence object'.format(row))
					else:
						ts = row[0]
						if ts is None:
							ts = 'N'
						elif isinstance(ts, datetime):
							ts = int(mktime(ts.timetuple()))
						elif isinstance(ts, str):
							ts = int(ts)
						elif not isinstance(ts, int):
							raise ValueError('Unsupported type')

					v = '{}:{}'.format(ts, ':'.join([str(x) for x in row[1:]]))
					vl.append(v)

		arglist = tuple(vl + list(args))
		return rrdtool.update(self.filename, *arglist)

	def __repr__(self):
		return '<RRD {!r}>'.format(self.filename)
