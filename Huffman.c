#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define MAX_TREE_HT 100//Taille maximul du TasMin



//la structure d'un noeud de l'arbre Huffman
struct elem{
    char    val;
    int     freq;
    struct elem  *  droite, *   gauche;
};
typedef struct elem * noeud;




//la structure de la collection des noeuds de l'arbre
struct arbre{
    noeud * tab;
    int cap;
    int taille;
};
typedef struct arbre * TasMin;




//La fonction permettant d'allouer un nouveau noeud
noeud nouveaunoeud(char v, int f){
    noeud Y;
    Y=(noeud)malloc(sizeof(struct elem));
    Y->droite=NULL;
    Y->gauche=NULL;
    Y->val=v;
    Y->freq=f;
    return Y;
}




//La fonction permettant de créer un tas minimum de capacité donnée
TasMin New_Tas_Min_Cap(int c){
   TasMin Y;
   Y=(TasMin)malloc(sizeof(struct arbre));
   Y->taille=0;
   Y->cap=c;
   Y->tab=(noeud*)malloc(Y->cap * sizeof(noeud));
   return Y;
}




//La fonction permettant d'échanger deux noeuds de tas-min
void echangerNoeud(noeud * a,noeud * b){
    noeud t=*a;
    *a  =   *b;
    *b  =   t;
}




//La fonction standard entasserMin
void entasserMin(TasMin Y, int i){
    int min=i; // l'indice de la valeur min dans le tableu
    int gauche=2*i+1;//gauche de i
    int droite=2*i+2;//droite de i
    if ((gauche < Y->taille) && (Y->tab[gauche]->freq < Y->tab[min]->freq))
        min = gauche;
    if ((droite < Y->taille) && (Y->tab[droite]->freq < Y->tab[min]->freq))
        min = droite;
    if ( min != i)
    {
        echangerNoeud(&Y->tab[min],&Y->tab[i] );
        entasserMin(Y, min);
    }
 }




//La fonction standard extraire-Tas-Min permettant d'extraire le noeud de valeur minimale du tas
noeud Extraire_Tas_Min(TasMin Y){
    noeud temp=Y->tab[0];
    Y->tab[0]=Y->tab[Y->taille-1];
    Y->taille--;
    entasserMin(Y,0);
    return temp;
}




//La fonction standard inserer-Tas-Min pour insérer un nouveau noeud dans un tas min
void Inserer_Tas_Min(TasMin Y,noeud X){
    int i=Y->taille;
    Y->taille++;
    while(i && X->freq < Y->tab[(i-1)/2]->freq){//i-1/2 est le parent de
        Y->tab[i]=Y->tab[(i-1)/2];
        i=(i-1)/2;
    }
    Y->tab[i]=X;
}




//La fonction standard permettant de construire un tas min
void Construire_Tas_Min(TasMin Y)

{
    int n = Y->taille - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        entasserMin(Y, i);
}




//La fonction permettant d'initialiser et créer un tas min.
TasMin InietCreer_TasMin(char mot[], int freq[], int tai)
{

    TasMin Y = New_Tas_Min_Cap(tai);

    for (int i = 0; i < tai; ++i)
        Y->tab[i] = nouveaunoeud(mot[i], freq[i]);

    Y->taille = tai;
    Construire_Tas_Min(Y);

    return Y;
}




//La fonction principale qui construit l'arbre Huffman
noeud const_Huffman(char mot[], int freq[], int tai)

{
    noeud   fgauche, fdroite, parent;
    TasMin Y = InietCreer_TasMin(mot, freq, tai);// On crée un tas min de capacité tai
    while (Y->taille != 1) { //On repète la boucle while jusqu'a ce qu'il un seule element dans le tas min Initiale

        // On extrait le deux element de frequence la plus basse(les moins répéter)
        fgauche = Extraire_Tas_Min(Y);
        fdroite = Extraire_Tas_Min(Y);


        // On crée un nouveau neoud interne spécial
        // ce noud aura comme frequence la somme des deux noeud extrait recement
        //On mettera comme valeur '*' pour differencier ce noeud des autres
        parent = nouveaunoeud('*', fgauche->freq + fdroite->freq);
        //Ce noeud aura comme fils droite et gauche les deux noeuds extrait recement
        parent->gauche = fgauche;
        parent->droite = fdroite;

        // On insere le neoud crée dans Notre Tas Min crée
        Inserer_Tas_Min(Y, parent);
    }

    // le noeud retourner est le dernier noeud qui est le noeud racine
    return Extraire_Tas_Min(Y);
}





// Fonction utilitaire qui permet d'afficher les codes de chaque lettre en parcourant un Tas grace au pointeur
void afficheCodes(noeud Y, int arr[], int z)

{

    // On ajoute 0 au code si Y à un fils gauche et on passe Y->gauche à la fonction afficheCodes(recursive)
    if (Y->gauche) {

        arr[z] = 0;
        afficheCodes(Y->gauche, arr, z + 1);
    }

     // On ajoute 1 au code si Y à un fils droite et on passe Y->droite à la fonction afficheCodes(recursive)
    if (Y->droite) {

        arr[z] = 1;
        afficheCodes(Y->droite, arr, z + 1);
    }

    //Si le noeud n'a pas de fils droite ni de fils gauche càd que c'est un neoud de lettere
    //qui contient un alphabet
    if (!(Y->gauche) && !(Y->droite)) {
        int i;
        printf("%c: ", Y->val);
        for (i = 0; i < z; ++i)
        printf("%d", arr[i]);

        printf("\n");
    }
}




// Fonction utilitaire qui permet d'ecrire dans un fichier les codes de chaque lettre en parcourant un Tas grace au pointeur
void afficheCodes2(FILE *fptr, noeud Y, int arr[], int z)

{
    // On ajoute 0 au code si Y à un fils gauche et on passe Y->gauche à la fonction afficheCodes(recursive)
    if (Y->gauche) {

        arr[z] = 0;
        afficheCodes2(fptr,Y->gauche, arr, z + 1);
    }
     // On ajoute 1 au code si Y à un fils droite et on passe Y->droite à la fonction afficheCodes(recursive)
    if (Y->droite) {

        arr[z] = 1;
        afficheCodes2(fptr,Y->droite, arr, z + 1);
    }
    //Si le noeud n'a pas de fils droite ni de fils gauche càd que c'est un neoud de lettere
    //qui contient un alphabet
    if (!(Y->gauche) && !(Y->droite)) {
        int i;
        fprintf(fptr,"%c: ", Y->val);
        for (i = 0; i < z; ++i)
        fprintf(fptr,"%d", arr[i]);

        fprintf(fptr,"\n");
    }
}




//La fonction permettant de construire un arbre Huffman et imprime les codes en parcourant l'arbre
void CodesHuffman(char mot[], int freq[], int tai)
{
    // construire un arbre Huffman
    noeud Y;
    Y= const_Huffman(mot, freq, tai);
    int codes[MAX_TREE_HT], z = 0;
    //Afficher les codes des element de l'abre huffman Y construit
    afficheCodes(Y, codes, z);

}





//fonction utilitaire : Vérifie Si un charactère existe dans un tableau
int Existe(char V,char * T){
    int i=0;
    while(T[i]!='\0'){
        if(T[i]==V){
            return 1;
        }
        i++;
    }
    return 0;
}




//fonction utilitaire : Extrait les lettres utilisées dans un mot
char * elem(char * TAB){
    int i=0,cont=0;
    char * resu=malloc(26*sizeof(char *));//tableau de 26 alphabet au max
    resu[0]='\0';
    while(TAB[i]!='\0'){
        char A=TAB[i];
        if(Existe(A, resu) == 0){
            resu[cont]=A;
            cont++;
            resu[cont]='\0';
        }
        i++;
        }
    return resu;
}




//fonction utilitaire : Calcule la fréquence de chaque lettres
int * freq(char * TAB,char * ELEM){
    char A;
    int cont=0,i,j=0;
    int size = sizeof(ELEM) / sizeof(ELEM[0]);
    int * resu=malloc(26*sizeof(int *));
    for(i=0;i<size;i++){
        A=ELEM[i];
        cont=0;
        j=0;
    while(TAB[j]!='\0'){
        if(TAB[j]==A){
            cont++;
        }
        j++;
        }
        resu[i]=cont;

    }
    return resu;
}

// fonction utilitaire : Decode le code de huffman contenu dans un tableaux
void decode(char * arr,noeud racine){
    int i=0;
    noeud Y=racine;
     while(arr[i]!='\0'){
        if(arr[i]=='0'){
            Y=Y->gauche;
            if(Y->gauche==NULL && Y->droite==NULL){
            printf("%c",Y->val);
            Y=racine;
        }
    }
    else{
        Y=Y->droite;
        if(Y->gauche==NULL && Y->droite==NULL){
            printf("%c",Y->val);
            Y=racine;
        }
    }
    i++;
    }
}

// ----------------- Les Fonctions Main -----------------




//Main 1 : Lecture du texte à partir d'un fichier source et stockage dans un tableau
/*int main()
{

    char mot[] = { 'y', 'a', 's', 'e', 'r','h' };
    int freq[] = { 5, 9, 12, 13, 16 ,58};
    //Taille du tableau de lettres qu'on passera à la fonction codesHuffman
    int taille = sizeof(mot) / sizeof(mot[0]);

    CodesHuffman(mot, freq, taille);

    return 0;
}*/




//Main 2 : Calcul de l'alphabet et des fréquences.
int main(){
    int i=0,j=0;
    int * frequ;
    char INI[30];
    char * res;
    printf("Le mot est : ");
    fgets(INI,30,stdin);
    res=elem(INI);
    frequ=freq(INI,res);
    printf("Frequence de chaque lettres :\n");
    //cette boucle est pour afficher les lettres du mots et leur frequence
    //et compter le nombre d'alphabet utilisé pour crée un autre tableau sur mesure qui sera passé à la fonction codeshuffman
    while(res[i]!='\n'){
        printf("%c : %d \t",res[i],frequ[i]);
        i++;
    }
    int freqf[i];
    char valf[i];
    while(res[j]!='\n'){// la fonction fgets ajoute \n donc la condition est pour l'exclure
        valf[j]=res[j];
        freqf[j]=frequ[j];
        j++;
    }
    int size = sizeof(valf)/sizeof(valf[0]);
    printf("\nCodes de Huffman :\n");
    CodesHuffman(valf, freqf, size);

    return 0;
}



//Main 3 : Lecture du texte à partir d'un fichier source et stockage dans un tableau.
/*int main() {
    FILE *fich;
    int i=0,j=0;
    char * res;
    int * frequ;
    char str[30];
    char* filename = "C:\\Users\\ULTRA PC\\Desktop\\monfich.txt";//Chemin du fichier de lecture

    fich = fopen(filename, "r");
    if (fich == NULL){
        printf("Erreur d'ouverture du fichier %s",filename);
        return 1;
    }
    while (fgets(str, 30, fich) != NULL) printf("%s\n", str);
    fclose(fich);

    res=elem(str);
    frequ=freq(str,res);
    printf("Frequence de chaque lettres :\n");
    while(res[i]!='\0'){
        printf("%c : %d \t",res[i],frequ[i]);
        i++;
    }
    int freqf[i];
    char valf[i];
    while(res[j]!='\0'){
        valf[j]=res[j];
        freqf[j]=frequ[j];
        j++;
    }
    int size = sizeof(valf)/sizeof(valf[0]);
    printf("\nCodes de Huffman :\n");
    CodesHuffman(valf, freqf, size);


    return 0;
}
*/


//Main 4 : Codage et sauvegarde des codes et du texte codé dans des fichiers.
/*int main() {
    char mot[]={'i','n','f','o','r','m','a','t','q','u','e'};
    int freq[]={2,1,1,1,1,1,1,1,1,1,1};
    FILE *fich;
    char* filename = "C:\\Users\\ULTRA PC\\Desktop\\monfich2.txt";//Chemin du fichier d'ecriture
    int size = sizeof(mot)/sizeof(mot[0]);
    noeud Y;
    Y= const_Huffman(mot, freq, size);
    int codes[MAX_TREE_HT], z = 0;
    fich = fopen(filename, "w");
    if (fich == NULL){
        printf("Erreur d'ouverture du fichier %s",filename);
        return 1;
    }
    afficheCodes2(fich,Y, codes, z);
    fclose(fich);

}*/
//Main 5 : Fonctions permettent d'effectuer des tests et afficher les résultats
/*int main(){
    //Tableaux du codes principales
    char arr[] = { 'y', 'a', 's', 'e', 'r', 'h','d','m' };
    int freq[] = { 1,3,2,1,1,1,2,1 };
    // code à décoder
    char code[]={'1','1','1','0','1','0','0','0','0','0','1','1','1','1','0','1','1','1'};
    noeud Y;
    int size = sizeof(arr) / sizeof(arr[0]);
    Y=const_Huffman(arr,freq,size);
    decode(code,Y);


    return 0;
}*/
