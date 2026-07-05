#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"
#include "types.h"


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    decInfo->stego_image_fname = argv[2];
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

    if (decInfo->fptr_stego_image == NULL)
    {
        printf("Error: Stego image file not found.\n");
        return e_failure;
    }

    if (argv[3] != NULL)
    {
        // Copy the base name (remove .bmp using strtok)
        char *dot = strtok(argv[3], ".");  // Get 'hai' from 'hai.bmp'.
        static char final_name[100];       // Static to persist after function returns.

        strcpy(final_name, dot);           // Copy 'hai' into final_name.
        strcat(final_name, ".txt");        // Append .txt => 'hai.txt'.

        decInfo->output_fname = final_name;
    }
    else
    {
        decInfo->output_fname = "output.txt";
        printf("No output filename provided. Using default: output.txt\n");  // <-- Added line
    }

    return e_success;
}


Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    if (decInfo->fptr_output == NULL)
    {
        printf("Error opening output file\n");
        return e_failure;
    }

    return e_success;
}


Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) != e_success)
        return e_failure;

    fseek(decInfo->fptr_stego_image, 54, SEEK_SET); // Skip BMP header.

    int magic_size = decode_size_from_lsb(decInfo->fptr_stego_image);

    char *magic_string = malloc(magic_size + 1);
    if (magic_string == NULL)
    {
        printf("Memory allocation failed for magic string\n");
        return e_failure;
    }

    decode_magic_string(magic_string, magic_size, decInfo);
    magic_string[magic_size] = '\0';

    // Ask user to input expected magic string.
    char user_input[100];
    printf("Enter expected magic string to validate: ");
    scanf("%s", user_input);

    // Compare with decoded magic string.
    if (strcmp(user_input, magic_string) != 0)
    {
        printf("Error: Magic string mismatch! Decoding aborted.\n");
        free(magic_string);
        return e_failure;
    }

    printf("Magic string matched: %s\n", magic_string);

    int extn_size = decode_size_from_lsb(decInfo->fptr_stego_image);
    char *extension = malloc(extn_size + 1);
    if (extension == NULL)
    {
        printf("Memory allocation failed for extension\n");
        free(magic_string);
        return e_failure;
    }

    decode_secret_file_extn(extension, extn_size, decInfo);
    extension[extn_size] = '\0';
    printf("Extension: %s\n", extension);

    decInfo->size_secret_file = decode_size_from_lsb(decInfo->fptr_stego_image);
    printf("Secret file size: %ld\n", decInfo->size_secret_file);

    decode_secret_file_data(decInfo, decInfo->size_secret_file);

    free(magic_string);
    free(extension);

    return e_success;
}


int decode_size_from_lsb(FILE *fptr)
{
    int size = 0;
    char ch;
    for (int i = 0; i < 32; i++)
    {
        fread(&ch, 1, 1, fptr);
        size = (size << 1) | (ch & 1);
    }
    return size;
}

char decode_byte_from_lsb(char *buffer)
{
    char data = 0;
    for (int i = 0; i < 8; i++)
    {
        data = (data << 1) | (buffer[i] & 1);
    }
    return data;
}

Status decode_magic_string(char *magic_str, int size, DecodeInfo *decInfo)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(buffer, sizeof(char), 8, decInfo->fptr_stego_image);
        magic_str[i] = decode_byte_from_lsb(buffer);
    }
    return e_success;
}

Status decode_secret_file_extn(char *extn, int size, DecodeInfo *decInfo)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(buffer, sizeof(char), 8, decInfo->fptr_stego_image);
        extn[i] = decode_byte_from_lsb(buffer);
    }
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo, int size)
{
    char buffer[8];
    char ch;

    for (int i = 0; i < size; i++)
    {
        fread(buffer, sizeof(char), 8, decInfo->fptr_stego_image);
        ch = decode_byte_from_lsb(buffer);
        fwrite(&ch, sizeof(char), 1, decInfo->fptr_output);
    }

    return e_success;
}
