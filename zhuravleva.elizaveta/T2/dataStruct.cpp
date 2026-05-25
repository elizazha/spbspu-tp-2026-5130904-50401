#include "dataStruct.hpp"
#include <istream>
#include <ostream>
#include <algorithm>
#include <string>

zhuravleva::IOGuard::IOGuard(std::basic_ios< char >& stream):
  stream_(stream),
  width_(stream.width()),
  precision_(stream.precision()),
  flags_(stream.flags()),
  fill_(stream.fill())
{}

zhuravleva::IOGuard::~IOGuard()
{
  stream_.width(width_);
  stream_.precision(precision_);
  stream_.flags(flags_);
  stream_.fill(fill_);
}

namespace
{
  bool isDigit(char symbol)
  {
    return symbol >= '0' && symbol <= '9';
  }

  bool isDoubleLiteral(const std::string& value)
  {
    const size_t dot = value.find('.');
    if (dot == std::string::npos) {
      return false;
    }
    if (dot == 0 || dot + 1 == value.size()) {
      return false;
    }
    return std::all_of(value.begin(), value.end(), [](char symbol)
    {
      return isDigit(symbol) || symbol == '.' || symbol == '-';
    });
  }

  bool isSllLiteral(const std::string& value)
  {
    if (value.empty()) {
      return false;
    }

    const size_t start = value[0] == '-' ? 1 : 0;
    if (start == value.size()) {
      return false;
    }

    return std::all_of(value.begin() + start, value.end(), isDigit);
  }
}

std::istream& zhuravleva::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  char symbol = 0;
  in >> symbol;
  if (std::find(dest.expected.begin(), dest.expected.end(), symbol) == dest.expected.end()) {
    in.setstate(std::ios::failbit);
  }

  return in;
}
