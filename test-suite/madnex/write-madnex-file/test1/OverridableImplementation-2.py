import mfront

i = mfront.OverridableImplementation(
    "mdnx:Plasticity.madnex:behaviour:A316LN:Plasticity_SRMA2022")
i.overrideBehaviourName("Plasticity_SRMA2022b")
i.overrideByAParameter("H", 2.4e9)
mfront.write(i, "Plasticity-2.madnex")
