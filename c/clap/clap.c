#include "clap.h"
#include <string.h>

/**
 * enum clap_arg_type - bitmask-like enumeration for parsed argument
 * @FREE: argument is free and its index should be saved in free_args array
 * @SEPARATOR: argument is free argument separator "--"
 * @LETTER: one character option matched with optional value included
 * @WORD: word-like option matched with optional value included
 * @WITH_VALUE: bit enabled if parsed argument contains value itself
 */
enum clap_arg_type {
    FREE = 0,
    SEPARATOR = 1 << 0,
    LETTER = 1 << 1,
    WORD = 1 << 2,
    WITH_VALUE = 1 << 3
};

/**
 * struct clap_arg - represents single parsed command line argument
 * @type: represents type of argument according enum clap_arg_type description
 * @label_length: because label is not null-terminated string its length saved
 *                always should be >= 1, and in case of 1 indicates that
 *                type LETTER bit should be enabled
 * @value: null terminated string contains 
 */
struct clap_arg {
    enum clap_arg_type type;
    size_t label_length;
    const char* label;
    const char* value;
};

/**
 * clap_arg_separator - predefined constant representing separator argument
 */
static const struct clap_arg clap_arg_separator = {
    SEPARATOR, 0, NULL, NULL
};

/**
 * clap_arg_separator - predefined constant representing free argument
 */
static const struct clap_arg clap_arg_free = {
    FREE, 0, NULL, NULL
};

static
struct clap_arg
clap_match_letter_arg(const char* arg) {
    struct clap_arg result;
    size_t arg_len = strlen(arg);
    if (arg_len == 2) {
        result.type = LETTER;
        result.label_length = 1;
        result.label = arg + 1;
        result.value = 0;
    }
    else {
        result.type = LETTER | WITH_VALUE;
        result.label_length = 1;
        result.label = arg + 1;
        result.value = arg + 2;
    }
    return result;
}

static
struct clap_arg
clap_match_word_arg(const char* arg) {
    /* Long arg: --label | --label=VALUE */
    struct clap_arg result;
    const char* eq_loc = strrchr(arg, '=');
    if (eq_loc == 0) {
        /* Long arg: --label */
        result.type = WORD;
        result.label_length = strlen(arg) - 2;
        result.label = arg + 2;
        result.value = 0;
    }
    else {
        /* Long arg: --label=VALUE */
        result.type = WORD | WITH_VALUE;
        result.label_length = (size_t)(eq_loc - arg) - 2;
        result.label = arg + 2;
        result.value = eq_loc + 1;
    }

    return result;
}

static struct clap_arg clap_match_arg(const char* arg) {
    if (arg[0] != 0 && arg[0] == '-') {
        if (arg[1] != 0) {
            if (arg[1] != '-') {
                return clap_match_letter_arg(arg); 
            }
            else {
                if (arg[2] == 0) {
                    return clap_arg_separator;
                }
                else if (arg[2] != '-') {
                    return clap_match_word_arg(arg);
                }
            }
        }
    }

    return clap_arg_free;
}

static int clap_find_letter_option(
        int number_of_options,
        const struct clap_option* options,
        char letter) {
    int i;

    for (i = 0; i < number_of_options; ++i) {
        if (options[i].letter == letter) {
            return i;
        }
    }

    return -1;
}

static int clap_find_word_option(
        int number_of_options,
        const struct clap_option* options,
        size_t label_length,
        const char* label) {
    int i;

    for (i = 0; i < number_of_options; ++i) {
        if (options[i].word != NULL
                && strncmp(options[i].word, label, label_length) == 0) {
            return i;
        }
    }

    return -1;
}
   
static int clap_find_option(
        int number_of_options,
        const struct clap_option* options,
        struct clap_arg arg) {
    if (((arg.type & LETTER) != 0)  && arg.label != NULL) {
        return clap_find_letter_option(
                number_of_options,
                options,
                *arg.label);
    }
    else if ((arg.type & WORD) != 0) {
        return clap_find_word_option(
                number_of_options,
                options,
                arg.label_length,
                arg.label);
    }

    return -1;
}

/**
 * clap_parse() - Routine for parsing command line arguments.
 * NOTE: description of function arguments available in header file
 */
int clap_parse(
        int argc,
        const char* argv[],
        int number_of_options,
        const struct clap_option* options,
        struct clap_value* values,
        int* free_args) {

    int i;

    if (argc == 0 || argv == NULL
            || number_of_options == 0 || options == NULL
            || values == NULL || free_args == NULL) {
        return 1;
    }

    memset(
        values,
        0,
        sizeof(struct clap_value) * (unsigned long)number_of_options);

    for (i = 0; i < argc; ++i) {
        const char* arg_string = argv[i];

        struct clap_arg arg = clap_match_arg(arg_string);

        if (((arg.type & LETTER) != 0) || ((arg.type & WORD) != 0)) {
            int option_index 
                = clap_find_option(number_of_options, options, arg);
            if (option_index != -1) {
                values[option_index].enabled = CLAP_ENABLED;
                if (options[option_index].value_required) {
                    if ((arg.type & WITH_VALUE) != 0) {
                        values[option_index].string = arg.value;
                    }
                    else {
                        values[option_index].string = argv[++i];
                    }
                }
            }
        }
        else if ((arg.type & SEPARATOR) != 0) {
            ++i;
            break;
        }
        else {
            *(free_args++) = i;  
        }
    }

    for (; i < argc; ++i) {
        *(free_args++) = i;  
    }

    return 0;
}

/**
 * clap_print_help() - Routine for printing generated help string
 * NOTE: description of function arguments available in header file
 */
int clap_print_help(
        FILE* output,
        const char* program_name,
        int number_of_free_args,
        const char* free_args[],
        int number_of_options,
        const struct clap_option* options) {
    int i;
    int column;

    fprintf(output, "Usage: %s [options]", program_name);
    if (free_args != NULL) {
        for (i = 0; i < number_of_free_args - 1; ++i) {
            fprintf(output, " %s", free_args[i]);
        }
        
        if (free_args[i] == free_args[i + 1]) {
            fprintf(output, "...\n");
        }
        else {
            fprintf(output, " %s\n", free_args[i + 1]);
        }
    }
    else {
        fprintf(output, "\n");
    }
    

    fprintf(output, "Options:\n");

    for (i = 0; i < number_of_options; ++i) {
        column = 0;
        column += fprintf(output, "  ");
        if (options[i].letter != 0) {
            column += fprintf(output, "-%c", options[i].letter);
            if (options[i].word != NULL) {
                column += fprintf(output, " | ");
            }
        }
        if (options[i].word != NULL) {
            column += fprintf(output, "--%s", options[i].word);
        }
        if (options[i].value_required) {
            column += fprintf(output, " <value>");
        }
        /* Should be always print on 28 column */
        if (options[i].description != NULL) {
            if (column < 27) {
                fprintf(output, "%*s%s", 28 - column, "", options[i].description);
            }
            else {
                fprintf(output, "\n%*s%s", 28, "", options[i].description);
            }
        }
        fprintf(output, "\n");
    }
  
    return 0;
}
