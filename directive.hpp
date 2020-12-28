
// ========================================================================== //

#ifndef INCLUDE_DIRECTIVE_HPP
#define INCLUDE_DIRECTIVE_HPP

// -------------------------------------------------------------------------- //

#include <functional>
#include <string_view>

// -------------------------------------------------------------------------- //

struct CDirective {

  using FCallback = std::function<bool ()>;

  std::string_view const key;
  FCallback const callback;

  CDirective(
    std::string_view key,
    FCallback callback
  );

  static CDirective const * Fetch(std::string_view key);

  private:

  CDirective * next;

  static CDirective * sFirst;

};

// -------------------------------------------------------------------------- //

// ========================================================================== //

#endif
