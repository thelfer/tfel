#include<iostream>
#include<cstdlib>

#include"Utilities/TerminalColors.hxx"

void print(const char c, const char * const couleur, size_t size)
{
  using namespace std;
  cout.write(couleur,size);
  cout << c;		  						  
}

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;

  print('s',TerminalColors::Red,sizeof(TerminalColors::Red));
  print('u',TerminalColors::Green,sizeof(TerminalColors::Green));
  print('c',TerminalColors::Yellow,sizeof(TerminalColors::Yellow));
  print('c',TerminalColors::Blue,sizeof(TerminalColors::Blue));
  print('e',TerminalColors::Purple,sizeof(TerminalColors::Purple));
  print('s',TerminalColors::LightBlue,sizeof(TerminalColors::LightBlue));
  print('s',TerminalColors::White,sizeof(TerminalColors::White));

  cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
  cout << endl;

  return EXIT_SUCCESS;
}
