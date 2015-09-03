The `@OutOfBoundsPolicy` let the user to precise the out of bounds
policy. Three policies are available:

- `None`. If this policy is selected, nothing is done when a variable
  is out of bounds.
- `Warning`. If this policy is selected, a message is selected when a
  variable is out of bounds.
- `Strict`. If this policy is selected, the computation fails when a
  variable is out of bounds. Most mechanical behaviours
  implementations throw an exception which is caught by `MTest`.

## Example

~~~~ {.cpp}
@OutOfBoundsPolicy 'Strict';
~~~~~~~~
