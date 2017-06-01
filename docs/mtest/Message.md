The `@Message` keyword is used to display some informative message on
the standard output.

This keyword is followed by floatting point values and/or
strings.

Strings are first interpreted as formulae. If the interpretation is
successfull, the result is printed. Otherwise, the string is display
witout interpretation.

All the following tokens are appended to the message up to a
final semi-colon.

## Example:

~~~~{.cpp}
@Message "12*5";
~~~~~~~~
