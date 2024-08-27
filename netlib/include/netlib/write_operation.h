#pragma once
#include "netlib/loop.h"

namespace netlib {
    struct write_result {
        int fd;
        const char *buffer;
        int buffer_size;
        int bytes_written;
    };

    struct write_operation {
        typedef write_result result_type;

        int fd;
        const char *buffer;
        int buffer_size;

        std::errc begin(loop &loop, unsigned int idx) const;

        std::errc handle_completion(const io_uring_cqe *cqe, write_result &res) const;
    };

    void async_write(loop &loop, int fd, char *buffer, int buffer_size,
                     const std::function<void(std::errc, write_operation::result_type &&res)> &cb);
}
