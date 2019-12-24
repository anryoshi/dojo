/*
 * MIT License
 * Copyright (c) 2019 Anton Rybakov
 * Full license text is available at https://opensource.org/licenses/MIT
 *
 * Greetings, Wanderer!
 * This is CLAP - Command Line Argument Parser
 * It provides basic functionality to parse command line arguments
 * in portable way relying only on ISO C90 standard
 * and consists of two files (one of which you are currently reading):
 *      clap.h - public API
 *      clap.c - implementation and internal API
 * All code is documented with kernel-doc comments
 * 
 * Format overview:
 *   Valid optional arguments formats:
 *     -h                            :: single letter argument without value
 *     -hVALUE | -h VALUE            :: single letter argument with value
 *     --hello                       :: word argument without value
 *     --hello=VALUE | --hello value :: word argument with value
 *   "--" - specifies that all arguments after it are free
 *   All other formats will be threated as free arguments including:
 *   "---", "-", "---hello" and etc.
 *
 * I hope CLAP will be useful for you
 */

#ifndef CLAP_H
#define CLAP_H

#include <stdio.h>

/**
 * struct clap_option - descriptor of command line option to extract
 * @letter: single character preceded by a single hyphen (e.g. -a)
 *          0 or '\0' should be used if no corresponding letter assumed
 * @word: string preceeded by a double hyphen (e.g. --abc)
 *        null pointer should be used if no corresponding word assumed
 * @value_required: enum specifying if string value required for option
 *                  or it acts only in switch like manner
 * @value_requied.CLAP_NO_VALUE: switch-like option
 * @value_requied.CLAP_VALUE: option requires a string a value
 * @description - description of option used in help generation
 */
struct clap_option {
    char letter;
    const char* word;
    enum {
        CLAP_NO_VALUE = 0,
        CLAP_VALUE = 1
    } value_required;
    const char* description;
};

/**
 * struct clap_value - clap_option corresponding extracted value
 * @enabled: indicate that option was met or not among argv arguments
 * @enabled.CLAP_NOT_ENABLED: option was not met
 * @enabled.CLAP_ENABLED: option was met
 * @string: if option requires value this field will be populated
 *          with null terminated string representing option value from argv
 */
struct clap_value {
    enum {
        CLAP_NOT_ENABLED = 0,
        CLAP_ENABLED = 1
    } enabled;
    const char* string;
};

/**
 * clap_parse() - Routine for parsing command line arguments.
 * @argc: main argc corresponding argument
 * @argv: main argv corresponding argument
 * @number_of_options: indicates number of options to parse and as result
 *                     length of options and values provided arguments
 * @options: pointer to array of length number_of_options containing
 *           descriptors of options to parse
 * @values: pointer to array of length number_of_options containing
 *          output data for corresponding option in options array
 * @free_args: pointer to array with length greater or equal argc + 1
 *             will be populated with indexes of free args in argv array
 *             the end of array is marked with -1
 *
 * Return: 0 - if everything was successful
 */
int clap_parse(
    int argc,
    const char* argv[],
    int number_of_options,
    const struct clap_option* options,
    struct clap_value* values,
    int* free_args);

/**
 * clap_print_help() - Routine for printing generated help string
 * @output: file descriptor to print in
 * @program_name: program_name to describe
 * @number_of_free_args: size of array of free arguments name
 * @free_args: free arguments names to put in help string
 *             if last two consequent pointer equals produced help will contain
 *             recurrent string of arg in form of "arg_name..."
 * @number_of_options: indicates number of options to parse and as result
 *                     length of options
 * @options: pointer to array of length number_of_options containing
 *           descriptors of options to serialize into help
 *
 * Return: 0 if everthing was successful
 */
int clap_print_help(
    FILE* output,
    const char* program_name,
    int number_of_free_args,
    const char* free_args[],
    int number_of_options,
    const struct clap_option* options);

#endif /* CLAP_H */
