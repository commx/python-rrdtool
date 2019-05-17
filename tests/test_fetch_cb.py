import math
import rrdtool
import unittest
import sys


class TestFetchCallback(unittest.TestCase):
    @staticmethod
    def check_skip():
        if not hasattr(rrdtool, 'register_fetch_cb'):
            if sys.version_info >= (2, 7):
                raise unittest.SkipTest('register_fetch_cb not available')
            return True

    def setUp(self):
        self.graphv_args = [
            '-',
            '--title', 'Callback Demo',
            '--start', '1424540800',
            '--end', 'start+24h',
            '--lower-limit=0',
            '--interlaced',
            '--imgformat', 'PNG',
            '--width=450',
            'DEF:a=cb//extrainfo:a:AVERAGE',
            'DEF:b=cb//:b:AVERAGE',
            'DEF:c=cb//:c:AVERAGE',
            'LINE:a#00b6e4:a',
            'LINE:b#10b634:b',
            'LINE:c#503d14:c',
            'VDEF:av=a,AVERAGE',
            'PRINT:av:%8.6lf'
        ]

    def test_callback_return_type(self):
        """
        Test whether callback return type is checked correctly.
        The callback must always return a dict.
        """
        if self.check_skip():
            return

        def my_callback(*args, **kwargs):
            return None

        rrdtool.register_fetch_cb(my_callback)

        # TestCase.assertRaisesRegexp was renamed in Python 3.2
        if sys.version_info >= (3, 2):
            assert_raises_regex = self.assertRaisesRegex
        else:
            assert_raises_regex = self.assertRaisesRegexp

        assert_raises_regex(
            rrdtool.OperationalError,
            'expected callback method to be a dict',
            rrdtool.graphv,
            self.graphv_args
        )

    def test_callback_args(self):
        """
        Test whether all required arguments are passed in kwargs.
        """
        if self.check_skip():
            return

        def my_callback(*args, **kwargs):
            required_args = ('filename', 'cf', 'start', 'end', 'step')
            for k in required_args:
                self.assertIn(k, kwargs)

            items = int((kwargs['end'] - kwargs['start']) / kwargs['step'])
            return {
                'start': kwargs['start'],
                'step': 300,
                'data': {
                    'a': [math.sin(x / 200) for x in range(0, items)],
                    'b': [math.cos(x / 200) for x in range(10, items)],
                    'c': [math.sin(x / 100) for x in range(100, items)],
                }
            }

        rrdtool.register_fetch_cb(my_callback)
        rrdtool.graphv(*self.graphv_args)


if __name__ == '__main__':
    unittest.main()
