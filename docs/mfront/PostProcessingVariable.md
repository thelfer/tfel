Post-processing variables are introduced by the
`@PostProcessingVariable` keyword.

Post-processing variables are only defined in post-processings, and
can't be used in the other code blocks.

Contrary most variables (internal state variables, external state
variables, etc.), post-processing variables can be defined after the
first code block. However, care must be taken to declare post-processing
variables **before** their use in a post-processing.

Note that a post-processing variable can be used in differents
post-processings. Typically, one may compute the principal strains in a
dedicated post-processing and in a post-processing computing the
principal strains and the strain eigen vectors.

## Exemple of usage

~~~~{.cxx}
//! principal strains
@PostProcessingVariable tvector<3u,strain> εᵖ;
~~~~