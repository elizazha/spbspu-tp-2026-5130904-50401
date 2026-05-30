#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <vector>

#include "polygon.hpp"

namespace zhuravleva
{
  void area(std::istream & in, std::ostream & out,
      const std::vector< Polygon > & polygons);

  void max(std::istream & in, std::ostream & out,
      const std::vector< Polygon > & polygons);

  void min(std::istream & in, std::ostream & out,
      const std::vector< Polygon > & polygons);

  void count(std::istream & in, std::ostream & out,
      const std::vector< Polygon > & polygons);

  void same(std::istream & in, std::ostream & out,
      const std::vector< Polygon > & polygons);

  void rightShapes(std::istream & in, std::ostream & out,
      const std::vector< Polygon > & polygons);
}

#endif