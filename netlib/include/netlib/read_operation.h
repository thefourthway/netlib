#ifndef NETLIB_READ_OPERATION_H
#define NETLIB_READ_OPERATION_H

#include "netlib/loop.h"

namespace netlib {
    struct read_result {
        int fd;
        char* buffer;
        int buffer_size;

        int bytes_read;
    };

    struct read_operation {
        typedef read_result result_type;

        int fd;
        char* buffer;
        int buffer_size;

        std::errc begin(loop& l, unsigned int idx) const;
        std::errc handle_completion(const io_uring_cqe* cqe, read_result& res) const;
    };

    void async_read(loop& loop, int fd, char* buffer, int buffer_size,
        std::function<void(std::errc, read_operation::result_type && res)> cb);
}

#endif