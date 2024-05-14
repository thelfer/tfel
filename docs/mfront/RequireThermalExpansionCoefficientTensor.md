The `@RequireThermalExpansionCoefficientTensor` keyword requires the
thermal expansion coefficient tensor to be computed by the calling
code. This generally means that some extra material properties will be
introduced and handled by the interface before the behaviour
integration.

The thermal expansion tensor will have the same symmetry than the
behaviour. For example, for an isotropic behaviour, the thermal
expansion tensor will be proportional to the identity. For an
orthotropic behaviour, the thermal expansion tensor is diagonal and
expressed in the material frame.

~~~~{.cpp}
@RequireThermalExpansionCoefficientTensor true;
~~~~
