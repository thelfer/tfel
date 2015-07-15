The `@InitializeJacobian` keyword let the user introduce an initial
estimate for the jacobian matrix used by quasi-newton methods, such as
the first Broyden method.

### Example

~~~~{.cpp}
@InitializeJacobian{
  // setting the jacobian to identity (which is the default)
  fill(this->jacobian.begin(),this->jacobian.end(),real(0));
  for(unsigned short i=0;i!=this->jacobian.size();++i){
    this->jacobian(i,i)=real(1);
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
