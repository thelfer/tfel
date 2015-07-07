The `@PerturbationValueForNumericalJacobianComputation` keyword let the
user defines the pertubation value used to compute the numerical
jacobian.

If \(Y\) is the vector of integration variables and \(F\) the function
defining the implicit system, the \(j^{\text{th}}\) column of the
numerical jacobian \(J^{n}\) is defined by the centered finite difference
formulae:
\[
J^{n}(i,j)=\frac{F(Y_{i}^{+\epsilon})-F(Y_{i}^{-\epsilon})}{2\,\epsilon}
\]
where:
\[
Y_{i}^{+\epsilon}(j)=
\left\{
  \begin{aligned}
    Y(j)          & \text{ si } &j \neq i \\
    Y(i)+\epsilon & \text{ si } &j =    i
  \end{aligned}
\right.
\]

### Example

~~~~{.cpp}
@PerturbationValueForNumericalJacobianComputation 1.e-7;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
