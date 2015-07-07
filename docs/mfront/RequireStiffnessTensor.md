The `@RequireStiffnessTensor` keyword requires the stiffness tensor to
be computed by the calling code. This generally means that some extra
material properties will be introduced and handled by the interface
before the behaviour integration.

By default, the stiffness tensor will have the same symmetry than the
behaviour (see @IsotropicElasticBehaviour to change this in the case
of an orthotropic behaviour).

~~~~{.cpp}
@RequireStiffnessTensor true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


