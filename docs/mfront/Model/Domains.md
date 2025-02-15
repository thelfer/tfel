The `@Domains` let the user define the default domains on which a model
shall be applied. This keyword is followed by an array of strings.

This keyword is deprecated as domains shall be explicitely specified
by specialisation.

## Example

~~~~{.cpp}
@Domains {"MATRIX","CLADDING"};
~~~~
