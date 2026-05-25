#include "dataStruct.hpp"
#include <istream>
#include <ostream>

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
