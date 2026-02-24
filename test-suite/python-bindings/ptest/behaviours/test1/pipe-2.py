import tfel.tests
from mtest import PipeTest,StudyCurrentState,SolverWorkSpace, \
    setVerboseMode, VerboseLevel

setVerboseMode(VerboseLevel.VERBOSE_QUIET)

t = PipeTest()
t.setMaximumNumberOfSubSteps(1)
# geometry and meshing
t.setInnerRadius(4.2e-3)
t.setOuterRadius(4.7e-3)
t.setNumberOfElements(10)
t.setElementType('Linear')

# modelling hypothesis
t.setAxialLoading('None')
t.performSmallStrainAnalysis()

t.setTimes([0, 1])
t.setInnerPressureEvolution(1.5e6)
t.setOuterPressureEvolution({0 : 1.5e6, 1 : 10e6})

t.setBehaviour('castem', 'src/libUmatBehaviour.so', 'umatelasticity')
t.setMaterialProperty('YoungModulus', 150e9)
t.setMaterialProperty('PoissonRatio', 0.3)
t.setExternalStateVariable('Temperature', 293.15)

t.setOutputFilePrecision(14)
t.setOutputFileName("pipe-2.res")

t.completeInitialisation()

s  = StudyCurrentState()
wk = SolverWorkSpace()
t.initializeCurrentState(s)
t.initializeWorkSpace(wk)

# values at 0
t.printOutput(0, s)
# resolution from 0 to 1
t.execute(s, wk, 0, 1)
# values at 1
t.printOutput(1, s)
