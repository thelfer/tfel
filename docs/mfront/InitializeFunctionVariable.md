Initialize function variables are introduced by the
`@InitializeFunctionVariable` keyword.

Initialize function variables are only defined in initialize functions,
and can't be used in the other code blocks.

Contrary most variables (internal state variables, external state
variables, etc.), initialize function variables can be defined after the
first code block. However, care must be taken to declare initialize
function variables **before** their use in an initialize function.

Note that an initialize function variable can be used in differents
initialize function.
