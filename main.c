#include "include/library.h"

void showPartitionInfo(Partition *partition);

void generateLoremIpsum(File *file, Partition *partition);

int main() {

    // La Partition dans laquelle on effectue toutes les manipulations de test
    Partition *partition = NULL;

    // Le fichier actuellement ouvert pour faire les tests
    File *file = NULL;

    // Tous les autres paramètres qui devront être saisi pour tester les fonctions
    int choice;
    char partitionName[PARTITION_METADATA_MAX_SIZE];
    char fileName[FILENAME_MAX_SIZE];
    char *content;
    int size;

    // Stocke les valeurs de retour lors des appels de fonctions
    int ret;

    printf("INFOS :\nDans ce programme de test :\n");
    printf("-Vous pouvez ouvrir un seul fichier à la fois.\n");
    printf("-Lorsqu'un fichier est ouvert, vous pouvez utilisez les fonctions de la bibliothèques.\n");
    printf("-Lorsque \">>\" s'affiche vous pouvez faire une saisie.\n");
    printf("ATTENTION à ne saisir de type invalide / chaine de caractère vide\n"
           "Cela pourrais provoquer des erreurs dans le programme et dans la partition\n\n");

    printf("Saisir le NOM DE LA PARTITION à formater avec \"myFormat\" (si elle existe déjà alors elle sera récupérée) :\n");
    printf(">>");

    // Attention : choisir un nom d'une taille pouvant rentrer dans partitionName.
    scanf("%s", &partitionName);
    printf("\n");

    // Formater / Charger une partition
    if (!partitionAlreadyExists(partitionName)) {
        ret = myFormat(partitionName);

        if (ret != 0) {
            perror("La partition n'a pas pu être formatée.\n");
            return -1;
        }
        printf("Partition formatée avec succès.\n");

    } else {
        printf("Partition chargée avec succès\n");
    }

    // Charger la partition existante dans la structure Partition pour manipuler les données
    partition = readPartitionMetadata(partitionName);


    // Menu
    while (1) {
        printf("\n******************************************************************\n");

        if (file == NULL) {
            printf("INFOS : AUCUN FICHIER N'EST OUVERT !\n");
        } else {
            printf("INFOS : FICHIER OUVERT \"%s\" (%d octets) (offset=%d)\n", file->name, file->size, file->offset);
        }
        printf("\n");
        printf("(1) - Ouvrir/Créer un fichier \t\t\t\t'myOpen'\n");
        printf("(2) - Placer le pointeur dans un fichier \t'mySeek'\n");
        printf("(3) - Ecrire dans un fichier \t\t\t\t'myWrite'\n");
        printf("(4) - Lire dans un fichier \t\t\t\t\t'myRead'\n");
        printf("(5) - Supprimer un fichier \t\t\t\t\t'myDelete'\n");
        printf("(6) - Générer Lorem Ipsum (255 Octets pour occuper plus de pages)\n");
        printf("(7) - Visualiser le contenu de la partition (fichiers, pages, ...)\n");
        printf("(8) - Quitter\n");
        printf("-----------------------------------------------------------------\n");

        printf("Saisir un NUMERO allant de (1) à (8) :\n");
        printf(">>");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Utilisation de myOpen(Partition *partition, const char *filename);
                printf("Saisir le NOM DU FICHIER à ouvrir ou créer :\n");
                printf(">>");
                scanf("%s", &fileName);
                printf("\n\n\n");

                file = myOpen(fileName, partition);

                if (file == NULL) {
                    printf("Une erreur est survenue lors de la création / ouverture du fichier !\n");
                } else {
                    printf("Le fichier \"%s\" a été ouvert ou créé avec succès !\n", file->name);
                }

                break;

            case 2: // Utilisation de mySeek(File *file, int offset);
                if (file == NULL) {
                    printf("Vous devez d'abord ouvrir un fichier pour effectuer cette action !\n\n\n");
                    break;
                }
                printf("-----------------------------------------------------------------\n");
                printf("(1) - Se placer au début du fichier\n");
                printf("(2) - Se placer à la fin du fichier\n");
                printf("(3) - Choisir ou se placer\n");
                printf(">>");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        setSeekToStart(file);
                        break;

                    case 2:
                        setSeekToEnd(file);
                        break;

                    case 3:
                        printf("Saisir l'emplacement :\n");
                        printf(">>");
                        scanf("%d", &choice);
                        printf("\n\n\n");
                        mySeek(file, choice);
                        break;
                }
                break;

            case 3: // Utilisation de myWrite(File *file, const char *buffer, int nBytes, Partition *partition);
                if (file == NULL) {
                    printf("Vous devez d'abord ouvrir un fichier pour effectuer cette action !\n\n\n");
                    break;
                }

                printf("Saisir la taille des données à écrire (nombre de caractères):\n");
                printf(">>");
                scanf("%d", &size);

                content = malloc(size + 1); // Ajouter 1 pour le "\0"

                printf("Saisir les données à écrire :\n");
                printf(">>");
                scanf("%s", content);
                printf("\n\n\n");

                // setSeekToEnd(file);
                ret = myWrite(file, content, size, partition);

                printf("Nombre d'octets écrits = %d\n", ret);

                free(content);
                break;

            case 4: // Utilisation de myRead(File *file, char *buffer, int nBytes, Partition *partition);
                if (file == NULL) {
                    printf("Vous devez d'abord ouvrir un fichier pour effectuer cette action !\n\n\n");
                    break;
                }

                printf("Saisir la taille des données à lire :\n");
                printf(">>");
                scanf("%d", &size);
                printf("\n\n\n");

                content = malloc(size + 1); // Ajouter 1 pour le "\0"

                // setSeekToStart(file);
                ret = myRead(file, content, size, partition);

                content[size] = '\0';

                printf("Nombre d'octets lus = %d\n", ret);
                if (ret > 0) {
                    printf("Contenu lu :\"%s\"\n", content);
                } else {
                    if (file->size == 0) {
                        printf("Aucun contenu lu car le fichier est vide !\n");
                    } else if (file->offset == file->size) {
                        printf("Aucun contenu lu car le curseur est à la fin du fichier !\n");
                    } else {
                        printf("Aucun contenu lu !\n");
                    }
                }

                free(content);
                break;

            case 5: // Utilisation de myDelete(const char *fileName, Partition *partition);
                if (file == NULL) {
                    printf("Vous devez d'abord ouvrir un fichier pour effectuer cette action !\n\n\n");
                    break;
                }
                printf("Tentative de suppression du fichier \"%s\"\n", file->name);
                ret = myDelete(file->name, partition);
                if (ret == 0) {
                    file = NULL;
                    printf("Fichier supprimé avec succès !\n");
                } else {
                    printf("Le fichier n'a pas pu être supprimé.");
                }
                break;

            case 6:
                if (file == NULL) {
                    printf("Vous devez d'abord ouvrir un fichier pour effectuer cette action !\n\n\n");
                    break;
                }
                generateLoremIpsum(file, partition);
                printf("\nContenu généré dans le fichier !\n\n\n");
                break;

            case 7:
                printf("\n\n\n");
                showPartitionInfo(partition);
                break;

            case 8 :
                printf("Fermeture du programme.\n");
                writePartitionMetadata(partition, partitionName);
                cleanup(partition);
                return 0;

            default:
                cleanup(partition);
                return -1;
        }
    }
    return 0;
}

void showPartitionInfo(Partition *partition) {
    printf("\n-----------------------------------------------------------------\n");
    printf("INFORMATIONS SUR LA PARTITION :\n");
    printf("Nom : \t\t\t\t\t\t\t\t\t\"%s\"\n", partition->name);
    printf("Nombre de fichiers : \t\t\t\t\t%d\n", partition->file_table.total_files);
    printf("Espace réservé aux métadonnées : \t\t%d Octets\n", PARTITION_METADATA_MAX_SIZE);
    printf("Nombre de pages utilisées : \t\t\t%d\n", getAllPageCount(*partition));
    printf("Nombre de pages max : \t\t\t\t\t%d\n", getMaxPagesAmount());
    printf("Taille de chaque page : \t\t\t\t%d Octets\n", PAGE_MAX_SIZE);
    printf("Espace utilisé par les fichiers : \t\t%d Octets\n", getUsedSpace(*partition));
    printf("Espace disponible : \t\t\t\t\t%d Octets\n",
           PARTITION_MAX_SIZE - PARTITION_METADATA_MAX_SIZE - getUsedSpace(*partition));

    printf("\nLISTE DES FICHIERS (et leurs pages) :\n");
    printf("\n");
    for (int i = 0; i < partition->file_table.total_files; ++i) {
        File *file = partition->file_table.files[i];
        printf("Nom du fichier : \"%s\" (%d Octets) (offset=%d)\n", file->name, file->size, file->offset);
        printf("Page(s) : ");

        PageAddressList *tmp = &file->first_page_address;
        while (tmp != NULL) {
            printf("[%d] -> ", tmp->address);
            tmp = tmp->next;
        }
        printf("[]");
        printf("\n\n");
    }
}

void generateLoremIpsum(File *file, Partition *partition) {
    int LOREM_IPSUM_SIZE = 255;
    char *LOREM_IPSUM_CONTENT = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce aliquam dui id purus tincidunt, "
                                "eget congue tortor feugiat. Maecenas a pellentesque purus. Phasellus felis eros, viverra ac "
                                "viverra sit amet, ornare a nisi. Sed luctus rhoncus faucibus turpis.";
    myWrite(file, LOREM_IPSUM_CONTENT, LOREM_IPSUM_SIZE, partition);
}