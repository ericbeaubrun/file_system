#include "partition.h"

/*
 * Formate une partition avec le nom donné, la fonction retourne 0 si le formatage a réussi.
 * */
int myFormat(char *partitionName);

/*
 * Déplace le pointeur du fichier à l'offset donné,
 * Si l'offset est supérieur à la taille, le pointeur est placé à la fin du fichier.
 * Si l'offset est négatif, il est placé au début du fichier.
 * */
void mySeek(File *file, int offset, int base);

/*
 * Ouvre un fichier dans la partition.
 * Si le fichier n'existe pas, il est créé et la fonction retourne un pointeur vers la structure File.
 * (File représentant le fichier ouvert)
 * Sinon retourne NULL en, cas d'erreur.
 * */
File *myOpen(const char *fileName, Partition *partition);

/*
 * Lit le contenu d'un fichier dans la partition donnée.
 * Retourne le nombre d’octets lus.
 * */
int myRead(File *file, char *buffer, int nBytes, Partition *partition);

/*
 * Ecrit le contenu d'un fichier dans la partition donnée.
 * Retourne le nombre d’octets écrit.
 * */
int myWrite(File *file, const char *buffer, int nBytes, Partition *partition);

/*
 * Supprime un fichier de la partition.
 * Retourne 0 si le fichier a bien été supprimé.
 * */
int myDelete(const char *fileName, Partition *partition);


/*
 * Retourne la taille du fichier, -1 en cas de fichier NULL.
 * */
int mySize(File *file);