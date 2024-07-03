#ifndef APP_H
#define APP_H

#include "logger.h"
#include <filesystem>
#include <libgen.h> // 用于 dirname 函数
#include <mach-o/dyld.h>
#include <string>

class App {
public:
  static std::filesystem::path ApplicationDirectory() {
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
      char *dir = dirname(path);
      LOG_INFO("executable path is {}\n", dir);
      return std::filesystem::path(dir);
    } else {
      LOG_CRITICAL("buffer too small; need size %u\n", size);
      return "";
    }
  }
};
#endif