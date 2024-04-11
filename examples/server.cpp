#include <co_async/co_async.hpp>/*{import co_async;}*/
#include <cmake/clang_std_modules_source/std.hpp>/*{import std;}*/

using namespace co_async;
using namespace std::literals;

Task<> amain() {
    auto listener = co_await listener_bind({"127.0.0.1", 8080});
    HTTPServer http;
    http.route("/", [](HTTPRequest const &request) -> Task<HTTPResponse> {
        if (request.method != "GET")
            co_return HTTPServer::make_error_response(405);
        co_return {
            .status = 200,
            .headers =
                {
                    {"content-type", "text/html;charset=utf-8"},
                },
            .body = "<meta http-equiv=refresh content=1><h1>It works!</h1>",
        };
    });

    co_await stdio().putline("正在监听: " + listener.address().toString());
    while (1) {
        auto conn = co_await listener_accept(listener);
        co_spawn([](auto msg) -> Task<> {
                co_await stdio().putline(msg);
            }, "线程 " + to_string(globalSystemLoop.this_thread_worker_id()) +
                 " 收到请求: " + listener.address().toString());
        co_spawn(http.process_connection(SocketStream(std::move(conn))));
    }
}

int main() {
    co_synchronize(amain());
    return 0;
}
