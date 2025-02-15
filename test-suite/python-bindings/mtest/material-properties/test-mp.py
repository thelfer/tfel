try:
    import unittest2 as unittest
except ImportError:
    import unittest
import mtest
    
class MaterialPropertyTest(unittest.TestCase):
    def test1(self):
        E_ref = 125111405902.5
        young_modulus = mtest.MaterialProperty(
            'src/libCastemVanadium.so', 'VanadiumAlloy_YoungModulus_SRMA')
        # using setVariableValue / getValue
        young_modulus.setVariableValue('Temperature', 562)
        E = young_modulus.getValue()
        self.assertAlmostEqual(E, E_ref)
        # using a vector
        E = young_modulus.getValue([562])
        self.assertAlmostEqual(E, E_ref)
        # using a dictionary
        E = young_modulus.getValue({'Temperature': 562})
        self.assertAlmostEqual(E, E_ref)
        # for material properties with only one argument
        E = young_modulus.getValue(562)
        self.assertAlmostEqual(E, E_ref)

    def test2(self):
        E_ref = 125111405902.5
        young_modulus = mtest.MaterialProperty(
            'src/libCastemVanadium.so', 'VanadiumAlloy_YoungModulus_SRMA')
        # using a vector
        E = young_modulus([562])
        self.assertAlmostEqual(E, E_ref)
        # using a dictionary
        E = young_modulus({'Temperature': 562})
        self.assertAlmostEqual(E, E_ref)
        # for material properties with only one argument
        E = young_modulus(562)
        self.assertAlmostEqual(E, E_ref)
        # for material properties with only one argument
        E = young_modulus(562)

if __name__ == '__main__':
    unittest.main()
