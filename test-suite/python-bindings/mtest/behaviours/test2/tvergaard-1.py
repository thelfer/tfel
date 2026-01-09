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

m.setExternalStateVariable("Temperature",293.15)
m.setImposedOpeningDisplacement('Un',{0.:0.,1800.:umax/2,
                      2400.:0.,2600:-1.e-8,3000:0.,
                      3600.:umax})

m.setTimes([3.6*i for i in range(0,1001)])
m.setOutputFileName("castemtvergaard.res")
m.execute()
