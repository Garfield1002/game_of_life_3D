#include <stdio.h>

#include "settings.h"
#include "window.h"

int main() {
  std::cout << 'v' << Settings::VERSION;

  Window window{};

  window.run();

  exit(EXIT_SUCCESS);
}
