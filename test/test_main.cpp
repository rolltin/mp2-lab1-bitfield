#include <gtest.h>

int main(int argc, char **argv) {
    char* locale = setlocale(LC_ALL, "");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
