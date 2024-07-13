#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "nan/logger.h"
#include <filesystem>
#include <libgen.h> // 用于 dirname 函数
#include <mach-o/dyld.h>
#include <string>

class FileSystem {
public:
  static std::filesystem::path RootDirectory() {
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
      char *dir = dirname(path);
      return std::filesystem::path(dir);
    } else {
      LOG_CRITICAL("buffer too small; need size %u\n", size);
      return "";
    }
  }

  static std::filesystem::path GetPath(const char *file) {
    return RootDirectory() / file;
  }
};

#endif