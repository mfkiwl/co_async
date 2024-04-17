#pragma once/*{export module co_async:iostream.socket_stream;}*/

#include <co_async/std.hpp>/*{import std;}*/
#include <co_async/system/socket.hpp>/*{import :system.socket;}*/
#include <co_async/awaiter/task.hpp>/*{import :awaiter.task;}*/
#include <co_async/iostream/stream_base.hpp>/*{import :iostream.stream_base;}*/

namespace co_async {

struct SocketBuf {
    Task<std::size_t> raw_read(std::span<char> buffer) {
        return socket_read(mFile, buffer, mTimeout);
    }

    Task<std::size_t> raw_write(std::span<char const> buffer) {
        return socket_write(mFile, buffer, mTimeout);
    }

    SocketHandle release() noexcept {
        return std::move(mFile);
    }

    SocketHandle &get() noexcept {
        return mFile;
    }

    explicit SocketBuf(SocketHandle file) : mFile(std::move(file)) {}

    std::chrono::nanoseconds timeout() const {
        return mTimeout;
    }

    void timeout(std::chrono::nanoseconds timeout) {
        mTimeout = timeout;
    }

private:
    SocketHandle mFile;
    std::chrono::nanoseconds mTimeout = std::chrono::seconds(10);
};

/*[export]*/ using SocketIStream = IStream<SocketBuf>;
/*[export]*/ using SocketOStream = OStream<SocketBuf>;

/*[export]*/ struct SocketStream : IOStream<SocketBuf> {
    using IOStream<SocketBuf>::IOStream;

    Task<SocketStream> connect(const char *host, int port) {
        auto conn = co_await socket_connect({host, port});
        SocketStream sock(std::move(conn));
        co_return sock;
    }

    Task<SocketStream> accept(SocketListener &listener) {
        auto conn = co_await listener_accept(listener);
        SocketStream sock(std::move(conn));
        co_return sock;
    }
};


} // namespace co_async
