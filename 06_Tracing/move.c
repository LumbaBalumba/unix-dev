#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 8192

void
usage(const char *progname)
{
    fprintf(stderr, "Usage: %s infile outfile\n", progname);
    exit(1);
}

int
main(int argc, char *argv[])
{
    int exit_code = 0;
    if (argc != 3) {
        usage(argv[0]);
    }

    const char *infile = argv[1];
    const char *outfile = argv[2];

    int fd_in = -1;
    int fd_out = -1;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    struct stat st;

    fd_in = open(infile, O_RDONLY);
    if (fd_in == -1) {
        fprintf(stderr, "Error opening input file '%s': %s\n", infile, strerror(errno));
        exit_code = 2;
        goto cleanup;
    }

    if (fstat(fd_in, &st) == -1) {
        fprintf(stderr, "Error getting file status for '%s': %s\n", infile, strerror(errno));
        exit_code = 3;
        goto cleanup;
    }

    fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, st.st_mode);
    if (fd_out == -1) {
        fprintf(stderr, "Error opening output file '%s': %s\n", outfile, strerror(errno));
        exit_code = 4;
        goto cleanup;
    }

    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        char *out_ptr = buffer;
        ssize_t bytes_left = bytes_read;

        while (bytes_left > 0) {
            bytes_written = write(fd_out, out_ptr, bytes_left);
            if (bytes_written == -1) {
                fprintf(stderr, "Error writing to output file '%s': %s\n", outfile, strerror(errno));
                exit_code = 5;
                goto cleanup;
            }
            bytes_left -= bytes_written;
            out_ptr += bytes_written;
        }
    }

    if (bytes_read == -1) {
        fprintf(stderr, "Error reading from input file '%s': %s\n", infile, strerror(errno));
        exit_code = 6;
        goto cleanup;
    }

    if (close(fd_in) == -1) {
        fprintf(stderr, "Error closing input file '%s': %s\n", infile, strerror(errno));
        exit_code = 7;
        fd_in = -1;
        goto cleanup;
    }
    fd_in = -1;

    if (close(fd_out) == -1) {
        fprintf(stderr, "Error closing output file '%s': %s\n", outfile, strerror(errno));
        exit_code = 8;
        fd_out = -1;
        goto cleanup;
    }
    fd_out = -1;

    if (unlink(infile) == -1 && errno != EPERM) {
        fprintf(stderr, "Error deleting input file '%s': %s\n", infile, strerror(errno));
        exit_code = 9;
        goto cleanup;
    }

    exit_code = 0;

cleanup:
    if (fd_in != -1) {
        close(fd_in);
    }
    if (fd_out != -1) {
        close(fd_out);
        if (exit_code != 0 && exit_code != 2) {
            if (unlink(outfile) == -1) {
                fprintf(stderr, "Error deleting output file '%s': %s\n", outfile, strerror(errno));
            }
        }
    }

    return exit_code;
}
