#ifndef LIB_IPS_H
#define LIB_IPS_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
/*
 * Returns one-of:
 * 0: success
 * 1: magic-number failed
 * 2: EOF failed
 * 3: Insufficent buffer length
 */
#define PROCESS_PATCH_SUCCESS 0
#define MAGIC_NUM_FAILED 1
#define EOF_FAILED 2
#define INSUFFICENT_BUFFER_LENGTH 3
#define UNEXPECTED_EOF 4
int process_patch(uint8_t *patch_data, uint8_t* src_data,
                  size_t srcdatalen, size_t patchdatalen,
                  bool verify_magicnumber, bool verify_eof);

int make_patch(uint8_t *src_data, uint8_t *tgt_data,
	       size_t srcdatalen, size_t tgtdatalen,
	       FILE *out_file);
#endif /* LIB_IPS_H */
