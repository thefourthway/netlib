#include "netlib/fdops.h"

#include <cstdio>
#include <unistd.h>

namespace netlib
{
    void close_descriptor(int& fd) noexcept
    {
        if (fd >= 0) {
            if (::close(fd) < 0)
                perror("close");
            fd = -1;
        }
    }
}
