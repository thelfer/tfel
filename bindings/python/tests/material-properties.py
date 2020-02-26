import os
try:
    import unittest2 as unittest
except ImportError:
    import unittest

import tfel.system

class PyMFrontMaterialProperty(unittest.TestCase):

    def test_pass(self):

        eps = 1.e-14
        
        l = os.environ['MFRONT_MATERIAL_PROPERTY_LIBRARY']
        mp = os.environ['MFRONT_MATERIAL_PROPERTY_FUNCTION']
        
        elm = tfel.system.ExternalLibraryManager.getExternalLibraryManager()

        names = elm.getMaterialPropertyVariables(l,mp)
        self.assertTrue(len(names)==1)
        self.assertTrue(names[0]=="Temperature")
        self.assertTrue(elm.hasPhysicalBounds(l,mp,"Temperature"))
        self.assertTrue(not elm.hasUpperPhysicalBound(l,mp,"Temperature"))
        self.assertTrue(elm.hasLowerPhysicalBound(l,mp,"Temperature"))
        self.assertTrue(elm.hasUpperBound(l,mp,"Temperature"))
        self.assertTrue(elm.hasLowerBound(l,mp,"Temperature"))
        self.assertTrue(abs(elm.getLowerPhysicalBound(l,mp,"Temperature"))<eps)
        self.assertTrue(abs(elm.getLowerBound(l,mp,"Temperature")-293.15)<eps)
        self.assertTrue(abs(elm.getUpperBound(l,mp,"Temperature")-973.15)<eps)
        
if __name__ == '__main__':
    unittest.main()
