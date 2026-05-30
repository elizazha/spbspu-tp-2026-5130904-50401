#include "cli.hpp"
#include "commands.hpp"
#include <iostream>
#include <functional>
#include <string>
#include <limits>
#include <stdexcept>
#include <unordered_map>

const std::vector< zhuravleva::Polygon >* zhuravleva::Command::polygons = nullptr;
std::istream& zhuravleva::operator>>(std::istream& in, Command&)
{
  std::string name;
  in >> name;
  if (!in)
  {
    return in;
  }
  using command_t = std::function< void(std::istream&, std::ostream&, const std::vector< Polygon >&) >;
  static std::unordered_map< std::string, command_t > commands;
  commands["AREA"] = area;
  commands["COUNT"] = count;
  commands["MAX"] = max;
  commands["MIN"] = min;
  commands["SAME"] = same;
  commands["RIGHTSHAPES"] = rightShapes;

  try
  {
    commands.at(name)(in, std::cout, *Command::polygons);
  }catch(...)
  {
    std::cout << "<INVALID COMMAND>" << '\n';
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return in;
}
