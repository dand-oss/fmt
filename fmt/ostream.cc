/*
 Formatting library for C++ - std::ostream support

 Copyright (c) 2012 - 2016, Victor Zverovich
 All rights reserved.

 For the license information refer to format.h.
 */

#include "ostream.h"

#if defined( _WIN64 ) || defined( _WIN32 )
#include <windows.h>
#endif // defined( _WIN64 ) || defined( _WIN32 )
#include <sstream>

namespace fmt {

namespace internal {
FMT_FUNC void write(std::ostream &os, Writer &w) {
  const char *data = w.data();
  typedef internal::MakeUnsigned<std::streamsize>::Type UnsignedStreamSize;
  UnsignedStreamSize size = w.size();
  UnsignedStreamSize max_size =
      internal::to_unsigned((std::numeric_limits<std::streamsize>::max)());
  do {
    UnsignedStreamSize n = size <= max_size ? size : max_size;
    os.write(data, static_cast<std::streamsize>(n));
    if ( os.fail() ) {
       std::ostringstream ostr ;
       ostr << "write failed" ;
#if defined( _WIN64 ) || defined( _WIN32 )
       ostr << " " << GetLastError() << ": " << strerror(GetLastError());
#endif // defined( _WIN64 ) || defined( _WIN32 )
       throw std::runtime_error(ostr.str());
    }
    data += n;
    size -= n;
  } while (size != 0);
  os.flush() ; // need to get stuff printed...
} // write()
}

FMT_FUNC void print(std::ostream &os, CStringRef format_str, ArgList args) {
  MemoryWriter w;
  w.write(format_str, args);
  internal::write(os, w);
}
}  // namespace fmt
