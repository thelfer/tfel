The `@Swelling` keyword allow the user to specify that an additional
stress free expansion must be taken into account.

This keyword has the following options:

- `Volume`, stating that the swelling is given as a volume change
  (\(\displaystyle\frac{\Delta\,V}{V_{0}}\))
- `Linear` (the default), stating that the swelling is given as a
  linear change (\(\displaystyle\frac{\Delta\,l}{l_{0}}\))

The `@Swelling` keyword is then followed by one or three definition of
stress free expansion.

If one stress free expansion is defined, the swelling is computed as
an isotropic second order tensor.

If three stress free expansions are defined, the swelling is computed
as a diagonal second order tensor:
\[
s =
\left(
\begin{array}{ccc}
	\left.\frac{\Delta\,l}{l_{0}}\right|_{0} & 0 & 0 \\
	0 & \left.\frac{\Delta\,l}{l_{0}}\right|_{1} & 0 \\
	0 & 0 & \left.\frac{\Delta\,l}{l_{0}}\right|_{2} \\
\end{array}
\right)
\]

A stress free expansion is either defined by:

- A string pointing to a mfront model.
- `0`, which allow the definition of a null swelling.
- The name of an external state variable.

## Notes

A null swelling is not allowed when only one stress free expansion

The definition of three stress free expansion is only valid for
orthotropic behaviours and is not compatible with the `Volume` option.

## Examples

~~~~ {#Swelling .cpp}
@ExternalStateVariable strain s;
s.setGlossaryName("SolidSwelling");
@Swelling<Volume> s;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
