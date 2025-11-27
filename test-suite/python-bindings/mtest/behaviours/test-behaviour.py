try:
    import unittest2 as unittest
except ImportError:
    import unittest
import mtest
    
class BehaviourTest(unittest.TestCase):

    def test1(self):
        b = mtest.Behaviour('aster','src/libAsterBehaviour.so',
                            'asterplasticity','PlaneStrain')
        self.assertEqual(b.getBehaviourType(), 1)
        self.assertEqual(b.getBehaviourKinematic(), 0)
        self.assertEqual(b.getGradientsSize(), 4)
        self.assertEqual(b.getThermodynamicForcesSize(), 4)
        self.assertEqual(b.getSymmetryType(), 0)
        self.assertEqual(b.getStensorComponentsSuffixes(),
                         ["XX", "YY", "ZZ", "XY"])
        self.assertEqual(b.getVectorComponentsSuffixes(),
                         ["X", "Y"]) 
        self.assertEqual(b.getGradientsComponents(),
                         ["EXX", "EYY", "EZZ", "EXY"])
        self.assertEqual(b.getGradientComponentPosition("EXX"), 0)
        self.assertEqual(b.getDrivingVariablesComponents(),
                         ["EXX", "EYY", "EZZ", "EXY"])
        self.assertEqual(b.getDrivingVariableComponentPosition("EXX"), 0)
        self.assertEqual(b.getThermodynamicForcesComponents(),
                         ["SXX", "SYY", "SZZ", "SXY"])
        self.assertEqual(b.getThermodynamicForceComponentPosition("SYY"), 1)
        self.assertEqual(b.getMaterialPropertiesNames(),
                         ["H", "s0", "YoungModulus", "PoissonRatio"])
        self.assertEqual(b.getInternalStateVariablesNames(),
                         ["ElasticStrain", "EquivalentPlasticStrain"])
        self.assertEqual(b.expandInternalStateVariablesNames(),
                         ["ElasticStrainXX",
                          "ElasticStrainYY",
                          "ElasticStrainZZ",
                          "ElasticStrainXY",
                          "EquivalentPlasticStrain"])
        self.assertEqual(b.getInternalStateVariableType("ElasticStrain"), 1)
        self.assertEqual(b.getInternalStateVariableType("EquivalentPlasticStrain"), 0)
        self.assertEqual(b.getInternalStateVariablePosition("ElasticStrain"), 0)
        self.assertEqual(b.getInternalStateVariablePosition("EquivalentPlasticStrain"), 4)
        self.assertEqual(b.getExternalStateVariablesNames(), ["Temperature"])
        self.assertEqual(b.expandExternalStateVariablesNames(), ["Temperature"])
        self.assertEqual(b.getExternalStateVariableType("Temperature"), 0)
        self.assertEqual(b.getExternalStateVariablePosition("Temperature"), 0)
        self.assertEqual(set(b.getParametersNames()),
                         set(['minimal_time_step_scaling_factor',
                              'maximal_time_step_scaling_factor',
                              'theta',
                              'epsilon']))
        self.assertEqual(b.getIntegerParametersNames(), [])
        self.assertEqual(b.getUnsignedShortParametersNames(), ['iterMax'])

if __name__ == '__main__':
    unittest.main()
