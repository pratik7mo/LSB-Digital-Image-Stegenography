/*
NAME    : Pratik Kumar Sahoo
DATE    : 01-03-2024
PROJECT : LSB Image Steganograpy
*/
#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /*source image information*/
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char image_data[MAX_IMAGE_BUF_SIZE];
    char *magic_data;

    char *decode_extn_secret_file;
    int   secret_file_size;
    int   decoded_extn_size;
    //    char *decode_extn[4];
    char *secret_fname;
    FILE *fptr_secret;


    FILE *fptr_dest_image;
} DecodeInfo;



/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status decode_open_files ( DecodeInfo *decInfo );

/* Perform the encoding */
Status do_decoding ( char *arhv[],DecodeInfo *decInfo );

/*Decoding the magic string*/
Status decode_magic_string ( DecodeInfo *decInfo );

/* decode function, which does the real encoding */
Status decode_data_from_image (int size, FILE *fptr_stego_image, DecodeInfo *decInfo);

/* Encode a byte into LSB of image data array */
Status decode_byte_from_lsb ( char *data, char *image_buffer );

/*Decode file extension size*/
Status decode_file_extn_size (  FILE *fptr_stego_image ,DecodeInfo *decInfo);

/*Decode size into LSB of image data array */
Status decode_size_from_lsb (char *buffer, int *size);

/* Decode secret file extenstion */
Status decode_secret_file_extn (char *argv[], char *file_ext, DecodeInfo *decInfo );

/* Decode function, which does the real encoding */
Status decode_extn_data_from_image ( int size, FILE *fptr_stego_image, DecodeInfo *decInfo );

/* Decode secret file size*/
Status decode_secret_file_size ( int file_size, DecodeInfo *decInfo );

/* Decode secret file data*/
Status decode_secret_file_data ( DecodeInfo *decInfo );

#endif
