import numpy
import std
import tfel.math
import tfel.tests
from mfront.mtest import *
from observation import temps_instants, Observation


def DirectOperator(XX):
    v_KRS = 3.00E+10  # Pa
    v_KRD = 3.00E+10  # Pa
    v_ETARS = 5.00E+17  # Pa.s
    v_ETARD = 1.00E+16  # Pa.s
    v_ETAIS = 1.00E+18  # Pa.s
    v_ETAID = 1.00E+18  # Pa.s
    v_KAPPA = 1.00E-04
    TrueBackground = numpy.array(
        (v_KRS, v_KRD, v_ETARS, v_ETARD, v_ETAIS, v_ETAID, v_KAPPA))
    xx = numpy.ravel(XX) * TrueBackground
    print "xx=", XX
    t = temps_instants
    t[1] = 1.e-1

    m = MTest()
    setVerboseMode(VerboseLevel.VERBOSE_QUIET)
    m.setPredictionPolicy(PredictionPolicy.LINEARPREDICTION)
    m.setBehaviour('aster', 'src/libAsterBehaviour.so', 'asterburger')
    m.setMaterialProperty('young', 31000.e6)
    m.setMaterialProperty('nu', 0.2)
    m.setMaterialProperty('KRS', xx[0])
    m.setMaterialProperty('KRD', xx[1])
    m.setMaterialProperty('NRS', xx[2])
    m.setMaterialProperty('NRD', xx[3])
    m.setMaterialProperty('NIS', xx[4])
    m.setMaterialProperty('NID', xx[5])
    m.setMaterialProperty('KAPPA', xx[6])
    m.setMaterialProperty('NFD', 0.)
    m.setMaterialProperty('Cini', 1.)
    m.setExternalStateVariable("Temperature", 300.)
    m.setExternalStateVariable("C", 1.)
    m.setImposedStress('SXX', {0.: 0., 1.: 4E6, 3.E7: 4E6})
    s = MTestCurrentState()
    wk = MTestWorkSpace()
    m.completeInitialisation()
    m.initializeCurrentState(s)
    m.initializeWorkSpace(wk)
    YY1 = [0]
    for i in range(0, len(t) - 1):
        m.execute(s, wk, t[i], t[i + 1])
        YY1.append(s.e1[0])
    return numpy.array(YY1)


if __name__ == '__main__':
    zz = [0.391, 1.978, 0.757, 1.237, 0.1, 0.1, 0.461]
    y = DirectOperator(zz)
    print 'y=', y
    from observation import Observation as Yobs
    print "Yobs=", Yobs
    print "diff=", Yobs - y
