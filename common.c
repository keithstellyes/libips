#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t *read_all_file(FILE *f, size_t *datalen_p)
{
    fseek(f, 0L, SEEK_END);
    size_t datalen = ftell(f);
    uint8_t *data = (uint8_t*)malloc(datalen);
    rewind(f);
    fread(data, datalen, 1, f);
    fclose(f);

    *datalen_p = datalen;
    return data;
}
