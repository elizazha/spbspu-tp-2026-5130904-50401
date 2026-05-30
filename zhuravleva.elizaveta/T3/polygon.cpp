#include "polygon.hpp"
#include <limits>
#include <istream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>
#include <numeric>

namespace
{
  void readPoints(std::istream & in, std::vector< zhuravleva::Point > & points, size_t count)
  {
    if (count == 0)
    {
      return;
    }
    zhuravleva::Point point{};
    if (!(in >> point))
    {
      in.setstate(std::ios::failbit);
      return;
    }
    points.push_back(point);
    readPoints(in, points, count - 1);
  }

  int getDotProduct(const zhuravleva::Point & first, const zhuravleva::Point & second,
      const zhuravleva::Point & third)
  {
    int firstX = first.x - second.x;
    int firstY = first.y - second.y;
    int secondX = third.x - second.x;
    int secondY = third.y - second.y;
    return firstX * secondX + firstY * secondY;
  }

  bool hasRightAngleAtIndex(const zhuravleva::Polygon & polygon, size_t index)
  {
    size_t size = polygon.points.size();
    size_t prev = (index + size - 1) % size;
    size_t next = (index + 1) % size;
    return getDotProduct( polygon.points[prev], polygon.points[index], polygon.points[next]) == 0;
  }

  bool samePointByShift(const zhuravleva::Point & lhs,
      const zhuravleva::Point & rhs, int shiftX, int shiftY)
  {
    return lhs.x + shiftX == rhs.x && lhs.y + shiftY == rhs.y;
  }
}

std::istream & zhuravleva::operator>>(std::istream & in, DelimiterIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '\0';
  in >> c;
  if (in && c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & zhuravleva::operator>>(std::istream & in, Point & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point point{};
  in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' };
  in >> point.y >> DelimiterIO{ ')' };
  if (in)
  {
    dest = point;
  }
  return in;
}

std::istream & zhuravleva::operator>>(std::istream & in, Polygon & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Polygon polygon{};
  size_t count = 0;
  in >> count;
  if (!in || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points.reserve(count);
  readPoints(in, polygon.points, count);
  if (in && polygon.points.size() == count)
  {
    dest = polygon;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

void zhuravleva::readData(std::istream & in, std::vector< Polygon > & polygons)
{
  if (in.eof())
  {
    return;
  }
  Polygon polygon{};
  if (in >> polygon)
  {
    polygons.push_back(polygon);
    readData(in, polygons);
    return;
  }
  if (in.eof())
  {
    return;
  }
  in.clear();
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  readData(in, polygons);
}

bool zhuravleva::operator==(const Point & lhs, const Point & rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

double zhuravleva::getArea(const Polygon & polygon)
{
  double sum = 0.0;
  size_t size = polygon.points.size();
  for (size_t i = 0; i < size; ++i)
  {
    size_t next = (i + 1) % size;
    sum += polygon.points[i].x * polygon.points[next].y;
    sum -= polygon.points[next].x * polygon.points[i].y;
  }
  return std::fabs(sum) / 2.0;
}

bool zhuravleva::hasEvenVertexes(const Polygon & polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool zhuravleva::hasOddVertexes(const Polygon & polygon)
{
  return polygon.points.size() % 2 != 0;
}

bool zhuravleva::hasNVertexes(const Polygon & polygon, size_t count)
{
  return polygon.points.size() == count;
}

bool zhuravleva::areaLess(const Polygon & lhs, const Polygon & rhs)
{
  return getArea(lhs) < getArea(rhs);
}

bool zhuravleva::vertexesLess(const Polygon & lhs, const Polygon & rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool zhuravleva::hasRightAngle(const Polygon & polygon)
{
  std::vector< size_t > indexes(polygon.points.size());
  std::iota(indexes.begin(), indexes.end(), 0);
  return std::any_of(
      indexes.begin(),
      indexes.end(),
      std::bind(hasRightAngleAtIndex, std::cref(polygon), std::placeholders::_1));
}

bool zhuravleva::isSame(const Polygon & lhs, const Polygon & rhs)
{
  if (lhs.points.size() != rhs.points.size())
  {
    return false;
  }
  int shiftX = rhs.points.front().x - lhs.points.front().x;
  int shiftY = rhs.points.front().y - lhs.points.front().y;
  return std::equal(
      lhs.points.begin(),
      lhs.points.end(),
      rhs.points.begin(),
      std::bind(samePointByShift,
          std::placeholders::_1, std::placeholders::_2,
          shiftX, shiftY));
}
