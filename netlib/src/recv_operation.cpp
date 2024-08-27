#include "netlib/recv_operation.h"

namespace netlib {
    std::errc recv_operation::begin(loop &l, unsigned int idx) const {
        const auto sqe = l.sqe(idx);
        io_uring_prep_recv(sqe, fd, buffer, buffer_size, 0);
        return std::errc();
    }

    std::errc recv_operation::handle_completion(const io_uring_cqe *cqe, result_type &res) const {
        const int rr = cqe->res;

        if (rr < 0) {
            return static_cast<std::errc>(-rr);
        }

        res.fd = fd;
        res.buffer = buffer;
        res.buffer_size = buffer_size;
        res.bytes_read = rr;
        return std::errc();
    }

    void async_recv(loop &l, int fd, char *buffer, int buffer_size,
        std::function<void(std::errc, recv_operation::result_type &&)> cb) {
        l.post_raw(recv_operation{
            fd,
            buffer,
            buffer_size
        }, std::move(cb));
    }
}
