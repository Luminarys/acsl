//
// Utility functions and structs.
//

#ifndef ACSL_UTILITY_HH
#define ACSL_UTILITY_HH

namespace acsl {
inline void panic(std::string s) {
  printf("PANIC: %s\n", s);
  exit(1);
}
}

#endif //ACSL_UTILITY_HH
