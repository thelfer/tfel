import mfront

i = mfront.OverridableImplementation("Plasticity.mfront")
i.overrideBehaviourName("Plasticity_SRMA2022")
i.overrideMaterial("A316LN")
i.overrideAuthor("John Mac Enroe")
mfront.write(i, "Plasticity.madnex")
