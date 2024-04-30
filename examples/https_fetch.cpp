#include <co_async/co_async.hpp>
#include <co_async/std.hpp>

using namespace co_async;
using namespace std::literals;

Task<Expected<void, std::errc>> amain() {
    co_await https_load_ca_certificates();
    HTTPConnectionPool pool;
    std::vector<FutureSource<Expected<void, std::errc>>> res;
    for (std::string path: {"style.css", "koru-icon.png", "mtk_2021_200.png"}) {
        res.push_back(co_future(co_bind([&, path] () -> Task<Expected<void, std::errc>> {
            auto conn = co_await co_await pool.connect("https://man7.org");
            HTTPRequest req = {
                .method = "GET",
                .uri = URI::parse("/" + path),
            };
            HTTPResponse res;
            std::string body;
            debug(), "req", req;
            co_await co_await conn->request(req, {}, res, body);
            debug(), "res", body.size(), res;
            co_await co_await file_write(make_path("/tmp", path), body);
            co_return {};
        })));
    }
    for (auto &&r: res) {
        co_await co_await r;
    }
    /* Pid pid = co_await co_await ProcessBuilder() */
    /*     .path("display") */
    /*     .arg("koru-icon.png") */
    /*     .spawn(); */
    /* co_await co_await wait_process(pid); */
    co_return {};
}

int main() {
    co_synchronize(amain()).value();
    return 0;
}
