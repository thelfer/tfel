import os
try:
    import unittest2 as unittest
except ImportError:
    import unittest

import tfel.system


class PyMFrontMaterialProperty(unittest.TestCase):
    def test_elm(self):

        eps = 1.e-14

        l = os.environ['MFRONT_MATERIAL_PROPERTY_LIBRARY']
        mp = os.environ['MFRONT_MATERIAL_PROPERTY_FUNCTION']

        elm = tfel.system.ExternalLibraryManager.getExternalLibraryManager()

        names = elm.getMaterialPropertyVariables(l, mp)
        self.assertTrue(len(names) == 1)
        self.assertTrue(names[0] == "Temperature")
        self.assertTrue(elm.hasPhysicalBounds(l, mp, "Temperature"))
        self.assertTrue(not elm.hasUpperPhysicalBound(l, mp, "Temperature"))
        self.assertTrue(elm.hasLowerPhysicalBound(l, mp, "Temperature"))
        self.assertTrue(elm.hasUpperBound(l, mp, "Temperature"))
        self.assertTrue(elm.hasLowerBound(l, mp, "Temperature"))
        self.assertTrue(
            abs(elm.getLowerPhysicalBound(l, mp, "Temperature")) < eps)
        self.assertTrue(
            abs(elm.getLowerBound(l, mp, "Temperature") - 293.15) < eps)
        self.assertTrue(
            abs(elm.getUpperBound(l, mp, "Temperature") - 973.15) < eps)

    def test_externalmaterialpropertydescription(self):

        eps = 1.e-14

        l = os.environ['MFRONT_MATERIAL_PROPERTY_LIBRARY']
        mp = os.environ['MFRONT_MATERIAL_PROPERTY_FUNCTION']
        emp = tfel.system.ExternalMaterialPropertyDescription(l, mp)

        self.assertTrue(emp.author == "T . Helfer")
        self.assertTrue(emp.date == "2008 - 11 - 17")
        self.assertTrue(emp.law == "YoungModulus_SRMA")
        self.assertTrue(emp.material == "VanadiumAlloy")

        self.assertTrue(emp.description == (
            "Corrélation établie sur la nuance V - 4 Cr - 4 Ti .\n"
            "\n"
            "Propriétés extraites du document :\n"
            "\n"
            "Propriétés et comportement mécanique d alliages\n"
            "de Vanadium avant , après et sous irradiation\n"
            "\n"
            "Marion Le Flem , Jean - Luc Bechade , Annick Bougault ,\n"
            "Aurore Michaux , Lionel Gosmain , Jean - Louis Seran\n"
            "DMN / SRMA / LA2M / NT / 2008 - 2967 / A"))

        self.assertTrue(emp.output == "YoungModulus")
        self.assertTrue(len(emp.arguments) == 1)
        self.assertTrue(emp.arguments[0] == "Temperature")
        self.assertTrue(emp.hasPhysicalBounds("Temperature"))
        self.assertTrue(not emp.hasUpperPhysicalBound("Temperature"))
        self.assertTrue(emp.hasLowerPhysicalBound("Temperature"))
        self.assertTrue(emp.hasUpperBound("Temperature"))
        self.assertTrue(emp.hasLowerBound("Temperature"))
        self.assertTrue(abs(emp.getLowerPhysicalBound("Temperature")) < eps)
        self.assertTrue(abs(emp.getLowerBound("Temperature") - 293.15) < eps)
        self.assertTrue(abs(emp.getUpperBound("Temperature") - 973.15) < eps)


if __name__ == '__main__':
    unittest.main()
