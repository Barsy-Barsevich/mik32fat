#pragma once

#include <stdio.h>
#include "mik32fat.h"
#include "mik32fat_diag.h"

void mik32fat_command_init(void);
void mik32fat_command_fsparam(MIK32FAT_Descriptor_TypeDef *fs);
void mik32fat_command_pwd(MIK32FAT_Descriptor_TypeDef *fs);
void mik32fat_command_ls(MIK32FAT_Descriptor_TypeDef *fs);
void mik32fat_command_cdroot(MIK32FAT_Descriptor_TypeDef *fs);
void mik32fat_command_cd(MIK32FAT_Descriptor_TypeDef *fs, const char *path);
void mik32fat_command_mkdir(MIK32FAT_Descriptor_TypeDef *fs, const char *path);
void mik32fat_command_touch(MIK32FAT_Descriptor_TypeDef *fs, const char *path);
void mik32fat_command_rm(MIK32FAT_Descriptor_TypeDef *fs, const char *path);
void mik32fat_command_fileparam(MIK32FAT_File_TypeDef *file);
void mik32fat_command_file_open(MIK32FAT_File_TypeDef *file, MIK32FAT_Descriptor_TypeDef *fs, const char *path, const char *mode);
void mik32fat_command_file_close(MIK32FAT_File_TypeDef *file);