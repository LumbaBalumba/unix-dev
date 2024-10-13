#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
process_substitution(const char *substitution, const char *input_string, regmatch_t *pmatch, size_t nmatch, size_t nsub, int *error)
{
    size_t bufsize = 1024;
    char *result = malloc(bufsize);
    if (!result) {
        fprintf(stderr, "Memory allocation error\n");
        *error = 1;
        return NULL;
    }
    size_t result_len = 0;

    const char *p = substitution;

    while (*p) {
        if (*p == '\\') {
            p++;
            if (*p >= '1' && *p <= '9') {
                int group_num = *p - '0';
                if ((size_t) group_num <= nsub && pmatch[group_num].rm_so != -1) {
                    size_t len = pmatch[group_num].rm_eo - pmatch[group_num].rm_so;
                    while (result_len + len + 1 > bufsize) {
                        bufsize *= 2;
                        result = realloc(result, bufsize);
                        if (!result) {
                            fprintf(stderr, "Memory allocation error\n");
                            *error = 1;
                            return NULL;
                        }
                    }
                    strncpy(result + result_len, input_string + pmatch[group_num].rm_so, len);
                    result_len += len;
                    result[result_len] = '\0';
                } else {
                    fprintf(stderr, "Invalid backreference: \\%d\n", group_num);
                    *error = 1;
                    free(result);
                    return NULL;
                }
                p++;
            } else if (*p == '\\') {
                if (result_len + 1 >= bufsize) {
                    bufsize *= 2;
                    result = realloc(result, bufsize);
                    if (!result) {
                        fprintf(stderr, "Memory allocation error\n");
                        *error = 1;
                        return NULL;
                    }
                }
                result[result_len++] = '\\';
                result[result_len] = '\0';
                p++;
            } else {
                if (result_len + 1 >= bufsize) {
                    bufsize *= 2;
                    result = realloc(result, bufsize);
                    if (!result) {
                        fprintf(stderr, "Memory allocation error\n");
                        *error = 1;
                        return NULL;
                    }
                }
                result[result_len++] = *p;
                result[result_len] = '\0';
                p++;
            }
        } else {
            if (result_len + 1 >= bufsize) {
                bufsize *= 2;
                result = realloc(result, bufsize);
                if (!result) {
                    fprintf(stderr, "Memory allocation error\n");
                    *error = 1;
                    return NULL;
                }
            }
            result[result_len++] = *p;
            result[result_len] = '\0';
            p++;
        }
    }

    return result;
}

int
main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: esub regexp substitution string\n");
        return 1;
    }

    char *pattern = argv[1];
    char *substitution = argv[2];
    char *input_string = argv[3];

    regex_t regex;
    int reti;

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        char errbuf[1024];
        regerror(reti, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Error compiling regex: %s\n", errbuf);
        return 1;
    }

    size_t nmatch = regex.re_nsub + 1;
    regmatch_t pmatch[nmatch];
    reti = regexec(&regex, input_string, nmatch, pmatch, 0);
    if (!reti) {
        int error = 0;
        char *substituted = process_substitution(substitution, input_string, pmatch, nmatch, regex.re_nsub, &error);
        if (error) {
            regfree(&regex);
            return 1;
        }

        fwrite(input_string, 1, pmatch[0].rm_so, stdout);

        fputs(substituted, stdout);

        fputs(input_string + pmatch[0].rm_eo, stdout);
        fputs("\n", stdout);

        free(substituted);
    } else if (reti == REG_NOMATCH) {
        printf("%s\n", input_string);
    } else {
        char errbuf[1024];
        regerror(reti, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Regex match failed: %s\n", errbuf);
        regfree(&regex);
        return 1;
    }

    regfree(&regex);
    return 0;
}
