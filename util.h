#ifndef UTIL_H
#define UTIL_H

#include <GLFW/glfw3.h>

#define BLANC 0
#define NOIR  1

#define PION		0
#define TOUR		1
#define CAVALIER	2
#define FOU			3
#define REINE		4
#define ROI			5

#define DEBUG
//
enum joueur_tour_t { JOUEUR_BLANC = 0, JOUEUR_NOIR = 1 };
unsigned short joueur = JOUEUR_BLANC;
/*
float vertices[NB_VERTICES]; // NB_VERTICES

const int const indices[6] =
{  
    0, 1, 2, // 1er Triangle
    0, 3, 2  // 2e  Triangle
};
*/
unsigned int VAO1;
unsigned int EBO1;
unsigned int VBOPion,
			 VBOTour,
			 VBOCavalier,
			 VBOFou,
			 VBOReine,
			 VBORoi;

unsigned char rangeeTemp;
char colonneTemp;

unsigned char rangeeDepart;
char colonneDepart;
unsigned char rangeeArrivee;
char colonneArrivee;

unsigned char RangeeDuRoiBlanc = 1;
unsigned char RangeeDuRoiNoir  = 8;
char ColonneDuRoiBlanc = 'E';
char ColonneDuRoiNoir  = 'E';

unsigned char RangeeDuRoi		 = RangeeDuRoiBlanc;
unsigned char RangeeDuRoiAdverse = RangeeDuRoiNoir;
char ColonneDuRoi		 = ColonneDuRoiBlanc;
char ColonneDuRoiAdverse = ColonneDuRoiNoir;

unsigned short couleurDuRoi = BLANC,
			   couleurDuRoiAdverse = NOIR;

//-----------------------------------------//
extern void processInput(GLFWwindow *window);
//-----------------------------------------//

#endif // !UTIL_H

