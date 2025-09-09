#include "mik32fat_file.h"

MIK32FAT_Status_TypeDef mik32fat_file_open
(
    MIK32FAT_File_TypeDef *file,
    MIK32FAT_Descriptor_TypeDef *fs,
    const char *name,
    const char *mod
)
{
    if (file == NULL || fs == NULL || name == NULL || mod == NULL)
    {
        return MIK32FAT_STATUS_INCORRECT_ARGUMENT;
    }
    MIK32FAT_TempData_TypeDef temp = fs->temp;
    
    if (strcmp(mod, "r") == 0)
    {
        __SAVING_TEMP_ERROR_CHECK( mik32fat_find_by_path(fs, name) );
        file->r.enable = true;
        file->w.enable = false;
        file->r.idx = 0;
        file->r.temp_cluster = fs->temp.cluster;
        file->r.temp_sector_in_cluster = 0;
        file->data_to_read = fs->temp.len;
    }
    else if (strcmp(mod, "r+") == 0)
    {
        __SAVING_TEMP_ERROR_CHECK( mik32fat_find_by_path(fs, name) );
        file->r.enable = true;
        file->w.enable = true;
        file->r.idx = 0;
        file->r.temp_cluster = fs->temp.cluster;
        file->r.temp_sector_in_cluster = 0;
        file->w.idx = 0;
        file->w.temp_cluster = fs->temp.cluster;
        file->w.temp_sector_in_cluster = 0;
        file->written_bytes = 0;
        file->calculated_sector_to_write = fs->data_region_begin + file->w.temp_cluster *
             fs->param.sectors_per_cluster + file->w.temp_sector_in_cluster;
    }
    else if (strcmp(mod, "w") == 0)
    {
        __SAVING_TEMP_ERROR_CHECK( mik32fat_delete(fs, name));
        __SAVING_TEMP_ERROR_CHECK( mik32fat_find_or_create_by_path(fs, name, false) );
        file->r.enable = false;
        file->w.enable = true;
        file->w.idx = 0;
        file->w.temp_cluster = fs->temp.cluster;
        file->w.temp_sector_in_cluster = 0;
        file->written_bytes = 0;
        file->calculated_sector_to_write = fs->data_region_begin + file->w.temp_cluster *
             fs->param.sectors_per_cluster + file->w.temp_sector_in_cluster;
    }
    else if (strcmp(mod, "w+") == 0)
    {
        __SAVING_TEMP_ERROR_CHECK( mik32fat_delete(fs, name));
        __SAVING_TEMP_ERROR_CHECK( mik32fat_find_or_create_by_path(fs, name, false) );
        file->r.enable = true;
        file->w.enable = true;
        file->r.idx = 0;
        file->r.temp_cluster = fs->temp.cluster;
        file->r.temp_sector_in_cluster = 0;
        file->w.idx = 0;
        file->w.temp_cluster = fs->temp.cluster;
        file->w.temp_sector_in_cluster = 0;
        file->data_to_read = 0;
        file->written_bytes = 0;
        file->calculated_sector_to_write = fs->data_region_begin + file->w.temp_cluster *
             fs->param.sectors_per_cluster + file->w.temp_sector_in_cluster;
    }
    else if (strcmp(mod, "a") == 0)
    {
        __SAVING_TEMP_ERROR_CHECK( mik32fat_find_or_create_by_path(fs, name, false) );
        file->r.enable = false;
        file->w.enable = true;
        uint32_t len = fs->temp.len;
        uint32_t clusters_num = len / fs->param.cluster_len_bytes;
        uint32_t addr_in_cluster = len % fs->param.cluster_len_bytes;
        file->w.temp_sector_in_cluster = addr_in_cluster / fs->param.sector_len_bytes;
        file->w.idx = addr_in_cluster % fs->param.sector_len_bytes;
        MIK32FAT_Status_TypeDef res = MIK32FAT_STATUS_OK;

        printf("Clusters num: %u\n", (unsigned)clusters_num);

        while (res == MIK32FAT_STATUS_OK && clusters_num-- > 0)
        {
            res = mik32fat_find_next_cluster(fs);
        }
        if (res != MIK32FAT_STATUS_OK)
        {
            fs->temp = temp;
            if (res == MIK32FAT_STATUS_NOT_FOUND)
            {
                return MIK32FAT_STATUS_INCORRECT_FILE_LENGHT;
            }
            else
            {
                return res;
            }
        }
        file->w.temp_cluster = fs->temp.cluster;
        file->written_bytes = len;
        file->calculated_sector_to_write = fs->data_region_begin + file->w.temp_cluster *
             fs->param.sectors_per_cluster + file->w.temp_sector_in_cluster;
    }
    else if (strcmp(mod, "a+") == 0)
    {
        __SAVING_TEMP_ERROR_CHECK( mik32fat_find_or_create_by_path(fs, name, false) );
        file->r.enable = true;
        file->w.enable = true;
        uint32_t len = fs->temp.len;
        uint32_t clusters_num = len / fs->param.cluster_len_bytes;
        uint32_t addr_in_cluster = len % fs->param.cluster_len_bytes;
        file->r.temp_sector_in_cluster = addr_in_cluster / fs->param.sector_len_bytes;
        file->w.temp_sector_in_cluster = addr_in_cluster / fs->param.sector_len_bytes;
        file->r.idx = addr_in_cluster % fs->param.sector_len_bytes;
        file->w.idx = addr_in_cluster % fs->param.sector_len_bytes;
        MIK32FAT_Status_TypeDef res = MIK32FAT_STATUS_OK;
        while (res == MIK32FAT_STATUS_OK && clusters_num-- > 0)
        {
            res = mik32fat_find_next_cluster(fs);
        }
        if (res != MIK32FAT_STATUS_OK)
        {
            fs->temp = temp;
            if (res == MIK32FAT_STATUS_NOT_FOUND)
            {
                return MIK32FAT_STATUS_INCORRECT_FILE_LENGHT;
            }
            else
            {
                return res;
            }
        }
        file->r.temp_cluster = fs->temp.cluster;
        file->w.temp_cluster = fs->temp.cluster;
        file->data_to_read = fs->temp.len;
        file->written_bytes = len;
        file->calculated_sector_to_write = fs->data_region_begin + file->w.temp_cluster *
             fs->param.sectors_per_cluster + file->w.temp_sector_in_cluster;
    }
    else
    {
        fs->temp = temp;
        return MIK32FAT_STATUS_INCORRECT_ARGUMENT;
    }

    file->fs = fs;
    file->param = fs->temp;
    file->writing_not_finished = false;
    file->closed = false;

    fs->temp = temp;
    file->errcode = MIK32FAT_STATUS_OK;
    return MIK32FAT_STATUS_OK;
}


int mik32fat_file_read_byte
(
    MIK32FAT_File_TypeDef *file,
    char *symbol
)
{
    if (file == NULL || symbol == NULL)
    {
        file->errcode = MIK32FAT_STATUS_INCORRECT_ARGUMENT;
        return (int)MIK32FAT_STATUS_INCORRECT_ARGUMENT;
    }
    if (file->closed)
    {
        file->errcode = MIK32FAT_STATUS_CLOSED_FILE_ACCESS_ATTEMPT;
        return MIK32FAT_STATUS_CLOSED_FILE_ACCESS_ATTEMPT;
    }
    if (!file->r.enable)
    {
        file->errcode = MIK32FAT_STATUS_ERROR;
        return (int)MIK32FAT_STATUS_ERROR;
    }
    if (file->data_to_read == 0)
    {
        file->errcode = MIK32FAT_STATUS_END_OF_FILE;
        return (int)MIK32FAT_STATUS_END_OF_FILE;
    }
    
    MIK32FAT_Descriptor_TypeDef *fs = file->fs;

    uint32_t sector = fs->data_region_begin + file->r.temp_cluster *
        fs->param.sectors_per_cluster + file->r.temp_sector_in_cluster;
    MIK32FAT_Status_TypeDef res = __mik32fat_sector_sread(fs, sector);
    if (res != MIK32FAT_STATUS_OK)
    {
        file->errcode = res;
        return (int)res;
    }
    *symbol = fs->buffer[file->r.idx];
    file->data_to_read -= 1;
    if (file->data_to_read == 0)
    {
        file->errcode = MIK32FAT_STATUS_OK;
        return 0;
    }

    if (file->r.idx == fs->param.sector_len_bytes-1)
    {
        file->r.idx = 0;
        if (file->r.temp_sector_in_cluster == fs->param.sectors_per_cluster-1)
        {
            file->r.temp_sector_in_cluster = 0;
            MIK32FAT_TempData_TypeDef temp = fs->temp;
            fs->temp.cluster = file->r.temp_cluster;
            MIK32FAT_Status_TypeDef res = mik32fat_find_next_cluster(fs);
            if (res != MIK32FAT_STATUS_OK)
            {
                fs->temp = temp;
                if (res == MIK32FAT_STATUS_NOT_FOUND)
                {
                    file->errcode = MIK32FAT_STATUS_INCORRECT_FILE_LENGHT;
                    return (int)MIK32FAT_STATUS_INCORRECT_FILE_LENGHT;
                }
                else
                {
                    file->errcode = res;
                    return (int)res;
                }
            }
            file->r.temp_cluster = fs->temp.cluster;
            fs->temp = temp;
        }
        else
        {
            file->r.temp_sector_in_cluster += 1;
        }
    }
    else
    {
        file->r.idx += 1;
    }
    file->errcode = MIK32FAT_STATUS_OK;
    return 0;
}


int mik32fat_file_read
(
    MIK32FAT_File_TypeDef *file,
    char *dst,
    size_t len
)
{
    if (file == NULL || dst == NULL)
    {
        file->errcode = MIK32FAT_STATUS_INCORRECT_ARGUMENT;
        return 0;
    }
    if (file->closed)
    {
        file->errcode = MIK32FAT_STATUS_CLOSED_FILE_ACCESS_ATTEMPT;
        return MIK32FAT_STATUS_CLOSED_FILE_ACCESS_ATTEMPT;
    }
    if (!file->r.enable)
    {
        file->errcode = MIK32FAT_STATUS_ERROR;
        return 0;
    }
    if (file->data_to_read == 0)
    {
        file->errcode = MIK32FAT_STATUS_END_OF_FILE;
        return 0;
    }

    MIK32FAT_Descriptor_TypeDef *fs = file->fs;
    size_t dst_idx = 0;
    while (len > 0 && file->data_to_read > 0)
    {
        uint32_t sector = fs->data_region_begin + file->r.temp_cluster *
            fs->param.sectors_per_cluster + file->r.temp_sector_in_cluster;
        MIK32FAT_Status_TypeDef res = __mik32fat_sector_sread(fs, sector);
        if (res != MIK32FAT_STATUS_OK)
        {
            file->errcode = res;
            return 0;
        }

        dst[dst_idx++] = fs->buffer[file->r.idx++];
        len -= 1;
        file->data_to_read -= 1;

        if (len == 0 || file->data_to_read == 0)
        {
            break;
        }
        if (file->r.idx >= fs->param.sector_len_bytes)
        {
            file->r.idx = 0;
            file->r.temp_sector_in_cluster += 1;
            if (file->r.temp_sector_in_cluster >= fs->param.sectors_per_cluster)
            {
                file->r.temp_sector_in_cluster = 0;
                MIK32FAT_TempData_TypeDef temp = fs->temp;
                fs->temp.cluster = file->r.temp_cluster;
                MIK32FAT_Status_TypeDef res = mik32fat_find_next_cluster(fs);
                if (res != MIK32FAT_STATUS_OK)
                {
                    fs->temp = temp;
                    if (res == MIK32FAT_STATUS_NOT_FOUND)
                    {
                        file->errcode = MIK32FAT_STATUS_INCORRECT_FILE_LENGHT;
                    }
                    else
                    {
                        file->errcode = res;
                    }
                    return dst_idx;
                }
                file->r.temp_cluster = fs->temp.cluster;
                fs->temp = temp;
            }
        }
    }
    file->errcode = MIK32FAT_STATUS_OK;
    return dst_idx;
}


int mik32fat_file_write_byte
(
    MIK32FAT_File_TypeDef *file,
    char symbol
)
{
    if (file == NULL)
    {
        file->errcode = MIK32FAT_STATUS_INCORRECT_ARGUMENT;
        return (int)MIK32FAT_STATUS_INCORRECT_ARGUMENT;
    }
    if (file->closed)
    {
        file->errcode = MIK32FAT_STATUS_CLOSED_FILE_ACCESS_ATTEMPT;
        return MIK32FAT_STATUS_CLOSED_FILE_ACCESS_ATTEMPT;
    }
    if (!file->w.enable)
    {
        file->errcode = MIK32FAT_STATUS_ERROR;
        return (int)MIK32FAT_STATUS_ERROR;
    }

    MIK32FAT_Descriptor_TypeDef *fs = file->fs;

    if (file->w.idx == 0 && file->w.temp_sector_in_cluster == 0 && file->w.temp_cluster == file->param.cluster)
    {
        /* Increment index */
        file->w.idx += 1;
        if (file->w.idx >= fs->param.sector_len_bytes)
        {
            file->w.idx = 0;
            file->w.temp_sector_in_cluster += 1;
            if (file->w.temp_sector_in_cluster >= fs->param.sectors_per_cluster)
            {
                file->w.temp_sector_in_cluster = 0;
                MIK32FAT_TempData_TypeDef temp = fs->temp;
                fs->temp.cluster = file->w.temp_cluster;
                MIK32FAT_Status_TypeDef res = mik32fat_find_next_cluster(fs);
                if (res != MIK32FAT_STATUS_OK)
                {
                    fs->temp = temp;
                    if (res == MIK32FAT_STATUS_NOT_FOUND)
                    {
                        file->errcode = MIK32FAT_STATUS_INCORRECT_FILE_LENGHT;
                        return (int)MIK32FAT_STATUS_INCORRECT_FILE_LENGHT;
                    }
                    else
                    {
                        file->errcode = res;
                        return (int)res;
                    }
                }
                file->w.temp_cluster = fs->temp.cluster;
                fs->temp = temp;
            }
        }
    }

    uint32_t sector = fs->data_region_begin + file->w.temp_cluster *
        fs->param.sectors_per_cluster + file->w.temp_sector_in_cluster;
    MIK32FAT_Status_TypeDef res;
    res = __mik32fat_sector_sread(fs, sector);
    if (res != MIK32FAT_STATUS_OK)
    {
        file->errcode = res;
        return (int)res;
    }
    fs->buffer[file->w.idx] = symbol;
    file->errcode = MIK32FAT_STATUS_OK;
    return 0;
}


int mik32fat_file_write
(
    MIK32FAT_File_TypeDef *file,
    const char *src,
    size_t len
)
{
    if (file == NULL || src == NULL)
    {
        file->errcode = MIK32FAT_STATUS_INCORRECT_ARGUMENT;
        return 0;
    }
    if (file->closed)
    {
        file->errcode = MIK32FAT_STATUS_CLOSED_FILE_ACCESS_ATTEMPT;
        return MIK32FAT_STATUS_CLOSED_FILE_ACCESS_ATTEMPT;
    }
    if (!file->w.enable)
    {
        file->errcode = MIK32FAT_STATUS_ERROR;
        return 0;
    }

    MIK32FAT_Descriptor_TypeDef *fs = file->fs;
    size_t src_idx = 0;
    while (len > 0)
    {
        uint32_t sector = file->calculated_sector_to_write;
        if (file->w.idx == 0)
        {
            sector = fs->data_region_begin + file->w.temp_cluster *
                fs->param.sectors_per_cluster + file->w.temp_sector_in_cluster;
            file->calculated_sector_to_write = sector;
        }
        /* Read sector if buffer contains other sector data */
        if (sector != fs->prev_sector)
        {
            MIK32FAT_Status_TypeDef res = __mik32fat_sector_sread(fs, sector);
            if (res != MIK32FAT_STATUS_OK)
            {
                file->errcode = res;
                return src_idx;
            }
        }
        fs->buffer[file->w.idx++] = src[src_idx++];
        len -= 1;
        file->written_bytes += 1;
        file->writing_not_finished = true;
        if (file->w.idx >= fs->param.sector_len_bytes)
        {
            file->w.idx = 0;
            /* Send buffer contents to the disk */
            MIK32FAT_Status_TypeDef res;
            res = __mik32fat_sector_serase(fs, sector);
            if (res != MIK32FAT_STATUS_OK)
            {
                file->errcode = res;
                return res;
            }
            res = __mik32fat_sector_swrite(fs, sector);
            if (res != MIK32FAT_STATUS_OK)
            {
                file->errcode = res;
                return res;
            }
            file->writing_not_finished = false;
            /* Next sector */
            file->w.temp_sector_in_cluster += 1;
            if (file->w.temp_sector_in_cluster >= fs->param.sectors_per_cluster)
            {
                file->w.temp_sector_in_cluster = 0;
                uint32_t new_cluster = 0;
                MIK32FAT_Status_TypeDef res;
                res = mik32fat_take_free_cluster(fs, file->w.temp_cluster, &new_cluster);
                if (res != MIK32FAT_STATUS_OK)
                {
                    file->errcode = res;
                    return src_idx;
                }
                file->w.temp_cluster = new_cluster;
            }
        }
    }
    file->errcode = MIK32FAT_STATUS_OK;
    return src_idx;
}


MIK32FAT_Status_TypeDef mik32fat_file_close
(
    MIK32FAT_File_TypeDef *file
)
{
    if (file == NULL)
    {
        file->errcode = MIK32FAT_STATUS_INCORRECT_ARGUMENT;
        return MIK32FAT_STATUS_INCORRECT_ARGUMENT;
    }
    
    MIK32FAT_Descriptor_TypeDef *fs = file->fs;
    file->closed = true;
    if (file->writing_not_finished)
    {
        if (file->calculated_sector_to_write == 0)
        {
            file->errcode = MIK32FAT_STATUS_INCORRECT_SECTOR_LINK;
            return MIK32FAT_STATUS_INCORRECT_SECTOR_LINK;
        }
        MIK32FAT_Status_TypeDef res;
        res = __mik32fat_sector_serase(fs, file->calculated_sector_to_write);
        if (res != MIK32FAT_STATUS_OK)
        {
            file->errcode = res;
            return res;
        }
        res = __mik32fat_sector_swrite(fs, file->calculated_sector_to_write);
        if (res != MIK32FAT_STATUS_OK)
        {
            file->errcode = res;
            return res;
        }
        file->writing_not_finished = false;
    }
    /* Save filelength */
    uint32_t dir_sector = fs->data_region_begin + file->param.dir_cluster *
        fs->param.sectors_per_cluster + file->param.dir_sec_offset;
    MIK32FAT_Status_TypeDef res = __mik32fat_sector_sread(fs, dir_sector);
    if (res != MIK32FAT_STATUS_OK)
    {
        file->errcode = res;
        return res;
    }
    uint32_t dir_sec_idx = file->param.entire_in_dir_clust + MIK32FAT_DIR_FILE_SIZE_OFFSET;
    uint32_t *filelen = (uint32_t*)(fs->buffer + dir_sec_idx);
    if (*filelen != 0 && *filelen != file->written_bytes)
    {
        res = __mik32fat_sector_serase(fs, dir_sector);
        if (res != MIK32FAT_STATUS_OK)
        {
            file->errcode = res;
            return res;
        }
    }
    if (*filelen != file->written_bytes)
    {
        *filelen = file->written_bytes;
        res = __mik32fat_sector_swrite(fs, dir_sector);
        if (res != MIK32FAT_STATUS_OK)
        {
            file->errcode = res;
            return res;
        }
    }

    file->errcode = MIK32FAT_STATUS_OK;
    return MIK32FAT_STATUS_OK;
}
