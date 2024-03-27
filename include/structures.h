#include "stdbool.h"

/*
 * Pointeur vers une page dans la partition.
 * Contient un pointeur vers la page suivante ce qui constitue
 * une liste chainée de page.
 * address : indice de la page
 * */
typedef struct PageAddressNode {
    ushort address;
    struct PageAddressNode *next;
} PageAddressList;

/*
 * Représente un fichier. Contient un pointeur vers la première page
 * du fichier, chaque page ayant un pointeur vers la page suivante on a donc
 * accès au contenu du fichier.
 * name : nom du fichier
 * size : nombre d'octets écrits dans le fichier
 * offset : emplacement du pointeur de lecture/écriture
 * */
typedef struct {
    char *name;
    PageAddressList first_page_address;
    unsigned int size;
    unsigned int offset;
} File;

/*
 * files : Tableau dynamique contenant tous les fichiers enregistrés dans la partition
 * total_files : nombre de fichiers enregistrés
 * max_files : nombre de fichiers maximum
 * */
typedef struct {
    File **files;
    ushort total_files;
    ushort max_files;
} FileTable;

/*
 * Représente la partition.
 * Possède un nom et contient la liste des fichiers.
 * */
typedef struct {
    const char *name;
    FileTable file_table;
} Partition;