/*
NAME: BHARATHKUMAR K
DATE: 2/4/2024
DESCRIPTION: MP3 tag reader
SAMPLE INPUT:
SAMPLE OUTPUT:
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "view.h"
#include "edit.h"
#include "type.h"

int main(int argc, char *argv[])
{
    int ret;
    ViewInfo viInfo;
    EditInfo edInfo;

    if(argv[1] != NULL)
    {
	ret = chech_option(argv[1]);
	if(ret == edit)
	{
	    printf("INFO : Selected edit option\n");
	    if(edit_validate(argv,&edInfo) == e_success)
	    {
		printf("INFO : Editing started\n");
		if(do_editing(&edInfo) == e_success)
		{
		    printf("INFO : Editing completed\n");
		}
		else
		{
		    printf("Error : Editing failed\n");
		}
	    }
	    else
	    {
		printf("Error : Editing failed\n");
	    }
	}
	else if(ret == view)
	{
	    printf("INFO : Selected view option\n");
	    if(validation_part(argv,&viInfo) == e_success)
	    {
		printf("INFO : Validation is done\n");
		if(do_viewing(&viInfo) == e_success)
		{
		    //display(&viInfo);
		    printf("INFO : Viewing is done\n");
		}
		else
		{
		    printf("Error: viewing is not done\n");
		}

	    }
	    else
	    {
		printf("Error: validation part failed\n");
	    }
	}
	else if(ret == help)
	{
	    printf("Selected help option\n");
	    printf("usage:\n");
	    printf("for viewing: ./a.out -v filename.mpeg\n");
	    printf("for editing: ./a.out -e <option> filename.mpeg\n");
	    printf("-t : to modify title\n");
	    printf("-a : to modify artist\n");
	    printf("-A : to modify album\n");
	    printf("-y : to modify year\n");
	    printf("-g : to modify content type\n");
	    printf("-c : to modify comment\n");
	}
	else
	{
	    printf("Error: unsupported option\ntype ./a.out --help to get help\n");
	}
    }
    else
    {
	printf("Error: pass -v for view and -e to edit\n");
    }


    return 0;
}
OperationType chech_option(char *argv)
{
    if(strcmp(argv,"-e") == 0)
    {
	return edit;
    }
    else if(strcmp(argv,"-v") == 0)
    {
	return view;
    }
    else if(strcmp(argv,"--help") == 0)
    {
	printf("help = %d\n",help);
	return help;
    }
    else
    {
	return unsupported;
    }
}
