The `@DissipatedEnergy` keyword allows the computation of the internal
energy per unit of volume in the reference configuration.

The code provided must update a variable called `Psi_d`.

This update is called at the end of the behaviour integration, if
convergence was reached, after that:

- Internal state variables have been updated.
- The stress at the end of the time step have been computed.

## Example

~~~~ {#DissipatedEnergy .cpp}
@DissipatedEnergy{
	Psi_d += sig|(deto-deel)
	// If would have been better to store the initial value of the
    // stress in a local variable sig0 and then used 

}
~~~~

~~~~ {#DissipatedEnergy2 .cpp}
@DissipatedEnergy{
	// Here, we have stored the initial value of the
    // stress in a local variable sig0 
	Psi_d += (sig0+sig)|(deto-deel)/2
}
~~~~
