#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <time.h>

// Implement the given function (see the wiki) in the sysv_abi.S file

// Feel free to change this file however you want, it will not affect
// your result on the test system (e.g. you can add more test here)
ssize_t fsize(int fd, ssize_t *size);

#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
struct stat fstat_memory;
ssize_t fsize_ref(int fd, ssize_t *size)
{
    int err = fstat(fd, &fstat_memory);
    if (err)
    {
        return 1;
    }
    else
    {
        *size = fstat_memory.st_size;
        return 0;
    }
}

int main(int argc, char **argv)
{
    printf("Test start\n");

    {
        ssize_t size = 0;
        int fd = 10;
        ssize_t err = fsize(fd, &size);

        printf("Size of invalid file: %ld\n", size);
        assert(err == 1);
    }
    {
        ssize_t size = 0;
        int fd = stdin->_fileno;
        ssize_t err = fsize(fd, &size);

        printf("Size of file: %ld\n", size);
        assert(err == 0);
    }
    // Add more tests here to test your implementation...

    printf("Test exit\n");
    return 0;
}
