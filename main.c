/*
    proiect SMECHER pt PCLP 2!!

    - ar trebui sa fie ceva evidenta cu studentii din facultate cred
    - todo: opengl, consola e cringe
    - todo: sa pot adauga studenti noi, sa ii pot sterge, sa ii pot edita, sa ii pot sorta dupa nume, prenume, data nasterii sau gen
    - todo++: graph-uri cu note, nr studenti etc

    12.04.2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GEN_MASCULIN                (0)
#define GEN_FEMININ                 (1)

#define FISIER_STUDENTI             "studenti.bin"

struct student 
{
    int id; // ar trebui sa fie unic, dar nu am chef sa ma complic cu asta acum
    char nume[20];
    char prenume[20];

    short int dataNasterii[3]; // zi, luna, an
    char CNP[14];
};

FILE *fisier_studenti = NULL;

int genStudent(struct student s)
{
    if (s.CNP[0] == '1' || s.CNP[0] == '5') return GEN_MASCULIN;
    else if (s.CNP[0] == '2' || s.CNP[0] == '6') return GEN_FEMININ;

    return -1;
}

int elibereazaMemorie(struct student *studenti)
{
    if(studenti)
    {
        free(studenti);
        return 1;
    }

    return 0;
}

int main()
{
    // asa a zis profu, nu-mi place ᓚᘏᗢ
    int i = 0, j = 0;

    // variabile
    int ultimulId = 0;
    int nrStudenti = 0, optiuneAleasa = 0;
    struct student *studenti = NULL; // alocat & realocat dinamic mai tarziu

    fisier_studenti = fopen(FISIER_STUDENTI, "rb");
    if (fisier_studenti == NULL)
    {
        printf("Fisierul cu studenti nu exista. Se creeaza unul...\n");
        fisier_studenti = fopen(FISIER_STUDENTI, "wb");
        fclose(fisier_studenti);
        fisier_studenti = fopen(FISIER_STUDENTI, "rb");
    }

    /*
        incarca studentii din fisier
    */

    // 1. numara studentii
    fseek(fisier_studenti, 0, SEEK_END);
    nrStudenti = ftell(fisier_studenti) / sizeof(struct student);

    // 2. daca exista studenti, aloca memorie pentru ei si-i incarca in memorie
    if(nrStudenti > 0)
    {
        studenti = (struct student*)malloc(nrStudenti * sizeof(struct student));
        fseek(fisier_studenti, 0, SEEK_SET);

        for (i = 0; i < nrStudenti; i++)
        {
            fread(&studenti[i], sizeof(struct student), 1, fisier_studenti);
        }
    }

    fclose(fisier_studenti);

    // 3. ar trebui sa afiseze ceva ca s-au incarcat studenti / nu s-au incarcat
    if(nrStudenti)
    {
        printf("[LOADING] %d studenti au fost incarcati din fisier...\n", nrStudenti);
    }

    else
    {
        printf("[LOADING] Nu au fost gasiti studenti in fisier...\n");
    }
    fflush(stdout);

    // 4. actualizeaza ultimul id folosit (daca exista studenti, altfel ramane 0)
    if(nrStudenti > 0)
    {
        ultimulId = studenti[nrStudenti - 1].id;
    }

    // intreaba userul ce vrea sa faca
    while(1) 
    {
        printf("\n--- MENIU STUDENTI ---\n");
        printf("1. Afiseaza toti studentii\n");
        printf("2. Adauga un student nou\n");
        printf("3. Sterge un student\n");
        printf("4. Editeaza un student\n");
        printf("5. Inchide acest gunoi de aplicatie :))))\n");
        printf("Alege o optiune: ");
        fflush(stdout);

        if (scanf("%d", &optiuneAleasa) != 1) 
        {
            while (getchar() != '\n'); 
            continue;
        }

        switch(optiuneAleasa)
        {
            case 1:
            {
                printf("\n[LISTA STUDENTI]\n");
                if (nrStudenti == 0) printf("Lista este goala.\n");

                for(i = 0; i < nrStudenti; i++) 
                {
                    printf("%d. %s %s | CNP: %s | Data nasterii: %02d.%02d.%04d\n", studenti[i].id, studenti[i].nume, studenti[i].prenume, studenti[i].CNP, studenti[i].dataNasterii[0], studenti[i].dataNasterii[1], studenti[i].dataNasterii[2]);
                }
                break;
            }

            case 2:
            {
                fflush(stdout);
                printf("\n[ADAUGA STUDENT]\n");
                struct student s;
                printf("Nume: ");
                scanf("%s", s.nume);
                printf("Prenume: ");
                scanf("%s", s.prenume);
                printf("CNP: ");
                scanf("%s", s.CNP);
                printf("Data nasterii (zi.luna.an): ");
                scanf("%hd.%hd.%hd", &s.dataNasterii[0], &s.dataNasterii[1], &s.dataNasterii[2]);
                ultimulId++;
                s.id = ultimulId;
                
                studenti = (struct student*)realloc(studenti, (nrStudenti + 1) * sizeof(struct student));
                studenti[nrStudenti] = s;
                nrStudenti++;

                fflush(stdout);

                printf("Student adaugat cu succes! ID: %d\n", s.id);

                break;
            }

            case 3:
            {
                printf("\nTODO: sterge student\n");
                break;
            }

            case 4:
                printf("\nTODO: editeaza student\n");
                break;

            case 5:
            {
                printf("p@! Ne vedem la restanta!\n");
                elibereazaMemorie(studenti);
                return 0;
            }

            default:
            {
                printf("Optiune invalida! Incearca 1-5.\n");
                break;
        
            }
        }

        optiuneAleasa = 0; 
    }

    elibereazaMemorie(studenti);
    return 0;
}

