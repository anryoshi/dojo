#include "nanotest.h"

struct nanotest_error test_example() {
    nanotest_assert(1 + 1 == 2, "1 + 1 != 2");
    nanotest_success();
}

int main() {
    nanotest_run(test_example);
    return 0;
}
