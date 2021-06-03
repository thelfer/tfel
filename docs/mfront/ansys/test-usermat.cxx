#include "usermat.cpp"

int main() {
  ansys::UserMaterialManager m;
  return m.getBehaviour(2) != nullptr;
}
