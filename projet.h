#include<stdio.h>
#include<stdlib.h>
#define MAX_PIXEL 255

typedef struct {
     unsigned char r,v,b;
} Pixel;

typedef struct {
     unsigned char g;
} PixelGris;

typedef struct {
     int h, l;
     Pixel *pixels;
} ImageCouleur;

typedef struct {
     int h, l;
     PixelGris *pixels;
} ImageGris;

ImageCouleur *chargerImage(const char *fichier);
void sauvgarderImage(const char *fichier, ImageCouleur *img);
void sauvgarderImageGris(const char *fichier, ImageGris *img);
Pixel* inversionCouleur(Pixel *pixels,int taille);
PixelGris* changerCouleurGris(Pixel *pixels,int taille);
int * calculHistogramme(PixelGris* pixels,int taille);
Pixel * fusionImages(Pixel *pixels1,Pixel *pixels2,int taille);
ImageGris * creerImageGris(int h,int l);
ImageCouleur * creerImageCouleur(int h,int l);
void afficherHistogramme(int* histogramme);
