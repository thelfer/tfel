% `UTF-8` support in `MFront`
% Thomas Helfer
% 16/07/2019

Some recent scientific programming languages like
[`Julia`](https://julialang.org/) or \(\nabla\) allow the usage of (a
subset of) unicode characters
[@camier_improving_2015;@bezanson_julia:_2017]. This allows a much more
readable code, very close to the mathematical expressions.

![Implementation of the Norton behaviour with and without unicode support](img/mfront-unicode.png ""){width=100%}

This page describes the subset of `UTF-8` characters supported in MFront
files. This subset includes:

- Greek small letters and greek capital letters.
- Mathematical symbols
- Latin letter superscripts and subscripts.
- Number superscripts and subscripts.

# Common symbols

- F₀: deformation gradient at the beginning of the time step
- F₁: deformation gradient at the end of the time step
- ∂ₜF: rate of the deformation gradient
- εᵗᵒ: total strain
- Δεᵗᵒ: total strain increment
- ∂ₜεᵗᵒ: rate of the total strain
- Δεᵗᵒ: total strain increment
- εᵉˡ: elastic strain
- σ: stress tensor
- ℝ₊: ranges of positive numbers
- ℝ₋: ranges of negative numbers

# List of all supported symbols

## Greek letters

+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+
| Α | Β | Γ | Δ | Ε | Ζ | Η | Θ | Ι | Κ | Λ | Μ |
+---+---+---+---+---+---+---+---+---+---+---+---+
| Ν | Ξ | Ο | Π | Ρ | Σ | Τ | Υ | Φ | Χ | Ψ | Ω | 
+---+---+---+---+---+---+---+---+---+---+---+---+

: Capital greek letters

+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+
| α | β | γ | δ | ε | ζ | η | θ | ι | κ | λ | μ | 
+---+---+---+---+---+---+---+---+---+---+---+---+
| ν | ξ | ο | π | ρ | σ | τ | υ | φ | χ | ψ | ω | 
+---+---+---+---+---+---+---+---+---+---+---+---+

: Small greek letters

## Mathematical operators

+:-:+:-:+
| ⋅ | ⊗ |
+---+---+

: Mathematical operators

## Mathematical symbols

+:-:+:-:+:-:+
| ∕ | ∂ | ∇ |
+---+---+---+

: Mathematical symbols

## Superscripts and subscripts

### Superscripts

+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+
| ᴬ | ᴰ | ᴱ | ᴳ | ᴴ | ᴵ | ᴶ | ᴷ | ᴸ |
+---+---+---+---+---+---+---+---+---+
| ᴹ | ᴺ | ᴼ | ᴾ | ᴿ | ᵀ | ᵁ | ᵂ |
+---+---+---+---+---+---+---+---+

: Capital latin letters superscripts

+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+
| ᵃ | ᵇ | ᶜ | ᵈ | ᵉ | ᶠ | ᵍ | ʰ | ⁱ | ʲ | ᵏ | ˡ | ᵐ |
+---+---+---+---+---+---+---+---+---+---+---+---+---+
| ⁿ | ᵒ | ᵖ | ʳ | ˢ | ᵗ | ᵘ | ᵛ | ʷ | ˣ | ʸ | ᶻ |   |
+---+---+---+---+---+---+---+---+---+---+---+---+---+

: Small latin letters superscripts

+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+
| ⁰ | ¹ | ² | ³ | ⁴ | ⁵ | ⁶ | ⁷ | ⁸ | ⁹ |
+---+---+---+---+---+---+---+---+---+---+

: Arabic numbers superscripts

### Subscripts

+:-:+:-:+:-:+:-:+:-:+
| ᵦ | ᵧ | ᵨ | ᵩ | ᵪ |
+---+---+---+---+---+

: Greek letters subscripts

+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+
| ₐ | ₑ | ₒ | ₓ | ₕ | ₖ | ₗ | ₘ | ₚ | ₛ | ₜ |
+---+---+---+---+---+---+---+---+---+---+---+

: Latin letters subscripts

+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+:-:+
| ₀ | ₁ | ₂ | ₃ | ₄ | ₅ | ₆ | ₇ | ₈ | ₉ |
+---+---+---+---+---+---+---+---+---+---+

: Arabic numbers subscripts

+:-:+:-:+
| ₊ | ₋ |
+---+---+

: Mathematic symbols subscripts