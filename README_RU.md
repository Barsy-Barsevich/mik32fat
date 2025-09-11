# MIK32FAT
*Совместимая с FAT32 файловая система для встраиваемых систем*
\[RU/[EN](./README.md)]

---
## Описание
Файловая система MIK32FAT разработана для использования во встраиваемых (embedded) системах. Файловая система совместима с FAT32, что позволяет использовать ее для работы с файлами на картах SD, которые могут быть прочитаны персональным компьютером.

MIK32FAT позволяет записывать, читать, дозаписывать файлы произвольного объема; создавать и удалять файлы и директории; перемещаться по диску, используя пути.

MIK32FAT не поддерживает длинные имена файлов (LFN), что накладывает ограничения на используемые называния файлов. Так, имена файлов и директорий не должны быть больше 8 символов. Имена должны записываться заглавными латинскими буквами или цифрами и не должны содержать символа точки (`.`). Расширения файлов и директорий могут иметь не более трех символов длины и также должны записываться заглавными буквами.

Перед началом работы убедитесь, что используемый диск отформатирован для использования файловой системы FAT32.

---
## Методы

#### `MIK32FAT_I` (`mik32fat_init`)
(`MIK32FAT_Descriptor_TypeDef *fs`, `void *disk`);

Alias: `mik32fat_init`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма инициализации работы файловой системы.

---
#### `MIK32FAT_SPR` (`mik32fat_set_pointer_to_root`)
(`MIK32FAT_Descriptor_TypeDef *fs`);

Alias: `mik32fat_init`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма устанавливает root-директорию в качестве текущей.

---
#### `MIK32FAT_FNC` (`mik32fat_find_next_cluster`)
(`MIK32FAT_Descriptor_TypeDef *fs`);

Alias: `mik32fat_find_next_cluster`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма поизводит поиск следующего кластера файла/директории.

---
#### `MIK32FAT_FBN` (`mik32fat_find_by_name`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *name`);

Alias: `mik32fat_find_by_name`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма производит поиск первого кластера файла/субдиректории в текущей директории по имени. Подпрограмма также получает сопутствующую информацию об объекте (статус, длина, положение дескриптора на диске).

---
#### `MIK32FAT_FBP` (`mik32fat_find_by_path`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *path`);

Alias: `mik32fat_find_by_path`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма производит поиск первого кластера файла/субдиректории по строке-пути относительно текущей директории.

---
#### `MIK32FAT_FCBP` (`mik32fat_find_or_create_by_path`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *path`, `bool dir`);

Alias: `mik32fat_find_or_create_by_path`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма производит поиск первого кластера файла/субдиректории по строке-пути относительно текущей директории. Если искомый объект (и/или его master-директории) не существует, подпрограмма создает его (их).

---
#### `MIK32FAT_TFC` (`mik32fat_take_free_cluster`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `uint32_t clust`, `uint32_t *new_clust`);

Alias: `mik32fat_take_free_cluster`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма выделяет новый свободный кластер для файла или директории и соединяет его с уже имеющейся последовательностью кластеров для данного файла/директории.

---
#### `MIK32FAT_C` (`mik32fat_create`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *name`, `bool dir`);

Alias: `mik32fat_create`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма создания файла/директории.

---
#### `MIK32FAT_D` (`mik32fat_delete`)

(`MIK32FAT_Descriptor_TypeDef *fs`, `const char *name`);

Alias: `mik32fat_delete`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма удаляет файл или директорию, стирая его/ее с диска.

---
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

---
#### `MIK32FAT_FR` (`mik32fat_file_read`)

(`MIK32FAT_File_TypeDef *file`, `char *dst`, `size_t len`);

Alias: `mik32fat_file_read`;

Returns: `int`, количество прочитанных байт.

Подпрограмма чтения данных из файла. Работает только если при открытии файла было допущено чтение (`mode` был "`r`", "`r+`", "`w+`" или "`a+`"), иначе ошибка.

---
#### `MIK32FAT_FW` (`mik32fat_file_write`)

(`MIK32FAT_File_TypeDef *file`, `const char *src`, `size_t len`);

Alias: `mik32fat_file_write`;

Returns: `int`, количество записанных байт.

Подпрограмма записи данных в файл. Работает только если при открытии файла была допущена запись (`mode` был "`r+`", "`w`", "`w+`", "`a`" или "`a+`"), иначе ошибка.

---
#### `MIK32FAT_FC` (`mik32fat_file_close`)

(`MIK32FAT_File_TypeDef *file`);

Alias: `mik32fat_file_close`;

Returns: `MIK32FAT_Status_TypeDef`

Подпрограмма закрытия файла. Необходима, если информация записывалась в файл. Подпрограмма переносит в накопитель оставшиеся байты в буфере сектора, а также обновляет значение длины файла в дескрипторе в master-директории.


