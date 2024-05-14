The `ComputeStiffnessTensor` keyword is used to define the elastic
stiffness tensor based on the elastic material properites given as an
array of entries. After this array, a semi-colon is expected.

This array is used to automatically used to declared the elastic
material properties of the behaviour (see the
`@ElasticMaterialProperties` keyword for details).

An entry can be either a string referring to an external `MFront`
file, a formula or a numerical value.

If an entry refers to an external `MFront` file or the formula, all
the inputs of this material property must be either:

- a material property
- a parameter
- a state variable
- an external state variable

of the behaviour.

## Isotropic case

In the isotropic case, two entries are expected in the array, in that
order:

- the Young Modulus
- the Poisson ratio

## Orthotropic case

In the orthotropic case, 9 entries are expected in the array, in that
order:

- three Young modulus \((E_{1},E_{2},E_{3})\)
- three Poisson ratio \((nu_{12},nu_{23},nu_{13})\)
- three shear modulus \((G_{12},G_{23},G_{13})\)

In the orthoropic case, computation of the stiffness tensor rely on the
definition of an orthotropic convention. For example, the `Pipe`
orthotropic convention will lead to automatically exchange the second
and first axes when computing the stiffness tensor for the plane strain,
plane stress and generalised plane strain hypotheses. See the
`OrthotropicBehaviour` keyword for details.

## Tensors computed

In an implicit scheme, this keyword leads to the definition and the
automatic computation of the tensors `D` and `D_tdt` which
respectively refer to the stiffness tensor at \(t+\theta\,dt\) and
\(t+dt\).

In an explicit scheme, this this keyword leads to the definition of
the stiffness tensor which is automatically updated if this tensor
evolves during the time step.

## Example

~~~~ {#ComputeStiffnessTensor .cpp}
@ComputeStiffnessTensor{7.8e+4,2.64233e+5,3.32e+5,
    0.13,0.24,0.18,
    4.8e+4,1.16418e+5,7.8e+4};
~~~~
