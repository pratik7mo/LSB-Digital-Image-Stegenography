#include<stdio.h>
#include<string.h>
#include"encode.h"
#include"types.h"
int main(int argc, char *argv[])
{
	if(argc<3)
	{
		printf("Lesser number of arguments\n");
	}						
	else
	{
		//chrck if -e is passed
		if(check_operation_type(argv)==e_encode)
		{
			printf("Selected encoding\n");
		    EncodeInfo e;
			if(read_and_validate_encode_args(argv, &e)==e_success)
			{
				printf("Reading and validating the arguments is a success\n");	
				printf("-----------Started Encoding---------");
				if(do_encoding(&e)==e_success)
				{
					printf("Encoding is successful\n");
				}	
				else
				{
					printf("Failed to perform encoding\n");
				}
			}
			else
			{
					printf("Failed to read and validate the encoding arguments\n");
			}
		}
		else if(check_operation_type(argv)==e_decode)
		{
			printf("Selected decoding\n");
		}
		else
		{
			printf("Invalid option.please pass\nFor encoding: ./a.out -e beautiful.bmp secret.txt[stego.bmp] \n for decoding: ./a.out -d stego.bmp[decode.t]");
		}
	}
	return 0;
}
OperationType check_operation_type(char *argv[])
{
	if(strcmp(argv[1],"-e")==0)
	return e_encode;
	else if(strcmp(argv[1],"-d")==0)
	return e_decode;
	else
	return e_unsupported;
}

