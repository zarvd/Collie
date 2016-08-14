#ifndef COLLIE_BASE_STRING_H_
#define COLLIE_BASE_STRING_H_

#include <ostream>
#include <string>
#include <vector>
#include "../collie.h"

namespace collie {
namespace string {

inline void trim_left(std::string& str,
                      const std::string& delimeter = " ") noexcept {
  if (str.empty() || delimeter.empty() || str.length() < delimeter.length())
    return;
  unsigned i = 0, j = 0;
  while (i < str.length()) {
    if (str[i] != delimeter[j]) break;
    ++i;
    ++j;
    if (j == str.length()) j = 0;
  }
  str = str.substr(i);
}

inline void trim_right(std::string& str,
                       const std::string& delimeter = " ") noexcept {
  if (str.empty() || delimeter.empty() || str.length() < delimeter.length())
    return;
  int i = str.length() - 1, j = delimeter.length() - 1;
  while (i >= 0) {
    if (str[i] != delimeter[j]) break;
    --i;
    --j;

    if (j < 0) j = delimeter.length() - 1;
  }
  str = str.substr(0, i + 1);
}

inline void trim(std::string& str,
                 const std::string& delimeter = " ") noexcept {
  if (str.empty() || delimeter.empty() || str.length() < delimeter.length())
    return;

  trim_left(str, delimeter);
  trim_right(str, delimeter);
}

inline std::vector<std::string> split(const std::string& str,
                                      const std::string& delimeter) noexcept {}

void replace(std::string& str, const std::string& old_value,
             const std::string& new_value) noexcept;
void remove(std::string&, const std::string&) noexcept;
}
}

#endif /* COLLIE_BASE_STRING_H_ */
