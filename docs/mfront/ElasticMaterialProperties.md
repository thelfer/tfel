The `@ElasticMaterialProperties` keyword is used give the material
properties for standard mechanical behaviours.

This keywords is followed by an array or a dictionay which values can be
either a string referring to a formula, an external `MFront` file or a
numerical value. After this array or dictiornay, a semi-colon is expected.

If an entry refers to an external `MFront` file or a formula, all the
inputs of this material property must be either:

- a material property
- a parameter
- a state variable
- an external state variable

of the behaviour.

Elastic material properties are used by behaviours bricks.

## Isotropic case

### Syntax using a dictionary

A dictionary with two entries named `young_modulus` and `poisson_ratio`
is expected.

### Syntax using an array

In the isotropic case, two entries are expected in the array defining, in that
order, the Young Modulus and the Poisson ratio.

###  Automatically defined variables

In domain specific languages providing an implicit scheme
(`Implicit`,`ImplicitII`, `ImplicitFiniteStrain`), the following local
variables are automatically defined and computed:

- `young`, `young_tdt` which respectively stand for the Young modulus
  at \(t+\theta\,dt\) and \(t+\,dt\).
- `nu`, `nu_tdt` which respectively stand for the Poisson ratio at
  \(t+\theta\,dt\) and \(t+\,dt\).

## Orthotropic case

### Syntax using a dictionary

A dictionary with nine entries named 
`young_modulus1`, `young_modulus2`, `young_modulus3`,
`poisson_ratio12`, `poisson_ratio23`, `poisson_ratio13`, 
`shear_modulus12`, `shear_modulus23` and `shear_modulus13`
is expected.

### Syntax using an array

In the orthotropic case, 9 entries are expected in the array defining,
in that order:

- three Young modulus \((E_{1},E_{2},E_{3})\),
- three Poisson ratio \((nu_{12},nu_{23},nu_{13})\),
- three shear modulus \((G_{12},G_{23},G_{13})\).

### Orthotropic axes convention

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

~~~~ {#ElasticMaterialProperties .cpp}
@ElasticMaterialProperties {
  young_modulus: "AISI348_YoungModulus.mfront",
  poisson_ratio: 0.3};
~~~~
