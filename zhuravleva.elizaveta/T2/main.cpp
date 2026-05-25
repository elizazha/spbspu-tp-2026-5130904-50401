#include "dataStruct.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

int main()
{
  std::vector< zhuravleva::DataStruct > data;
  using output_iterator_t = std::ostream_iterator< zhuravleva::DataStruct >;
  using input_iterator_t = std::istream_iterator< zhuravleva::DataStruct >;
  while (!std::cin.eof()) {
    std::copy(input_iterator_t{std::cin}, input_iterator_t{}, std::back_inserter(data));
    if (std::cin.fail()) {
      std::cin.clear();
      const std::streamsize maxSize = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(maxSize, '\n');
    }
  }
  std::sort(data.begin(), data.end(), std::less< zhuravleva::DataStruct >{});
  std::copy(data.begin(), data.end(), output_iterator_t{std::cout, "\n"});
  return 0;
}
