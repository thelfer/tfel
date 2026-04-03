import mfront

i = mfront.OverridableImplementation("Plasticity.mfront")
i.overrideBehaviourName("Plasticity_SRMA2022")
i.overrideMaterial("A316LN")
i.overrideAuthor("John Mac Enroe")
i.overrideByAParameter("H", 2e9)
mfront.write(i, "Plasticity.madnex")
