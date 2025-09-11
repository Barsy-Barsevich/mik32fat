# MIK32FAT
*Совместимая с FAT32 файловая система для встраиваемых систем*
\[RU/EN]

---
## Описание
Файловая система MIK32FAT разработана для использования во встраиваемых (embedded) системах. Файловая система совместима с FAT32, что позволяет использовать ее для работы с файлами на картах SD, которые могут быть прочитаны персональным компьютером.

MIK32FAT позволяет записывать, читать, дозаписывать файлы произвольного объема; создавать и удалять файлы и директории; перемещаться по диску, используя пути.

MIK32FAT не поддерживает длинные имена файлов (LFN), что накладывает ограничения на используемые называния файлов. Так, имена файлов и директорий не должны быть больше 8 символов. Имена должны записываться заглавными латинскими буквами или цифрами и не должны содержать символа точки (`.`). Расширения файлов и директорий могут иметь не более трех символов длины и также должны записываться заглавными буквами.

Перед началом работы убедитесь, что используемый диск отформатирован для использования файловой системы FAT32.

## Методы

#### `MIK32FAT_I` (`mik32fat_init`)
(`MIK32FAT_Descriptor_TypeDef *fs`, `void *disk`);

Alias: `mik32fat_init`;

Returns: `MIK32FAT_Status_TypeDef`

The method initializes the file system.

#### `MIK32FAT_SPR` (`mik32fat_set_pointer_to_root`)
(`MIK32FAT_Descriptor_TypeDef *fs`);

Alias: `mik32fat_init`;

Returns: `MIK32FAT_Status_TypeDef`

The methods move file systems's directory pointer to root directory.

#### `MIK32FAT_FNC` (`mik32fat_find_next_cluster`)
(`MIK32FAT_Descriptor_TypeDef *fs`);

Alias: `mik32fat_find_next_cluster`;

Returns: `MIK32FAT_Status_TypeDef`

The method moves file system's cluster pointer to the next cluster of file/directory.

#### `MIK32FAT_FBN` (`mik32fat_find_by_name`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *name`);

Alias: `mik32fat_find_by_name`;

Returns: `MIK32FAT_Status_TypeDef`

The method finds a first cluster of file/directory in temporary directory by name.

#### `MIK32FAT_FBP` (`mik32fat_find_by_path`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *path`);

Alias: `mik32fat_find_by_path`;

Returns: `MIK32FAT_Status_TypeDef`

The method finds a first cluster of file/directory by path. The search starts at temporary directory. The path can contain "`.`" and "`..`" elements that means "this directory" and "master directory".

#### `MIK32FAT_FCBP` (`mik32fat_find_or_create_by_path`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *path`, `bool dir`);

Alias: `mik32fat_find_or_create_by_path`;

Returns: `MIK32FAT_Status_TypeDef`

The method finds or creates file or directory by path. The process is the same as in `MIK32FAT_FBP` method, however if the program does not find the directory or the file, it creates it.

#### `MIK32FAT_TFC` (`mik32fat_take_free_cluster`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `uint32_t clust`, `uint32_t *new_clust`);

Alias: `mik32fat_take_free_cluster`;

Returns: `MIK32FAT_Status_TypeDef`

The method allocates a new free cluster for the directory or file and links it with already existing FAT cluster sequence.

#### `MIK32FAT_C` (`mik32fat_create`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *name`, `bool dir`);

Alias: `mik32fat_create`;

Returns: `MIK32FAT_Status_TypeDef`

The method creates a new file or directory.

#### `MIK32FAT_D` (`mik32fat_delete`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *name`);

Alias: `mik32fat_delete`;

Returns: `MIK32FAT_Status_TypeDef`

The method deletes a file or directory.

#### `MIK32FAT_FO` (`mik32fat_file_open`)

(`MIK32FAT_File_TypeDef *file`, `MIK32FAT_Descriptor_TypeDef *fs`, `const char *name`, `const char *mod`);

Alias: `mik32fat_file_open`;

Returns: `MIK32FAT_Status_TypeDef`

The method opens a file. Строка-путь (поле `path`):
- Совпадает с именем файла, если файл находится в корневом каталоге диска;
- Содержит перед имененм файла названия субдиректорий, отделенные символом "`/`" ("слеш").

Так, например, если необходимо прочитать файл `TEXT.TXT`, который находится в папке `FOLDER`, строка-путь должна иметь вид "`FOLDER/TEXT.TXT`".

Поле `mode` имеет следующие возможные значения:
- "`r`" - читать файл (сначала);
- "`r+`" - читать файл и записывать сначала, без удаления файла;
- "`w`" - создать файл, если не существует и записывать сначала;
- "`w+`" - то же, но с возможностью чтения;
- "`a`" - создать файл, если не существует и записывать сначала, в противном случае дозаписывать, сохраняя прежнее содержимое;
- "`a+`" - то же, но с возможностью чтения.

#### `MIK32FAT_FR` (`mik32fat_file_read`)

(`MIK32FAT_File_TypeDef *file`, `char *dst`, `size_t len`);

Alias: `mik32fat_file_read`;

Returns: `int`

The method reads data from a file.

#### `MIK32FAT_FW` (`mik32fat_file_write`)

(`MIK32FAT_File_TypeDef *file`, `const char *src`, `size_t len`);

Alias: `mik32fat_file_write`;

Returns: `int`

The method writes data to a file.

#### `MIK32FAT_FC` (`mik32fat_file_close`)

(`MIK32FAT_File_TypeDef *file`);

Alias: `mik32fat_file_close`;

Returns: `MIK32FAT_Status_TypeDef`

The method closes a file.



