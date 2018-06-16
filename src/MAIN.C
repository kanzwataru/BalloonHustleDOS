#include "src/TEST.H"
#include "src/wrecki~1/wb.h"

int main(int argc, char **argv)
{
    if(argc == 2) {
        if(0 == strcmp(argv[1], "test"))
            test_start(0, 0);
        if(0 == strcmp(argv[1], "test-keyboard"))
            test_keyboard();
    }
    else if(argc == 3) {
        if(0 == strcmp(argv[1], "benchmark"))
            test_start(1, atoi(argv[2]));
    }
    else {
        wrecking_balloon_start();
    }

    return 0;
}