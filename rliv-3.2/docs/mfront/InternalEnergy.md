The `@InternalEnergy` keyword allows the computation of the internal
energy density per unit of volume in the reference configuration.

The code provided must update a variable called `Psi_s`.

This update is called at the end of the behaviour integration, if
convergence was reached, after that:

- Internal state variables have been updated.
- The stress at the end of the time step have been computed.

## Example

~~~~ {#InternalEnergy .cpp}
@InternalEnergy{
  // updating the previous value
  // of the stored energy
  Psi_s += (sig|deel)/2;
}
~~~~

~~~~ {#InternalEnergy2 .cpp}
@InternalEnergy{
	Psi_s = (sig|eel)/2;
}
~~~~

~~~~ {#InternalEnergy2 .cpp}
@InternalEnergy{
	const strain tr = trace(eel);
	Psi_s = lambda*tr*tr/2+mu*(eel|eel);
}
~~~~
