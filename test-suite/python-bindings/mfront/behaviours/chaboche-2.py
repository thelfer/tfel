try:
    import unittest2 as unittest
except ImportError:
    import unittest
from tfel.material import ModellingHypothesis
import mfront

class ChabocheTest(unittest.TestCase):

    def test1(self):

        dsl = mfront.getDSL("Chaboche.mfront")
        dsl.analyseFile("Chaboche.mfront",[])
        
        # behaviour description
        bd = dsl.getBehaviourDescription()
        
        self.assertEqual(bd.getModellingHypotheses(),
                         [ModellingHypothesis.AXISYMMETRICALGENERALISEDPLANESTRAIN,
                          ModellingHypothesis.AXISYMMETRICAL,
                          ModellingHypothesis.PLANESTRAIN,
                          ModellingHypothesis.GENERALISEDPLANESTRAIN,
                          ModellingHypothesis.TRIDIMENSIONAL])
        
        # a deeper look at the 3D case
        d = bd.getBehaviourData(ModellingHypothesis.TRIDIMENSIONAL)
        mps_ref = [["stress", "young", "YoungModulus"],
                   ["real", "nu", "PoissonRatio"],
                   ["stress", "R_inf", "R_inf"],
                   ["stress", "R_0", "R_0"],
                   ["real", "b", "b"],
                   ["real", "k", "k"],
                   ["real", "w", "w"],
                   ["stress", "C_inf", "C_inf"],
                   ["real", "g_0", "g_0"],
                   ["real", "a_inf", "a_inf"]]
        mps = d.getMaterialProperties();
        
        for mp, mp_ref in zip(mps, mps_ref):
            self.assertEqual(mp.type, mp_ref[0])
            self.assertEqual(mp.name, mp_ref[1])
            self.assertEqual(mp.getExternalName(), mp_ref[2])
            # old syntax
            self.assertEqual(d.getExternalName(mp.name), mp_ref[2])

        isvs = d.getPersistentVariables()
        isvs_ref = [["StrainStensor", "eel", "ElasticStrain"],
                   ["strain", "p", "p"],
                   ["StrainStensor", "a", "a"]]

        for isv, isv_ref in zip(isvs, isvs_ref):
            self.assertEqual(isv.type, isv_ref[0])
            self.assertEqual(isv.name, isv_ref[1])
            self.assertEqual(isv.getExternalName(), isv_ref[2])

    def test2(self):
        f   = mfront.DSLFactory.getDSLFactory()
        dsl = f.createNewDSL("Implicit")
        dsl.analyseFile("Chaboche.mfront")
        fd = dsl.getFileDescription()
        self.assertEqual(fd.authorName, "Jean - Michel Proix")
        self.assertEqual(fd.date, "26 / 11 / 2013")
        self.assertEqual(fd.description, "")

    def test3(self):
        dsl = mfront.getDSL("Chaboche.mfront")
        dsl.setInterfaces(['generic'])
        dsl.analyseFile("Chaboche.mfront",[])
        td = dsl.getTargetsDescription()
        libs = td.getLibraries()
        self.assertEqual(len(libs), 1)
        self.assertEqual(libs[0].name, "Behaviour")
        self.assertEqual(len(libs[0].epts), 5)
        self.assertEqual(libs[0].epts,
                         ['Chaboche_AxisymmetricalGeneralisedPlaneStrain',
                          'Chaboche_Axisymmetrical',
                          'Chaboche_PlaneStrain',
                          'Chaboche_GeneralisedPlaneStrain',
                          'Chaboche_Tridimensional'])
            
if __name__ == '__main__':
    unittest.main()
