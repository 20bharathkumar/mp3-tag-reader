#ifndef VIEW_H
#define VIEW_H
#include "type.h"


typedef struct _ViewInfo
{
    char *audio_file_name;
    FILE *audio_fptr;

    long title_size;
    char title_name[100];

}ViewInfo;

OperationType chech_option(char *argv);

Status validation_part(char *argv[],ViewInfo *viInfo);

Status do_viewing(ViewInfo *viInfo);

Status check_header_tag(FILE *audio_fptr,ViewInfo *viInfo);

void check_title_tag(FILE *audio_fptr,ViewInfo *viInfo);

long extract_size(char *buf);

void name(long size,char *comm_name,ViewInfo *viInfo);
#endif
