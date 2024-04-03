#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "type.h"
#include "view.h"

Status open_file(ViewInfo *viInfo)
{
    // Src Image file
    viInfo->audio_fptr = fopen(viInfo->audio_file_name, "r");
    // Do Error handling
    if (viInfo->audio_file_name == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", viInfo->audio_file_name);

	return e_failure;
    }
    return e_success;

}
Status validation_part(char *argv[],ViewInfo *viInfo)
{
    if(argv[2] != NULL)
    {
	if(strchr(argv[2],'.') != NULL)
	{
	    if(strcmp(strchr(argv[2],'.'),".mpeg") == 0 || strcmp(strchr(argv[2],'.'),".mp3") == 0)
	    {
		viInfo->audio_file_name = argv[2];
	    }
	    else
	    {
		printf("Error: pass proper extension .mpeg\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Error: pass proper extension with .\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Error: argument 2 is not passed\n");
	return e_failure;
    }
    return e_success;
}

Status do_viewing(ViewInfo *viInfo)
{
    if(open_file(viInfo) == e_success)
    {
	if(check_header_tag(viInfo->audio_fptr,viInfo) == e_success)
	{
	    for(int i=0; i<=100; i++)
	    {
		if(i < 100)
		    printf("\r\033[0;31mDisplaying[%d%%]",i);
		else
		    printf("\r\033[0;32mDisplaying[%d%%]",i);
		usleep(5000);
		fflush(stdout);
	    }
	    printf("\n");
	    printf("\033[0m"); // Reset color and print percentage

	    printf("------------------------------SELECTED VIEW DETAILS--------------------------------\n");
	    printf("-----------------------MP3 TAG READER AND EDITOR FOR ID3V2-------------------------\n");

	    while(1)
	    {
		char tag[4];
		fread(tag,4,1,viInfo->audio_fptr);
		if(strcmp(tag,"TIT2") == 0)
		{
		    check_title_tag(viInfo->audio_fptr,viInfo);
		    printf("Title\t\t\t:\t\t%s\n",viInfo->title_name);
		}
		else if(strcmp(tag,"TPE1") == 0)
		{
		    check_title_tag(viInfo->audio_fptr,viInfo);
		    printf("Artist\t\t\t:\t\t%s\n",viInfo->title_name);
		}
		else if(strcmp(tag,"TALB") == 0)
		{
		    check_title_tag(viInfo->audio_fptr,viInfo);
		    printf("Album\t\t\t:\t\t%s\n",viInfo->title_name);
		}
		else if(strcmp(tag,"TYER") == 0)
		{
		    check_title_tag(viInfo->audio_fptr,viInfo);
		    printf("Year\t\t\t:\t\t%s\n",viInfo->title_name);
		}
		else if(strcmp(tag,"TCON") == 0)
		{
		    check_title_tag(viInfo->audio_fptr,viInfo);
		    printf("Content\t\t\t:\t\t%s\n",viInfo->title_name);
		}
		else if(strcmp(tag,"COMM") == 0)
		{
		    check_title_tag(viInfo->audio_fptr,viInfo);
		    printf("Comment\t\t\t:\t\t%s\n",viInfo->title_name);
		    break;
		}
		else
		{
		    //printf("non tag %s\n",tag);
		    check_title_tag(viInfo->audio_fptr,viInfo);
		    //fseek(viInfo->audio_fptr,viInfo->title_size,SEEK_CUR);
		}
	    }
	    return e_success;
	}
	else
	{
	    printf("Header tag not qualified\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Error: file openening failed\n");
	return e_failure;
    }
}

Status check_header_tag(FILE *audio_fptr,ViewInfo *viInfo)
{
    char buffer[3];
    char buff[2];
    fread(buffer,3,1,audio_fptr);
    fread(buff,2,1,audio_fptr);
    if(strcmp(buffer,"ID3") == 0)
    {
	printf("INFO : header tag qualified\n");
	if(buff[0] == 0x03 && buff[1] == 0x00)
	{
	    printf("INFO : Hexa val is same\n");
	    rewind(audio_fptr);
	    fseek(audio_fptr,10,SEEK_SET);
	}
	else
	{
	    printf("Error:Hexa val is not same\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Error : header tag not matched\n");
	return e_failure;
    }
    return e_success;
}

void check_title_tag(FILE *audio_fptr,ViewInfo *viInfo)
{
    char size[4];
    fread(size,4,1,audio_fptr);
    viInfo->title_size = extract_size(size);
    char comm_name[viInfo->title_size];
    name(viInfo->title_size,comm_name,viInfo);
    strcpy(viInfo->title_name, comm_name);
}

long extract_size(char *buf)
{
    long size = 0;
    for(int i=0; i<4; i++)
    {
	size = (buf[i] << (24-(8*i))) | size;
    }
    return size;
}

void name(long size,char *comm_name,ViewInfo *viInfo)
{
    long s = size-1;
    char buffer[s];
    int j=0;
    fseek(viInfo->audio_fptr,3,SEEK_CUR);
    fread(buffer,s,1,viInfo->audio_fptr);
    for(int i=0; i<s; i++)
    {
	if(buffer[i] != '\0')
	{
	    comm_name[j] = buffer[i];
	    j++;
	}
    }
    comm_name[s] = '\0';
}

