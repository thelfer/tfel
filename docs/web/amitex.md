---
title: MFront interface for AMITEX_FFTP
author: Thomas Helfer
date: March 1, 2021
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
bibliography: bibliography.bib
---

<div id="slideshow">
  <ul class="slides">
	<li><img src="img/AMITEX_FFTP_sig1.png" width="620" height="320" alt="Polycrystals computation based on FFT" /></li>
  </ul>
  <span class="arrow previous"></span>
  <span class="arrow next"></span>
</div>
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js"></script>
<script src="js/slideshow.js"></script>

The `amitex` interface is mostly an alias for the `castem` interface,
i.e. the interface for the `Cast3M` interface prior to Version `21` and
also generates an input file template which is meant to be copy-pasted
into the `material.xml` file of the user. See [this page](castem.html)
for a description of the `castem` interface

# Generated input file template

Here is an example of such a template:

~~~~{.xml}
<!--
    Simple template for use in AMITEX_FFTP.
    
    You may copy-paste those lines in your material.xml file
    and adjust:

- the material number I,
- the path to the library (currently UmatBehaviour),
- the coefficients and initial internal variables values
which are currently replaced by three dots.
-->

<Material numM="I" Lib="UmatBehaviour" Law="umatplasticity">
  <!-- material property YoungModulus -->
  <Coeff Index = "1" Type = "Constant" Value = "..." />
  <!-- material property PoissonRatio -->
  <Coeff Index = "2" Type = "Constant" Value = "..." />
  <!-- material property MassDensity -->
  <Coeff Index = "3" Type = "Constant" Value = "..." />
  <!-- material property ThermalExpansion -->
  <Coeff Index = "4" Type = "Constant" Value = "..." />
  <!-- material property H -->
  <Coeff Index = "5" Type = "Constant" Value = "..." />
  <!-- material property s0 -->
  <Coeff Index = "6" Type = "Constant" Value = "..." />
  <!-- internal state variable ElasticStrain -->
  <IntVar Index = "1" Type = "Constant" Value = "..." />
  <IntVar Index = "2" Type = "Constant" Value = "..." />
  <IntVar Index = "3" Type = "Constant" Value = "..." />
  <IntVar Index = "4" Type = "Constant" Value = "..." />
  <IntVar Index = "5" Type = "Constant" Value = "..." />
  <IntVar Index = "6" Type = "Constant" Value = "..." />
  <!-- internal state variable EquivalentPlasticStrain -->
  <IntVar Index = "7" Type = "Constant" Value = "..." />
</Material>
~~~~
