The `@Profiling` keyword is followed by a boolean. If true, several
high resolutions clocks will be introduced in the generated code to
profile performance bottlenecks. The total time spend in various
portions of the generated code will be stored and displayed when the
calling process exits.

## Example

~~~~{.cpp}
@Profiling true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
