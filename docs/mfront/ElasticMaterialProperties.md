The `@ElasticMaterialProperties` keyword is used give the material
properties for standard mechanical behaviours.

This keywords is followed by an array which values can be either a
string referring to a formula, an external `MFront` file or a
numerical value. After this array, a semi-colon is expected.

If an entry refers to an external `MFront` file or a formula, all the
inputs of this material property must be either:

- a material property
- a parameter
- a state variable
- an external state variable

of the behaviour.

Elastic material properties are used by behaviours bricks.

## Isotropic case

In the isotropic case, two entries are expected in the array, in that
order:

- the Young Modulus
- the Poisson ratio

In domain specific languages providing an implicit scheme
(`Implicit`,`ImplicitII`, `ImplicitFiniteStrain`), the following local
variables are automatically defined and computed:

- `young`, `young_tdt` which respectively stand for the Young modulus
  at \(t+\theta\,dt\) and \(t+\,dt\).
- `nu`, `nu_tdt` which respectively stand for the Poisson ratio at
  \(t+\theta\,dt\) and \(t+\,dt\).

## Orthotropic case

In the orthotropic case, 9 entries are expected in the array, in that
order:

- three Young modulus \((E_{1},E_{2},E_{3})\)
- three Poisson ratio \((nu_{12},nu_{23},nu_{13})\)
- three shear modulus \((G_{12},G_{23},G_{13})\)

In the orthoropic case, behaviours bricks will rely on the definition
of an orthotropic convention to compute the stiffness tensor. For
example, the `Pipe` orthotropic convention will lead to automatically
exchange the second and first axises when computing the stiffness
tensor for the plane strain, plane stress and generalised plane strain
hypotheses.

## Example

~~~~ {#ElasticMaterialProperties .cpp}
@ElasticMaterialProperties {"AISI348_YoungModulus.mfront",0.3};
~~~~
