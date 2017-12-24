#include <stdio.h>
#include <stdint.h>

#include "libips.h"
#include "common.h"

int main(int argc, char **argv)
{
    if(argc != 4) {
        printf("USAGE:\n");
	printf("<SOURCE> <TARGET> <PATCH FILE>\n");
	return -1;
    }
    FILE *source_file = fopen(argv[1], "rb");
    FILE *target_file = fopen(argv[2], "rb");
    FILE *out_file = fopen(argv[3], "wb");

    size_t srcdatalen, tgtdatalen;

    uint8_t *src_data = read_all_file(source_file, &srcdatalen);
    uint8_t *tgt_data = read_all_file(target_file, &tgtdatalen);
    make_patch(src_data, tgt_data, srcdatalen, tgtdatalen, out_file);  
}
