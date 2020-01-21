#include "nanotest.h"
#include "clap.h"

#include <string.h>

static const struct clap_option options[] = {
    { 'a', NULL, CLAP_NO_VALUE, "a option without arg" },
    { 'b', NULL, CLAP_VALUE_REQUIRED, "b option with arg" },
    { 'c', NULL, CLAP_VALUE_REQUIRED, "c option with arg" },
    { 'd', NULL, CLAP_VALUE_OPTIONAL, "d option with arg" },
    { 0, "aword", CLAP_NO_VALUE, "aword option without arg" },
    { 0, "bword", CLAP_VALUE_REQUIRED, "bword option with arg" },
    { 'z', "zword", CLAP_VALUE_REQUIRED, "zword option with arg" }
};

#define NUMBER_OF_OPTIONS (sizeof(options) / sizeof(struct clap_option))

struct nanotest_error clap_smoke_test() {
    size_t i;
    const char* argv[] = {
        "program_name",
        "-a",
        "-bBVAL",
        "-c", "CVAL",
        "-d",
        "--aword",
        "--bword=EWORDVAL",
        "--zword", "FWORDVAL",
        "FREE_ARG"
    };
    int argc = sizeof(argv) / sizeof(const char*);
    struct clap_value values[NUMBER_OF_OPTIONS];
    int free_args[NUMBER_OF_OPTIONS];

    clap_parse(
        argc,
        argv,
        NUMBER_OF_OPTIONS,
        options,
        values,
        free_args);

    for (i = 0; i < NUMBER_OF_OPTIONS; ++i) {
        nanotest_assert(
            values[i].enabled,
            "Option was not matched"
            );
    }

    nanotest_success();
}

struct nanotest_error clap_null_argv_test() {
    struct clap_value values[NUMBER_OF_OPTIONS];
    int free_args[NUMBER_OF_OPTIONS];
    int status;

    status = clap_parse(
        0,
        NULL,
        NUMBER_OF_OPTIONS,
        options,
        values,
        free_args);

    nanotest_assert(
        status == 1,
        "Status is not 1"
        );

    nanotest_success();
}

struct nanotest_error clap_null_options_test() {
    struct clap_value values[NUMBER_OF_OPTIONS];
    int free_args[NUMBER_OF_OPTIONS];
    int status;

    const char* argv[] = {
        "program_name",
    };
    int argc = sizeof(argv) / sizeof(const char*);

    status = clap_parse(
        argc,
        argv,
        0,
        NULL,
        values,
        free_args);

    nanotest_assert(
        status == 1,
        "Status is not 1"
        );

    nanotest_success();
}

struct nanotest_error clap_null_values_test() {
    int free_args[NUMBER_OF_OPTIONS];
    int status;

    const char* argv[] = {
        "program_name",
    };
    int argc = sizeof(argv) / sizeof(const char*);

    status = clap_parse(
        argc,
        argv,
        NUMBER_OF_OPTIONS,
        options,
        NULL,
        free_args);

    nanotest_assert(
        status == 1,
        "Status is not 1"
        );

    nanotest_success();
}


struct nanotest_error clap_null_free_args_test() {
    struct clap_value values[NUMBER_OF_OPTIONS];
    int status;

    const char* argv[] = {
        "program_name",
    };
    int argc = sizeof(argv) / sizeof(const char*);

    status = clap_parse(
        argc,
        argv,
        NUMBER_OF_OPTIONS,
        options,
        values,
        NULL);

    nanotest_assert(
        status == 1,
        "Status is not 1"
        );

    nanotest_success();
}

struct nanotest_error clap_only_free_argument_test() {
    size_t i;
    const char* argv[] = {
        "program_name",
        "FREE_ARG"
    };
    int argc = sizeof(argv) / sizeof(const char*);
    struct clap_value values[NUMBER_OF_OPTIONS];
    int free_args[NUMBER_OF_OPTIONS];
    int status;

    status = clap_parse(
        argc,
        argv,
        NUMBER_OF_OPTIONS,
        options,
        values,
        free_args);

    nanotest_assert(
        status == 0,
        "Status code is not 0"
        );

    for (i = 0; i < NUMBER_OF_OPTIONS; ++i) {
        nanotest_assert(
            !values[i].enabled,
            "Option was matched"
            );
        nanotest_assert(
            values[i].string == NULL,
            "Option has string"
            );
    }

    for (i = 0; ((int)i < argc) && (free_args[i] != -1); ++i) {
        nanotest_assert(
            free_args[i] == (int)i,
            "Incorrect free_args elemenent value"
            );
    }

    nanotest_assert(
        i == 2,
        "Incorrect number of free_args"
        );

    nanotest_success();
}

struct nanotest_error clap_only_letter_argument_test() {
    size_t i;
    const char* argv[] = {
        "program_name",
        "-a"
    };
    int argc = sizeof(argv) / sizeof(const char*);
    struct clap_value values[NUMBER_OF_OPTIONS];
    int free_args[NUMBER_OF_OPTIONS];
    int status;

    status = clap_parse(
        argc,
        argv,
        NUMBER_OF_OPTIONS,
        options,
        values,
        free_args);

    nanotest_assert(
        status == 0,
        "Status code is not 0"
        );

    nanotest_assert(
        values[0].enabled,
        "Option was matched"
        );
    nanotest_assert(
        values[0].string == NULL,
        "Option has string"
        );

    for (i = 1; i < NUMBER_OF_OPTIONS; ++i) {
        nanotest_assert(
            !values[i].enabled,
            "Option was matched"
            );
        nanotest_assert(
            values[i].string == NULL,
            "Option has string"
            );
    }

    nanotest_success();
}

struct nanotest_error clap_only_word_argument_test() {
    size_t i;
    const char* argv[] = {
        "program_name",
        "--aword"
    };
    int argc = sizeof(argv) / sizeof(const char*);
    struct clap_value values[NUMBER_OF_OPTIONS];
    int free_args[NUMBER_OF_OPTIONS];
    int status;

    status = clap_parse(
        argc,
        argv,
        NUMBER_OF_OPTIONS,
        options,
        values,
        free_args);

    nanotest_assert(
        status == 0,
        "Status code is not 0"
        );

    nanotest_assert(
        values[4].enabled,
        "Option was matched"
        );
    nanotest_assert(
        values[4].string == NULL,
        "Option has string"
        );

   for (i = 0; i < NUMBER_OF_OPTIONS; ++i) {
       if (i == 4) {
           continue;
       }
       nanotest_assert(
           !values[i].enabled,
           "Option was matched"
           );
       nanotest_assert(
           values[i].string == NULL,
           "Option has string"
           );
    }

    nanotest_success();
}

struct nanotest_error clap_separator_test() {
    nanotest_success();
}

struct nanotest_error clap_help_test() {
    FILE* temp;
    unsigned long fsize;
    char* string ;
    const char* expected_string
       = "Usage: my_program [options]\n"
         "Options:\n"
         "  -a                        a option without arg\n"
         "  -b <value>                b option with arg\n"
         "  -c <value>                c option with arg\n"
         "  -d [value]                d option with arg\n"
         "  --aword                   aword option without arg\n"
         "  --bword <value>           bword option with arg\n"
         "  -z | --zword <value>      zword option with arg\n";

    temp = tmpfile();
    clap_print_help(
        temp,
        "my_program",
        0,
        NULL,
        NUMBER_OF_OPTIONS,
        options
        );

    fsize = (unsigned long) ftell(temp);
    string = malloc(fsize + 1);
    rewind(temp);
    fread(string, 1, fsize, temp);
    string[fsize] = 0;

    nanotest_assert(
        strcmp(string, expected_string) == 0,
        "Help string not equal with expected"
        );

    nanotest_success();
}

int main() {
    nanotest_run(clap_smoke_test);
    nanotest_run(clap_null_argv_test);
    nanotest_run(clap_null_options_test);
    nanotest_run(clap_null_values_test);
    nanotest_run(clap_null_free_args_test);
    nanotest_run(clap_only_free_argument_test);
    nanotest_run(clap_only_letter_argument_test);
    nanotest_run(clap_only_word_argument_test);
    nanotest_run(clap_separator_test);
    nanotest_run(clap_help_test);
    return 0;
}
