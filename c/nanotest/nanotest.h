#ifndef NANOTEST_H
#define NANOTEST_H

#include <stdlib.h>
#include <stdio.h>

struct nanotest_error {
    const char* source_file;
    int line;
    const char* message;
};

#define nanotest_assert(test, msg)\
    do {\
        if (!(test)) {\
            struct nanotest_error error;\
            error.source_file = __FILE__;\
            error.line = __LINE__;\
            error.message = msg;\
            return error;\
        }\
    } while (0)

#define nanotest_success()\
    do {\
        const struct nanotest_error error = {\
            NULL, 0, NULL\
        };\
        return error;\
    } while (0)

#define nanotest_run(test)\
    do {\
        struct nanotest_error error;\
        printf("Running: %s ...", #test);\
        error = test();\
        if (error.message) {\
            printf(" FAILED\n");\
            printf("%s:%d - %s",\
                    error.source_file,\
                    error.line,\
                    error.message);\
            exit(1);\
        }\
        printf(" OK\n");\
    } while (0)

#endif /* NANOTEST_H */
