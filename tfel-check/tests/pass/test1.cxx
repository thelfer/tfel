#include<cstdlib>
#include<cstring>

int main(void) {
  const char * my_env = ::getenv("TFEL_CHECK_TEST1_ENV");
  if (my_env == 0) {
    return EXIT_FAILURE;
  }
  if (::strcmp(my_env, "./libs2:./libs") != 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
