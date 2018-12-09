#include"projet.h"

int main(){

    ImageCouleur *imageLena;
    ImageCouleur *imageMire;

    imageLena = chargerImage("C:/Users/AYOUB LABIDI/Desktop/Lena.ppm");
    imageMire = chargerImage("C:/Users/AYOUB LABIDI/Desktop/Mire.ppm");

    ImageGris *image1 = creerImageGris(imageLena->h,imageLena->l);
    image1->pixels = changerCouleurGris(imageLena->pixels,imageLena->h*imageLena->l);

    ImageCouleur *image2 = creerImageCouleur(imageLena->h,imageLena->l);
    image2->pixels= fusionImages(imageLena->pixels,imageMire->pixels,imageLena->h*imageLena->l);

    ImageCouleur *image3 = creerImageCouleur(imageLena->h,imageLena->l);
    image3->pixels = inversionCouleur(imageLena->pixels,imageLena->h*imageLena->l);

    sauvgarderImageGris("gris.ppm",image1);
    sauvgarderImage("fusion.ppm",image2);
    sauvgarderImage("inversion.ppm",image3);

    int* histogramme;
    histogramme = calculHistogramme(image1->pixels,image1->h*image1->l);
    afficherHistogramme(histogramme);

    printf("...");
    getchar();
}


ImageCouleur *chargerImage(const char *fichier)
{
    char format[10];
    ImageCouleur *img;
    FILE *f;
    int max_pix,commentaire;

    //ouvrir le fichier de l'image
    f = fopen(fichier, "rb");
    if (!f) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier '%s'\n", fichier);
        exit(1);
    }

    //lire le format de l'image
    if (!fgets(format, sizeof(format), f)) {
        perror(fichier);
        exit(1);
    }

    //Verifier le format de l'image
    if (format[0] != 'P' || format[1] != '6') {
        fprintf(stderr, "Format de l'image est invalide\n");
        exit(1);
    }

    //allocation de l'image
    img = (ImageCouleur *)malloc(sizeof(ImageCouleur));

    //enlever les commentaires
    commentaire = getc(f);
    while (commentaire == '#') {
        while (getc(f) != '\n') ;
            commentaire = getc(f);
    }
    ungetc(commentaire,f);

    //lire les dimensions de l'image
    if (fscanf(f, "%d %d", &img->h, &img->l) != 2) {
        fprintf(stderr, "Les dimensions de l'image sont invalides\n");
        exit(1);
    }

    //lire la valeur max d'un pixel
    if (fscanf(f, "%d", &max_pix) != 1) {
        fprintf(stderr, "La valeur maximal d'un pixel est invalide\n");
        exit(1);
    }

    //vérifier la valeur max d'un pixel
    if (max_pix != MAX_PIXEL) {
        fprintf(stderr, "Format de l'image est invalide\n");
        exit(1);
    }

    //supprimer l'espace vide
    while (fgetc(f) != '\n') ;

    //allocation des pixels
    img->pixels = (Pixel*)malloc(img->h * img->l * sizeof(Pixel));

    //lecture des pixels
    if (fread(img->pixels, 3 * img->h, img->l, f) != img->l) {
        fprintf(stderr, "Erreur lors de la chargement de l'image '%s'\n", fichier);
        exit(1);
    }
    fclose(f);
    return img;
}

void sauvgarderImage(const char *fichier, ImageCouleur *img)
{
    FILE *f;
    //ouverture du fichier
    f = fopen(fichier, "wb");
    if (!f) {
         fprintf(stderr, "Erreur lors de l'ouverture du fichier '%s'\n", fichier);
         exit(1);
    }

    //ecriture de l'entete:
    //format de l'image
    fprintf(f, "P6\n");

    //dimensions de l'image
    fprintf(f, "%d %d\n",img->h,img->l);

    //valeur max d'un pixel
    fprintf(f, "%d\n",MAX_PIXEL);

    //ecriture des pixels
    fwrite(img->pixels, 3 * img->h, img->l, f);
    fclose(f);
}

void sauvgarderImageGris(const char *fichier, ImageGris *img)
{
    FILE *f;
    int i;
    //ouverture du fichier
    f = fopen(fichier, "wb");
    if (!f) {
         fprintf(stderr, "Erreur lors de l'ouverture du fichier '%s'\n", fichier);
         exit(1);
    }

    //ecriture de l'entete
    //format de l'image
    fprintf(f, "P2\n");

    //dimensions de l'image
    fprintf(f, "%d %d\n",img->h,img->l);

    //valeur max d'un pixel
    fprintf(f, "%d\n",MAX_PIXEL);

    //ecriture des pixels
    for(i=0;i<img->h*img->l;i++){
        fprintf(f, "%d ",img->pixels[i].g);
    }
    fclose(f);
}

Pixel* inversionCouleur(Pixel *pixels,int taille)
{
    int i;
    if(pixels){
    //parcour du tableau et soustraction des valeurs de chaque pixel de la valeur max que peut prendre un pixel (255)
         for(i=0;i<taille;i++){
              pixels[i].r=MAX_PIXEL-pixels[i].r;
              pixels[i].v=MAX_PIXEL-pixels[i].v;
              pixels[i].b=MAX_PIXEL-pixels[i].b;
         }
    }
    return pixels;
}

PixelGris* changerCouleurGris(Pixel *pixels,int taille)
{
    int i;
    if(pixels){
        //parcour du tableau et remplissage des valeurs des pixels gris par la moyenne des pixels de l'image d'origine
        PixelGris *pixelsGris =(PixelGris*)malloc(taille * sizeof(PixelGris));
        for(i=0;i<taille;i++){
            pixelsGris[i].g =(pixels[i].r+pixels[i].v+pixels[i].b)/3;
        }
        return pixelsGris;
    }
}

int * calculHistogramme(PixelGris* pixels,int taille)
{
    //allocation du tableau d'histogramme
    int* histogramme = (int*)malloc(256*sizeof(int));
    int i,j;
    //parcour des cases de l'histogramme
    for(i=0;i<256;i++)
    {
        //initialisation de la case par 0
        histogramme[i]=0;
        //parcour du tableau des pixels et calcul d'occurence
        for(j=0;j<taille;j++){
            if(pixels[j].g == i)
                histogramme[i]=histogramme[i]+1;
        }
    }
    return histogramme;
}

Pixel * fusionImages(Pixel *pixels1,Pixel *pixels2,int taille)
{
    int i;
    //allocation du tableau des pixels de l'image du fusion
    Pixel *fusion=(Pixel*)malloc(taille * sizeof(Pixel));
    if(pixels1){
    //parcour du tableau et remlissage des valeurs par la moyenne des pixels des deux images à fusionner
        for(i=0;i<taille;i++){
            fusion[i].r=(pixels1[i].r+pixels2[i].r)/2;
            fusion[i].v=(pixels1[i].v+pixels2[i].v)/2;
            fusion[i].b=(pixels1[i].b+pixels2[i].b)/2;
        }
        return fusion;
    }
}

ImageGris * creerImageGris(int h,int l)
{
    //allocation de l'image
    ImageGris* img = (ImageGris*)malloc(sizeof(ImageGris));
    img->h=h;
    img->l=l;
    return img;
}

ImageCouleur * creerImageCouleur(int h,int l)
{
    //allocation de l'image
    ImageCouleur* img = (ImageCouleur*)malloc(sizeof(ImageCouleur));
    img->h=h;
    img->l=l;
    return img;
}

void afficherHistogramme(int* histogramme)
{
    int i;
    for(i=0;i<256;i++){
        printf("%d ",histogramme[i]);
    }
    printf("\n");
}
