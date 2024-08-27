#include <iostream>
#include <memory>
#include <netlib.h>
#include <array>

class conn : public std::enable_shared_from_this<conn> {
public:
    explicit conn(netlib::loop &l) : fd(-1), m_loop(l), read_buffer() {
    }

    ~conn() = default;

    void connect() {
        auto interm = [self=shared_from_this()](auto e, auto &&r) {
            self->on_connect(e, std::forward<decltype(r)>(r));
        };

        async_connect(m_loop, "127.0.0.1", 11050, std::move(interm));
    }

    void start_reading() {
        auto interm = [self=shared_from_this()](auto e, auto && res) {
            self->on_read(e, std::forward<decltype(res)>(res));
        };

        async_recv(m_loop, this->fd, this->read_buffer.data(), this->read_buffer.size(),
            std::move(interm));
    }

    void on_connect(const std::errc err, netlib::connect_operation::result_type &&res) {
        if (err != std::errc{}) {
            std::cout << "FAILED TO CONNECT " << std::endl;
            return;
        }

        this->fd = res.fd;

        start_reading();
    }

    void on_read(const std::errc err, netlib::recv_operation::result_type && res) {
        if (err != std::errc{} || res.bytes_read == 0) {
            std::cout << "FAILED TO READ" << std::endl;
            return;
        }

        std::cout << std::string(read_buffer.data(), res.bytes_read) << std::endl;
        start_reading();
    }

    int fd;
    netlib::loop &m_loop;
    std::array<char, 2048> read_buffer;
};


int main() {
    const auto loop = std::make_shared<netlib::loop>();

    auto c = std::make_shared<conn>(*loop);

    c->connect();

    while (true) {
        loop->wait();
    }
}
