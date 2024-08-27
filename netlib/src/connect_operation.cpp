#include "netlib/connect_operation.h"
#include <liburing.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace netlib {
    connect_operation::connect_operation(const std::string &h, const unsigned short p) : // NOLINT(*-pass-by-value)
    hostname(h), port(p), address{},
        fd(-1) {
    }


    std::errc connect_operation::begin(loop &l, const unsigned int idx) {
        fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

        if (fd < 0) {
            return static_cast<std::errc>(fd);
        }

        address = {0};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        inet_pton(AF_INET, hostname.c_str(), &address.sin_addr);

        const auto sqe = l.sqe(idx);
        io_uring_prep_connect(sqe, fd, reinterpret_cast<sockaddr *>(&address), sizeof(address));
        return std::errc{};
    }

    std::errc connect_operation::handle_completion(const io_uring_cqe *cqe, connect_result &res) const {
        if (const int rr = cqe->res; rr < 0) {
            close(this->fd);
            return static_cast<std::errc>(-rr);
        }

        res.fd = fd;
        return std::errc();
    }

    void async_connect(loop &l, const std::string &hostname, const unsigned short port,
    std::function<void(std::errc, connect_operation::result_type &&res)> cb) {
        l.post_raw(connect_operation(hostname, port), std::move(cb));
    }
}
