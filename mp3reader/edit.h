#ifndef EDIT_H
#define EDIT_H

#include "type.h"

typedef struct Edit
{
    char *tag;

    long title_size;
    long dest_size;

    long file_size;
    long next_tag;

    long copy_size;

    char *dest_audio_fname;
    FILE *dest_audio_fptr;

    char *backup_audio_fname;
    FILE *backup_audio_fptr;
    
    char *edit_option;
    char *edit_str;

}EditInfo;

Status edit_validate(char *argv[], EditInfo *edInfo);

Status do_editing(EditInfo *edInfo);

Status open_edit_file(EditInfo *edInfo);

Status store_size(char *str,EditInfo *edInfo);

Status add_str(char *str,EditInfo *edInfo);

long extract_sizee(char *buf);

Status copy_size_null_data(FILE *dest_audio,FILE *src_audio,EditInfo *edInfo);

void store_backup(FILE *dest_fptr,FILE *backup_fptr,EditInfo *edInfo);

Status edit_data(EditInfo *edInfo);
#endif
