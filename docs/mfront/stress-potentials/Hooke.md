The `Hooke` brick describes the linear elastic part of
the behavior of an isotropic or orthotropic material.

## Evolution of the elastic strain

This bricks relies on the fact that the behavior is based on the
strain split hypothesis. 

The elastic strain must be defined as the first integration
variable. The associated variable must be called `eel` and its
glossary name must be `ElasticStrain`. This is automatically the case
with the `@Implicit` dsl.

The total strain increment `deto` is automatically subtracted to the
equation associated with the elastic (`feel`), which is equivalent to
the following statement:
\[
feel -= deto
\]

## Computation of the stress

If the elastic behavior is orthotropic, the stiffness tensor must be
available available (using the keyword `@RequireStiffnessTensor`) or
computed by the behavior (using the keyword
`@ComputeStiffnessTensor`). If those keywords are not explicitly
used, the brick will automatically sets the attribute
`requireStiffnessTensor` to `true` which has the same effect than the
`@RequireStiffnessTensor` keyword.

Thus, two cases arise:

- the stiffness tensor is available (using keyword
  `@RequireStiffnessTensor`) or computed by the behavior
  (`@ComputeStiffnessTensor`).
- the behavior has is an isotropic elastic behavior and the
  stiffness tensor is not available.

### First case: the stiffness tensor is available

#### Computation of the stress at \(t+\theta\,dt\)

At \(t+\theta\,dt\), the stress are computed using:

\[
\mts{\sigma}=\tenseurq{D}\,\colon\,\mts{\tepsilonel}
\]

#### Computation of the final stress at \(t+dt\)

If the stiffness tensor is available using the
`@RequireStiffnessTensor`, the final stress \(\ets{\sigma}\) is
computed using the following formula :

\[
\ets{\sigma}=\tenseurq{D}\,\colon\,\ets{\tepsilonel}
\]

If the stiffness tensor is computed using `@ComputeStiffnessTensor`,
the final \(\ets{\sigma}\) stress is computed using:

\[
\ets{\sigma}=\ets{\tenseurq{D}}\,\colon\,\ets{\tepsilonel}
\]

### Second case: the stiffness tensor is not available

In this case, the elastic behavior of the material is isotropic. The
computation of the stress requires the definition of the first Lamé
coefficient and the shear modulus (second Lamé coefficient).

The Lamé coefficients are derived from the Young modulus and Poisson
ratio. They can be defined using:

- the `@ElasticMaterialProperties` keyword. In this case, the
  `Implicit` dsl already automatically computes the following
  variables (See the documentation of the `@ElasticMaterialProperties`
  keyword):
    - `young`: the Young modulus at \(t+\theta\,dt\)
    - `nu`: the Poisson ratio modulus at \(t+\theta\,dt\)
    - `lambda`: the first Lamé coefficient at \(t+\theta\,dt\)
    - `mu`: the second Lamé coefficient at \(t+\theta\,dt\)
    - `young_tdt`: the Young modulus at \(t+dt\)
    - `nu_tdt`: the Poisson ratio modulus at \(t+dt\)
    - `lambda_tdt`: the first Lamé coefficient at \(t+dt\)
    - `mu_tdt`: the second Lamé coefficient at \(t+dt\)
- the Young modulus and Poisson ratio has been defined as material
  properties or parameters. In this case, the names of those variables
  must be `young` and the `nu` and the glossary names associated with
  those variables must be respectively `YoungModulus` and
  `PoissonRatio`. The Lamé coefficients will be computed and stored in
  a data structure used internally by the brick.

If the material properties are not defined using one of those two
ways, the appropriate material properties will be automatically
defined by the brick.

#### Computation of the stress at \(t+\theta\,dt\)

At \(t+\theta\,dt\), the stress are computed using the following
formula:
\[
\mts{\sigma}=\lambda\,\trace{\mts{\tepsilonel}}+2\,\mu\,\mts{\tepsilonel}
\]
where \(\lambda\) and \(\mu\) are respectively the values of the first
and second Lamé coefficients at \(t+\theta\,dt\)

#### Computation of the final stress at \(t+dt\)

The final stress \(\ets{\sigma}\) is computed using the following
formula :

\[
\ets{\sigma}=\ets{\lambda}\,\trace{\ets{\tepsilonel}}+2\,\ets{\mu}\,\ets{\tepsilonel}
\]

## Enforcement of the plane stress conditions: computation of the axial strain

If the user has explicitly specified that the axisymmetric generalized
plane stress modeling hypothesis must be supported by the behavior
using the `@ModellingHypothesis` keyword or the `@ModellingHypotheses`
keyword, this support is performed by automatically introducing an
additional state variable: the axial strain. The associated variable
is `etozz`, although this variable shall not be used by the end
user. The glossary name of this variable is `AxialStrain`.

The introduction of the variable modify the strain split equation like
this:
\[
feel(2) += detozz;
\]
where \(detozz\) is the increment of the axial strain. The associated
jacobian term is added if necessary.

The plane stress condition is enforced by adding an additional
equation to the implicit system ensuring that:
\[
\ets{\sigma_{zz}}=0
\]

This equation is appropriately normalized using one of the elastic
properties. The associated jacobian term are added if necessary.

## Enforcement of the generalized plane stress conditions: computation of the axial strain

If the user has explicitly specified that the axisymmetric generalized
plane stress modeling hypothesis must be supported by the behavior
using the `@ModellingHypothesis` keyword or the `@ModellingHypotheses`
keyword, this support is performed by automatically introducing an
additional state variable, the axial strain and an additional external
state variable, the axial stress.

The variable associated to the axial strain is `etozz`, although this
variable shall not be used by the end user. The glossary name of this
variable is `AxialStrain`.

The variable associated to the axial stress is `sigzz`, although this
variable shall not be used by the end user. The glossary name of this
variable is `AxialStress`.

The introduction of the variable modify the strain split equation like
this:
\[
feel(1) += detozz;
\]
where \(detozz\) is the increment of the axial strain. The associated
jacobian term is added if necessary.

The plane stress condition is enforced by adding an additional
equation to the implicit system ensuring that:
\[
\ets{\sigma_{zz}}-\sigma^{zz}-d\sigma^{zz}=0
\]

where \(\sigma^{zz}\) is the value of the axial stress at the
beginning of the time step and \(d\sigma^{zz}\) is the value of the
increment of the axial stress.

This equation is appropriately normalized using one of the elastic
properties. The associated jacobian terms are added if necessary.

## Generic computation of the tangent operator

The elastic and secant operator are equal to the elastic stiffness
matrix at the end of the time step. How this elastic stiffness matrix
is obtained depends on the many cases described before.

The consistent tangent operator is computed by multiplying the elastic
stiffness matrix at the end of the time step by a partial invert of
the jacobian matrix. This procedure is discussed in depth in the
MFront manuals.

## Options of the brick

The `Hooke` brick supports the following options:

- `Isotropic`: the elastic part of the behavior will be assumed
  isotropic. This is the default for an isotropic material. This is
  useful if the material is declared orthotropic.
- `Orthotropic`: the elastic part of the behavior will be assumed
  isotropic. This is the default for an isotropic material.
- `NoPlaneStressSupport`: plane stress and axisymmetrical generalized
  plane stress hypotheses support will not be added.
- `NoGenericTangentOperator`: generic computation of the consistent
  tangent operator will not be added.
- `NoGenericPredictionOperator`: generic computation of the prediction
  operator will not be added.
