#ifndef FONCTIONS_BAS_NIVEAU_H
#define FONCTIONS_BAS_NIVEAU_H

#define LINUX_COMMAND "sudo mkfs.ext4 %s %d"; // Commande utilisée avec Linux
#define WINDOWS_COMMAND "mkfs.ext4 %s %d";  // Commande utilisée avec Cygwin64 sur WINDOWS

#include "../include/library.h"

int myFormat(char *partitionName) {
    char command[128];
    int totalBlocks = PARTITION_MAX_SIZE / 100;
    int k;

    char *command_str = LINUX_COMMAND;

    // Tentative d'exécution de la commande pour Linux
    k = snprintf(command, sizeof(command), command_str, partitionName, totalBlocks);
    if (k >= 0 && system(command) == 0) {
        initPartition(partitionName);
        return 0;
    }

    // Si la commande Linux n'a pas fonctionnée :
    printf("WARN : Erreur de formatage de la partition avec la commande linux, tentative avec la commande windows.\n");

    command_str = WINDOWS_COMMAND;

    // Tentative d'exécution de la commande pour Windows
    k = snprintf(command, sizeof(command), command_str, partitionName, totalBlocks);
    if (k >= 0 && system(command) == 0) {
        initPartition(partitionName);
        return 0;
    }

    perror("FATAL : Erreur lors de l'exécution de la commande mkfs.ext4.");
    return -1;
}

File *myOpen(const char *fileName, Partition *partition) {

    if (fileName == NULL || partition == NULL) {
        perror("Ouverture de fichier impossible avec des données NULL en entrée");
        return NULL;
    }

    // Vérifier si le fichier existe déjà
    File *existing_file = findFileWithName(&partition->file_table, fileName);

    if (existing_file != NULL) {
        return existing_file;
    }

    // Si le fichier n'existe pas, on tente de créer un nouveau fichier
    free(existing_file);

    if (getAllPageCount(*partition) < getMaxPagesAmount()) {
        // S'il y a assez de la place pour une nouvelle page

        // On initialise un nouveau fichier
        File *newFile = initNewFile(fileName, *partition);

        // On ajoute le fichier à la table de fichier dans les métadonnées de la partition
        addFileToTable(&partition->file_table, &newFile);

        // On sauvegarde les modifications effectuées dans la partition
        writePartitionMetadata(partition, partition->name);

        return newFile;
    } else {
        perror("Espace insuffisant pour créer un nouveau fichier");
        return NULL;
    }
}

void mySeek(File *file, int offset) {
    if (file != NULL) {
        if (offset < 0) {
            setSeekToStart(file);
        } else if (offset >= file->size) {
            setSeekToEnd(file);
        } else {
            file->offset = offset;
        }
    }
}

int myRead(File *file, char *buffer, int nBytes, Partition *partition) {

    if (file == NULL) return -1;
    if (file->size == 0) return 0; // fichier vide
    if (file->size == file->offset) return 0; // Lecture en fin de fichier
    if (buffer == NULL) buffer = (char *) malloc(sizeof(char) * nBytes + 1);

    int fd = open(partition->name, O_RDONLY);

    if (fd == -1) {
        perror("Erreur lors de l'ouverture de la partition");
        return -1;
    }

    int bytesRead = 0;
    int pageIndex = (int) file->offset / PAGE_MAX_SIZE;
    int offsetInPage = (int) file->offset % PAGE_MAX_SIZE;
    int readableBytesInPage = PAGE_MAX_SIZE - offsetInPage - sizeof(bool);

    // Rien à lire dans la page
    if (nBytes <= 0 || (pageIndex <= 0 && readableBytesInPage <= 0)) return 0;
    if (offsetInPage > readableBytesInPage) {
        printf("Rien à lire (l'offset est à la fin du fichier)\n");
        return 0;
    }

    // Trouver la page actuelle en fonction de l'offset
    PageAddressList *tmp = &(file->first_page_address);
    while (pageIndex > 0 && tmp != NULL) {
        tmp = tmp->next;
        pageIndex--;
    }

    int bufferSize = (int) (file->size - (getPageCount(*file) * sizeof(bool)));

    if (nBytes > bufferSize) {
        nBytes = bufferSize;
    }

    bool finishReading = false;

    int remainingBytesToRead;
    int k, j, n;
    j = 0; // Itère le tableau de résultat TODO : variable j simplifiable
    n = 0; // Définit le nombre maximum d'itérations lors de la lecture

    while (!finishReading) {
        readableBytesInPage = PAGE_MAX_SIZE - offsetInPage - sizeof(bool);

        int valSeek = (int) ((convertAddressToPhysic(tmp->address) + offsetInPage)); //  Ajustement avec -1
        // int valSeek = (int) ((convertAddressToPhysic(tmp->address) + offsetInPage - 1));
        lseek(fd, valSeek, SEEK_SET);

        // Le nombre total de bytes restant à lire
        remainingBytesToRead = nBytes - bytesRead;


        if (readableBytesInPage <= remainingBytesToRead) {
            // Si Le nombre d'octets restants à écrire est supérieur au nombre de bytes qu'on peut écrire dans la page.
            // On remplit la page et on en initialise une nouvelle.
            n += (PAGE_MAX_SIZE - 1);
            k = readableBytesInPage;

            // Lire le contenu dans la partition
            for (int i = bytesRead; i < n; ++i) {
                read(fd, &buffer[j], sizeof(char));
                j++;
            }

            // Augmenter le nombre de bytes qui ont été lus
            bytesRead += k;
            tmp = tmp->next;

        } else {
            // Sinon lire le reste des bytes (inférieur à la taille du page)
            n += remainingBytesToRead;
            char c;
            k = 0;

            // Lire le contenu dans la partition
            for (int i = bytesRead; i < n; ++i) { // TODO "<="
                if (file->size < file->offset + i) break; // Si on dépasse la taille du fichier on arrête
                // read(fd, &buffer[j], sizeof(char));
                read(fd, &c, sizeof(char));
                buffer[j] = c;
                j++;
                k++;
            }

            finishReading = true;
            bytesRead += k;
        }
        offsetInPage = 1;
    }

    close(fd);

    return bytesRead;
}

int myWrite(File *file, const char *buffer, int nBytes, Partition *partition) {

    if (file == NULL || buffer == NULL || nBytes <= 0) {
        return -1;
    }

    int fd = open(partition->name, O_WRONLY | O_CREAT, 0644);

    if (fd == -1) {
        perror("Erreur lors de l'ouverture de la partition");
        return -1;
    }

    PageAddressList *tmp = &(file->first_page_address);

    int bytesWritten = 0;
    int pageIndex = (int) file->offset / PAGE_MAX_SIZE;
    int offsetInPage = (int) file->offset % PAGE_MAX_SIZE;
    if (offsetInPage == 0) offsetInPage = 1; // Ajustement
    if (offsetInPage == 1 && pageIndex == 0) file->offset = 1;

    // Trouver la page actuelle en fonction de l'offset
    while (pageIndex > 0 && tmp != NULL) {
        tmp = tmp->next;
        pageIndex--;
    }

    int k, remainingBytesToWrite;

    char *s;

    bool finishWriting = false;

    while (!finishWriting) {
        int writableBytesInPage = PAGE_MAX_SIZE - offsetInPage; // TODO "- (int) sizeof(bool)"

        int valSeek = (int) (convertAddressToPhysic(tmp->address) + offsetInPage);
        lseek(fd, valSeek, SEEK_SET);

        // Le nombre total de bytes restant à écrire
        remainingBytesToWrite = nBytes - bytesWritten;

        if (writableBytesInPage <= remainingBytesToWrite) {
            // Si Le nombre de bytes restants à écrire est supérieur au nombre de bytes qu'on peut écrire dans la page.
            // On remplit la page et on en initialise une nouvelle.

            k = writableBytesInPage;
            s = (char *) malloc(sizeof(char) * k);

            for (int i = bytesWritten; i < k; ++i) {
                s[i] = buffer[i];
            }

            // Ecrire le contenu dans la partition
            write(fd, s, k);

            // Initialiser une nouvelle page
            // TODO : fonction à extraire pour la lisibilité du code
            PageAddressList *newPage = malloc(sizeof(PageAddressList));
            newPage->address = findAvailablePageAddress(*partition);
            newPage->next = NULL;
            // Chainer la nouvelle page à la liste
            tmp->next = newPage;
            tmp = newPage;
            setPageUnavailable(newPage->address, partition->name);

        } else {
            // Sinon écrire le reste des bytes (inférieur à la taille d'une page)
            k = remainingBytesToWrite;

            s = (char *) malloc(sizeof(char) * k);

            int j = 0;
            for (int i = bytesWritten; i < nBytes; ++i) {
                s[j] = buffer[i];
                j++;
            }
            write(fd, s, k);
            finishWriting = true;
        }

        // Augmenter le nombre d'octets qui ont été écrits
        bytesWritten += k;

        offsetInPage = 1;

        free(s);
    }

    // On recalcule l'offset du fichier
    pageIndex = (int) file->offset / PAGE_MAX_SIZE;
    file->offset += bytesWritten + pageIndex; // TODO : A RE-VERIFIER

    if (file->offset > file->size) {
        // Mettre à jour la taille du fichier
        file->size = file->offset;
    }

    close(fd);
    return bytesWritten;
}

int myDelete(const char *fileName, Partition *partition) {
    int fd = open(partition->name, O_WRONLY | O_CREAT, 0644);

    if (fd == -1) {
        perror("Erreur lors de l'ouverture de la partition");
        return -1;
    }

    File *file = findFileWithName(&partition->file_table, fileName);

    if (file == NULL) {
        return 1;
    }

    PageAddressList *tmp = &file->first_page_address;
    while (tmp != NULL) {
        setPageAvailable(tmp->address, partition->name);
        tmp = tmp->next;
    }

    removeFromFileTable(&partition->file_table, fileName);

//    freePageAddressList(&file->first_page_address);
    freeFile(&file);

    close(fd);
    return 0;
}

int mySize(File *file) {
    return (int) (file != NULL ? file->size : -1);
}

#endif