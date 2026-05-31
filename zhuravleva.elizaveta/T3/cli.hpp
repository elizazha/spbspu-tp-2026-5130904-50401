#ifndef CLI_HPP
#define CLI_HPP
#include <iosfwd>
#include <vector>
#include "polygon.hpp"

namespace zhuravleva
{
  struct Command
  {
    static const std::vector< Polygon > * polygons;
  };

  std::istream & operator>>(std::istream & in, Command & command);
}

#endif
