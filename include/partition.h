#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include "structures.h"

#define PARTITION_MAX_SIZE 204800 // En octets (soit 2 Mo de partition)
#define PARTITION_METADATA_MAX_SIZE 10240 // En octets (5% de la partition, soit 1 Ko)
#define PAGE_MAX_SIZE 256 // En octets (soit 1600 octets par page)
#define FILENAME_MAX_SIZE 32 // En octets
#define DEFAULT_MAX_FILE_NAME 8

/*
 * Calcul et retourne le nombre de pages maximum possible dans la partition.
 * */
int getMaxPagesAmount();

/*
 * Fonction pour initialiser une FileTable vide.
 * */
FileTable initFileTable();

/*
 * Libère la mémoire d'un fichier File.
 * */
void freeFile(File **file);

/*
 * Initialiser l'offset d'un fichier, permet de choisir
 * d'initialiser l'offset au début ou à la fin du fichier.
 * */
void initOffset(File *file);

/*
 * Place le pointeur de fichier à la fin du fichier.
 * */
void setSeekToEnd(File *file);

/*
 * Place le pointeur du fichier au début du fichier.
 * */
void setSeekToStart(File *file);

/*
 * Retourne le nombre de page que possède un fichier File.
 * */
ushort getPageCount(File file);

/*
 * Libère la mémoire de la table de fichiers.
 * */
void freeFileTable(FileTable *fileTable);

/*
 * Supprime un fichier de FileTable (décalages des éléments du tableau dynamique)
 * */
void removeFromFileTable(FileTable *fileTable, const char* fileName);

/*
 * Libère la mémoire de toute la partition.
 * */
void cleanup(Partition *partition);

/*
 * Retourne le nombre de pages présentes dans la partition.
 * */
ushort getAllPageCount(Partition partition);

/*
 * Initialise une partition en écrivant une nouvelle structure sur le fichier de partition
 * supposé déja existant partitionName
 * Retourne 0 si l'initialisation a réussi, -1 sinon.
 * */
int initPartition(const char *partitionName);

/*
 * Retourne 1 si la partition existe.
 * 0 Sinon.
 * */
int partitionAlreadyExists(const char *filename);

/*
 * Libère la mémoire de la liste de pages d'un fichier.
 * */
void freePageAddressList(PageAddressList *head);

/*
 * Converti l'indice dans la partition en adresse physique dans la partition.
 * */
unsigned int convertAddressToPhysic(ushort address);

/*
* Ajoute un fichier à la FileTable.
* */
void addFileToTable(FileTable *table, File **file);

/*
 * Définit toutes les pages de la partition comme étant disponible en écrivant un
 * booléen au début de chaque page.
 * */
void setPartitionPagesAvailable(const char *partitionName);


/*
 * Retrouve la première page qui est libre dans la partition en se basant sur le booléen
 * écrit au début des pages.
 * */
ushort findAvailablePageAddress(Partition partition);

/*
 * Calcule et retourne le nombre total d'octets utilisés par les fichiers dans la partition.
 * */
int getUsedSpace(Partition partition);

/*
 * Lit le contenu de la partition à une adresse donnée.
 * */
bool readAtAddress(ushort address, Partition partition);

/*
 * Lit la structure Partition stockée au début de la partition du chemin d'accès partitionName donné.
 * La lecture s'effectue dans le même ordre que l'écriture pour ne ps avoir à déplacer le pointeur avec
 * la fonction lseek().
 * Retourne un pointeur vers la structure Partition lue, ou NULL en cas d'erreur.
 * */
Partition *readPartitionMetadata(const char *partitionName);

/*
 * Initialise un nouveau fichier sans adresse de première page et le retourne.
 * */
File *initNewFile(const char *filename, Partition partition);

/*
 * Définit une page comme étant non disponible en écrivant un booléen dans la partition à
 * l'adresse donnée.
 * */
void setPageAvailable(ushort address, const char *partitionName);

/*
 * Définit une page comme étant dispobile en écrivant un booléen dans la partition à
 * l'adresse donnée.
 * */
void setPageUnavailable(ushort address, const char *partitionName);

/*
 * Fonction pour rechercher un fichier par son nom.
 * Retourne NULL si aucun fichier n'est trouvé.
 * */
File *findFileWithName(FileTable *fileTable, const char *filename);

/*
 * Lit un booléen à l'offset donné de la partition.
 * */
bool readBooleanAtOffset(Partition partition, unsigned int offset);

/*
 * Ecrit les métadonnées de Partition au début de la partition au chemin d'accès partitionName donné.
 * Retourne 0 si l'écriture a réussi, -1 sinon.
 * */
int writePartitionMetadata(Partition *partition, const char *partitionName);

/*
 * Ecrit un booléen à l'offset donné de la partition.
 * */
void writeBooleanAtOffset(const char *partitionName, unsigned int offset, bool val);