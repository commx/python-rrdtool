/*
 * rrdtool-py3k, rrdtool bindings for Python 3
 *
 * Copyright 2012 Christian Jurk <commx@commx.ws>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <Python.h>
#include <rrd.h>

/* Exception types */
static PyObject *rrdtool_OperationalError;
static PyObject *rrdtool_ProgrammingError;

/* Wrapper for rrd_create() */
static PyObject *
_rrdtool_create(PyObject *self, PyObject *args)
{
	PyObject *o;
	char **argv;
	int i, argc;

	argc = PyTuple_Size(args);
	argv = PyMem_New(char *, argc + 1);

	for (i = 0; i < argc; i++) {
		o = PyTuple_GET_ITEM(args, i);
		if (!PyBytes_Check(o)) {
			if (!PyUnicode_Check(o)) {
				PyErr_Format(rrdtool_ProgrammingError,
			                 "Argument %d must be a unicode or bytes object",
			                 i);
				PyMem_Del(argv);
				return NULL;
			} else
				argv[i+1] = PyBytes_AS_STRING(PyUnicode_AsUTF8String(o));

		} else
			argv[i+1] = PyBytes_AS_STRING(o);

	}

	if (argc == 0) {
		PyErr_SetString(rrdtool_ProgrammingError, "No arguments specified.");
		PyMem_Del(argv);
		return NULL;
	}

	argv[0] = "create";

	if (rrd_create(argc + 1, argv) == -1) {
		PyErr_SetString(rrdtool_OperationalError, rrd_get_error());
		rrd_clear_error();
		PyMem_Del(argv);
		return NULL;
	}

	Py_RETURN_TRUE;
}

static PyMethodDef rrdtool_methods[] = {
	{"create", (PyCFunction)_rrdtool_create, METH_VARARGS | METH_KEYWORDS,
	 "Create a new Round Robin Database"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef rrdtoolmodule = {
	PyModuleDef_HEAD_INIT,
	"rrdtool",
	"rrdtool bindings for Python 3",
	-1,
	rrdtool_methods
};

PyMODINIT_FUNC
PyInit_rrdtool(void)
{
	PyObject *m;

	m = PyModule_Create(&rrdtoolmodule);
	if (m == NULL)
		return NULL;

	rrdtool_ProgrammingError = PyErr_NewException("rrdtool.ProgrammingError",
	                                              NULL, NULL);
	Py_INCREF(rrdtool_ProgrammingError);
	PyModule_AddObject(m, "ProgrammingError", rrdtool_ProgrammingError);

	rrdtool_OperationalError = PyErr_NewException("rrdtool.OperationalError",
	                                              NULL, NULL);
	Py_INCREF(rrdtool_OperationalError);
	PyModule_AddObject(m, "OperationalError", rrdtool_OperationalError);

	return m;
}
