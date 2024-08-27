#pragma once

#include <netinet/in.h>
#include "netlib/loop.h"

namespace netlib {
    struct connect_result {
        int fd;
    };

    struct connect_operation {
        typedef connect_result result_type;

        std::string hostname;
        unsigned short port;

        sockaddr_in address;

        int fd;

        connect_operation(const std::string& h, unsigned short p);

        std::errc begin(loop &l, unsigned int idx);

        std::errc handle_completion(const io_uring_cqe *cqe, connect_result &res);
    };

    void async_connect(loop &l, const std::string &hostname, unsigned short port,
                        std::function<void(std::errc, connect_operation::result_type &&res)> cb);


}
