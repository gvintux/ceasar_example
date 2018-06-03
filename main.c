#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STR_LEN 255

static const char *E_MSG_HELP = "usage: ceasar.exe -(e|d) -n N [-src srcfile] [-abc abcfile] [-out outfile]";
static const char *E_MSG_OPEN_SRC_FILE = "Failed to open src file";
static const char *E_MSG_OPEN_OUT_FILE = "Failed to open out file";
static const char *E_MSG_OPEN_ABC_FILE = "Failed to open abc file";

static const char *ABC = "abcdefghijklmnopqrstuvwxyz";

enum DIRECTION
{
    DIRECTION_UNKNOWN, DIRECTION_ENCODE, DIRECTION_DECODE
};

typedef struct _params_tag
{
    enum DIRECTION direction;
    int n;
    char *src;
    char *abc;
    char *out;
} parameters;

typedef char string[STR_LEN + 1];

static void exit_with_error(const char *msg);
static parameters get_parameters(int argc, char *argv[]);
static void read_string(parameters params, string *msg);
static void process_string(parameters params, string msg);

int main(int argc, char *argv[])
{
    parameters params = get_parameters(argc, argv);
    string msg;
    read_string(params, &msg);
    process_string(params, msg);
    return EXIT_SUCCESS;
}

static void exit_with_error(const char *msg)
{
    fputs(msg, stderr);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

static parameters get_parameters(int argc, char *argv[])
{
    if (argc < 3) exit_with_error(E_MSG_HELP);
    size_t i;
    parameters params = {DIRECTION_UNKNOWN, 0, NULL, NULL, NULL};
    for (i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-e") == 0)
        {
            if (params.direction == DIRECTION_UNKNOWN) params.direction = DIRECTION_ENCODE;
            else exit_with_error(E_MSG_HELP);
        }
        if (strcmp(argv[i], "-d") == 0)
        {
            if (params.direction == DIRECTION_UNKNOWN) params.direction = DIRECTION_DECODE;
            else exit_with_error(E_MSG_HELP);
        }
    }

    for (i = 1; i < argc - 1; ++i)
    {
        if (strcmp(argv[i], "-n") == 0) params.n = atoi(argv[i + 1]);
        if (strcmp(argv[i], "-abc") == 0) params.abc = argv[i + 1];
        if (strcmp(argv[i], "-src") == 0) params.src = argv[i + 1];
        if (strcmp(argv[i], "-out") == 0) params.out = argv[i + 1];
    }
    return params;
}

static void read_string(parameters params, string *msg)
{
    string m;
    if (params.src)
    {
        FILE *src_file = fopen(params.src, "rt");
        if (src_file)
        {
            fgets(&m[0], STR_LEN, src_file);
            fclose(src_file);
        } else exit_with_error(E_MSG_OPEN_SRC_FILE);
    } else fgets(&m[0], STR_LEN, stdin);
    size_t msg_len = strlen(&m[0]);
    m[msg_len - 1] = '\0';
    msg_len--;
    size_t i;
    for (i = 0; i < msg_len; ++i) m[i] = tolower(m[i]);
    strcpy(msg[0], &m[0]);
}

static void process_string(parameters params, string msg)
{
    string result_msg;
    string abc;

    int d = (params.direction == DIRECTION_ENCODE) ? 1 : -1;
    size_t msg_len = strlen(&msg[0]);
    if (params.abc)
    {
        FILE *abc_file = fopen(params.abc, "rt");
        if (abc_file)
        {
            fgets(&abc[0], STR_LEN, abc_file);
            fclose(abc_file);
            abc[strlen(&abc[0]) - 1] = '\0';
        } else exit_with_error(E_MSG_OPEN_ABC_FILE);
    } else strcpy(&abc[0], ABC);
    size_t abc_len = strlen(&abc[0]);
    size_t i;
    for (i = 0; i < msg_len; i++)
    {
        size_t j;
        for (j = 0; j < abc_len; ++j) if (abc[j] == msg[i]) break;
        result_msg[i] = abc[(j + params.n * d) % abc_len];
    }
    if (params.out)
    {
        FILE *out_file = fopen(params.out, "wt");
        if (out_file)
        {
            fputs(&result_msg[0], out_file);
            fclose(out_file);
        } else exit_with_error(E_MSG_OPEN_OUT_FILE);
    } else fputs(&result_msg[0], stdout);
}