#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <string>
#include <cctype>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <istream>
#include <ostream>
#include <limits>

namespace
{
  bool isNumber(const std::string & str)
  {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
  }

  void checkLineEnd(std::istream& in)
  {
    int c = in.peek();
    if (!in)
    {
      return;
    }
    if (c == ' ' || c == '\t')
    {
      in.get();
      checkLineEnd(in);
      return;
    }
    if (c == '\n' && c != std::char_traits< char >::eof())
    {
      throw std::invalid_argument("invalid command");
    }
  }
}

void zhuravleva::count(std::istream & in, std::ostream & out,
    const std::vector< Polygon > & polygons)
{
  std::string arg;
  if (!(in >> arg))
  {
    throw std::invalid_argument("invalid command");
  }
  size_t result = 0;
  if (arg == "EVEN")
  {
    result = std::count_if(polygons.begin(), polygons.end(), hasEvenVertexes);
  }
  else if (arg == "ODD")
  {
    result = std::count_if(polygons.begin(), polygons.end(), hasOddVertexes);
  }
  else if (isNumber(arg))
  {
    size_t vertexCount = std::stoull(arg);
    if (vertexCount < 3)
    {
      throw std::invalid_argument("invalid command");
    }
    result = std::count_if(
        polygons.begin(), polygons.end(),
        std::bind(hasNVertexes, std::placeholders::_1, vertexCount));
  }
  else
  {
    throw std::invalid_argument("invalid command");
  }
  out << result << '\n';
}

void zhuravleva::area(std::istream & in,
    std::ostream & out,
    const std::vector< Polygon > & polygons)
{
  std::string arg;
  if (!(in >> arg))
  {
    throw std::invalid_argument("invalid command");
  }
  std::vector< Polygon > filteredPolygons;
  if (arg == "EVEN")
  {
    std::copy_if(polygons.begin(), polygons.end(),
        std::back_inserter(filteredPolygons), hasEvenVertexes);
  }
  else if (arg == "ODD")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), hasOddVertexes);
  }
  else if (arg == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid command");
    }
    filteredPolygons = polygons;
  }
  else if (isNumber(arg))
  {
    size_t vertexCount = std::stoull(arg);
    if (vertexCount < 3)
    {
      throw std::invalid_argument("invalid command");
    }
    std::copy_if( polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons),
        std::bind(hasNVertexes, std::placeholders::_1, vertexCount));
  }
  else
  {
    throw std::invalid_argument("invalid command");
  }
  std::vector< double > areas(filteredPolygons.size());
  std::transform(filteredPolygons.begin(), filteredPolygons.end(), areas.begin(), getArea);
  double result = std::accumulate(areas.begin(), areas.end(), 0.0);
  if (arg == "MEAN")
  {
    result /= polygons.size();
  }
  out << std::fixed << std::setprecision(1) << result << '\n';
}

void zhuravleva::max(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("invalid command");
  }
  std::string arg;
  if (!(in >> arg))
  {
    throw std::invalid_argument("invalid command");
  }
  if (arg == "AREA")
  {
    std::vector< Polygon >::const_iterator result = std::max_element(polygons.begin(),
        polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(*result) << '\n';
  }
  else if (arg == "VERTEXES")
  {
    std::vector< Polygon >::const_iterator result = std::max_element(polygons.begin(),
        polygons.end(), vertexesLess);
    out << result->points.size() << '\n';
  }
  else
  {
    throw std::invalid_argument("invalid command");
  }
}

void zhuravleva::min(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("invalid command");
  }

  std::string arg;
  if (!(in >> arg))
  {
    throw std::invalid_argument("invalid command");
  }
  if (arg == "AREA")
  {
    std::vector< Polygon >::const_iterator result = std::min_element(
        polygons.begin(),
        polygons.end(),
        areaLess
    );
    out << std::fixed << std::setprecision(1) << getArea(*result) << '\n';
  }
  else if (arg == "VERTEXES")
  {
    std::vector< Polygon >::const_iterator result = std::min_element(
        polygons.begin(),
        polygons.end(),
        vertexesLess
    );
    out << result->points.size() << '\n';
  }
  else
  {
    throw std::invalid_argument("invalid command");
  }
}

void zhuravleva::same(std::istream & in, std::ostream & out,
    const std::vector< Polygon > & polygons)
{
  Polygon ref{};
  if (!(in >> ref))
  {
    throw std::invalid_argument("invalid command");
  }
  checkLineEnd(in);
  size_t result = std::count_if(
      polygons.begin(),
      polygons.end(),
      std::bind(isSame, std::placeholders::_1, std::cref(ref))
  );
  out << result << '\n';
}

void zhuravleva::rightShapes(std::istream &, std::ostream & out,
    const std::vector< Polygon > & polygons)
{
  size_t result = std::count_if(polygons.begin(), polygons.end(), hasRightAngle);
  out << result << '\n';
}
