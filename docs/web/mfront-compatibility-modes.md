---
title: Compatibility modes in `MFront`
author: Thomas Helfer
date: 10/11/2026
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

`MFront` generates assets which are used in studies that must remain
reproducible for many years. A development which does not fix an
explicit bug but changes the results of the generated code or the
behaviour of `MFront` itself, such as a new default value of an option
or a new default behaviour of a working keyword, is therefore handled
by a compatibility mode: the new behaviour is used by default from a
given version on, whereas users who need the previous one can restore
it by selecting an older compatibility mode. This policy is stated in
the
[coding guidelines](coding-guidelines.html#backward-compatibility).

The two developments which motivated this mechanism are the use of
more accurate and up-to-date values of the physical constants and the
evolution of the `PowellDogLeg` algorithms to a more robust and
classical version. Those evolutions do not correct bugs: they change
the default behaviour of `MFront` and the results of the generated
code for the better.

Compatibility modes are a version number: the mode associated with a
change is the version in which this change becomes the default. Such
evolutions of the default behaviour shall only be introduced on major
versions. The `future` compatibility mode allows testing them, or
anticipating them, before they become the default.

The way the compatibility modes are implemented in the `TFEL`
libraries is described in
[this page](tfel-compatibility-modes.html).

# Available compatibility modes

| Compatibility mode | Description |
|---|---|
| `5.2.0` | Values and behaviours of Version 5.2. First version supporting compatibility modes. |
| `future` | Values and behaviours of the next major version, which is still under development. |

A compatibility mode is a version: a mode will be added for each
version introducing a change of the default behaviour. Note that
`6.0.0` is not a compatibility mode yet: the values of the physical
constants of the next major version are available through the `future`
mode.

## Default compatibility mode

By default, no compatibility mode is declared: the generated code uses
the entities and the algorithms of the version of the `TFEL` libraries
which compiles it, that is to say the current behaviour of that
version. A default mode can be declared globally, for all the files
treated by a run of the tool, using the `--compatibility-mode` command
line argument, as explained below.

Declaring a compatibility mode makes the results of the generated code
independent of the version of the `TFEL` libraries used to compile it:
the generated code uses the entities of the mode it asks for, whatever
this version is, as described
[in the next section](#effect-on-the-generated-code). In Version 5.2,
both approaches give the same results since the current values of the
physical constants are those of the `5.2.0` mode.

# Setting the compatibility mode

## Globally: the `--compatibility-mode` argument

The `--compatibility-mode` argument declares a compatibility mode for
all the files treated by a run of the tool. It is supported by `mfront`,
`mfront-doc` and `mfront-query`, but only `mfront` generates code:

~~~~{.bash}
$ mfront --obuild --interface=generic --compatibility-mode=5.2.0 *.mfront
~~~~

The accepted values are a version, such as `5.2.0` or `5.3.1`, or the
`future` mode. A version which is not itself a known compatibility mode
is associated with the greatest known compatibility mode which does not
exceed it, which is reported by a warning: requesting `5.3.0` gives the
`5.2.0` mode as long as no other mode has been introduced. A version
strictly lower than `5.2.0` is rejected.

## For a given file: the `compatibility_mode` `DSL` option

The compatibility mode of a single file is set by the
`compatibility_mode` option of the domain specific language, which is
available for all `DSL`s. This option has precedence over the default
compatibility mode.

It can be defined in the `MFront` file itself, using the `@DSL`
keyword, as in the following behaviour:

~~~~{.cxx}
// Norton.mfront
@DSL Default{
  compatibility_mode : "5.2.0"
};
~~~~

The name of the domain specific language follows the `@DSL` keyword:
`Default` for a behaviour, `DefaultModel` for a model, `MaterialProperty`
for a material property, and so on.

It can also be defined on the command line, using the `--dsl-option`
argument, or one of its variants `--behaviour-dsl-option`,
`--material-property-dsl-option` and `--model-dsl-option`:

~~~~{.bash}
$ mfront --obuild --interface=generic --dsl-option=compatibility_mode:\"5.2.0\" Norton.mfront
~~~~

As for any option expecting a string value, special care must be taken
with the quotes.

Finally, it can be defined in the configuration file passed with the
`--configuration-file` argument, in the `dsl_options` section or in one
of the sections dedicated to a kind of material knowledge:

~~~~{.json}
dsl_options : {
  compatibility_mode : "5.2.0"
}
~~~~

As any `DSL` option, this option is listed by the `--list-dsl-options`
argument:

~~~~{.bash}
$ mfront --list-dsl-options=DefaultModel
- disable_runtime_checks        : boolean stating if interfaces shall remove as much runtime checks as possible. ...
...
- compatibility_mode            : string given the version to which the MFront file shall comply.
...
~~~~

# Effect on the generated code

When a compatibility mode is declared, the generated code only refers to
the entities of this mode: those are duplicated, one copy per mode, in
the `TFEL` libraries, as described in
[this page](tfel-compatibility-modes.html). The results it gives do not
depend on the version of the `TFEL` libraries used to compile it, except
for bug fixes. When no compatibility mode is declared, the generated
code refers to the entities of the version of the `TFEL` libraries which
compiles it, and follows therefore the current behaviour of that
version.

For instance, the alias giving access to the physical constants in the
generated code is:

~~~~{.cxx}
// no compatibility mode declared
using PhysicalConstants [[maybe_unused]] =
    tfel::PhysicalConstants<NumericType, false>;
~~~~

~~~~{.cxx}
// compatibility mode 5.2.0
using PhysicalConstants [[maybe_unused]] =
    tfel::v5_2_0::PhysicalConstants<NumericType, false>;
~~~~

~~~~{.cxx}
// future compatibility mode
using PhysicalConstants [[maybe_unused]] =
    tfel::future::PhysicalConstants<NumericType, false>;
~~~~

For an asset which uses quantities (`@UseQt`), the second template
argument of the alias is `use_qt` instead of `false`.

The physical constants are, at the time of writing, the only entities
whose behaviour is subject to a compatibility mode. Any other entity,
such as a brick or a non linear solver, shall be treated in the same
way: the code generated for an asset which declares a compatibility mode
uses the implementation designated by this mode.

# Knowing the compatibility mode of an asset

When a compatibility mode is declared for an asset, it is recorded in
the generated code as an exported string symbol, whose name is the name
of the entry point suffixed by `_compatibility_mode`:

~~~~{.cxx}
MFRONT_EXPORT_SYMBOL(const char*, Norton_compatibility_mode, "5.2.0");
~~~~

This symbol is generated whatever the way the mode has been declared, by
the `compatibility_mode` option of the `DSL` of the file or by the
`--compatibility-mode` argument: it is not generated when no
compatibility mode is declared. The `ExternalLibraryManager` class of the
`TFEL/System` library returns it:

~~~~{.cxx}
auto& elms =
    tfel::system::ExternalLibraryManager::getExternalLibraryManager();
std::cout << elms.getCompatibilityMode("libBehaviour.so", "Norton")
          << '\n';
~~~~

~~~~{.python}
>>> import tfel.system
>>> m = tfel.system.ExternalLibraryManager.getExternalLibraryManager()
>>> m.getCompatibilityMode("libBehaviour.so", "Norton")
'5.2.0'
~~~~

An empty string is returned when no compatibility mode is declared for
the asset, which means that it follows the current behaviour of the
version of the `TFEL` libraries which compiles it.

The `compatibility_mode` attribute of an `MFront` file is reported by the
`--attributes` query of `mfront-query`, which lists the names of the
attributes of a material knowledge, when this attribute is defined by the
file itself:

~~~~{.bash}
$ mfront-query --attributes Norton.mfront
automatic_declaration_of_the_temperature_as_first_external_state_variable
compatibility_mode use_stress_update_algorithm ...
~~~~

# The `future` compatibility mode

The `future` compatibility mode introduces changes which shall be the
default of the next major version. It allows users to test them in
advance and to prepare their assets, whereas the values and the
behaviours of the current version are left unchanged.

An asset generated with the `future` mode is thus expected to behave in
the same way as the same asset generated with the version which will
make those changes the default. This is the reason why the code
generated in the `future` mode refers to the constants of the next major
version, `tfel::future::PhysicalConstants`, and not to those of the
current version.

The `future` mode is by nature unstable: new changes can be added to it
until the corresponding major version is released.

# Adding a new compatibility mode

A development which changes the default behaviour of `MFront` or the
results of the code it generates, without fixing an explicit bug, shall
be gated by the compatibility mode of the file it compiles. Such a
development proceeds as follows:

1. Determine the version which shall make the new behaviour the
   default. If this is not the next major version, the change is added
   to the `future` mode only.
2. Declare the new mode in `MFront`, in
   `mfront/include/MFront/CompatibilityMode.hxx`, by adding a new
   version and inserting it in the `known_compatibility_modes` array:

   ~~~~{.cxx}
   static inline constexpr auto v6_0_0 =
       Version{.major = 6, .minor = 0, .patch = 0};
   ~~~~

   The name of the version in this array is the name of the `C++`
   namespace used by the versioned entities of the `TFEL` libraries, as
   described in the
   [corresponding section](tfel-compatibility-modes.html#adding-a-new-compatibility-mode).

3. Use the compatibility mode in the code generator, using the
   `getCompatibilityMode` function, which returns the mode of the
   material knowledge description, or an empty value if the file does not
   declare any. Since an asset which declares no mode must follow the
   current behaviour, the new behaviour is used when no mode is declared,
   or when the declared mode is greater than, or equal to, the version
   which introduces the change:

   ~~~~{.cxx}
   const auto ov = getCompatibilityMode(d);
   if ((not ov) || (*ov >= CompatibilityMode::v6_0_0)) {
     // new behaviour
   }
   ~~~~

   Comparing the value returned by `getCompatibilityMode` directly to a
   version, without checking that it is set, shall be avoided: an empty
   value compares as lower than any version, so the assets which declare
   no mode would keep the previous behaviour.

   Code which is not gated this way is common to all the compatibility
   modes.
4. Test both behaviours: a test behaviour, or model, or material
   property, generated with each mode, and an `MTest` file checking the
   results it gives, as the `PhysicalConstants_v5_2_0` and
   `PhysicalConstants_v6_0_0` test models do, the latter being generated
   with the `future` mode.
5. If the change affects the `TFEL` libraries, follow the checklist of
   the
   [corresponding section](tfel-compatibility-modes.html#adding-a-new-compatibility-mode).
6. Describe the change in the release notes of the version which
   introduces it, in the section describing the compatibility modes.

The unit test `CompatibilityModeTest` checks the behaviour of the
`CompatibilityMode` structure: a new mode shall be added to this test.

# See also

- [Compatibility modes in the `TFEL` libraries](tfel-compatibility-modes.html)
- [Coding guidelines](coding-guidelines.html#backward-compatibility)
- [Options common to all `DSL`s](Default-keywords.html#options-common-to-all-dsls)
