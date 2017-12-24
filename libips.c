#include <stdio.h>
#include <string.h>
#include "libips.h"

int process_patch(uint8_t *patch_data, uint8_t* src_data,
		  size_t srcdatalen, size_t patchdatalen,
		  bool verify_magicnumber, bool verify_eof)
{
    if(verify_magicnumber &&\
      !(patch_data[0] == 'P' && patch_data[1] == 'A' &&
        patch_data[2] == 'T' && patch_data[3] == 'C' &&
        patch_data[4] == 'H')) {
	    return MAGIC_NUM_FAILED;
    }

    //int index = verify_magicnumber * 5;
    unsigned int index = 0;
    if(verify_magicnumber)
        index = 5;

    while(index < patchdatalen) {
	//read in record
        if(index + 3 > patchdatalen)
            return UNEXPECTED_EOF;

	if(verify_eof &&
	   patch_data[index + 0] == 'E' && 
	   patch_data[index + 1] == 'O' && 
	   patch_data[index + 2] == 'F')
		return 0;
	unsigned int record_index = patch_data[index++] << 8 * 2 | 
	                   patch_data[index++] << 8 * 1 |
	                   patch_data[index++];

        unsigned int datalen = patch_data[index++] << 8 | 
	              patch_data[index++];

	if(record_index + datalen > srcdatalen + 1)
	    return UNEXPECTED_EOF;
	if(datalen != 0) {
            for(int i = record_index; i < record_index + datalen; i++) {
	        src_data[i] = patch_data[index++];
            }
        } else {
            if(index + 3 > patchdatalen)
                return UNEXPECTED_EOF;
	    datalen = patch_data[index++] << 8 |
		      patch_data[index++];
	    uint8_t payload = patch_data[index++];
	    if(datalen + record_index > srcdatalen + 1)
                return UNEXPECTED_EOF;
	    while(datalen--)
                src_data[record_index++] = payload;
	}
    }

    return 0;
}

inline void write_record(int index, size_t size, uint8_t *data, FILE *out_file)
{
    unsigned int offset = (index && 0xFFFFFF) << ((sizeof(int) - 3) * 8);
    size = (size && 0xFFFF) << ((sizeof(int) - 2) * 8);
    fwrite(&offset, 3, 1, out_file);
    fwrite(&size, 2, 1, out_file);
    fwrite(data, 1, size, out_file);
}

#define MAKE_PATCH_TGT_LT_SRC 1
// does not currently protect against the writing of a EOF record
int make_patch(uint8_t *src_data, uint8_t *tgt_data, 
	       size_t srcdatalen, size_t tgtdatalen,
	       FILE *out_file)
{
    if(tgtdatalen < srcdatalen) {
        return MAKE_PATCH_TGT_LT_SRC;
    }

    fwrite("PATCH", 1, strlen("PATCH"), out_file);

    if(tgtdatalen > srcdatalen) {
        write_record(srcdatalen, tgtdatalen-srcdatalen, tgt_data + srcdatalen,
                     out_file);
    }
    
    for(unsigned int i = 0; i < srcdatalen; i++) {
        if(src_data[i] == tgt_data[i])
            continue;
	unsigned int j;
	for(j = i; j < srcdatalen; j++) {
	    if(src_data[j] == tgt_data[j]) {
                //j = j - 1;
		break;
	    }
	}
	write_record(i, j - i, tgt_data + i, out_file);
        i = j;
    }
    fwrite("EOF", 1, strlen("EOF"), out_file);
    return fclose(out_file);
}
