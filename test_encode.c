#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

// ./a.out -e beautiful.bmp secret.txt stego.bmp
// ./a.out -d stego.bmp output.txt

int main(int argc,char *argv[])
{
    if (argc < 3 || (strcmp(argv[1], "-e") == 0 && argc < 4)) 
    {
        printf("Arguments are not correct:\n");
        return e_failure;
    }

    if(check_operation_type(argv) == e_encode)
    {
        // printf("Choosed encoding\n");
        EncodeInfo encode;
        if(read_and_validate_encode_args(argv, &encode)==e_success)
        {
            printf("Read and validation is succesfull\n");
        }
        else
        {
            printf("Read and validation is not successfull\n");
            return e_failure;
        }

        printf("<----------------Encoding Started------------------>\n");

        if(do_encoding(&encode) == e_success)
        {
            printf("Encoding successfull\n");
        }
        else
        {
            printf("Encoding is unsuccessfull\n");
            return e_failure;
        }
        return e_success;
    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("Choosed decoding\n");

        DecodeInfo decode;

        if (read_and_validate_decode_args(argv, &decode) == e_success)
        {
            printf("Read and validation for decoding successful\n");
        }
        else
        {
            printf("Read and validation for decoding failed\n");
            return e_failure;
        }

        printf("<----------------Decoding Started------------------>\n");

        if (do_decoding(&decode) == e_success)
        {
            printf("Decoding successfull\n");
        }
        else
        {
            printf("Decoding is unsuccessfull\n");
            return e_failure;
        }

        return e_success;
    }
    else 
    {
        printf("pass valid arguments\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For decoding: ./a.out -d stego.bmp \n");
        return e_failure;
    }

}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
    {
        printf("You have choosed encoding\n");
        return e_encode;
    }
    else if(strcmp(argv[1],"-d") == 0)
    {
        printf("You have choosed decoding\n");
        return e_decode;
    }
    else 
    {
        printf("Invalid argument\n");
        return e_unsupported;
    }

    return e_success;
}
