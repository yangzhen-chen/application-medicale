/****************************************************************
Author: Chenghao WANG
Contact: chenghao.wang@hds.utc.fr
Institute: Heudiasyc, université de technologie de compiègne
Date: 12/10/2020
*****************************************************************/
#include "../include/TP3.h"

/**
 * @brief Ajout d’un soigneur, où la liste des intervalles de temps disponibles pour un nouveau soigneur
 * est initialisée avec un seul intervalle [0, inf[.
 * @param listeSoigneurs une liste de soigneurs laquelle ne doit pas être vide.
 * @param idSoi un identifiant d'un soigneur.
 * @param nom le nom d'un sogineur.
 * @param prenom le prénom d'un soigneur.
 */
T_Soigneur *ajouterSoigneur
        (T_Soigneur *listeSoigneurs, Index_Soigneur idSoi, char *nom, char *prenom) {
    T_Soigneur *s = malloc(sizeof(T_Soigneur));
    s->prenom = malloc(40 * sizeof(char));
    s->nom = malloc(40 * sizeof(char));
    s->listeIntervalle = malloc(sizeof(T_Intervalle));

    s->id_soi = idSoi;
    strcpy(s->nom, nom);
    strcpy(s->prenom, prenom);
    s->listeIntervalle->debut = 0;
    s->listeIntervalle->fin = 1000000;
    s->listeIntervalle->suivant = NULL;
    s->suivant = NULL;
    if (listeSoigneurs == NULL) return s;
    T_Soigneur *s_temp = listeSoigneurs;
    while (s_temp->suivant != NULL) {
        s_temp = s_temp->suivant;
    }

    s_temp->suivant = s;
    return listeSoigneurs;

}

/**
 * @brief Ajout d'un patient, où la liste de rendez-vous médicaux pour un nouveau patient est initialement vide.
 * @param listePatients une liste de patients laquelle ne doit pas être vide.
 * @param idPat un identifiant d'un patient.
 * @param nom le nom d'un patient.
 * @param prenom le prénom d'un patient.
 */
T_Patient *ajouterPatient
        (T_Patient *listePatients,
         Index_Patient idPat,
         char *nom,
         char *prenom) {
    T_Patient *p = malloc(sizeof(T_Patient));
    p->prenom = malloc(40 * sizeof(char));
    p->nom = malloc(40 * sizeof(char));

    p->id_pat = idPat;
    strcpy(p->nom, nom);
    strcpy(p->prenom, prenom);
    p->listeRendezVous = NULL;
    p->suivant = NULL;
    if (listePatients == NULL) return p;
    T_Patient *p_temp = listePatients;
    while (p_temp->suivant != NULL) {
        p_temp = p_temp->suivant;
    }

    p_temp->suivant = p;

    return (listePatients);

}

/**
 * @brief Ajout d’un rendez-vous médical pour un patient.
 * @param listeRdV une liste de Rendezvous laquelle ne doit pas être vide.
 * @param idSoi un identifiant d'un soigneur associé.
 * @param dateDebutSouhaitee  la date de début souhaitée d'un RdV.
 * @param dateFinSouhaitee  la date de fin souhaitée d'un RdV.
 * @param tempsDeplacement la temps de déplacement depuis un RdV précédent.
 * @param desc une discription brève.
 */
T_RendezVous *ajouterRendezVous//加入预约
        (T_RendezVous *listeRdV,
         Index_Soigneur idSoi,
         Time dateDebutSouhaitee,
         Time dateFinSouhaitee,
         Time tempsDeplacement,
         char *desc) {
    T_RendezVous *r = malloc(sizeof(T_RendezVous));
    r->desc = malloc(125 * sizeof(char));

    strcpy(r->desc, desc);
    r->debut_souhaitee = dateDebutSouhaitee;
    r->fin_souhaitee = dateFinSouhaitee;
    r->temps_deplacement = tempsDeplacement;
    r->id_soi = idSoi;
    r->debut_affectee = 0;
    r->fin_affectee = 0;
    r->suivant = NULL;
    if (listeRdV == NULL) return r;
    T_RendezVous *r_temp = listeRdV;
    while (r_temp->suivant != NULL) {
        r_temp = r_temp->suivant;
    }
    r_temp->suivant = r;

    return listeRdV;
}

/**
 * @brief Modification d’ un rendez-vous médical pour un patient par une date, le temps de déplacement ou une description nouvelle :
 * @param RdV un Rendezvous déja trouvé
 * @param idSoi un identifiant d'un soigneur associé.
 * @param dateDebutSouhaitee la date de début souhaitée d'un RdV.
 * @param dateFinSouhaitee la date de fin souhaitée d'un RdV.
 * @param tempsDeplacement la temps de déplacement depuis un RdV précédent.
 * @param desc une discription brève.
 */
void modifierRendezVous//ici le rdv est déja trouvé
        (T_RendezVous *RdV,
         Index_Soigneur idSoi,
         Time dateDebutSouhaitee,
         Time dateFinSouhaitee,
         Time tempsDeplacement,
         char *desc) {
    RdV->id_soi = idSoi;
    RdV->debut_souhaitee = dateDebutSouhaitee;
    RdV->fin_souhaitee = dateFinSouhaitee;
    RdV->temps_deplacement = tempsDeplacement;
    strcpy(RdV->desc, desc);
}

/**
 * @brief Suppression d’un rendez-vous médical pour un patient en donnant l’identifiant du soigneur correspondant.
 * Si un rendez-vous, ni premier ni dernier, est enlevé, le temps de déplacement du traitement suivant doit être mis à jour.
 * Par exemple, un patient ayant trois rendez-vous : rendez-vous 1 chez le soigneur A,
 * rendez-vous 2 chez le soigneur B et rendez-vous 3 chez le soigneur C, temps de déplacement de 1 à 2 de 30 mn,
 * temps de déplacement de 2 à 3 de 40 mn, le temps de déplacement de 1 à 3 est défini
 * par sqrt(30^2+40^2)=50mn quand le rendez-vous 2 est supprimé.
 * @param listeRdV une liste de RdV.
 * @param idSoi identifiant d'un soigneur.
 * @return un pointeur vers la tête de la liste de RdV médicaux après une suppression.
 */
T_RendezVous *supprimerRendezVous
        (T_RendezVous *listeRdV, Index_Soigneur idSoi) {
    T_RendezVous *p_suc = listeRdV;
    T_RendezVous *p_pre;
    if (listeRdV == NULL) {
        printf("le liste est vide\n");
        return NULL;
    } else {//listeRDV n'est pas NULL
        if (p_suc->id_soi == idSoi) {//le RDV qu'on veut supprimer dans la tete
            listeRdV = listeRdV->suivant;
            listeRdV->temps_deplacement = 0;
            free(p_suc->desc);
            free(p_suc);
            return listeRdV;
        }
        while (p_suc->id_soi != idSoi && p_suc != NULL) {//cherche le RDV qu'on veut supprimer
            p_pre = p_suc;
            p_suc = p_suc->suivant;
        }
        if (p_suc == NULL) {
            printf("ce rendez-vous n'est pas trouvé");
            return NULL;
        } else {//trouver le RDV
            if (p_suc->suivant != NULL) {
                p_suc->suivant->temps_deplacement = sqrt(
                        (p_suc->suivant->temps_deplacement) * (p_suc->suivant->temps_deplacement) +
                        (p_suc->temps_deplacement) * (p_suc->temps_deplacement));
            }
            p_pre->suivant = p_suc->suivant;
        }
        free(p_suc->desc);
        free(p_suc);
        return listeRdV;
    }
}


/**
 * @brief Afficher tous les soigneurs et leurs intervalles de temps disponibles
 * @param listeSoigneurs une liste de soigneurs.
 */
void affichage_Soigneurs(T_Soigneur *listeSoigneurs) {
    //return provided_affichage_Soigneurs(listeSoigneurs);
    printf("Les informations de soigneurs sont:\n");
    T_Soigneur *s = listeSoigneurs;
    while (s != NULL) {//boucle pour soigneur
        printf("Id:%d, Nom:%s, Prénom:%s, Intervalles de temps disponibles:\n", s->id_soi, s->nom, s->prenom);
        T_Intervalle *i = s->listeIntervalle;
        while (i != NULL) {//pour intervalle de chaque soigneur
            printf("           [ %d , %d [\n", i->debut, i->fin);
            i = i->suivant;
        }
        s = s->suivant;
    }
    printf("==================================\n");
}


/**
 * @brief Afficher un rendez-vous en indiquant l’identifiant du patient et le soigneur correspondant
 * @param rendezVous un rendez-vous.
 */
void affichage_RendezVous(T_RendezVous *rendezVous) {//affichage pour tous les rdvs
    //return provided_affichage_un_RendezVous(rendezVous);
    T_RendezVous *r = rendezVous;
    while (r != NULL) {
        printf("     Desc:%s,id soigneur associé:%d ,temps de déplacement depuis rdv précédent:%d\n",
               r->desc, r->id_soi, r->temps_deplacement);
        printf("        Rdv souhaité:[%d , %d[\n", r->debut_souhaitee, r->fin_souhaitee);
        printf("        Rdv affecté :[%d , %d[\n", r->debut_affectee, r->fin_affectee);
        r = r->suivant;
    }
}

/**
 * @brief Afficher tous les patients et leurs rendez-vous
 * @param listePatients une liste de patients.
 */
void affichage_Patients(T_Patient *listePatients) {
    printf("Les informations de patients sont :\n");
    T_Patient *p = listePatients;
    while (p != NULL) {
        printf("Id: %u, Nom:%s, Prénom:%s, Ses Rdv médicaux:\n", p->id_pat, p->nom, p->prenom);
        affichage_RendezVous(p->listeRendezVous);
        p = p->suivant;
    }
    printf("================================\n");
}


/**
 * @brief Créer une instance de l’ordonnancement en important à partir d’un fichier.
 * @param filename le nom d'un fichier importé.
 * @return un pointeur vers l’instance.
 */
T_Ordonnancement *creerInstance(char *filename) {
    //return provided_creerInstance(filename);
    FILE *fp = fopen(filename, "r");
    int nb_patiants;
    int nb_soigneur;
    int nb_rdv;
    Index_Patient id_patient;
    Index_Soigneur id_soigneur;
    Time date_debut_s;
    Time date_fin_s;
    Time date_deplacement;
    char descripton[125];
    T_Patient *patientactuel;


    T_Ordonnancement *o;
    o = (T_Ordonnancement *) malloc(sizeof(T_Ordonnancement));
    o->date = (char *) malloc(125 * sizeof(char));
    o->listeSoigneurs = NULL;
    o->listePatients = NULL;


    fscanf(fp, "%u %u\n", &nb_patiants, &nb_soigneur);
    for (int i = 0; i < nb_patiants; i++) {//boucle pour les patients
        char nom[40];
        char prenom[40];
        fscanf(fp, "%u %u %s %s\n", &id_patient, &nb_rdv, nom, prenom);
        o->listePatients = ajouterPatient(o->listePatients, id_patient, nom, prenom);
        T_Patient *p_temp = o->listePatients;
        while (p_temp->suivant != NULL) {
            p_temp = p_temp->suivant;
        }
        for (int j = 0; j < nb_rdv; j++) {//boucle pour les rendez-vous
            fscanf(fp, "%u %u %u %u %s\n", &id_soigneur, &date_debut_s, &date_fin_s, &date_deplacement, descripton);
            p_temp->listeRendezVous = ajouterRendezVous(p_temp->listeRendezVous, id_soigneur, date_debut_s, date_fin_s,
                                                        date_deplacement, descripton);
        }
    }

    for (int i = 0; i < nb_soigneur; i++) {//boucle pour soigneur
        char nom[40];
        char prenom[40];
        fscanf(fp, "%u %s %s\n", &id_soigneur, nom, prenom);
        o->listeSoigneurs = ajouterSoigneur(o->listeSoigneurs, id_soigneur, nom, prenom);
    }
    struct tm *ptr;
    time_t lt;
    lt = time(NULL);
    ptr = localtime(&lt);
    strftime(o->date, 125*sizeof(char), "%Y-%m-%d", ptr);
    fclose(fp);
    return o;
}

/**
 * @brief Affectation d’un rendez-vous en fonction des intervalles de temps disponibles d’un soigneur
 * (mettre à jour la date de début affectée et la date de fin affectée du rendez-vous ) 
 * @param rdv un RdV.
 * @param soigneur un soigneur.
 */
void affecterRdV(T_RendezVous *rdv, T_Soigneur *soigneur) {
    T_Intervalle *t1;
    T_Intervalle *t2 = soigneur->listeIntervalle;
    if (soigneur->listeIntervalle->suivant == NULL) {            //cas il y a seule intervalle dans listeintervalle
        if (rdv->debut_affectee == 0 && rdv->fin_affectee == 0) {// il y a pas de contrainte de temps de déplacemnt
            rdv->debut_affectee = rdv->debut_souhaitee;          //donc le temps affecteé==souhaiteé
            rdv->fin_affectee = rdv->fin_souhaitee;
        }                                                        //else rdv->debut_affecte et fin_affecte ne change pas
        if (t2->debut == rdv->debut_affectee &&
            t2->fin != rdv->fin_affectee) {                      //pour debut:rdv=inter pour fin:rdv!=inter,générer une nouvelle inter
            T_Intervalle *i = (T_Intervalle *) malloc(sizeof(T_Intervalle));
            i->debut = rdv->fin_affectee;
            i->fin = t2->fin;
            i->suivant = NULL;
            soigneur->listeIntervalle = i;
            free(t2);
            return;
        } else {                                                //pour debut et fin:rdv!=inter generer deux inter
            T_Intervalle *i1 = malloc(sizeof(T_Intervalle));    // seule deux cas
            T_Intervalle *i2 = malloc(sizeof(T_Intervalle));    //parce que le cas de seule intervalle est[0,inf]
            i1->debut = t2->debut;                              //donc pas le cas de t2->debut==rdv->debut && t2->fin==rdv->fin
            i1->fin = rdv->debut_affectee;                      //aussi pas le cas de t2->debut!=rdv->debut && t2->fin==rdv->fin
            i2->debut = rdv->fin_affectee;
            i2->fin = t2->fin;
            i2->suivant = NULL;
            i1->suivant = i2;
            soigneur->listeIntervalle = i1;

            free(t2);
            return;
        }
    }
    //cas il y a plusieur intervalle
    if (rdv->debut_affectee == 0 && rdv->fin_affectee == 0) {//cas il n'y a pas de contrainte de temps de déplacement
        t1 = soigneur->listeIntervalle;
        t2 = soigneur->listeIntervalle;
        Time time_rdv = (rdv->fin_souhaitee) - (rdv->debut_souhaitee);
        while (((t2->fin - t2->debut) < time_rdv || t2->fin < rdv->fin_souhaitee) && t2 != NULL) {//cherche intervalle
            t1 = t2;
            t2 = t2->suivant;
        }
        if (t2 != NULL) {
            if (t2->debut <= rdv->debut_souhaitee && t2->fin >= rdv->fin_souhaitee) {//Les horaires du rendez-vous souhaité appartiennent à l'inter
                rdv->debut_affectee = rdv->debut_souhaitee;                          //donc pas besoin de changer le temps souhaiteé
                rdv->fin_affectee = rdv->fin_souhaitee;
                if (rdv->debut_affectee == t2->debut && rdv->fin_affectee == t2->fin) {//pour debut et fin:rdv=inter, supprimer inter
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = t2->suivant;
                    else t1->suivant = t2->suivant;
                    free(t2);
                    return;
                } else if (t2->debut < rdv->debut_affectee  && rdv->fin_affectee < t2->fin) {//générer deux nouvelle inter
                    T_Intervalle *i1 = malloc(sizeof(T_Intervalle));
                    T_Intervalle *i2 = malloc(sizeof(T_Intervalle));
                    i1->debut = t2->debut;
                    i1->fin = rdv->debut_affectee;
                    i2->debut = rdv->fin_affectee;
                    i2->fin = t2->fin;
                    i1->suivant = i2;
                    i2->suivant = t2->suivant;
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = i1;
                    else t1->suivant = i1;
                    free(t2);
                    return;
                } else if (t2->debut == rdv->debut_affectee && t2->fin > rdv->fin_affectee) {//générer une nouvelle inter
                    T_Intervalle *i = malloc(sizeof(T_Intervalle));
                    i->debut = rdv->fin_affectee;
                    i->fin = t2->fin;
                    i->suivant = t2->suivant;
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = i;
                    else t1->suivant = i;
                    free(t2);
                    return;
                } else {                                                                      //générer une nouvelle inter
                    T_Intervalle *i = malloc(sizeof(T_Intervalle));
                    i->debut = t2->debut;
                    i->fin = rdv->debut_affectee;
                    i->suivant = t2->suivant;
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = t2->suivant;
                    else t1->suivant = t2->suivant;
                    free(t2);
                    return;
                }
            } else {                                                     //Les horaires du rendez-vous souhaité n'appartiennent pas à l'inter
                rdv->debut_affectee = t2->debut;                         //changer le temps pour rdv
                rdv->fin_affectee = rdv->debut_affectee + time_rdv;
                if ((t2->fin - t2->debut) == time_rdv) {                 //dureé de rdv=durée de inter, supprimer inter
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = t2->suivant;
                    else t1->suivant = t2->suivant;
                    free(t2);
                    return;
                } else {                                                 //dureé de rdvx!=durée de inter, générer une nouvelle inter
                    T_Intervalle *i = malloc(sizeof(T_Intervalle));
                    i->debut = rdv->fin_affectee;
                    i->fin = t2->fin;
                    i->suivant = t2->suivant;
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = i;
                    else t1->suivant = i;
                    free(t2);
                    return;
                }
            }
        }
    } else {                                                                        //il y a contrainte de temps de déplacement
        t1 = soigneur->listeIntervalle;
        t2 = soigneur->listeIntervalle;
        Time time_rdv = (rdv->fin_souhaitee) - (rdv->debut_souhaitee);
        while (((t2->fin - t2->debut) < time_rdv || t2->fin < rdv->fin_affectee) && t2 != NULL) {//cherche le inter
            t1 = t2;
            t2 = t2->suivant;
        }
        if (t2 != NULL) {
            if (t2->debut <= rdv->debut_affectee && t2->fin >= rdv->fin_affectee) {//Les horaires du rendez-vous souhaité appartiennent à l'intervalle
                                                                                   //donc debut fin affecte ne change pas
                if (rdv->debut_affectee == t2->debut && rdv->fin_affectee == t2->fin) {//pour debut et fin : rdv=inter, supprimer inter
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = t2->suivant;
                    else t1->suivant = t2->suivant;
                    free(t2);
                    return;
                } else if (t2->debut < rdv->debut_affectee && rdv->fin_affectee < t2->fin) {//générer deux nouelle inter
                    T_Intervalle *i1 = malloc(sizeof(T_Intervalle));
                    T_Intervalle *i2 = malloc(sizeof(T_Intervalle));
                    i1->debut = t2->debut;
                    i1->fin = rdv->debut_affectee;
                    i2->debut = rdv->fin_affectee;
                    i2->fin = t2->fin;
                    i1->suivant = i2;
                    i2->suivant = t2->suivant;
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = i1;
                    else t1->suivant = i1;
                    free(t2);
                    return;
                } else if (t2->debut == rdv->debut_affectee && t2->fin > rdv->fin_affectee) {//générer une noubelle inter
                    T_Intervalle *i = malloc(sizeof(T_Intervalle));
                    i->debut = rdv->fin_affectee;
                    i->fin = t2->fin;
                    i->suivant = t2->suivant;
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = i;
                    else t1->suivant = i;
                    free(t2);
                    return;
                } else {                                                                       //géneérer une nouvelle inter
                    T_Intervalle *i = malloc(sizeof(T_Intervalle));
                    i->debut = t2->debut;
                    i->fin = rdv->debut_affectee;
                    i->suivant = t2->suivant;
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = i;
                    else t1->suivant = i;
                    free(t2);
                    return;
                }
            } else {                                                               //Les horaires du rendez-vous souhaité n'appartiennent pas à l'intervalle
                rdv->debut_affectee = t2->debut;                                   // le temps change
                rdv->fin_affectee = rdv->debut_affectee + time_rdv;
                if (t2->fin - t2->debut == time_rdv) {                            //dureé de inter==dureé de rdv,supprimer inter
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = t2->suivant;
                    else t1->suivant = t2->suivant;
                    free(t2);
                    return;
                } else {                                                          //dureé de inter!=dureé de rdv,générer une nouvelle inter
                    T_Intervalle *i = malloc(sizeof(T_Intervalle));
                    i->debut = rdv->fin_affectee;
                    i->fin = t2->fin;
                    i->suivant = t2->suivant;
                    if (soigneur->listeIntervalle == t2) soigneur->listeIntervalle = i;
                    else t1->suivant = i;
                    free(t2);
                    return;
                }
            }
        }
    }
}


/**
 * @brief Ordonnancer les rendez-vous des patients en fonction des intervalles de temps disponibles
 * pour l’ensemble des soigneurs en minimisant la somme des temps d’attente des patients
 * (le temps d’attente est calculé par la date de début affectée – la date de début souhaitée).
 * L’algorithme glouton d'ordonnancement en minimisant la somme du temps d’attente des patients se construit comme suit :
 * Étape 1 : Trier les patients par ordre décroissant de durée totale des rendez-vous
 *  (la durée d’un rendez-vous est calculée par la date de fin souhaitée – la date de début souhaitée)
 * Étape 2 : Affecter les rendez-vous des patients dans l’ordre ci-dessus aux intervalles de temps disponible des soigneurs.
 * @param solution un instance
 */
void ordonnancer(T_Ordonnancement *solution) {
    provided_MergeSort(&(solution->listePatients));
    T_Patient *p = solution->listePatients;
    T_Soigneur *s_temp;
    while (p != NULL) {//pour chaque patients
        T_RendezVous *r = p->listeRendezVous;
        while (r != NULL) {//pour chaque rendez-vous
            //printf("ids pour rdv%u\n", r->id_soi);
            s_temp = solution->listeSoigneurs;
            while (s_temp != NULL && s_temp->id_soi != r->id_soi) {//trouver soigneur
                s_temp = s_temp->suivant;
            }
            if (s_temp != NULL)affecterRdV(r, s_temp);
            else r->desc = "soigneur nul, affectation échec";
            if (r->suivant != NULL) {//imposer des contrainte pour rdv suivant pour contenir le temps de déplacement
                Time t = (r->suivant->fin_souhaitee) - (r->suivant->debut_souhaitee);
                r->suivant->debut_affectee = (r->fin_affectee) + (r->suivant->temps_deplacement);
                r->suivant->fin_affectee = (r->suivant->debut_affectee) + t;
            }
            r = r->suivant;
        }
        p = p->suivant;
    }
}

/**
 * @brief Exporter la solution d’un ordonnancement.
 * Le nom du fichier exporté a sufix de la date de création  d’un ordonnancement, par exemple filename=’solution.txt’,
 * et la date de creation est ‘2020-10-13’, donc le nom de ficher exporté est ‘solution.txt.2020-10-13.txt’.
 * @param solution une solution d'un ordonnancement.
 * @param filename le nom d'un ficher exporté.
 */
void exportSolution(T_Ordonnancement *solution, char *filename) {
    //printf("在exportsolution开头的打印%s\n",solution->date);
    int nb_patients = 0;
    int nb_soigneur = 0;
    int nb_rdv = 0;
    T_Soigneur *s = solution->listeSoigneurs;
    T_Patient *p = solution->listePatients;
    while (s != NULL) {//calcule le nombre de soigneur
        nb_soigneur = nb_soigneur + 1;
        s = s->suivant;
    }
    while (p != NULL) {//calcule le nombre de patients
        nb_patients = nb_patients + 1;
        p = p->suivant;
    }
    filename = strcat(filename, solution->date);
    filename = strcat(filename, ".txt");//gerer le nom de fichier
    FILE *fp;
    fp = fopen(filename, "w");
    fprintf(fp, "%d %d\n", nb_patients, nb_soigneur);
    p = solution->listePatients;
    while (p != NULL) {
        T_RendezVous *rdv = p->listeRendezVous;
        while (rdv != NULL) {//calcule le nombre de rdv de chaque patients
            nb_rdv = nb_rdv + 1;
            rdv = rdv->suivant;
        }
        fprintf(fp, "%d %d\n", p->id_pat, nb_rdv);
        rdv = p->listeRendezVous;
        while (rdv != NULL) {
            fprintf(fp, "%d %d %d %d %s\n",
                    rdv->id_soi, rdv->debut_affectee, rdv->fin_affectee, rdv->temps_deplacement, rdv->desc);
            rdv = rdv->suivant;
        }
        p = p->suivant;
    }
    fclose(fp);
}

void affichage_patient_disponible(T_Patient *p) {
    printf("Les patients disponible sont:\n");
    while (p != NULL) {
        printf("    Id patient:%d    ", p->id_pat);
        p = p->suivant;
    }
    printf("\n");
};

void affichage_soigneur_du_rdv(T_RendezVous *r) {
    printf("les soigneur associés sont:\n");
    while (r != NULL) {
        printf("     Id soigneur: %d     ", r->id_soi);
        r = r->suivant;
    }
    printf("\n");
}

void affichage_tous_les_soigneur_ordonnancement(T_Ordonnancement *o) {
    T_Soigneur *s = o->listeSoigneurs;
    printf("les soigneurs associés sont:\n");
    while (s != NULL) {
        printf("     Id soigneur: %d      ", s->id_soi);
        s = s->suivant;
    }
    printf("\n");
}


Index_Soigneur modifier_le_soigneur_associe(T_RendezVous *r, T_Ordonnancement *o) {
    Index_Soigneur id_s;
    int cherche_s;
    int signe = 0;
    while (signe == 0) {
        affichage_un_rdv(r);
        printf("Modifier le soigneur associé? y pour oui, d'autre pour non\n");
        char choix;
        fflush(stdin);
        scanf("%c", &choix);
        if (choix == 'y') {//modifier le soigneur
            printf("Veuillez saisir un identifiant du soigneur\n");
            scanf("%u", &id_s);
            cherche_s = cherche_soigneur_dans_ordonancement(id_s, o);
            if (cherche_s == 0) {//ne trouver pas le soigneur
                printf("ce soigneur n'existe pas!");
                affichage_tous_les_soigneur_ordonnancement(o);
                printf("Continuer? y pour oui, d'autre pour non\n");
                fflush(stdin);
                scanf("%c", &choix);
                if (choix == 'y') continue;//toujours voudrais modifier le soigner
                else signe = 1;//ne veut pas modifier le soigneur
            } else {//trouver le soigneur
                signe = 2;
            }
        } else {//ne veut pas modifier
            signe = 1;
        }
    }
    if (signe == 1) return r->id_soi;
    else if (signe == 2) return id_s;
}

Time modifier_la_date_de_debut_rdv(T_RendezVous *r) {
    char choix_d;
    printf("Modifier la date de début souhaitée? y pour oui, d'autre pour non\n");
    fflush(stdin);
    scanf("%c", &choix_d);
    Time d;
    if (choix_d == 'y') {//modifier la date de debut souhaitee
        printf("Veuillez saisir le date de debut\n");
        scanf("%d", &d);
        return d;
    } else {//ne veut pas modifier la date de debut souhaitee
        return r->debut_souhaitee;
    }
}

Time modifier_la_date_de_fin_rdv(T_RendezVous *r) {
    char choix_f;
    printf("Modifier la date de fin souhaitée? y pour oui, d'autre pour non\n");
    fflush(stdin);
    scanf("%c", &choix_f);
    Time f;
    if (choix_f == 'y') {//modifier la date de fin souhaitee
        printf("Veuillez saisir la date de fin, elle doit supérieur a la date de debut\n");
        scanf("%d", &f);
        return f;
    } else {//ne veut pas modifier la date de fin souhaitee
        return r->fin_souhaitee;
    }
}

Time modifier_le_temps_deplacement(T_RendezVous *r) {
    char choix;
    printf("Modifier le temps de déplacement? y pour oui, d'autre pour non\n");
    fflush(stdin);
    scanf("%c", &choix);
    Time td;
    if (choix == 'y') {//modifier le temps de deplacement
        printf("Veuillez saisir le temps de déplacement\n");
        scanf("%d", &td);
        return td;
    } else {//ne modifier pas le temps de deplacement
        return r->temps_deplacement;
    }
}

char *modifier_des(T_RendezVous *r) {
    char choix;
    printf("Modifier la description? y pour oui, d'autre pour non\n");
    fflush(stdin);
    scanf("%c", &choix);
    char a[125];
    if (choix == 'y') {
        printf("Veuillez saisir la description\n");
        scanf("%s", a);
        strcpy(r->desc, a);
        return r->desc;
    } else {
        return r->desc;
    }
}

void tout_modifier(T_RendezVous *r, T_Ordonnancement *o) {
    int signe = 0;
    Index_Soigneur id_s = 0;
    Time d_d, d_f, t_d;
    char des[125];
    id_s = modifier_le_soigneur_associe(r, o);
    while (signe == 0) {
        d_d = modifier_la_date_de_debut_rdv(r);
        d_f = modifier_la_date_de_fin_rdv(r);
        if (d_f <= d_d) {
            printf("pas possible! la date de fin est inférieur de la date de début\n");
            continue;
        } else signe = 1;
    }
    t_d = modifier_le_temps_deplacement(r);
    strcpy(des, modifier_des(r));
    modifierRendezVous(r, id_s, d_d, d_f, t_d, des);
    printf("modification réussie\n");
    printf("======================\n");
}

T_Patient *cherche_patient(T_Patient *listepatients, Index_Patient id_p) {
    T_Patient *p_temp = listepatients;
    while (p_temp != NULL && p_temp->id_pat != id_p) {
        p_temp = p_temp->suivant;
    }
    if (p_temp == NULL) return NULL;
    else return p_temp;
}

T_RendezVous *cherche_soigneur_du_rdv(T_RendezVous *rdv, Index_Soigneur id_s) {
    T_RendezVous *r_temp = rdv;
    while (r_temp != NULL && r_temp->id_soi != id_s) {
        r_temp = r_temp->suivant;
    }
    if (r_temp == NULL) return NULL;
    else return r_temp;
}

int cherche_soigneur_dans_ordonancement(Index_Soigneur id_s, T_Ordonnancement *o) {//trouver return 1, sinon return 0
    T_Soigneur *s_temp = o->listeSoigneurs;
    while (s_temp != NULL && s_temp->id_soi != id_s) {
        s_temp = s_temp->suivant;
    }
    if (s_temp == NULL) return 0;
    else return 1;
}

void affichage_un_rdv(T_RendezVous *r) {
    printf("Desc:%s , id soigneur associé: %u , temps de déplacement depuis le rdv précédent: %u\n", r->desc, r->id_soi,
           r->temps_deplacement);
    printf("   Rdv souhaité:[%u , %u[\n", r->debut_souhaitee, r->fin_souhaitee);
    printf("   Rdv affecté :[%u , %u[\n", r->debut_affectee, r->fin_affectee);
    printf("============================\n");

}


/**
 * @brief un menu principal pour le programme.
 */
void menuPrincipal(void) {
    int a = 0;
    T_Ordonnancement *o = NULL;
    char filename[30];
    char choix;
    int signe = 0;
    do {
        printf("1 Créer une instance à partir d’un fichier\n"
               "2 Afficher tous les patients et leurs rendez-vous\n"
               "3 Afficher tous les soigneurs et leurs intervalles de temps disponibles\n"
               "4 Afficher un rendez-vous en indiquant l’identifiant du patient et le soigneur correspondant\n"
               "5 Modifier un rendez-vous en indiquant l’identifiant du patient et celui du soigneur correspondant\n"
               "6 Supprimer un rendez-vous en indiquant l’identifiant du patient et celui du soigneur correspondant\n"
               "7 Ordonnancer\n"
               "8 Exporter la solution d’un ordonnancement\n"
               "9 Quitter\n");
        printf("Veuillez sasir votre choix\n");
        scanf("%d", &a);
        while (!(a > 0 && a < 10)) {
            printf("entrée faulte\n");
            printf("Veuillez sasir votre choix\n");
            fflush(stdin);
            scanf("%d", &a);
        }
        switch (a) {
            case 1: {//Créer une instance à partir d’un fichier
                signe = 0;
                while (signe == 0) {
                    printf("Veuillez saisir le nom du fichier d'une instance\n");
                    scanf("%s", filename);
                    FILE *fp = fopen(filename, "r");
                    if (fp != NULL) {
                        signe = 2;
                        fclose(fp);
                    }
                    while (fp == NULL) {
                        printf("ce fichier n'existe pas\n");
                        printf("Continuer? y or n\n");
                        fflush(stdin);
                        scanf("%c", &choix);
                        if (choix == 'y') {//contiunuer
                            break;
                        } else if (choix == 'n') {//ne pas continuer return to menuprincipal
                            signe = 1;
                            break;
                        } else {
                            printf("fausse entrée\n");
                            continue;
                        }
                    }
                }
                if (signe == 1) break;
                o = creerInstance(filename);
                printf("réussie de creéer un instance\n");
                printf("===============================\n");
                break;
            }
            case 2: {//Afficher tous les patients et leurs rendez-vous
                if (o != NULL) {
                    affichage_Patients(o->listePatients);
                    break;
                } else {//cas de il n'y a pas d'instance
                    printf("il n'y a pas une instance! Veuillez importer une instance.\n");
                    printf("==========================================================\n");
                    break;
                }
            }
            case 3: {//Afficher tous les soigneurs et leurs intervalles de temps disponibles
                if (o != NULL) {
                    affichage_Soigneurs(o->listeSoigneurs);
                    break;
                } else {//cas de il n'y a pas d'instance
                    printf("il n'y a pas une instance! Veuillez importer une instance.\n");
                    printf("===========================================================\n");
                    break;
                }
            }
            case 4: {//Afficher un rendez-vous en indiquant l’identifiant du patient et le soigneur correspondant
                Index_Patient id_p;
                Index_Soigneur id_s;
                T_Patient *p;
                T_RendezVous *r;
                if (o != NULL) {//il y a instance
                    signe = 0;
                    while (signe == 0) {
                        affichage_patient_disponible(o->listePatients);
                        printf("Veuillez saisir un identifiant du patient\n");
                        scanf("%u", &id_p);
                        p = cherche_patient(o->listePatients, id_p);
                        if (p == NULL) {//n'a pas trouver le patient
                            printf("ce patient n'existe pas!\n");
                            affichage_patient_disponible(o->listePatients);
                            printf("Continuer? y pour oui, d'autre pour non\n");
                            fflush(stdin);
                            scanf("%c", &choix);
                            if (choix == 'y') continue;//continuer
                            else {//return to menuprincipal
                                signe = 1;
                                break;
                            }
                        } else {//trouver le patients
                            signe = 0;
                            while (signe == 0) {
                                r = p->listeRendezVous;
                                affichage_soigneur_du_rdv(r);
                                printf("Veuillez saisir un identifiant du soigneur\n");
                                scanf("%u", &id_s);
                                r = cherche_soigneur_du_rdv(r, id_s);
                                if (r == NULL) {//n'a pas trouver le rdv correspodant
                                    printf("ce soigneur ne sert pas le patient\n");
                                    r = p->listeRendezVous;
                                    affichage_soigneur_du_rdv(r);
                                    printf("Continuer? y pour oui, d'autre pour non\n");
                                    fflush(stdin);
                                    scanf("%c", &choix);
                                    if (choix == 'y')signe = 0;//choix continuer
                                    else {//choix de pas continuer return to menuprincipal
                                        signe = 1;
                                        break;
                                    }
                                } else {//trouver le rdv correspodant
                                    affichage_un_rdv(r);
                                    signe = 1;
                                    break;
                                }
                            }
                        }
                    }
                } else {//cas de il n'y a pas d'instance
                    printf("il n'y a pas une instance! Veuillez importer une instance.\n");
                    printf("==========================================================\n");
                    break;
                }
                break;
            }
            case 5: {//Modifier un rendez-vous en indiquant l’identifiant du patient et celui du soigneur correspondant
                Index_Patient id_p;
                Index_Soigneur id_s;
                T_Patient *p;
                T_RendezVous *r;
                if (o != NULL) {//il y a instance
                    signe = 0;
                    while (signe == 0) {
                        affichage_patient_disponible(o->listePatients);
                        printf("Veuillez saisir un identifiant du patient\n");
                        scanf("%u", &id_p);
                        p = cherche_patient(o->listePatients, id_p);
                        if (p == NULL) {//n'a pas trouver le patient
                            printf("ce patient n'existe pas!\n");
                            affichage_patient_disponible(o->listePatients);
                            printf("Continuer? y pour oui, d'autre pour non\n");
                            fflush(stdin);
                            scanf("%c", &choix);
                            if (choix == 'y') continue;//continuer
                            else {//return to menuprincipal
                                signe = 1;
                                break;
                            }
                        } else {//trouver le patients
                            signe = 0;
                            while (signe == 0) {
                                r = p->listeRendezVous;
                                affichage_soigneur_du_rdv(r);
                                printf("Veuillez saisir un identifiant du soigneur\n");
                                scanf("%u", &id_s);
                                r = cherche_soigneur_du_rdv(r, id_s);
                                if (r == NULL) {//n'a pas trouver le rdv correspodant
                                    printf("ce soigneur ne sert pas le patient");
                                    r = p->listeRendezVous;
                                    affichage_soigneur_du_rdv(r);
                                    printf("Continuer? y pour oui, d'autre pour non\n");
                                    fflush(stdin);
                                    scanf("%c", &choix);
                                    if (choix == 'y')signe = 0;//choix continuer
                                    else {//choix de pas continuer return to menuprincipal
                                        signe = 1;
                                        break;
                                    }
                                } else {//trouver le rdv correspodant
                                    tout_modifier(r, o);
                                    signe = 1;
                                    break;
                                }
                            }
                        }
                    }
                } else {//cas de il n'y a pas d'instance
                    printf("il n'y a pas une instance! Veuillez importer une instance.\n");
                    printf("==========================================================\n");
                    break;
                }
                break;
            }
            case 6: {//Supprimer un rendez-vous en indiquant l’identifiant du patient et celui du soigneur correspondant
                Index_Patient id_p;
                Index_Soigneur id_s;
                T_Patient *p;
                T_RendezVous *r;
                if (o != NULL) {//il y a instance
                    signe = 0;
                    while (signe == 0) {
                        affichage_patient_disponible(o->listePatients);
                        printf("Veuillez saisir un identifiant du patient\n");
                        scanf("%u", &id_p);
                        p = cherche_patient(o->listePatients, id_p);
                        if (p == NULL) {//n'a pas trouver le patient
                            printf("ce patient n'existe pas!\n");
                            affichage_patient_disponible(o->listePatients);
                            printf("Continuer? y pour oui, d'autre pour non\n");
                            fflush(stdin);
                            scanf("%c", &choix);
                            if (choix == 'y') continue;//continuer
                            else {//return to menuprincipal
                                signe = 1;
                                break;
                            }
                        } else {//trouver le patients
                            signe = 0;
                            while (signe == 0) {
                                r = p->listeRendezVous;
                                affichage_soigneur_du_rdv(r);
                                printf("Veuillez saisir un identifiant du soigneur\n");
                                scanf("%u", &id_s);
                                r = cherche_soigneur_du_rdv(r, id_s);
                                if (r == NULL) {//n'a pas trouver le rdv correspodant
                                    printf("ce soigneur ne sert pas le patient\n");
                                    r = p->listeRendezVous;
                                    affichage_soigneur_du_rdv(r);
                                    printf("Continuer? y pour oui, d'autre pour non\n");
                                    fflush(stdin);
                                    scanf("%c", &choix);
                                    if (choix == 'y')signe = 0;//choix continuer
                                    else {//choix de pas continuer return to menuprincipal
                                        signe = 1;
                                        break;
                                    }
                                } else {//trouver le rdv correspodant
                                    T_RendezVous *r_t;
                                    r_t = supprimerRendezVous(p->listeRendezVous, id_s);
                                    if (r_t != NULL) {
                                        printf("réussie de supprimation\n");
                                        printf("========================\n");
                                        p->listeRendezVous = r_t;
                                    } else {
                                        printf("échec de supprimation\n");
                                        printf("======================\n");
                                    }
                                    signe = 1;
                                    break;
                                }
                            }
                        }
                    }
                } else {//cas de il n'y a pas d'instance
                    printf("il n'y a pas une instance! Veuillez importer une instance.\n");
                    printf("==========================================================\n");
                    break;
                }
                break;
            }
            case 7: {
                if (o != NULL) {
                    ordonnancer(o);
                    printf("fin d'ordonnancement\n");
                    printf("========================\n");
                    break;
                } else {//cas il n'y a pas d'instance
                    printf("il n'y a pas une instance! Veuillez importer une instance.\n");
                    printf("==========================================================\n");
                    break;
                }
            }
            case 8: {
                if (o != NULL) {
                    char filename[50];
                    printf("Veuillez saisir le nom du fichier d'une solution exportée\n");
                    scanf("%s", filename);
                    exportSolution(o, filename);
                    printf("réussie d'exporter le fichier\n");
                    printf("=================================\n");
                    break;
                } else {//cas il n'y a pas d'instance
                    printf("il n'y a pas une instance! Veuillez importer une instance.\n");
                    printf("==========================================================\n");
                    break;
                }
            }
            default: {
                printf("au revoir");
                return;
            }
        }
    } while (a != 9);
    printf("au revoir!");
    return;
}




