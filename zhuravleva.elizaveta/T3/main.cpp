#include "cli.hpp"
#include "polygon.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "wrong arguments" << '\n';
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "cannot open file" << '\n';
    return 1;
  }
  std::vector< zhuravleva::Polygon > polygons;
  zhuravleva::readData(input, polygons);
  zhuravleva::Command::polygons = &polygons;
  std::vector< zhuravleva::Command > commands;
  std::copy(std::istream_iterator< zhuravleva::Command >{ std::cin },
      std::istream_iterator< zhuravleva::Command >{}, std::back_inserter(commands));
  return 0;
}
