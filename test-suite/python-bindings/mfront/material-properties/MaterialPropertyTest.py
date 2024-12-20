try:
    import unittest2 as unittest
except ImportError:
    import unittest

from tfel.system import ExternalBehaviourDescription

class MaterialPropertyTest(unittest.TestCase):

    def test_pass(self):

        b = ExternalBehaviourDescription('src/libUmatBehaviour.so',
                                         'umattestmp', 'Tridimensional')
        self.assertTrue(b.mfront_interface == "Castem")

if __name__ == '__main__':
    unittest.main()
