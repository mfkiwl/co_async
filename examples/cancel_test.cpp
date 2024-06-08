#include <co_async/co_async.hpp>

using namespace co_async;
using namespace std::literals;

static Task<int> compute() {
    CancelToken cancel = co_await co_cancel;
    auto res = co_await co_sleep(200ms);
    debug(), "睡眠结果", res;
    if (cancel)
        co_return 0;
    co_return 42;
}

static Task<> amain() {
    auto ret = co_await co_timeout(compute(), 100ms);
    debug(), "计算结果", ret;
    co_return;
}

int main() {
    std::setlocale(LC_ALL, "");
    IOContext().join(amain());
}
