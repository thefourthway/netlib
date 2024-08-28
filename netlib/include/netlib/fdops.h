#ifndef NETLIB_FDOPS_H
#define NETLIB_FDOPS_H

namespace netlib
{
    void close_descriptor(int& fd) noexcept;
}

#endif