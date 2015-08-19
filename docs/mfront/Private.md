The `@Private` keyword let the user define private methods or members
of in the generated behaviour class.

## Example

~~~~{.cpp}
@Private{
  void display(void) const{
    std::cout << "eto " << this->eto << std::endl;
  }
} // end of @Private
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
