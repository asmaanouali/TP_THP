#include <stdio.h>
#include <string.h>

// Structure pour representer un automate
struct Automate {
    int tailleX, tailleS, tailleF, tailleI;
    char S0[50];
    char X[50][50];
    char S[50][50];
    char F[50][50];
    char I[50][3][50];
};

// Fonction pour verifier si une chaîne de caractères appartient à un ensemble de chaînes de caractères
int appartient(char element[], char ensemble[][50], int taille) {
    for (int i = 0; i < taille; i++) {
        if (strcmp(element, ensemble[i]) == 0) {
            return 1; // la chaîne de caractères appartient à l'ensemble
        }
    }
    return 0; // la chaîne de caractères n'appartient pas à l'ensemble
}

/*--------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------*/

// Procedure pour introduire un automate
void introduireAutomate(struct Automate *automate) {
    // Saisie de la taille de l'alphabet X
    printf("Entrez la taille de l'alphabet X : ");
    scanf("%d", &automate->tailleX);

    // Saisie de l'alphabet X, si l'automate contient des transitions epsilon on doit le declarer par e
    printf("Entrez les elements de l'alphabet X, utiliser e pour epsilon:\n");
    for (int i = 0; i < automate->tailleX; i++) {
        printf("X[%d] : ", i);
        scanf("%s", automate->X[i]);
    }

    // Saisie de la taille de l'ensemble des états S
    printf("Entrez la taille de l'ensemble des etats S : ");
    scanf("%d", &automate->tailleS);

    // Saisie de l'ensemble des états S
    printf("Entrez les elements de l'ensemble des etats S :\nEvitez d'utiliser les notations S0,S1... mais utilisez A,B,C..");
    printf("J'ai utilise les notations S0,S1.. pour les nouvels \netats dans la simplification\n");
    for (int i = 0; i < automate->tailleS; i++) {
        do {
            printf("S[%d] : ", i);
            scanf("%s", automate->S[i]);
            // Vérification de l'intersection vide entre X et S
            if (appartient(automate->S[i], automate->X, automate->tailleX)) {
                printf("Erreur : L'intersection entre X et S n'est pas vide.\n");
            }
        } while (appartient(automate->S[i], automate->X, automate->tailleX));
    }

    // Saisie de l'état initial S0 parmi les éléments de S
    do {
        printf("Selectionnez l'etat initial S0 parmi les elements de S : ");
        scanf("%s", automate->S0);
    } while (!appartient(automate->S0, automate->S, automate->tailleS));

    // Saisie de la taille de l'ensemble des états finaux F
    printf("Entrez la taille de l'ensemble des etats finaux F : ");
    scanf("%d", &automate->tailleF);

    // Saisie de l'ensemble des états finaux F
    printf("Entrez les elements de l'ensemble des etats finaux F :\n");
    for (int i = 0; i < automate->tailleF; i++) {
        do {
            printf("F[%d] : ", i);
            scanf("%s", automate->F[i]);
        } while (!appartient(automate->F[i], automate->S, automate->tailleS)); // Vérification que les états finaux appartiennent à l'ensemble des états S
    }

    // Saisie de la taille de l'ensemble des triplets I
    printf("Entrez la taille de l'ensemble des triplets I : ");
    scanf("%d", &automate->tailleI);

    // Saisie de l'ensemble des triplets I
    printf("Entrez les triplets de l'ensemble I (Si,xi,Sj) :\n");
    for (int i = 0; i < automate->tailleI; i++) {
        for (int j = 0; j < 3; j++) {
            printf("I[%d][%d] : ", i, j);
            scanf("%s", automate->I[i][j]);
        }
        printf("\n");

        // Vérification que Si et Sj appartiennent à l'ensemble des états S
        if (!appartient(automate->I[i][0], automate->S, automate->tailleS) || !appartient(automate->I[i][2], automate->S, automate->tailleS)) {
            printf("Erreur : Les etats dans les triplets doivent appartenir a l'ensemble des etats S.\n");
            return; // Fin de la fonction avec erreur
        }

        // Vérification que xi appartient à l'alphabet X
        char *xi = automate->I[i][1];
        int xi_length = strlen(xi);

        // Vérifiez que chaque sous-chaîne de xi appartient à X
        for (int k = 0; k < xi_length; k++) {
            char temp[2] = {xi[k], '\0'}; // Convertir le caractère en chaîne
            if (!appartient(temp, automate->X, automate->tailleX)) {
                printf("Erreur : Les symboles xi dans les triplets doivent appartenir à l'alphabet X.\n");
                return; // Fin de la fonction avec erreur
            }
        }
    }
}

/*--------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------*/

// Procedure pour afficher les détails de l'automate
void afficherDetailsAutomate(struct Automate automate) {
    // Affichage de l'alphabet X
    printf("\nAlphabet X : {");
    for (int i = 0; i < automate.tailleX; i++) {
        printf("%s", automate.X[i]);
        if (i < automate.tailleX - 1) {
            printf(", ");
        }
    }
    printf("}\n");

    // Affichage de l'ensemble des états S
    printf("Ensemble des etats S : {");
    for (int i = 0; i < automate.tailleS; i++) {
        printf("%s", automate.S[i]);
        if (i < automate.tailleS - 1) {
            printf(", ");
        }
    }
    printf("}\n");

    // Affichage de l'état initial S0
    printf("Etat initial S0 : %s\n", automate.S0);

    // Affichage de l'ensemble des états finaux F
    printf("Ensemble des etats finaux F : {");
    for (int i = 0; i < automate.tailleF; i++) {
        printf("%s", automate.F[i]);
        if (i < automate.tailleF - 1) {
            printf(", ");
        }
    }
    printf("}\n");

    // Affichage de l'ensemble des triplets I
    printf("Ensemble des triplets I :\n");
    for (int i = 0; i < automate.tailleI; i++) {
        printf("(%s, %s, %s)\n", automate.I[i][0], automate.I[i][1], automate.I[i][2]);
    }
}

/*--------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------*/

/* Procedure pour recuperer les etats accessibles depuis un etat de depart
ça nous servira à trouver les états accessibles en l'appliquant sur l'état initial,
et les etats co-accessibles en l'appliquant sur chacun des états et voir si un des
états finaux en fait partie du résultat */

void etatsAccessibles(struct Automate automate, char etatDepart[], char etatsAccessibles[], int *tailleEtatsAccessibles) {
    // Initialisation du tableau d'états accessibles
    *tailleEtatsAccessibles = 0;

    // Tableau pour marquer les états déjà visités afin d'éviter une boucle infinie
    int etatsVisites[automate.tailleS];
    for (int i = 0; i < automate.tailleS; i++) {
        etatsVisites[i] = 0;
    }

    // Pile pour effectuer un parcours en profondeur
    char pile[automate.tailleS][50];
    int sommetPile = -1;

    // Empiler l'état initial
    strcpy(pile[++sommetPile], etatDepart);

while (sommetPile != -1) {
    // Dépiler un état
    char etatCourant[50];
    strcpy(etatCourant, pile[sommetPile--]);

    // Marquer l'état comme visité
    int indexEtatCourant = -1;
    for (int i = 0; i < automate.tailleS; i++) {
        if (strcmp(etatCourant, automate.S[i]) == 0) {
            indexEtatCourant = i;
            break;
        }
    }
    etatsVisites[indexEtatCourant] = 1;

    // Ajouter l'état courant au tableau des états accessibles
    strcpy(etatsAccessibles + (*tailleEtatsAccessibles * 50), etatCourant);
    (*tailleEtatsAccessibles)++;

    // Parcourir les transitions sortantes de l'état courant
    for (int i = 0; i < automate.tailleI; i++) {
        if (strcmp(automate.I[i][0], etatCourant) == 0) {
            // Si la transition existe, empiler l'état suivant si non visité
            char etatSuivant[50];
            strcpy(etatSuivant, automate.I[i][2]);

            // Trouver l'index de l'état suivant dans l'ensemble des états S
            int indexEtatSuivant = -1;
            for (int j = 0; j < automate.tailleS; j++) {
                if (strcmp(etatSuivant, automate.S[j]) == 0) {
                    indexEtatSuivant = j;
                    break;
                }
            }

            if (!etatsVisites[indexEtatSuivant]) {
                strcpy(pile[++sommetPile], etatSuivant);
            }
        }
    }
}
}

/*--------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------*/

/* Procedure pour reduire l'automate en supprimant les etats non accessibles
ou non accessibles detectés par la procédure precedente */

void reduireAutomate(struct Automate *automate) {
    // Étape 1 : Appliquer la méthode etatsAccessibles sur l'état initial
    char etatsAccessiblesDepuisS0[50 * automate->tailleS];
    int tailleEtatsAccessibles = 0;
    etatsAccessibles(*automate, automate->S0, etatsAccessiblesDepuisS0, &tailleEtatsAccessibles);

    // Tableau pour marquer les états à supprimer
    int etatsASupprimer[automate->tailleS];
    for (int i = 0; i < automate->tailleS; i++) {
        etatsASupprimer[i] = 1; // Initialiser à 1 (supprimer par défaut)
    }
     // Étape 2 : Appliquer la méthode etatsAccessibles sur les états accessibles depuis S0
for (int i = 0; i < automate->tailleF; i++) {
    char etatsAccessiblesDepuisF[50 * automate->tailleS];
    int tailleEtatsAccessiblesF = 0;
    etatsAccessibles(*automate, automate->F[i], etatsAccessiblesDepuisF, &tailleEtatsAccessiblesF);

    // Marquer tous les états finaux accessibles comme ne devant pas être supprimés
    for (int j = 0; j < tailleEtatsAccessiblesF; j++) {
        for (int k = 0; k < automate->tailleS; k++) {
            if (strcmp(etatsAccessiblesDepuisF + (j * 50), automate->S[k]) == 0) {
                etatsASupprimer[k] = 0; // Ne pas supprimer cet état
                break;
            }
        }
    }
}


   // Supprimer les états marqués pour suppression
for (int i = automate->tailleS - 1; i >= 0; i--) {
    if (etatsASupprimer[i]) {
        // Supprimer les triplets associés à l'état supprimé
        for (int j = 0; j < automate->tailleI; j++) {
            if (strcmp(automate->I[j][0], automate->S[i]) == 0 || strcmp(automate->I[j][2], automate->S[i]) == 0) {
                // Supprimer le triplet j
                for (int k = j; k < automate->tailleI - 1; k++) {
                    strcpy(automate->I[k][0], automate->I[k + 1][0]);
                    strcpy(automate->I[k][1], automate->I[k + 1][1]);
                    strcpy(automate->I[k][2], automate->I[k + 1][2]);
                }
                automate->tailleI--;
                j--; // Décrémenter j car la taille de l'ensemble a diminué
            }
        }

        // Supprimer l'état i
        for (int j = i; j < automate->tailleS - 1; j++) {
            strcpy(automate->S[j], automate->S[j + 1]);
        }
        automate->tailleS--;

        // Ajuster les indices pour éviter des erreurs
        i--; // Décrémenter i car la taille de l'ensemble a diminué
    }
}
}

/*--------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------*/

// Procedure pour decomposer une transition ( element de I ) si |xi|>1
void genererNouvelEtat(char prefixe[], int numero, char nouvelEtat[]) {
    sprintf(nouvelEtat, "%s", prefixe);
    for (int i = 0; i < numero; i++) {
        strcat(nouvelEtat, "S");
    }
}

void decomposerElement(struct Automate *automate, int *tailleI, char element[]) {
    int length = strlen(element);
    char etatCourant[50];
    char etatSuivant[50];

    strcpy(etatCourant, element);

    for (int i = 0; i < length - 1; i++) {
        // Créer un nouvel état pour chaque caractère de xi
        char nouvelEtat[50];
        genererNouvelEtat("S", i + 1, nouvelEtat);

        // Vérifier si l'état existe déjà
        int etatExisteDeja = 0;
        for (int j = 0; j < automate->tailleS; j++) {
            if (strcmp(nouvelEtat, automate->S[j]) == 0) {
                etatExisteDeja = 1;
                break;
            }
        }

        if (!etatExisteDeja) {
            // Ajouter la transition (Si, xi[i], nouvelEtat)
            strcpy(automate->I[*tailleI][0], etatCourant);
            strcpy(automate->I[*tailleI][1], nouvelEtat);
            strcpy(automate->I[*tailleI][2], "");

            (*tailleI)++;
            // Mettre à jour l'état courant pour la transition suivante
            strcpy(etatCourant, nouvelEtat);
        }
        // Si l'état existe déjà, ignorez la transition
    }

    // Ajouter la transition finale (dernier état de la décomposition, xi[length-1], Sj)
    char dernierNouvelEtat[50];
    genererNouvelEtat("S", length - 1, dernierNouvelEtat);

    // Vérifier si l'état existe déjà
    int dernierEtatExisteDeja = 0;
    for (int j = 0; j < automate->tailleS; j++) {
        if (strcmp(dernierNouvelEtat, automate->S[j]) == 0) {
            dernierEtatExisteDeja = 1;
            break;
        }
    }

    if (!dernierEtatExisteDeja) {
        strcpy(automate->I[*tailleI][0], etatCourant);
        strcpy(automate->I[*tailleI][1], &element[length - 1]);
        strcpy(automate->I[*tailleI][2], dernierNouvelEtat);

        (*tailleI)++;
    }
    // Si l'état existe déjà, ignorez la transition
}


/*--------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------*/
int etatExisteDeja(const char *etat, struct Automate *automate) {
    for (int i = 0; i < automate->tailleS; i++) {
        if (strcmp(etat, automate->S[i]) == 0) {
            return 1;  // L'état existe déjà
        }
    }
    return 0;  // L'état n'existe pas encore
}

void insererEtatDansS(struct Automate *automate, char *etat) {
    // Vérifier si l'état existe déjà dans l'ensemble S
    for (int i = 0; i < automate->tailleS; i++) {
        if (strcmp(automate->S[i], etat) == 0) {
            // L'état existe déjà, générer un nouvel état
            char nouvelEtat[10];
            sprintf(nouvelEtat, "S%d", automate->tailleS);

            // Ajouter le nouvel état à l'ensemble S
            strcpy(automate->S[automate->tailleS], nouvelEtat);
            automate->tailleS++;

            // Terminer la fonction avec le nouvel état généré
            return;
        }
    }

    // L'état n'existe pas, l'ajouter à l'ensemble S
    strcpy(automate->S[automate->tailleS], etat);
    automate->tailleS++;
}


/* Procedure pour rendre un automate partiellement généralisé en decomposant
chaque element de I ou |xi|>0 */

void partiellementGeneralise(struct Automate *automate) {
    // Nouvelle taille de l'ensemble I après la généralisation
    int nouvelleTailleI = automate->tailleI;

    for (int i = 0; i < automate->tailleI; i++) {
        char *xi = automate->I[i][1];
        int xi_length = strlen(xi);

        if (xi_length > 1) {
            // Créer un nouvel état pour chaque caractère de xi
            char etatActuel[10];
            strcpy(etatActuel, automate->I[i][0]);

            for (int j = 0; j < xi_length - 1; j++) {
                char nouvelEtat[10];
                sprintf(nouvelEtat, "S%d", nouvelleTailleI);

                // Ajouter la transition (état actuel, xi[j], nouvel état)
                strcpy(automate->I[nouvelleTailleI][0], etatActuel);
                strncpy(automate->I[nouvelleTailleI][1], xi + j, 1);
                strcpy(automate->I[nouvelleTailleI][2], nouvelEtat);

                // Insérer le nouvel état dans S
                insererEtatDansS(automate, nouvelEtat);

                etatActuel[0] = '\0';  // Réinitialiser l'état actuel pour la transition suivante
                strcpy(etatActuel, nouvelEtat);

                nouvelleTailleI++;
            }

            // Ajouter la transition finale (dernier état de la décomposition, xi[length-1], Sj)
            strcpy(automate->I[nouvelleTailleI][0], etatActuel);
            strncpy(automate->I[nouvelleTailleI][1], xi + xi_length - 1, 1);
            strcpy(automate->I[nouvelleTailleI][2], automate->I[i][2]);

            // Nettoyer l'élément initial
            strcpy(automate->I[i][0], "");
            strcpy(automate->I[i][1], "");
            strcpy(automate->I[i][2], "");

            nouvelleTailleI++;
        }
    }

    // Mettre à jour la taille de l'ensemble I
    automate->tailleI = nouvelleTailleI;
}


/*--------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------*/
/*Procedure pour supprimer les elements de I qu'on a vidé aka les elements
initiaux avant la décomposition*/
void supprimerElementsVides(struct Automate *automate) {
    int nouvelleTailleI = 0;

    for (int i = 0; i < automate->tailleI; i++) {
        if (strcmp(automate->I[i][0], "") != 0 || strcmp(automate->I[i][1], "") != 0 || strcmp(automate->I[i][2], "") != 0) {
            // Copier l'élément non vide dans la nouvelle position
            strcpy(automate->I[nouvelleTailleI][0], automate->I[i][0]);
            strcpy(automate->I[nouvelleTailleI][1], automate->I[i][1]);
            strcpy(automate->I[nouvelleTailleI][2], automate->I[i][2]);

            nouvelleTailleI++;
        }
        // Sinon, l'élément est vide, donc on ne le copie pas
    }

    // Mettre à jour la taille de l'ensemble I
    automate->tailleI = nouvelleTailleI;
}

/*--------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------*/
int existeDansF(struct Automate *automate, const char *etat) {
    for (int i = 0; i < automate->tailleF; i++) {
        if (strcmp(automate->F[i], etat) == 0) {
            // l'état existe dans l'ensemble F
            return 1;
        }
    }
    return 0;
}
void insererDansF(struct Automate *automate, const char *etat) {
    for (int i = 0; i < automate->tailleF; i++) {
        if (strcmp(automate->F[i], etat) == 0) {
            // l'état existe déjà dans l'ensemble F
            return;
        }
    }
    strcpy(automate->F[automate->tailleF], etat);
    automate->tailleF++;
}

void eliminerTransitionsEpsilon(struct Automate *automate) {
    // Copie temporaire de l'ensemble I avant la modification
    char tempI[50][3][50];
    int tempTailleI = automate->tailleI;

    for (int i = 0; i < tempTailleI; i++) {
        strcpy(tempI[i][0], automate->I[i][0]);
        strcpy(tempI[i][1], automate->I[i][1]);
        strcpy(tempI[i][2], automate->I[i][2]);
    }

    // Parcours de l'ensemble I pour détecter les transitions epsilon
    for (int i = 0; i < tempTailleI; i++) {
        if (strcmp(tempI[i][1], "e") == 0) {
            // Si la transition est epsilon, éliminer et remplacer
            char etatCourant[50];
            char etatSuivant[50];

            strcpy(etatCourant, tempI[i][0]);
            strcpy(etatSuivant, tempI[i][2]);

            for (int j = 0; j < tempTailleI; j++) {
                if (strcmp(tempI[j][0], etatSuivant) == 0) {
                    // Pour chaque transition sortante de l'étatSuivant, ajouter une transition de etatCourant à etatSuivant
                    if (strcmp(tempI[j][1], "e") != 0) {
                        // Vérifier si la transition n'existe pas déjà
                        int existeDeja = 0;
                        for (int k = 0; k < automate->tailleI; k++) {
                            if (strcmp(automate->I[k][0], etatCourant) == 0 &&
                                strcmp(automate->I[k][1], tempI[j][1]) == 0 &&
                                strcmp(automate->I[k][2], tempI[j][2]) == 0) {
                                existeDeja = 1;
                                break;
                            }
                        }

                        // Ajouter la nouvelle transition uniquement si elle n'existe pas déjà
                        if (!existeDeja) {
                            strcpy(automate->I[automate->tailleI][0], etatCourant);
                            strcpy(automate->I[automate->tailleI][1], tempI[j][1]);
                            strcpy(automate->I[automate->tailleI][2], tempI[j][2]);
                            automate->tailleI++;
                        }
                    }
                }
            }

       if(existeDansF(automate,automate->I[i][2])){
            insererDansF(automate,automate->I[i][0]);
       }
       strcpy(automate->I[i][0], "");
       strcpy(automate->I[i][1], "");
       strcpy(automate->I[i][2], ""); // on vide l'état initial pour qu'on puisse le supprimer après


        }
    }


}

/*-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
/*--------------------------PROGRAMME PRINCIPAL--------------------------------*/
int main() {
    // Déclaration et initialisation de la structure Automate
    struct Automate monAutomate = {0};


    introduireAutomate(&monAutomate);

    printf("Est ce que votre automate est reduit, \nsi OUI repondez par 1 sinon par 0? \n ") ;
    // si l'automate est déjà réduit, on ne passe pas par la phase de réduction et on commence directement la simplification
    int n;
    scanf("%d",&n) ;
    switch(n)
    {case 0 : // cas automate non réduit
      {
       reduireAutomate(&monAutomate);
       printf("\nAutomate reduit :\n");
       afficherDetailsAutomate(monAutomate);
       partiellementGeneralise(&monAutomate);
       supprimerElementsVides(&monAutomate);
       // Affichage des détails de l'automate après la réduction
       printf("\nAutomate partiellement generalise :\n");
       afficherDetailsAutomate(monAutomate);

      } break;
     case 1 : // cas automate réduit
        {
           partiellementGeneralise(&monAutomate);
           supprimerElementsVides(&monAutomate);
          // Affichage des détailles de l'automate après la généralisation
           printf("\nAutomate partiellement generalise :\n");
           afficherDetailsAutomate(monAutomate);
        }
    }

        eliminerTransitionsEpsilon(&monAutomate);
        supprimerElementsVides(&monAutomate);

    printf("\nElimination d'Epsilon \nAutomate simple final:");
    afficherDetailsAutomate(monAutomate);



    return 0;
}
