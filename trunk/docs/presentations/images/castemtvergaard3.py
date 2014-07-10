import std
import tfel.tests
import tfel.math
from mfront.mtest import *

m = MTest()

umax = 3.e-6
m.setBehaviour("umat","src/libUmatBehaviour.so","umattvergaard")
m.setMaterialProperty('NormalStiffness',2.e16)
m.setMaterialProperty('TangentialStiffness',2.e16)
m.setMaterialProperty('MassDensity',0.)
m.setMaterialProperty('NormalThermalExpansion',0.)
m.setMaterialProperty('MaximumStress',200.e6)
m.setMaterialProperty('MaximumOpeningDisplacement',3.e-6)

m.setExternalStateVariable("Temperature",293.15)

# Attention, pour pouvoir modifier l'evolution du chargement, il faut
# le declarer avec un dictionnaire !
m.setImposedOpeningDisplacement('Un',{0.:0.})

s  = MTestCurrentState()
wk = MTestWorkSpace()

m.setOutputFileName("castemtvergaard3.res")
m.completeInitialisation()
m.initializeCurrentState(s)
m.initializeWorkSpace(wk)

t = [3.6*i for i in range(0,1001)]
m.printOutput(t[0],s)

# do the job
for i in range(0,len(t)-1):
    m.setEvolutionValue('Un',t[i+1],umax*t[i+1]/t[-1])
    m.execute(s,wk,t[i],t[i+1])
    print s.s0
    m.printOutput(t[i+1],s)
