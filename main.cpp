
// ========================================================================== //

#include <algorithm>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iterator>
#include <iostream>

#include "docopt.h"
#include "instruction.hpp"
#include "interpreter.hpp"
#include "processor.hpp"

// -------------------------------------------------------------------------- //

static char const DOCOPT_STRING[] =
R"(
  Usage:
    ippc [options] <input>

  Options:
    -h, --help              show this help
    -m=FILE, --memory=FILE  initialize memory with the contents of a file
)";

// -------------------------------------------------------------------------- //

int main(
  int const argc,
  char ** const argv
) {
  auto args = docopt::docopt(
    DOCOPT_STRING,
    { (argv + 1), (argv + argc) },
    true, "ippc++ v0.2.0"
  );

  std::ifstream stream;
  gStream = &stream;

  stream.open(args["<input>"].asString());

  if (!stream.is_open()) {
    std::cerr << "failed to open file." << std::endl;
    return 1;
  }

  CInterpreter interpreter;
  gInterpreter = &interpreter;

  CProcessor processor;
  gPPC = &processor;

  while (interpreter.interpret());

  return 0;
}

// -------------------------------------------------------------------------- //

// ========================================================================== //
