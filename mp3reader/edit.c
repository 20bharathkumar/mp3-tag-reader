#include <stdio.h>
#include <string.h>
#include "edit.h"
#include "type.h"


Status open_edit_file(EditInfo *edInfo)
{
    // Src Image file
    edInfo->backup_audio_fptr = fopen(edInfo->backup_audio_fname, "r+");

    // Do Error handling
    if (edInfo->backup_audio_fname == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", edInfo->backup_audio_fname);

	return e_failure;
    }
    return e_success;

}

Status edit_validate(char *argv[], EditInfo *edInfo)
{
    if(argv[2] != NULL)
    {
	if(strcmp(argv[2],"-t") == 0 || strcmp(argv[2],"-a") == 0 || strcmp(argv[2],"-A") == 0 || strcmp(argv[2],"-y") == 0 || strcmp(argv[2],"-g") == 0 || strcmp(argv[2],"-c") == 0)
	{
	    edInfo->edit_option = argv[2];
	}
	else
	{
	    printf("Error: Invalid option\n");
	    printf("Error: pass ./a.out --help to get help\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Error: pass ./a.out --help to get help\n");
	return e_failure;
    }

    if(argv[3] != NULL)
    {
	edInfo->edit_str = argv[3];
    }
    else
    {
	printf("Error: pass ./a.out --help to get help\n");
	return e_failure;
    }

    if(argv[4] != NULL)
    {
	if(strchr(argv[4],'.') != NULL)
	{
	    if(strcmp(strchr(argv[4],'.'),".mpeg") == 0 || strcmp(strchr(argv[4],'.'),".mp3") == 0)
	    {
		edInfo->backup_audio_fname = argv[4];
		if(strcmp(strchr(argv[4],'.'),".mpeg") == 0)
		{
		    edInfo->dest_audio_fname = "lol.mpeg";
		    edInfo->dest_audio_fptr = fopen(edInfo->dest_audio_fname, "w+");

		}
		else
		{
		    edInfo->dest_audio_fname = "lol.mp3";
		    edInfo->dest_audio_fptr = fopen(edInfo->dest_audio_fname, "w+");
		}
	    }
	    else
	    {
		printf("Error : pass file name with .mpeg extension\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Error : there is no extension\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Error: file name not passed\n");
	return e_failure;
    }
    return e_success;

}

Status do_editing(EditInfo *edInfo)
{
    if(open_edit_file(edInfo) == e_success)
    {
	char head_tag[3];
	fread(head_tag,3,1,edInfo->backup_audio_fptr);
	head_tag[3] = '\0';
	if(strcmp(head_tag,"ID3") == 0)
	{
	    char vers[2];
	    fread(vers,2,1,edInfo->backup_audio_fptr);
	    if(vers[0] == 0x03 && vers[1] == 0x00)
	    {
		rewind(edInfo->backup_audio_fptr);
		char head[10];
		fread(head,10,1,edInfo->backup_audio_fptr);
		fwrite(head,10,1,edInfo->dest_audio_fptr);
		while(1)
		{
		    char tag[4];
		    fread(tag,4,1,edInfo->backup_audio_fptr);
		    tag[4] = '\0';
		    if(strcmp(edInfo->edit_option,"-t") == 0 && strcmp(tag,"TIT2") == 0)
		    {
			fseek(edInfo->backup_audio_fptr,-4,SEEK_CUR);
			printf("INFO : Selected edit title option\n");
			if(edit_data(edInfo) == e_success)
			{
			    printf("INFO : Title name changed\n");
			    return e_success;
			}
			else
			{
			    printf("Error: Title name not changed\n");
			    return e_failure;
			}

			break;
		    }
		    else if(strcmp(edInfo->edit_option,"-a") == 0 && strcmp(tag,"TPE1") == 0)
		    {
			printf("INFO : Selected edit artist option\n");
			fseek(edInfo->backup_audio_fptr,-4,SEEK_CUR);
			if(edit_data(edInfo) == e_success)
			{
			    printf("INFO : Artist name changed\n");
			    return e_success;
			}
			else
			{
			    printf("Error: Artist name not changed\n");
			    return e_failure;
			}
			break;
		    }
		    else if(strcmp(edInfo->edit_option,"-A") == 0 && strcmp(tag,"TALB") == 0)
		    {
			printf("INFO : Selected edit album option\n");
			fseek(edInfo->backup_audio_fptr,-4,SEEK_CUR);
			if(edit_data(edInfo) == e_success)
			{
			    printf("INFO : Album name changed\n");
			    return e_success;
			}
			else
			{
			    printf("Error: Album name not changed\n");
			    return e_failure;
			}
			break;
		    }
		    else if(strcmp(edInfo->edit_option,"-y") == 0 && strcmp(tag,"TYER") == 0)
		    {
			printf("INFO : Selected edit year option\n");
			fseek(edInfo->backup_audio_fptr,-4,SEEK_CUR);
			if(edit_data(edInfo) == e_success)
			{
			    printf("INFO : Year changed\n");
			    return e_success;
			}
			else
			{
			    printf("Error: Year not changed\n");
			    return e_failure;
			}
			break;
		    }
		    else if(strcmp(edInfo->edit_option,"-g") == 0 && strcmp(tag,"TCON") == 0)
		    {
			printf("INFO : Selected edit content type option\n");
			fseek(edInfo->backup_audio_fptr,-4,SEEK_CUR);
			if(edit_data(edInfo) == e_success)
			{
			    printf("INFO : Content type changed\n");
			    return e_success;
			}
			else
			{
			    printf("Error: Content type not changed\n");
			    return e_failure;
			}
			break;
		    }
		    else if(strcmp(edInfo->edit_option,"-c") == 0 && strcmp(tag,"COMM") == 0)
		    {
			printf("INFO : Selected edit comment option\n");
			fseek(edInfo->backup_audio_fptr,-4,SEEK_CUR);
			if(edit_data(edInfo) == e_success)
			{
			    printf("INFO : Composer name changed\n");
			    return e_success;
			}
			else
			{
			    printf("Error: Composer name not changed\n");
			    return e_failure;
			}
			break;
		    }
		    else
		    {
			fseek(edInfo->backup_audio_fptr,-4,SEEK_CUR);
			char tag[4];
			fread(tag,4,1,edInfo->backup_audio_fptr);
			fwrite(tag,4,1,edInfo->dest_audio_fptr);
			copy_size_null_data(edInfo->dest_audio_fptr,edInfo->backup_audio_fptr,edInfo);
		    }
		}
	    }
	    else
	    {
		printf("Error: version not matching\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Error: header tag not matching\n");
	    return e_failure;
	}
    }

    else
    {
	printf("Error: file opening failed\n");
	return e_failure;
    }
}

Status store_size(char *str,EditInfo *edInfo)
{
    long size = strlen(str)+1;
    long siz = 0;
    char buf[4];
    for(int i=0; i<4; i++)
    {
	if(i < 2)
	    siz = (((0x000000ff << (8*i))&size) << (24-(8*i))) | siz;
	else
	    siz = (((0x000000ff << (8*i))&size) << (24-(8*i))) | siz;
    }
    char nul[3];
    fwrite(&siz,4,1,edInfo->dest_audio_fptr);

    fread(nul,3,1,edInfo->backup_audio_fptr);
    fwrite(nul,3,1,edInfo->dest_audio_fptr);
    return e_success;
}

long extract_sizee(char *buf)
{
    long size = 0;
    for(int i=0; i<4; i++)
    {
	size = (buf[i] << (24-(8*i))) | size;
    }
    return size;
}

Status copy_size_null_data(FILE *dest_audio,FILE *src_audio,EditInfo *edInfo)
{
    char siz[4];
    char nul[3];
    //copying size
    fread(siz,4,1,src_audio);
    fwrite(siz,4,1,dest_audio);
    edInfo->copy_size = extract_sizee(siz);
    //copying null
    fread(nul,3,1,src_audio);
    fwrite(nul,3,1,dest_audio);
    //copying data
    char data[edInfo->copy_size];
    fread(data,edInfo->copy_size-1,1,src_audio);
    fwrite(data,edInfo->copy_size-1,1,dest_audio);

    return e_success;
}

void store_backup(FILE *dest_fptr,FILE *backup_fptr,EditInfo *edInfo)
{
    rewind(dest_fptr);
    rewind(backup_fptr);
    char backup;
    while(fread(&backup,1,1,dest_fptr))
    {
	fwrite(&backup,1,1,backup_fptr);
    }
}

Status edit_data(EditInfo *edInfo)
{
    char tag[4];
    fread(tag,4,1,edInfo->backup_audio_fptr);
    fwrite(tag,4,1,edInfo->dest_audio_fptr);
    char buf[4];
    fread(buf,4,1,edInfo->backup_audio_fptr);
    edInfo->title_size = extract_sizee(buf);
    if(store_size(edInfo->edit_str,edInfo) == e_success)
    {
	fwrite(edInfo->edit_str,strlen(edInfo->edit_str),1,edInfo->dest_audio_fptr);
	char ch;
	fseek(edInfo->backup_audio_fptr,(edInfo->title_size-1),SEEK_CUR);
	while(fread(&ch,1,1,edInfo->backup_audio_fptr))
	{
	    fwrite(&ch,1,1,edInfo->dest_audio_fptr);
	}
	store_backup(edInfo->dest_audio_fptr,edInfo->backup_audio_fptr,edInfo);
	return e_success;
    }
    else
    {
	printf("Error: size not stored\n");
	return e_failure;
    }
}
