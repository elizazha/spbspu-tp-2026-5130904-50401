#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <string>
#include <vector>
#include <ios>
#include <istream>
#include <ostream>

namespace zhuravleva
{
  struct DataStruct
  {
    double key1;
    long long key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    std::vector< char > expected;
  };

  struct DoubleLitIO
  {
    double& ref;
  };

  struct SllLitIO
  {
    long long& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios< char >& stream);
    ~IOGuard();
  private:
    std::basic_ios< char >& stream_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };

  struct KeyValueIO
  {
    std::string key;
    std::vector< bool >& used;
    DataStruct& data;
  };


  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleLitIO&& dest);
  std::istream& operator>>(std::istream& in, SllLitIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, KeyValueIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
