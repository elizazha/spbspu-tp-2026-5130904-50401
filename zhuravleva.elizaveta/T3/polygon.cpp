#include "polygon.hpp"
#include <limits>
#include <istream>

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