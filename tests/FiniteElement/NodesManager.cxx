/*!
 * \file   NodesManager.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 mai 2007
 */

#include<iostream>
#include<cstdlib>

#include"FiniteElement/NodesManager.hxx"
#include"FiniteElement/Node.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::fe;
  using namespace tfel::meta;
  typedef Node<1u> Node;

  NodesManager<Node> n;
  n.push_back(Node(2.));
  
  cout << n.get<Node>(0u).getPosition() << endl;

  return EXIT_SUCCESS;
}
