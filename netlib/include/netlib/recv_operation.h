#ifndef NETLIB_RECV_OPERATION_H
#define NETLIB_RECV_OPERATION_H

#include <system_error>
#include "netlib/loop.h"

namespace netlib {
    struct recv_result {
        int fd;
        char *buffer;
        int buffer_size;
        int bytes_read;
    };

    struct recv_operation {
        typedef recv_result result_type;

        int fd;
        char* buffer;
        int buffer_size;

        std::errc begin(loop &l, unsigned int idx) const;

        std::errc handle_completion(const io_uring_cqe *cqe, result_type &res) const;
    };

    void async_recv(loop &l, int fd, char *buffer, int buffer_size,
                    std::function<void(std::errc, recv_operation::result_type &&)> cb);


}

#endif