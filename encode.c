#include <stdio.h>
#include "encode.h"
#include "types.h"

// Validate and assign input arguments to EncodeInfo structure.
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("Source image file must be a .bmp file\n");
        return e_failure;
    }

    if (strstr(argv[3], ".txt") != NULL)
    {
        encInfo->secret_fname = argv[3];
    }
    else
    {
        printf("Secret file must be a .txt file\n");
        return e_failure;
    }

    
    if (argv[4] != NULL)
    {
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";  // Default value
        printf("No output stego image given, defaulting to 'stego.bmp'\n");
    }

    return e_success;
}
     
// Perform encoding process using EncodeInfo
Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_success)
    {
        printf("Files are opened successfully\n");
    }

    else
    {
        printf("Files are not opened successfully\n");
        return e_failure;
    }

    printf("Enter magic string: ");
   scanf("%s", encInfo->magic_string);


     // Check if source image has enough capacity
    if(check_capacity(encInfo) == e_success)
    {
        printf("Capacity checked it is correct\n");
    }

    else
    {
        printf("Out of capacity\n");
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }

    // Copy BMP header from source to stego image
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
        printf("Header bytes are copied\n");
    }

    else
    {
        printf("Header bytes are not copied\n");
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }

    // Encode magic string size
    if (encode_magic_string_size(strlen(encInfo->magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }
    

    // Encode magic string
    if (encode_magic_string(encInfo->magic_string, encInfo) != e_success)
    {
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }


    // Get extension (e.g., .txt)
    char *ext = strrchr(encInfo->secret_fname, '.');
    if (ext == NULL)
    {
        printf("No extension found in secret file\n");
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }

    strncpy(encInfo->extn_secret_file, ext, MAX_FILE_SUFFIX);

    // Encode extension size
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }

    // Encode extension
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) != e_success)
    {
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }

    // Encode secret file size
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) != e_success)
    {
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }

    // Encode secret file data
    if (encode_secret_file_data(encInfo) != e_success)
    {
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }

    // Copy remaining image data
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;

    }
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    return e_success;
    
}

Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname,"r");

    if(encInfo->fptr_src_image ==NULL)
    {
        printf("Source file is not present\n");
        return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname,"r");

    if(encInfo->fptr_secret == NULL)
    {
        printf("Secret file is not present\n");
        return e_failure;
    }

    encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"w");

    return e_success;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width,height;

    fseek(fptr_image,18,SEEK_SET);
    printf("Now pointer is at %lu\n",ftell(fptr_image));

    fread(&width,4,1,fptr_image);

    printf("Now pointer is at %lu\n",ftell(fptr_image));

    fread(&height,4,1,fptr_image);

    return width * height * 3;

}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);

    return ftell(fptr);
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    printf("Source image is %u\n",encInfo->image_capacity);

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    printf("Secret image is %lu\n",encInfo->size_secret_file);

    if(encInfo->image_capacity > strlen(encInfo->magic_string) * 8 + 32 + 32 + 32 + encInfo->size_secret_file * 8)
    {
        printf("Source file has sufficient size\n");
    }

    else
    {
        printf("Source file has not sufficient size\n");
        return e_failure;
    }

    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    fseek(fptr_src_image,0,SEEK_SET);
    fseek(fptr_dest_image,0,SEEK_SET);

    char buffer[54];
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);

    return e_success;

} 
Status encode_size_to_lsb(int size, char *buffer)
{
    for (int i = 0; i < 32; i++)
    {
        buffer[i] &= 0xFE;  // Clear LSB
        int bit = (size >> (31 - i)) & 1;  // Get i-th bit
        buffer[i] |= bit;  // Set LSB
    }
    return e_success;
}


Status encode_magic_string_size(int size, FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[32];

    // Read 32 bytes from source image
    fread(buffer, sizeof(char), 32, fptr_src);

    // Call the helper to encode size into buffer
    encode_size_to_lsb(size, buffer);

    // Write modified buffer to stego image
    fwrite(buffer, sizeof(char), 32, fptr_dest);

    return e_success;
}
Status encode_byte_to_lsb(char data, char *buffer)
{
    for (int i = 0; i < 8; i++)
    {
        buffer[i] &= 0xFE; // Clear LSB
        int bit = (data >> (7 - i)) & 1; // Get bit
        buffer[i] |= bit; // Set LSB
    }

    
    return e_success;
}


Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];
    int length = strlen(magic_string);

    for (int i = 0; i < length; i++)
    {
        fread(buffer, sizeof(char), 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i], buffer);
        fwrite(buffer, sizeof(char), 8, encInfo->fptr_stego_image);
    }

    return e_success;
}
Status encode_secret_file_extn_size(int size, FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[32];

    // Read 32 bytes from source image
    fread(buffer, sizeof(char), 32, fptr_src);

    // Encode size using helper
    encode_size_to_lsb(size, buffer);

    // Write 32 bytes to stego image
    fwrite(buffer, sizeof(char), 32, fptr_dest);

    return e_success;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];
    int length = strlen(file_extn);

    for (int i = 0; i < length; i++)
    {
        // Step 1: Read 8 bytes from source image
        fread(buffer, sizeof(char), 8, encInfo->fptr_src_image);

        // Step 2: Encode extension character into LSBs
        encode_byte_to_lsb(file_extn[i], buffer);

        // Step 3: Write modified 8 bytes to stego image
        fwrite(buffer, sizeof(char), 8, encInfo->fptr_stego_image);
    }

    return e_success;
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];

    // Step 1: Read 32 bytes from source image
    fread(buffer, sizeof(char), 32, encInfo->fptr_src_image);

    // Step 2: Encode file size to LSBs of buffer
    encode_size_to_lsb((int)file_size, buffer);

    // Step 3: Write modified buffer to stego image
    fwrite(buffer, sizeof(char), 32, encInfo->fptr_stego_image);

    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[8];
    char ch;

    // Set file pointer to beginning of secret file
    fseek(encInfo->fptr_secret, 0, SEEK_SET);

    // Loop through secret file byte by byte
    while (fread(&ch, sizeof(char), 1, encInfo->fptr_secret) == 1)
    {
        // Step 1: Read 8 bytes from source image
        fread(buffer, sizeof(char), 8, encInfo->fptr_src_image);

        // Step 2: Encode the character
        encode_byte_to_lsb(ch, buffer);

        // Step 3: Write 8 modified bytes to stego image
        fwrite(buffer, sizeof(char), 8, encInfo->fptr_stego_image);
    }

    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;

    // Copy remaining bytes until end of source image
    while (fread(&ch, 1, 1, fptr_src) == 1)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }

    return e_success;
}