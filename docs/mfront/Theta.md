The `Theta` keyword is used to define the default value of \(\theta\)
parameter used by implicit schemes. If the `Theta` keyword is not
used, implicit domain specific languages provide their own specific
default value (either \(0.5\) or \(1\)).

The value given to \(\theta\) must be in the range \(]0:1]).

The value of \(\theta\) can be changed at runtime by modifying the
`theta` parameter (see the `@Parameter` keyword).

## Example

~~~~{.cpp}
@Theta 0.5;
~~~~
