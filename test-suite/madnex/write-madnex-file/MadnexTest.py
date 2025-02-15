try:
    import unittest2 as unittest
except ImportError:
    import unittest

from mfront import getImplementationsPaths


class MadnexTest(unittest.TestCase):
    def test_pass(self):

        paths = getImplementationsPaths(file='MFront-test1.madnex',
                                        behaviour=".+")
        self.assertTrue(len(paths) == 1)
        self.assertTrue(paths[0] == 'mdnx:MFront-test1.madnex:'
                        'behaviour::J2Plasticity')


if __name__ == '__main__':
    unittest.main()
