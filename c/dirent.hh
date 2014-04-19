#include <dirent.h>

#ifndef C_DIRENT_HH
#define C_DIRENT_HH

namespace std
{
     extern "C" {
          int closedir(DIR *);
          DIR *opendir(const char *);
          struct dirent *readdir(DIR *);
     }
}

#endif
