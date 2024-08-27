#pragma once
#include <liburing.h>
#include <functional>
#include <iostream>

namespace netlib {
    typedef std::function<void()> loop_callback_fn;

    struct loop;

    // bound_operation<Operation> is a struct that stores all of the relevant details for performing the action
    // and returning error codes and result data to the caller. it's first allocated by the loop, and it
    // is assigned an operation_id before it's downcast to a void() function in
    template<typename Operation>
    struct bound_operation {
        using ResultType = typename std::decay_t<Operation>::result_type;
        using callback_type = std::function<void(std::errc, ResultType &&)>;

        unsigned int operation_id;

        Operation op;
        std::errc error;
        ResultType res;
        callback_type callback;
    };

    template<typename T>
    std::function<void()> create_finalize_function(loop *loop, bound_operation<T> *op);

    struct loop {
        loop();

        ~loop();

        template<typename Operation>
        void post_raw(Operation op,
                      std::function<void(std::errc, typename std::decay_t<Operation>::result_type &&)> cb) {
            const auto idx = cqe_index_counter;

            // rather than passing in an io_uring_sqe* to our operation, there may be
            // failures occurring before we get to creating one. Cancelling an sqe is sort of annoying,
            // so i didn't bother and let operations decide if they want to request one or not.
            // they can call loop.sqe(operation_id) to create one if they so choose.
            if (const std::errc setup_result = op.begin(
                static_cast<loop&>(*this),
                static_cast<unsigned int>(idx)); setup_result != std::errc()) {
                cb(setup_result, {});
                return;
            }

            // allocat
            auto bound = new bound_operation<Operation>{
                idx,
                std::move(op),
                std::errc(),
                {0},
                std::move(cb)
            };

            cqe_index_counter++;
            live_callbacks[bound->operation_id] = create_finalize_function(this, bound);

            io_uring_submit(&ring);
        }

        void post(const loop_callback_fn& cb) {
            floating_callbacks.push_back(cb);
        }

        template<typename T>
        void finalize_operation(bound_operation<T> *op) {
            post([op, this]() {
                op->callback(op->error, std::move(op->res));
                if (this->live_callbacks.find(op->operation_id) != this->live_callbacks.end()) {
                    this->live_callbacks.erase(op->operation_id);
                }
                delete op;
            });
        }

        void wait();

        io_uring_sqe* sqe(const unsigned int client_id) {
            // see comment about this function under post_raw()
            io_uring_sqe* res = io_uring_get_sqe(&ring);
            res->user_data = client_id;
            return res;
        }

        io_uring ring;
        io_uring_params params;
        io_uring_cqe* cqe;
        unsigned int cqe_index_counter;
        std::unordered_map<unsigned int, loop_callback_fn> live_callbacks;
        std::vector<std::function<void()>> floating_callbacks;
    };

    template<typename T>
    std::function<void()> create_finalize_function(loop *loop, bound_operation<T> *op) {
        return [op, loop=loop] {
            op->error = op->op.handle_completion(loop->cqe, op->res);

            loop->finalize_operation(op);
        };
    }
}
