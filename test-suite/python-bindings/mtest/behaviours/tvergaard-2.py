import tfel.tests
from mtest import *

m = MTest()

umax = 3.e-6

setVerboseMode(VerboseLevel.VERBOSE_LEVEL2)
m.setPredictionPolicy(PredictionPolicy.LINEARPREDICTION)

m.setOutputFilePrecision(14)
m.setBehaviour("umat","src/libUmatBehaviour.so","umattvergaard")
m.setMaterialProperty('NormalStiffness',2.e16)
m.setMaterialProperty('TangentialStiffness',2.e16)
m.setMaterialProperty('MassDensity',0.)
m.setMaterialProperty('NormalThermalExpansion',0.)
m.setMaterialProperty('UltimateTensileStrength',200.e6)
m.setMaterialProperty('MaximumOpeningDisplacement',3.e-6)
m.setImposedOpeningDisplacement('Un',{0: 0})

m.setExternalStateVariable("Temperature",293.15)

m.setOutputFileName("castemtvergaard-2.res")
m.completeInitialisation()

s  = MTestCurrentState()
wk = MTestWorkSpace()
m.initializeCurrentState(s)
m.initializeWorkSpace(wk)

t = [3.6*i for i in range(0,1001)]
# values at 0
m.printOutput(t[0], s)

for i in range(0, len(t) - 1):
    m.setEvolutionValue('Un', t[i+1], umax * t[i+1] / t[-1])
    m.execute(s, wk, t[i], t[i+1])
    m.printOutput(t[i+1], s)
