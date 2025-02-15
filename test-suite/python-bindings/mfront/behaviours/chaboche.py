from tfel.material import ModellingHypothesis
import mfront

dsl = mfront.getDSL("Chaboche.mfront")
dsl.analyseFile("Chaboche.mfront",[])

# behaviour description
bd = dsl.getBehaviourDescription()

print("List of supported modelling hypotheses:\n")
print(bd.getModellingHypotheses())
print("\n")

# a deeper look at the 3D case
d = bd.getBehaviourData(ModellingHypothesis.TRIDIMENSIONAL)
print("List of material properties in 3D:\n")
for mp in d.getMaterialProperties():
    print("- "+mp.type+" "+mp.name+" as "+d.getExternalName(mp.name))

print("\nList of state variables in 3D:\n")
for iv in d.getPersistentVariables():
    print("- "+iv.type+" "+iv.name+" as "+d.getExternalName(iv.name))