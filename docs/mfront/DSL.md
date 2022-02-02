The `@DSL` keyword specify the domain specific language (dsl) used.

The list of available dsl's is returned by the `--list-dsl` option of
`mfront`:

~~~~{.bash}
$ mfront --list-dsl
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As the time of writting this notice, the following dsl's are available:

- `DefaultDSL`: this parser is the most generic one as it does not
  make any restriction on the behaviour or the integration method that
  may be used.
- `DefaultCZMDSL`: this parser is the most generic one as it does not
  make any restriction on the behaviour or the integration method that
  may be used.
- `DefaultFiniteStrainDSL`: this parser is the most generic one as it
  does not make any restriction on the behaviour or the integration
  method that may be used.
- `Implicit`: this parser provides a generic integrator based on an
  implicit scheme. The elastic strain is automatically defined as a
  state variable
- `ImplicitII`: this parser provides a generic integrator based on a
  theta method. Unlike `Implicit`, the elastic strain is not
  automatically defined as a state variable.
- `ImplicitFiniteStrain`: this parser provides a generic integrator
  based on a theta method..
- `IsotropicMisesCreep`: this parser is used for standard creep
  behaviours of the form \(\dot{p}=f(s)\) where \(p\) is the equivalent
  creep strain and \(s\) the equivalent mises stress.
- `IsotropicPlasticMisesFlow`: this parser is used for standard
  plastics behaviours with yield surface of the form \(f(s,p)=0\)
  where \(p\) is the equivalent creep strain and \(s\) the equivalent
  mises stress.
- `IsotropicStrainHardeningMisesCreep`: this parser is used for
  standard strain hardening creep behaviours of the form
  \(\dot{p}=f(s,p)\) where \(p\) is the equivalent creep strain and
  \(s\) the equivalent mises stress.
- `MaterialLaw`: this parser is used to define material properties.
- `Model`: this parser is used to define simple material models.
- `MultipleIsotropicMisesFlows `: this parser is used to define
  behaviours combining several isotropic flows. Supported flow type
  are 'Creep' (\(\dot{p}=f(s)\)) 'StrainHardeningCreep'
  (\(\dot{p}=f(s,p)\)) and 'Plasticity' (\(f(p,s)=0\)) where \(p\) is
  the equivalent plastic strain and \(s\) the equivalent mises stress.
- `RungeKutta`: this parser provides a generic integrator based on one
  of the many Runge-Kutta algorithm.

## DSL options

A DSL' behaviour may be changed using options defined either in the
`MFront` file using a JSON-like syntax or as a command line
argument.

The options related to a specific `DSL` can be retrieved using the
`--list-dsl-options` command line argument, as follows:

~~~~{.bash}
$ mfront --list-dsl-options=Model
- parameters_as_static_variables: boolean stating if the parameter shall be treated as static variables.
- build_identifier              : string specifying a build identifier. This option shall only be specified on the command line.
~~~~

### Defining `DSL` options in the MFront file

Options can be passed to a DSL as follows:

~~~~{.cxx}
@DSL Default{
  parameters_as_static_variables : true
};
~~~~

### Defining `DSL` options on the command line:

`MFront` have various command line arguments to define options passed to DSLs:

- `--dsl-option`, which allows to define options passed to domain
  specific languages.
- `--behaviour-dsl-option`, which allows to define options passed to
  domain specific languages related to behaviours.
- `--material-property-dsl-option`, which allows to define options
  passed to domain specific languages related to material properties.
- `--model-dsl-option`, which allows to define options passed to domain
  specific languages related to models.

For example, the `--dsl-option` can be used as follows:

~~~~{.bash}
$ mfront --obuild --interface=cyrano --dsl-option=build_identifier:\"Cyrano-3.2\" Elasticity.mfront
~~~~

This example illustrates that special care must be taken when defining
an option expecting a string value.

### Options common to all DSLs

The following options are available for all DSLs:

- `build_identifier`, which must be associated to a string value.
  However, the `build_identifier` is not meant to be directly specified
  in the `MFront` source file. It shall rather be defined on the command
  line.
- `parameters_as_static_variables`, which states if parameters shall
  be treated as static variables.

### Options common to all DSLs related to behaviours

- `automatically_declare_temperature_as_first_external_state_variable`:
  boolean stating if the temperature shall be automatically declared as
  an external state variable.

## Example

~~~~{.cpp}
@DSL Implicit;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
