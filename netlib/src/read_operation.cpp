#include "netlib/read_operation.h"

namespace netlib {
    std::errc read_operation::begin(loop &l, const unsigned int idx) const {
        const auto sqe = l.sqe(idx);
        io_uring_prep_read(sqe, fd, buffer, buffer_size, 0);
        return std::errc();
    }

    std::errc read_operation::handle_completion(const io_uring_cqe *cqe, read_result &res) const {
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

    void async_read(loop &loop, const int fd, char *buffer, const int buffer_size,
        std::function<void(std::errc, read_operation::result_type &&res)> cb) {
        loop.post_raw(read_operation{
                          fd,
                          buffer,
                          buffer_size
                      }, std::move(cb));
    }
}
