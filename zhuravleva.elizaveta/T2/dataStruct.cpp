#include "dataStruct.hpp"
#include <istream>
#include <ostream>
#include <algorithm>
#include <string>
#include <iomanip>

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

std::istream& zhuravleva::operator>>(std::istream& in, DoubleLitIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string value;
  char symbol = 0;
  while (in.get(symbol)) {
    if (symbol == 'd' || symbol == 'D') {
      break;
    }
    if (symbol == ':') {
      in.setstate(std::ios::failbit);
      return in;
    }
    value += symbol;
  }
  if (!in || (symbol != 'd' && symbol != 'D') || !isDoubleLiteral(value)) {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.ref = std::stod(value);
  return in;
}

std::istream& zhuravleva::operator>>(std::istream& in, SllLitIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string value;
  char symbol = 0;
  while (in.get(symbol)) {
    if (symbol == 'l' || symbol == 'L') {
      break;
    }
    if (symbol == ':') {
      in.setstate(std::ios::failbit);
      return in;
    }
    value += symbol;
  }
  char second = 0;
  in.get(second);
  if (!in || !isSllLiteral(value)) {
    in.setstate(std::ios::failbit);
    return in;
  }
  if ((symbol != 'l' && symbol != 'L') || (second != 'l' && second != 'L')) {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.ref = std::stoll(value);
  return in;
}

std::istream& zhuravleva::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> DelimiterIO{{'"'}}, dest.ref, '"');
}

std::istream& zhuravleva::operator>>(std::istream& in, KeyValueIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  if (dest.key == "key1" && !dest.used[0]) {
    in >> DoubleLitIO{dest.data.key1};
    dest.used[0] = true;
  } else if (dest.key == "key2" && !dest.used[1]) {
    in >> SllLitIO{dest.data.key2};
    dest.used[1] = true;
  } else if (dest.key == "key3" && !dest.used[2]) {
    in >> StringIO{dest.data.key3};
    dest.used[2] = true;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& zhuravleva::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DataStruct input;
  std::vector< bool > used(3, false);
  std::string key;

  in >> DelimiterIO{{'('}} >> DelimiterIO{{':'}};
  in >> key >> KeyValueIO{key, used, input} >> DelimiterIO{{':'}};
  in >> key >> KeyValueIO{key, used, input} >> DelimiterIO{{':'}};
  in >> key >> KeyValueIO{key, used, input} >> DelimiterIO{{':'}};
  in >> DelimiterIO{{')'}};

  if (!used[0] || !used[1] || !used[2]) {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (in) {
    dest = input;
  }

  return in;
}

std::ostream& zhuravleva::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  out << "(:key1 ";
  out << std::fixed << std::setprecision(1) << dest.key1 << "d:";
  out << "key2 " << dest.key2 << "ll:";
  out << "key3 " << std::quoted(dest.key3) << ":)";
  return out;
}

bool zhuravleva::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}
