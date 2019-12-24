clap
====

__clap__ is Command Line Argument Parser.

Goals
-----
* ANSI C89 / ISO C90
* Self-documented code with kernel-doc comments
* Single header / Single source file

Usage example
-------------
```c

#define LENGTH(array) (sizeof(array) / sizeof(0[array]))

int main(int argc, char* argv[]) {
    const struct clap_option options[] = {
        { 'h', "help", CLAP_NO_VALUE, "prints help" },
        { 'o', "option", CLAP_VALUE, "some option"}
    };
    struct clap_value value[LENGTH(options)];
    int free_args[LENGTH(options)];
    const char* free_args_names[] = {
        "files",
        NULL /* Additional NULL indicates one or many free argument */
    }
    int status;

    status = clap_parse(
        argc,
        argv,
        LENGTH(options),
        options,
        values,
        free_args);

    if (status != 0) {
        /* Handle parsing error */
    }

    /* Check help was met in arguments */
    if (values[0].enabled) {
        status = clap_print_help(
            stderr, /* FILE to print into */
            argv[0], /* Program name - can be customized */
            LENGTH(free_args_names),
            free_args_names,
            LENGTH(options),
            options);
        if (status != 0) {
            /* Handle printing error error */
        }
        return 1;
    }

    /* ... */

    return 0;
}
```