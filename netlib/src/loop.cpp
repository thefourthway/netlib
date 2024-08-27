#include "netlib/loop.h"

namespace netlib {
    loop::loop() : ring(), params{0}, cqe(nullptr), cqe_index_counter(0) {
        ::io_uring_queue_init_params(30, &ring, &params);
    }

    loop::~loop() {
        cqe = nullptr;
        ::io_uring_queue_exit(&ring);
    }

    void loop::wait() {
        while (!floating_callbacks.empty() || !live_callbacks.empty()) {
            while (!floating_callbacks.empty()) {
                // exception here = game over. don't do that?
                floating_callbacks[0]();
                floating_callbacks.erase(floating_callbacks.begin());
            }

            const int ret = ::io_uring_peek_cqe(&ring, &cqe);

            if (ret == -EAGAIN) {
                continue;
            }

            if (ret < 0) {
                std::terminate();
            }

            if (!cqe) continue;

            if (live_callbacks.find(cqe->user_data) != live_callbacks.end()) {
                // exception here = game over. don't do that?
                live_callbacks[cqe->user_data]();
            }

            ::io_uring_cqe_seen(&ring, cqe);
        }
    }
}
