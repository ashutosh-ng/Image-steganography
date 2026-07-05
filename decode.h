// decode.h
#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_FILE_SUFFIX 4

// Structure for decoding
typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char extn_secret_file[MAX_FILE_SUFFIX];
    char *output_fname;
    FILE *fptr_output;

    long size_secret_file;
} DecodeInfo;

// Function declarations
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);
Status open_decode_files(DecodeInfo *decInfo);
int decode_size_from_lsb(FILE *fptr);
char decode_byte_from_lsb(char *buffer);
Status decode_magic_string(char *magic_str, int size, DecodeInfo *decInfo);
Status decode_secret_file_extn(char *extn, int size, DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo, int size);

#endif