The `@RejectCurrentCorrection` keyword introduces a code block called
when the current correction is rejected.

This method can be used to reject the decondensation step when some
integration variables were eliminated by static condensation.

This increment is stored in an array called `delta_zeros`. The
`delta_zeros` variable is not meant to be used directly by the users and
views to the corrections of the increments of the integration variables
are automatically declared in this code block.
