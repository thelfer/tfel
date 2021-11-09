import os
try:
    import unittest2 as unittest
except ImportError:
    import unittest
import tfel.material
import mtest

class SmallStrainTridimensionalBehaviourWrapper(unittest.TestCase):

    def test(self):
        l = os.environ['MTEST_BEHAVIOUR_LIBRARY']
        b = mtest.Behaviour(interface = 'generic',
                            library = l,
                            function = 'ImplicitNorton',
                            hypothesis = 'PlaneStrain',
                            wrapper = 'SmallStrainTridimensionalBehaviourWrapper')
        self.checkBehaviour(b)
        
    def test2(self):
        l = os.environ['MTEST_BEHAVIOUR_LIBRARY']
        b = mtest.Behaviour(wrapper = 'SmallStrainTridimensionalBehaviourWrapper',
                            library = l,
                            function = 'ImplicitNorton',
                            hypothesis = 'PlaneStrain')
        self.checkBehaviour(b)

    def checkBehaviour(self, b):
        isvs = b.getInternalStateVariablesNames()
        self.assertTrue(len(isvs) == 7)
        self.assertTrue(isvs[0] == 'ElasticStrainXX')
        self.assertTrue(isvs[1] == 'ElasticStrainYY')
        self.assertTrue(isvs[2] == 'ElasticStrainZZ')
        self.assertTrue(isvs[3] == 'ElasticStrainXY')
        self.assertTrue(isvs[4] == 'ElasticStrainXZ')
        self.assertTrue(isvs[5] == 'ElasticStrainYZ')
        self.assertTrue(isvs[6] == 'p')

if __name__ == '__main__':
    unittest.main()
