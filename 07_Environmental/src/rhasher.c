#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <rhash.h>

#ifdef USE_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

int
main(void)
{
    rhash_library_init();
    int interactive = isatty(fileno(stdin));

    while (1) {
        char *line = NULL;
#ifdef USE_READLINE
        if (interactive) {
            line = readline("rhasher> ");
        } else {
            line = readline(NULL);
        }
        if (!line) break;
        if (*line && interactive) add_history(line);
#else
        size_t len = 0;
        if (interactive) {
            printf("rhasher> ");
            fflush(stdout);
        }
        if (getline(&line, &len, stdin) == -1) {
            if (line) free(line);
            break;
        }
        line[strcspn(line, "\n")] = 0;
#endif
        if (line[0] == '\0') {
            if (line) free(line);
            continue;
        }

        char *hash_name = strtok(line, " \t");
        if (!hash_name) {
            if (line) free(line);
            continue;
        }
        char *input = strtok(NULL, " \t");
        if (!input) {
            fprintf(stderr, "Ошибка: не указан вход\n");
            if (line) free(line);
            if (interactive) {
                continue;
            } else {
                return 1;
            }
        }

        int is_string = 0;
        if (input[0] == '"') {
            is_string = 1;
            input++;
            char *end_quote = strchr(input, '"');
            if (end_quote) {
                *end_quote = '\0';
            } else {
                fprintf(stderr, "Ошибка: не найдена закрывающая кавычка в строке\n");
                if (line) free(line);
                if (interactive) {
                    continue;
                } else {
                    return 1;
                }
            }
        }

        unsigned hash_id = 0;
        if (strcasecmp(hash_name, "MD5") == 0 || strcasecmp(hash_name, "md5") == 0) {
            hash_id = RHASH_MD5;
        } else if (strcasecmp(hash_name, "SHA1") == 0 || strcasecmp(hash_name, "sha1") == 0) {
            hash_id = RHASH_SHA1;
        } else if (strcasecmp(hash_name, "TTH") == 0 || strcasecmp(hash_name, "tth") == 0) {
            hash_id = RHASH_TTH;
        } else {
            fprintf(stderr, "Ошибка: неизвестный алгоритм хеширования '%s'\n", hash_name);
            if (line) free(line);
            if (interactive) {
                continue;
            } else {
                return 1;
            }
        }

        unsigned char digest[64];
        int res;

        if (is_string) {
            res = rhash_msg(hash_id, (unsigned char *) input, strlen(input), digest);
            if (res < 0) {
                fprintf(stderr, "Ошибка при вычислении хеша\n");
                if (line) free(line);
                if (interactive) {
                    continue;
                } else {
                    return 1;
                }
            }
        } else {
            res = rhash_file(hash_id, input, digest);
            if (res < 0) {
                fprintf(stderr, "Ошибка: не удалось открыть файл '%s'\n", input);
                if (line) free(line);
                if (interactive) {
                    continue;
                } else {
                    return 1;
                }
            }
        }

        char output[130];

        if (isupper(hash_name[0])) {
            rhash_print_bytes(output, digest, rhash_get_digest_size(hash_id), RHPR_HEX);
        } else {
            rhash_print_bytes(output, digest, rhash_get_digest_size(hash_id), RHPR_BASE64);
        }

        for (int i = 0; i < 130; ++i) {
            if (isalpha(output[i]) && !isupper(output[i])) {
                output[i] = toupper(output[i]);
            }
        }

        printf("%s\n", output);

        if (line) free(line);
    }

    return 0;
}
