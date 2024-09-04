The `@Sources` keyword let the user define a code block that will be
integrated in the generated sources of a behaviour. This allows the
user to implement their own classes or functions. This declarations of
such classes or functions can be made in a code block introduced by
the `@Includes` keyword.

~~~~{.cpp}
@Includes{
  //! declaration of the function f
  void f(void);
}

@Sources{
  // source of the function
  void f(void){
    std::cout << "Example of a function " << std::endl;
  } // end of f
}
~~~~

