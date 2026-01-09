from mtest import PipeTest,StudyCurrentState,SolverWorkSpace, \
    setVerboseMode, VerboseLevel

# no output
setVerboseMode(VerboseLevel.VERBOSE_QUIET)

Re = 4.18e-3
Fz = 3.1415926 * Re * Re * 25e6

t = PipeTest()

# geometric and meshing options
t.setInnerRadius(0)
t.setOuterRadius(Re)
t.setNumberOfElements(1)
t.setElementType('Quadratic')

# Type of loading
t.setAxialLoading('ImposedAxialForce')

# no geometrical update
t.performSmallStrainAnalysis()

t.setMaximumNumberOfSubSteps(1)
# by default, with the 'castem' interface, one does not use the
# consistent tangent operator. Using the acceleration algorithm
# is mandatory for convergence.
t.setUseCastemAccelerationAlgorithm(True)

# axial loading
t.setAxialForceEvolution({0.:0., 1.e-6 :Fz, 3600:Fz})

# definition of the mechanical behaviour and the material properties
t.setBehaviour('castem', 'src/libUmatBehaviour.so', 'umatimplicitnorton')
t.setMaterialProperty('YoungModulus', 150e9)
t.setMaterialProperty('PoissonRatio', 0.3)
t.setMaterialProperty('NortonCoefficient', 8.e-67)
t.setMaterialProperty('NortonExponent', 8.2)

# temperature (mandatory)
t.setExternalStateVariable('Temperature',293.15)

# results file
t.setOutputFilePrecision(14)
t.setOutputFileName('pipe-3a.res')

s  = StudyCurrentState()
wk = SolverWorkSpace()

# complete initialisation of the problem
t.completeInitialisation()
t.initializeCurrentState(s)
t.initializeWorkSpace(wk)

# time discretization
times = [0]+[1.e-6+((3600.-1.e-6)/20.)*i for i in range(21)];

# get the state of the pipe (unique structure defined)
scs = s.getStructureCurrentState("")
p   = max([cs.getInternalStateVariableValue("EquivalentViscoplasticStrain")
           for cs in scs.istates])

out = open("pipe-3b.res", "w")

out.write(f"{times[0]} {p}\n")

# loop over the time steps
for i in range(0,len(times)-1):
    # search the mechanical equilibrium at the end of the time step
    t.execute(s,wk,times[i],times[i+1])
    # write in the output file
    t.printOutput(times[i+1],s)
    # iterate of over the integration points
    p = max([cs.getInternalStateVariableValue("EquivalentViscoplasticStrain")
             for cs in scs.istates])
    out.write(f"{times[i+1]} {p}\n")
