/*!
 * \file   Mesh.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   14 Sep 2006
 */

#include<iostream>
#include<cstdlib>

#include"FiniteElement/Mesh.hxx"
#include"FiniteElement/Node.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::fe;
  
  typedef Node<1u> Node;
  typedef Mesh<Node,double> Mesh;

  Mesh mesh;  
  mesh.push_back<Node>(Node(12.));
  mesh.push_back<double>(12.);

  return EXIT_SUCCESS;
}
