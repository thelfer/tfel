The `@XMLOutputFile` keyword specifies the name of the file in which
the results of the tests are written in the `XML` format. This file is
only written if the `--xml-output` command line option is given. The
`--xml-output-file` command line option overrides this keyword. By
default, the name of this file is the name of the input file, minus
its extension, plus `.xml`.

## Example

~~~~ {.cpp}
@XMLOutputFile 'results.xml';
~~~~~~~~
