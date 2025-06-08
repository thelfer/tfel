The `@HillTensor` keyword lets the user define a new second order tensor
(`st2tost2`) suitable for computing the Hill stress.

The Hill's tensor per se is defined by 6 material properties named
\(F\), \(G\), \(H\), \(L\), \(M\), \(N\), as follows:

In \(3D\), The Hill tensor is defined by:
\[
\left(
\begin{array}{cccccc}
F+H & -F  & -H  & 0 & 0 & 0 \\
-F  & G+F & -G  & 0 & 0 & 0 \\
-H  & -G  & H+G & 0 & 0 & 0 \\
0   & 0   & 0   & L & 0 & 0 \\
0   & 0   & 0   & 0 & M & 0 \\
0   & 0   & 0   & 0 & 0 & N \\
\end{array}
\right)
\]

The Hill stress is defined by:
\[
\sqrt{\tsigma\,\colon\,\tenseurq{H}\,\colon\,\tsigma}
\sqrt{ F\,\paren{\sigma_{11}-\sigma_{22}}^2+
       G\,\paren{\sigma_{22}-\sigma_{33}}^2+
       H\,\paren{\sigma_{33}-\sigma_{11}}^2+
	  2\,L\sigma_{12}^{2}+
	  2\,M\sigma_{13}^{2}+
	  2\,N\sigma_{23}^{2}}
\]

> **Warning** This convention is given in the book of Lemaître et
> Chaboche and seems to differ from the one described in most other
> books.

The `HillTensor` keyword is followed by:

- the name of the Hill's tensor,
- an array of 6 material properties or a dictionary.

Those material properties can be defined by:

- a constant value.
- an external `MFront` file.
- an analytic value.

## Orthotropic axis convention

The computation of the Hill tensor rely on the definition of an
orthotropic convention. For example, the `Pipe` orthotropic convention
will lead to automatically exchange the second and first axes for the
plane strain, plane stress and generalised plane strain hypotheses. See
the `OrthotropicBehaviour` keyword for details.

## Example

~~~~{#HillTensor .cpp}
@HillTensor H {0.371,0.629,4.052,1.5,1.5,1.5};
~~~~

~~~~{#HillTensor .cpp}
@HillTensor H {F: 0.371, G: 0.629, H: 4.052, L: 1.5, M: 1.5, N: 1.5};
~~~~
