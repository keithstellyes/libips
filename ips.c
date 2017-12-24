#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "libips.h"
#include "common.h"

int main(int argc, char **argv)
{
    if(argc != 3 && argc != 4) {
        printf("USAGE: <DATA FILE> <PATCH FILE>\n");
        return -1;
    }
    char *out_filename = "patched";
    if(argc == 4)
        out_filename = argv[3];
    printf("Reading %s as the source data file\n", argv[1]);
    FILE *src_file = fopen(argv[1], "rb");
    size_t srcdatalen;
    uint8_t *src_data = read_all_file(src_file, &srcdatalen);

    printf("Reading %s as the patch data file\n", argv[2]);
    FILE *patch_file = fopen(argv[2], "rb");
    size_t patchdatalen;
    uint8_t *patch_data = read_all_file(patch_file, &patchdatalen);

    int result = process_patch(patch_data, src_data, srcdatalen, patchdatalen,
                  true, true);
    printf("Writing %s as the output\n", out_filename);
    FILE *out_file = fopen(out_filename, "wb");
    fwrite(src_data, srcdatalen, 1, out_file);
    
    return result;
}
