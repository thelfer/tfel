---
title: The DDIF2 brick
author: Bruno Michel, Thomas Helfer
date: 12/2017
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This page describes the `DDIF2` brick.

The `DDIF2` behaviour is used to describe the brittle nature of nuclear
fuel ceramics (see [@michel_3d_2008;@michel_3d_2016;@michel_new_2017])
and is usually coupled with a description of the viscoplasticity of
those ceramics (See for example
[@monerie_overall_2006,@salvo_experimental_2015;@salvo_experimental_2015-1]).

The `DDIF2` brick can be used in place of the `StandardElasticity`
brick. Internally, the `DDIF2` brick is derived from the
`StandardElasticity` brick, so the definition of the elastic
properties follows the same rules.

This brick is mostly a front-end to the `DDIF2` stress potential. The
reader may thus refer to [this page](DDIF2StressPotential.html) for a
complete description.

# References