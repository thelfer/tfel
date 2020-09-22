try:
    import unittest2 as unittest
except ImportError:
    import unittest

from mfront import getMFrontImplementationsPaths
    
class MadnexTest(unittest.TestCase):

    def test_pass(self):

        paths = getMFrontImplementationsPaths(file = 'MFront-test1.madnex',
                                              behaviour = ".+")
        
        
if __name__ == '__main__':
    unittest.main()
