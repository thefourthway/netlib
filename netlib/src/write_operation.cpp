#include "netlib/write_operation.h"

namespace netlib {
    std::errc write_operation::begin(loop &loop, unsigned int idx) const {
        const auto sqe = loop.sqe(idx);

        ::io_uring_prep_write(sqe, this->fd, this->buffer, this->buffer_size, 0);

        return std::errc();
    }


    std::errc write_operation::handle_completion(const io_uring_cqe *cqe, write_result &res) const {
        res.fd = this->fd;
        res.buffer = this->buffer;
        res.buffer_size = this->buffer_size;

        if (cqe->res < 0) {
            res.bytes_written = 0;
            return static_cast<std::errc>(-cqe->res);
        }

        res.bytes_written = cqe->res;

        return std::errc();
    }
}
