#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <cstddef>
#include <iosfwd>
#include <vector>

namespace zhuravleva
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  struct DelimiterIO
  {
    char exp;
  };

  std::istream & operator>>(std::istream & in, DelimiterIO && dest);
  std::istream & operator>>(std::istream & in, Point & dest);
  std::istream & operator>>(std::istream & in, Polygon & dest);

  bool operator==(const Point & lhs, const Point & rhs);

  void readData(std::istream & in, std::vector< Polygon > & polygons);

  double getArea(const Polygon & polygon);

  bool hasEvenVertexes(const Polygon & polygon);
  bool hasOddVertexes(const Polygon & polygon);
  bool hasNVertexes(const Polygon & polygon, size_t count);

  bool areaLess(const Polygon & lhs, const Polygon & rhs);
  bool vertexesLess(const Polygon & lhs, const Polygon & rhs);

  bool hasRightAngle(const Polygon & polygon);
  bool isSame(const Polygon & lhs, const Polygon & rhs);
}

#endif
