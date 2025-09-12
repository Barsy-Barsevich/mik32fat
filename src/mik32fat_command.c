#include "mik32fat_command.h"



void mik32fat_command_init(void)
{

}

void mik32fat_command_fsparam(MIK32FAT_Descriptor_TypeDef *fs)
{
    mik32fat_diag_fat_info(fs);
}

void mik32fat_command_pwd(MIK32FAT_Descriptor_TypeDef *fs)
{
    printf("%s\n", fs->temp.name);
}

void mik32fat_command_ls(MIK32FAT_Descriptor_TypeDef *fs)
{
    MIK32FAT_Status_TypeDef res = mik32fat_utils_ls(fs, stdout);
    if (res != MIK32FAT_STATUS_OK)
    {
        printf("mik32fat_ls: \n");
        mik32fat_decode_status(res);
    }
}

void mik32fat_command_cdroot(MIK32FAT_Descriptor_TypeDef *fs)
{
    mik32fat_decode_status(mik32fat_set_pointer_to_root(fs));
}

void mik32fat_command_cd(MIK32FAT_Descriptor_TypeDef *fs, const char *path)
{
    MIK32FAT_TempData_TypeDef temp = fs->temp;
    mik32fat_decode_status(mik32fat_find_by_path(fs, path));
    if ((fs->temp.status & MIK32FAT_ATTR_DIRECTORY) == 0)
    {
        fs->temp = temp;
        printf("Cannot enter, %s is a file, stop.\n", path);
    }
}

void mik32fat_command_mkdir(MIK32FAT_Descriptor_TypeDef *fs, const char *path)
{
    mik32fat_decode_status(mik32fat_create(fs, path, true));
}

void mik32fat_command_touch(MIK32FAT_Descriptor_TypeDef *fs, const char *path)
{
    mik32fat_decode_status(mik32fat_create(fs, path, false));
}

void mik32fat_command_rm(MIK32FAT_Descriptor_TypeDef *fs, const char *path)
{
    mik32fat_decode_status(mik32fat_delete(fs, path));
}

void mik32fat_command_fileparam(MIK32FAT_File_TypeDef *file)
{
    mik32fat_diag_decode_file(file);
}

void mik32fat_command_file_open(MIK32FAT_File_TypeDef *file, MIK32FAT_Descriptor_TypeDef *fs, const char *path, const char *mode)
{
    mik32fat_decode_status(mik32fat_file_open(file, fs, path, mode));
}

void mik32fat_command_file_close(MIK32FAT_File_TypeDef *file)
{
    mik32fat_decode_status(mik32fat_file_close(file));
}
