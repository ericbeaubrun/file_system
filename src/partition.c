#ifndef PARTITION_H
#define PARTITION_H

#include "../include/partition.h"

int partitionAlreadyExists(const char *filename) {
    struct stat buffer;
    int exist = stat(filename, &buffer);
    return exist == 0 ? 1 : 0;
}

void freePageAddressList(PageAddressList *head) {
    PageAddressList *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void freeFile(File **file) {
    if (file != NULL && *file != NULL) {
        if ((*file)->name != NULL) {
            free((*file)->name);
        }
        freePageAddressList(&((*file)->first_page_address));
        free(*file);
        *file = NULL;
    }
}

void freeFileTable(FileTable *fileTable) {
    if (fileTable->files != NULL) {
        for (int i = 0; i < fileTable->total_files; i++) {
            freeFile(&(fileTable->files[i]));
        }
        free(fileTable->files);
        fileTable->files = NULL;
    }
}

void cleanup(Partition *partition) {
    if (partition != NULL) {
        freeFileTable(&partition->file_table);
    }
}

void removeFromFileTable(FileTable *fileTable, const char* fileName) {

    int index = -1;

    // Trouver l'adresse du fichier dans le tableau
    for (int j = 0; j < fileTable->total_files; j++) {
        if (strcmp(fileTable->files[j]->name, fileName) == 0) {
            index = j;
            break;
        }
    }

    if (index != -1) {
        // Décalage des éléments du tableau dynamique
        for (int i = index; i < fileTable->total_files; i++) {
            fileTable->files[i] = fileTable->files[i + 1];
        }

        fileTable->files[fileTable->total_files] = NULL;
        fileTable->total_files--;
    }
}

ushort getPageCount(File file) {
    return (file.size / PAGE_MAX_SIZE) + 1;
}

ushort getAllPageCount(Partition partition) {

    int count = 0;
    for (int i = 0; i < partition.file_table.total_files; i++) {
        count += getPageCount(*partition.file_table.files[i]);
    }
    return count;
}

int getMaxPagesAmount() {
    return (PARTITION_MAX_SIZE - PARTITION_METADATA_MAX_SIZE) / PAGE_MAX_SIZE;
}

int getUsedSpace(Partition partition) {
    int usedSpace = 0;
    FileTable fileTable = partition.file_table;
    for (int i = 0; i < fileTable.total_files; i++) {
        usedSpace += (int) fileTable.files[i]->size;
    }
    return usedSpace;
}

void setSeekToEnd(File *file) {
    file->offset = file->size;
}

void setSeekToStart(File *file) {
    file->offset = 1;
}

void initOffset(File *file) {
    setSeekToEnd(file);
}

Partition *readPartitionMetadata(const char *partitionName) {
    int fd = open(partitionName, O_RDONLY);
    if (fd == -1) {
        perror("Erreur lors de l'ouverture de la partition");
        return NULL;
    }

    // Allouer de la mémoire pour la nouvelle structure Partition
    Partition *partition = malloc(sizeof(Partition));
    partition->name = partitionName;

    // Lire FileTable dans la partition
    read(fd, &partition->file_table.total_files, sizeof(ushort));
    read(fd, &partition->file_table.max_files, sizeof(ushort));

    partition->file_table.files = (File **) malloc(partition->file_table.max_files * sizeof(File *));

    for (int i = 0; i < partition->file_table.total_files; ++i) {
        // Lire le nom du fichier
        partition->file_table.files[i] = malloc(sizeof(File));

        ushort nameLength;
        read(fd, &nameLength, sizeof(ushort));
        partition->file_table.files[i]->name = malloc(nameLength);
        read(fd, partition->file_table.files[i]->name, nameLength);

        // Lire size
        read(fd, &partition->file_table.files[i]->size, sizeof(unsigned int));

        initOffset(partition->file_table.files[i]);

        // Lire la liste PageAddressList
        ushort listLength;
        read(fd, &listLength, sizeof(ushort));

        PageAddressList *currentNode = &partition->file_table.files[i]->first_page_address;
        for (int j = 0; j < listLength; ++j) {
            if (j > 0) {
                currentNode->next = malloc(sizeof(PageAddressList));
                currentNode = currentNode->next;
            }

            read(fd, &currentNode->address, sizeof(ushort));
            currentNode->next = NULL;
        }
    }

    close(fd);
    return partition;
}

int writePartitionMetadata(Partition *partition, const char *partitionName) {
    int fd = open(partitionName, O_WRONLY | O_CREAT, 0644);

    if (fd == -1) {
        perror("Erreur lors de l'ouverture de la partition");
        return -1;
    }

    // Ecrire les métadonnées de FileTable
    write(fd, &partition->file_table.total_files, sizeof(ushort));
    write(fd, &partition->file_table.max_files, sizeof(ushort));

    for (int i = 0; i < partition->file_table.total_files; ++i) {
        // Ecrire le nom du fichier
        ushort nameLength = strlen(partition->file_table.files[i]->name) + 1;
        write(fd, &nameLength, sizeof(ushort));
        write(fd, partition->file_table.files[i]->name, nameLength);

        // Ecrire length
        write(fd, &partition->file_table.files[i]->size, sizeof(unsigned int));

        // Ecrire la taille de la liste
        unsigned int length = partition->file_table.files[i]->size;
        ushort listLength = length <= PAGE_MAX_SIZE ? 1 : (length / PAGE_MAX_SIZE) + 1;
        write(fd, &listLength, sizeof(ushort));

        // Ecrire les elements de la liste
        PageAddressList *tmp = &partition->file_table.files[i]->first_page_address;

        for (ushort j = 0; j < listLength; ++j) {
            write(fd, &tmp->address, sizeof(ushort));
            tmp = tmp->next;
        }
    }

    close(fd);
    return 0;
}

FileTable initFileTable() {
    FileTable table;
    table.files = (File **) malloc(DEFAULT_MAX_FILE_NAME * sizeof(File *));
    table.total_files = 0;
    table.max_files = DEFAULT_MAX_FILE_NAME;
    return table;
}

bool readBooleanAtOffset(Partition partition, unsigned int offset) {
    int fd = open(partition.name, O_RDONLY);
    if (fd == -1) {
        perror("Ouverture de la partition impossible");
    }
    lseek(fd, offset, SEEK_SET);
    bool val;
    read(fd, &val, sizeof(bool));
    close(fd);
    return val;
}

void writeBooleanAtOffset(const char *partitionName, unsigned int offset, bool val) {
    int fd = open(partitionName, O_WRONLY, 0644);
    if (fd == -1) {
        perror("Ouverture de la partition impossible");
    }
    lseek(fd, offset, SEEK_SET);
    write(fd, &val, sizeof(bool));
    close(fd);
}

unsigned int convertAddressToPhysic(ushort address) {
    return (address * PAGE_MAX_SIZE) + PARTITION_METADATA_MAX_SIZE;
}

// TODO
//calculate offsetInPage()
//calculatePageIndex()

void setPageAvailable(ushort address, const char *partitionName) {
    writeBooleanAtOffset(partitionName, convertAddressToPhysic(address), 0);
}

void setPageUnavailable(ushort address, const char *partitionName) {
    writeBooleanAtOffset(partitionName, convertAddressToPhysic(address), 1);
}

bool readAtAddress(ushort address, Partition partition) {
    return readBooleanAtOffset(partition, convertAddressToPhysic(address));
}

ushort findAvailablePageAddress(Partition partition) {
    for (int i = 0; i < getMaxPagesAmount(); ++i) {
        bool b = readAtAddress(i, partition);
        if (b == 0) {
            return i;
        }
    }
    return -1; // Page non trouvée
}

void setPartitionPagesAvailable(const char *partitionName) {
    for (int i = 0; i < getMaxPagesAmount(); ++i) {
        setPageAvailable(i, partitionName);
    }
}

int initPartition(const char *partitionName) {
    Partition partition;
    // Aucune page au début
    partition.file_table = initFileTable();
    setPartitionPagesAvailable(partitionName);
    return writePartitionMetadata(&partition, partitionName);
}

File *initNewFile(const char *filename, Partition partition) {
    if (strlen(filename) + 1 >= FILENAME_MAX_SIZE) {
        perror("Initialisation impossible du fichier car le nom est trop long");
        return NULL;
    }

    File *file = malloc(sizeof(File));
    file->name = strdup(filename);

    PageAddressList pageAddressList;
    pageAddressList.next = NULL;
    pageAddressList.address = findAvailablePageAddress(partition);
    setPageUnavailable(pageAddressList.address, partition.name);

    file->first_page_address = pageAddressList;
    file->size = 0;
    initOffset(file);

    return file;
}

void addFileToTable(FileTable *table, File **file) {
    if (file != NULL) {
        if (table->total_files >= table->max_files) {
            // Si la table de fichier n'a plus assez de place
            // Réallouer de la mémoire avec une capacité plus grande
            table->max_files *= 2;
            table->files = (File **) realloc(table->files, table->max_files * sizeof(File *));

            if (table->files == NULL) {
                perror("Erreur : Impossible de réallouer de la mémoire.\n");
                exit(1);
            }
        }
        table->files[table->total_files] = *file;
        table->total_files++;
    }
}

File *findFileWithName(FileTable *fileTable, const char *filename) {
    if (filename != NULL && fileTable != NULL) {
        for (int i = 0; i < fileTable->total_files; i++) {
            if (strcmp(fileTable->files[i]->name, filename) == 0) {
                return fileTable->files[i];
            }
        }
    }
    return NULL; // Fichier non trouvé
}

#endif