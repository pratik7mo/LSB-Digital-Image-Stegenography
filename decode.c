#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

//function defination for read and validate decoing arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(argv[2] != NULL)
    {
        char *temp= strstr(argv[2], ".bmp");
        if(temp == NULL)
        {
            return e_failure;
        }
        else
        {
            decInfo->stego_image_fname = argv[2];
        }
    }
    else
    {
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        decInfo->secret_fname = argv[3];
    }
    else
    {
        decInfo->secret_fname = "decoded_data.txt";
    }
    return e_success;
}

//decoding starts here//
//function defination for decoding screct message//
Status do_decoding(char *argv[],DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");
    if(decode_open_files(decInfo) == e_success )
    {
        printf("INFO: opened %s\n",decInfo->stego_image_fname);
    }
    else
    {
        printf("INFO: opening required file failed\n");
        return e_failure;
    }

    printf("INFO: Decoding magic string Signature\n");
    if(decode_magic_string(decInfo) == e_success)
    {
        printf("INFO: Done\n");
    }
    else
    {
        printf("INFO: Decoding magic string Signature failed\n");
        return e_failure;
    }
    if(decode_file_extn_size(decInfo->fptr_stego_image,decInfo) == e_failure)
    {
        printf("INFO: Decoding file extension size failed\n");
        return e_failure;
    }

    printf("INFO: Decoding output file Extension \n");
    if(decode_secret_file_extn(argv,decInfo->decode_extn_secret_file,decInfo) == e_failure)
    {
        printf("INFO: Decoding output file Extension failed \n");
        return e_failure;
    }

    printf("INFO: Decoding  %s file size\n",decInfo->secret_fname);
    if(decode_secret_file_size(decInfo->secret_file_size,decInfo ) ==e_success)
    {
        printf("INFO: Done\n");
    }
    else
    {
        printf("INFO: Decoding file size failed\n");
        return e_failure;
    }
    printf("INFO: Decoding %s file data \n",decInfo->secret_fname);
    if(decode_secret_file_data(decInfo) == e_success)
    {
        printf("INFO: Done\n");
    }
    else
    {
        printf("INFO: Decoding 'secret' file data failed \n");
        return e_failure;
    }
    return e_success;
}

//opening files and checking is opening file successful//
Status decode_open_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname,"r");

    if(decInfo->fptr_stego_image == NULL )
    {
        perror("fopen");
        fprintf(stderr,"ERRROR: Unable to open the file %s\n",decInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}

//function to decode magic string//
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image,54L, SEEK_SET );
    int i=strlen(MAGIC_STRING);

    decInfo->magic_data = malloc(strlen(MAGIC_STRING) + 1);

    decode_data_from_image(strlen(MAGIC_STRING),decInfo->fptr_stego_image,decInfo);

    return e_success;
}

//function to decode data from image//
Status decode_data_from_image(int size, FILE *fptr_stego_image,DecodeInfo *decInfo)
{
    char str[8];
    int i;
    for(i=0; i<size; i++)
    {
        fread(str,8,sizeof(char), fptr_stego_image);
        decode_byte_from_lsb(&decInfo->magic_data[i],str);
    }
    return e_success;
}

//function to decode charcter from lsb of stego image data//
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    unsigned char ch =0x00;
    for(int j=0;j<8;j++)
    {
        ch =((image_buffer[j] & 0x01) << 7-j) | ch;
    }
    *data =ch;
    return e_success;
}

//function to integer from lsb of Stego image data//
Status decode_int_from_lsb(char *buffer , int *size)
{
    int num =0x00;
    for(int j=0;j<32;j++)
    {
        num =((buffer[j] & 0x01) << 31-j) | num;
    }
    *size =num;
    return e_success;
}

//function to decode file extention size//
Status decode_file_extn_size(FILE *fptr_stego_image,DecodeInfo *decInfo)
{
    char str[32];
    decInfo->decoded_extn_size=0;

    fread(str,sizeof(char),32,fptr_stego_image);
    decode_int_from_lsb(str, &decInfo->decoded_extn_size);
    return e_success;
}

//function to decode file extention data from stego image//
Status decode_extn_data_from_image(int size, FILE *fptr_stego_image,DecodeInfo *decInfo)
{
    char str[8];
    int i;
    for(i=0; i<size; i++)
    {
        fread(str,8,sizeof(char), fptr_stego_image);
        decode_byte_from_lsb(&decInfo->decode_extn_secret_file[i],str);
    }
    return e_success;
}

//function to decode file extention data//
Status decode_secret_file_extn(char *argv[],char *file_ext,DecodeInfo *decInfo)
{
    int i=decInfo->decoded_extn_size;
    decInfo->decode_extn_secret_file=malloc(i+1);
    decode_extn_data_from_image(i,decInfo->fptr_stego_image,decInfo);

    printf("INFO: done\n");

    if(argv[3] == NULL)
    {
        printf("INFO: Output file not mentioned. ");
        switch(decInfo->decoded_extn_size)
        {
            case 2:
                decInfo->secret_fname = "decoded_data.c";
                break;
            case 3:
                decInfo->secret_fname = "decoded_data.sh";
                break;
            case 4:
                decInfo->secret_fname = "decoded_data.txt";
                break;
        }
        printf("Creating %s as default\n",decInfo->secret_fname);
    }
    else if(argv[3] != NULL)
    {
        char *str=decInfo->secret_fname;
        char *ret=strtok(str,".");
        switch(decInfo->decoded_extn_size)

        {
            case 2:
                strcat(ret , ".c");
                break;
            case 3:
                strcat(ret , ".sh");
                break;
            case 4:
                strcat(ret , ".txt");
                break;

        }
    }

    //opening  file to save secret message and checking it is successfull or not//
    decInfo->fptr_secret = fopen(decInfo->secret_fname,"w");

    if(decInfo->fptr_secret ==NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERRROR: Unable to open the file %s\n",decInfo->secret_fname);

        return e_failure;
    }

    printf("INFO: Opened %s \nINFO: Done. opened all required files\n",decInfo->secret_fname);
    return e_success;
}

// function defination for secret file size
Status decode_secret_file_size(int file_size, DecodeInfo *decInfo)
{
    char str[32];
    fread(str,sizeof(char),32,decInfo->fptr_stego_image);
    decode_int_from_lsb(str,&file_size);
    decInfo->secret_file_size = file_size;

    return e_success;
}

// function defination for screct file data//
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char str[decInfo->secret_file_size];

    for(int i=0;i<decInfo->secret_file_size;i++)
    {
        fread(decInfo->stego_image_fname,8,sizeof(char),decInfo->fptr_stego_image);
        decode_byte_from_lsb(str,decInfo->stego_image_fname);
        fwrite(str,sizeof(char),sizeof(char),decInfo -> fptr_secret);
    }
    return e_success;
}

