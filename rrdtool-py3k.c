/*
 * rrdtool-py3k, rrdtool bindings for Python 3.
 * Based on the rrdtool Python bindings for Python 2 from
 * Hye-Shik Chang <perky@fallin.lv>.
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
 */

#include <Python.h>
#include <rrd.h>

/* Exception types */
static PyObject *rrdtool_OperationalError;
static PyObject *rrdtool_ProgrammingError;

static char **rrdtool_argv = NULL;
static int rrdtool_argc = 0;

static void
destroy_args(void)
{
    PyMem_Del(rrdtool_argv);
    rrdtool_argv = NULL;
}

/* Helper function to convert Python objects into a representation that the
 * rrdtool functions can work with.
 */
static int
convert_args(char *command, PyObject *args)
{
    PyObject *o, *lo;
    int i, j, args_count, argv_count, element_count;

    args_count = PyTuple_Size(args);
    element_count = 0;
    for (i = 0; i < args_count; i++)
    {
        o = PyTuple_GET_ITEM(args, i);
        if (PyUnicode_Check(o) || PyBytes_Check(o))
            element_count++;
        else if (PyList_CheckExact(o))
            element_count += PyList_Size(o);
        else {
            PyErr_Format(rrdtool_ProgrammingError,
                         "Argument %d must be string, bytes or list of " \
                         "string/bytes", i);
            return -1;
        }
    }

    rrdtool_argv = PyMem_New(char *, element_count + 1);

    if (rrdtool_argv == NULL)
        return -1;

    argv_count = 0;
    for (i = 0; i < args_count; i++) {
        o = PyTuple_GET_ITEM(args, i);

        if (PyUnicode_Check(o))
            rrdtool_argv[++argv_count] = PyBytes_AsString(
              PyUnicode_AsUTF8String(o));
        else if (PyBytes_Check(o))
            rrdtool_argv[++argv_count] = PyBytes_AS_STRING(o);
        else if (PyList_CheckExact(o)) {
            for (j = 0; j < PyList_Size(o); j++) {
                lo = PyList_GetItem(o, j);
                if (PyUnicode_Check(lo))
                    rrdtool_argv[++argv_count] = PyBytes_AS_STRING(
                      PyUnicode_AsUTF8String(lo));
                else if (PyBytes_Check(lo))
                    rrdtool_argv[++argv_count] = PyBytes_AS_STRING(lo);
                else {
                    PyMem_Del(rrdtool_argv);
                    PyErr_Format(rrdtool_ProgrammingError,
                      "Element %d in argument %d must be string", j, i);
                    return -1;
                }
            }
        } else {
            PyMem_Del(rrdtool_argv);
            PyErr_Format(rrdtool_ProgrammingError,
              "Argument %d must be string or list of strings", i);
            return -1;
        }
    }

    rrdtool_argv[0] = command;
    rrdtool_argc = element_count + 1;

    return 0;
}

static char _rrdtool_create__doc__[] = "Create a new Round Robin Database.\n\n\
  Usage: create(args...)\n\
  Arguments:\n\n\
    filename\n\
    [--start|-b start time]\n\
    [--step|-s step]\n\
    [DS:ds-name:DST:heartbeat:min:max]\n\
    [RRA:CF:xff:steps:rows]\n\n\
    Full documentation can be found at:\n\
    http://oss.oetiker.ch/rrdtool/doc/rrdcreate.en.html";

static PyObject *
_rrdtool_create(PyObject *self, PyObject *args)
{
    PyObject *ret;

    if (convert_args("create", args) == -1)
        return NULL;

	if (rrd_create(rrdtool_argc, rrdtool_argv) == -1) {
		PyErr_SetString(rrdtool_OperationalError, rrd_get_error());
		rrd_clear_error();
		ret = NULL;
	} else {
        Py_INCREF(Py_None);
        ret = Py_None;
    }

	destroy_args();
    return ret;
}

static char _rrdtool_update__doc__[] = "Store a new set of values into\
 the RRD.\n\n\
 Usage: update(args..)\n\
 Arguments:\n\n\
   filename\n\
   [--template|-t ds-name[:ds-name]...]\n\
   N|timestamp:value[:value...]\n\
   [timestamp:value[:value...] ...]\n\n\
   Full documentation can be found at:\n\
   http://oss.oetiker.ch/rrdtool/doc/rrdupdate.en.html";

static PyObject *
_rrdtool_update(PyObject *self, PyObject *args)
{
    PyObject *ret;

    if (convert_args("update", args) == -1)
        return NULL;

    if (rrd_update(rrdtool_argc, rrdtool_argv) == -1) {
        PyErr_SetString(rrdtool_OperationalError, rrd_get_error());
        rrd_clear_error();
        ret = NULL;
    } else {
        Py_INCREF(Py_None);
        ret = Py_None;
    }

	destroy_args();
    return ret;
}

static char _rrdtool_fetch__doc__[] = "Fetch data from an RRD.\n\n\
  Usage: fetch(args..)\n\
  Arguments:\n\n\
    filename\n\
    CF\n\
    [--resolution|-r resolution]\n\
    [--start|-s start]\n\
    [--end|-e end]\n\n\
    Full documentation can be found at:\n\
    http://oss.oetiker.ch/rrdtool/doc/rrdfetch.en.html";

static PyObject *
_rrdtool_fetch(PyObject *self, PyObject *args)
{
    PyObject *ret, *range_tup, *dsnam_tup, *data_list, *t;
    rrd_value_t *data, *datai, dv;
    unsigned long step, ds_cnt, i, j, row;
    time_t start, end;
    char **ds_namv;

    if (convert_args("fetch", args) == -1)
        return NULL;

    if (rrd_fetch(rrdtool_argc, rrdtool_argv, &start, &end, &step, &ds_cnt,
                  &ds_namv, &data) == -1) {
        PyErr_SetString(rrdtool_OperationalError, rrd_get_error());
        rrd_clear_error();
        ret = NULL;
    } else {
        row = (end - start) / step;
        ret = PyTuple_New(3);
        range_tup = PyTuple_New(3);
        dsnam_tup = PyTuple_New(ds_cnt);
        data_list = PyList_New(row);

        PyTuple_SET_ITEM(ret, 0, range_tup);
        PyTuple_SET_ITEM(ret, 1, dsnam_tup);
        PyTuple_SET_ITEM(ret, 2, data_list);

        datai = data;

        PyTuple_SET_ITEM(range_tup, 0, PyLong_FromLong((long)start));
        PyTuple_SET_ITEM(range_tup, 1, PyLong_FromLong((long)end));
        PyTuple_SET_ITEM(range_tup, 2, PyLong_FromLong((long)step));

        for (i = 0; i < ds_cnt; i++)
            PyTuple_SET_ITEM(dsnam_tup, i, PyUnicode_FromString(ds_namv[i]));

        for (i = 0; i < row; i++) {
            t = PyTuple_New(ds_cnt);
            PyList_SET_ITEM(data_list, i, t);

            for (j = 0; j < ds_cnt; j++) {
                dv = *(datai++);
                if (isnan(dv)) {
                    PyTuple_SET_ITEM(t, j, Py_None);
                    Py_INCREF(Py_None);
                } else
                    PyTuple_SET_ITEM(t, j, PyFloat_FromDouble((double)dv));
            }
        }

        for (i = 0; i < ds_cnt; i++)
            rrd_freemem(ds_namv[i]);

        rrd_freemem(ds_namv);
        rrd_freemem(data);
    }

    destroy_args();
    return ret;
}

static char _rrdtool_graph__doc__[] = "Create a graph based on one or more " \
  "RRDs.\n\n\
  Usage: graph(args..)\n\
  Arguments:\n\n\
    filename | -\n\
    [-s|--start start]\n\
    [-e|--end end]\n\
    [-S|--step step]\n\
    [-t|--title string]\n\
    [-v|--vertical-label string]\n\
    [-w|--width pixels]\n\
    [-h|--height pixels]\n\
    [-j|--only-graph]\n\
    [-D|--full-size-mode]\n\
    [-u|--upper-limit value]\n\
    [-l|--lower-limit value]\n\
    [-r|--rigid]\n\
    [-A|--alt-autoscale]\n\
    [-J|--alt-autoscale-min]\n\
    [-M|--alt-autoscale-max]\n\
    [-N|--no-gridfit]\n\
    [-x|--x-grid (GTM:GST:MTM:MST:LTM:LST:LPR:LFM|none)]\n\
    [-y|--y-grid (grid step:label factor|none)]\n\
    [-Y|--alt-y-grid]\n\
    [-o|--logarithmic]\n\
    [-X|--units-exponent value]\n\
    [-L|--units-length value]\n\
    [--units=si]\n\
    [--right-axis scale:shift]\n\
    [--right-axis-label label]\n\
    [--right-axis-format format-string]\n\
    [-g|--no-legend]\n\
    [-F|--force-rules-legend]\n\
    [--legend-position=(north|south|west|east)]\n\
    [--legend-direction=(topdown|bottomup)]\n\
    [-z|--lazy]\n\
    [--daemon address]\n\
    [-f|--imginfo printfstr]\n\
    [-c|--color COLORTAG#rrggbb[aa]]\n\
    [--grid-dash on:off]\n\
    [--border width]\n\
    [--dynamic-labels]\n\
    [-m|--zoom factor]\n\
    [-n|--font FONTTAG:size:[font]]\n\
    [-R|--font-render-mode {normal,light,mono}]\n\
    [-B|--font-smoothing-threshold size]\n\
    [-P|--pango-markup]\n\
    [-G|--graph-render-mode {normal,mono}]\n\
    [-E|--slope-mode]\n\
    [-a|--imgformat {PNG,SVG,EPS,PDF}]\n\
    [-T|--tabwidth value]\n\
    [-b|--base value]\n\
    [-W|--watermark string]\n\
    DEF:vname=rrdfile:ds-name:CF[:step=step][:start=time][:end=time]\n\
    CDEF:vname=RPN expression\n\
    VDEF=vname:RPN expression\n\n\
    Full documentation can be found at:\n\
    http://oss.oetiker.ch/rrdtool/doc/rrdgraph.en.html";

static PyObject *
_rrdtool_graph(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *ret;
    int orig_stdout, op[2], xsize, ysize, i;
    int keep_in_mem = 0;
    double ymin, ymax;
    char **calcpr;

    if (convert_args("graph", args) == -1)
        return NULL;

    if (rrdtool_argc >= 2 && strcmp(rrdtool_argv[1], "-") == 0)
        keep_in_mem = 1;

    if (keep_in_mem) {
        orig_stdout = dup(STDOUT_FILENO);
        if (pipe(op) != 0) {
            PyErr_Format(rrdtool_OperationalError,
            "Cannot create pipe for stdout: %s", strerror(errno));
            ret = NULL;
        } else {
            dup2(op[1], STDOUT_FILENO);
            close(op[1]);

            // do something on stdout
            // read(op[0], buffer, MAX_LEN);
        }
    }

    if (rrd_graph(rrdtool_argc, rrdtool_argv, &calcpr, &xsize, &ysize, NULL,
                  &ymin, &ymax) == -1) {
        PyErr_SetString(rrdtool_OperationalError, rrd_get_error());
        rrd_clear_error();
        ret = NULL;
    } else {
        ret = PyTuple_New(keep_in_mem ? 4 : 3);

        PyTuple_SET_ITEM(ret, 0, PyLong_FromLong((long)xsize));
        PyTuple_SET_ITEM(ret, 1, PyLong_FromLong((long)ysize));

        if (calcpr) {
            PyObject *e, *t;

            e = PyList_New(0);
            PyTuple_SET_ITEM(ret, 2, e);

            for (i = 0; calcpr[i]; i++) {
                t = PyUnicode_FromString(calcpr[i]);
                PyList_Append(e, t);
                Py_DECREF(t);
                rrd_freemem(calcpr[i]);
            }
        } else {
            Py_INCREF(Py_None);
            PyTuple_SET_ITEM(ret, 2, Py_None);
        }

        /* feed buffered contents into a PyBytes object */
        if (keep_in_mem) {
            PyObject *pb;
            ssize_t rs;
            char buffer[4096];

            pb = PyBytes_FromStringAndSize("", 0);
            for (;;) {
                if ((rs = read(op[0], buffer, 4096)) <= 0)
                    break;
                else
                    PyBytes_Concat(&pb, PyBytes_FromStringAndSize(buffer, rs));
            }

            PyTuple_SET_ITEM(ret, 3, pb);
        }
    }

    if (keep_in_mem)
        dup2(orig_stdout, STDOUT_FILENO);

    destroy_args();
    return ret;
}

static PyMethodDef rrdtool_methods[] = {
	{"create", (PyCFunction)_rrdtool_create,
     METH_VARARGS, _rrdtool_create__doc__},
    {"update", (PyCFunction)_rrdtool_update,
     METH_VARARGS, _rrdtool_update__doc__},
    {"fetch", (PyCFunction)_rrdtool_fetch,
     METH_VARARGS, _rrdtool_fetch__doc__},
    {"graph", (PyCFunction)_rrdtool_graph,
     METH_VARARGS | METH_KEYWORDS, _rrdtool_graph__doc__},
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
