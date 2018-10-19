#ifndef UTIL_H
#define UTIL_H

//extern void processInput(GLFWwindow *window);

#include <GLFW/glfw3.h>
//#include "Case.h"

#define BLANC 0
#define NOIR  1

#define PION		0
#define TOUR		1
#define CAVALIER	2
#define FOU			3
#define REINE		4
#define ROI			5

#define DEBUG

//Case echequier[NB_LIGNE][NB_COL];
//
enum joueur_tour_t { JOUEUR_BLANC = 0, JOUEUR_NOIR = 1 };
unsigned short joueur = JOUEUR_BLANC;

unsigned VAO1;
unsigned int VBOPion,
			 VBOTour,
			 VBOCavalier,
			 VBOFou,
			 VBOReine,
			 VBORoi;

//unsigned short BufferIndex = 0;

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

//// Compare les positions initiales et finales en tenant
//// compte du joueur qui joue...
//bool sousValiderDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
//{
//	bool PositionValide = false;
//
//	if ((rang1 >= 1) && (rang1 <= 8) && (col1 >= 'A') && (col1 <= 'H'))
//	{
//		if ((rang2 >= 1) && (rang2 <= 8) && (col2 >= 'A') && (col2 <= 'H'))
//		{
//			if ((rang1 != rang2) || (col1 != col2))
//			{
//				Case caseInitiale(echequier[rang1 - 1][col1 - 'A']),
//					 caseFinale(echequier[rang2 - 1][col2 - 'A']);
//
//				if (!caseFinale.getEtat())
//				{
//					PositionValide = true;
//				}
//				else
//				{
//					/*if ((caseInitiale.getPiecePtr()->couleur == NOIR) && (caseFinale.getPiecePtr()->couleur == BLANC))
//					{
//					PositionValide = true;
//					}
//					else if ((caseInitiale.getPiecePtr()->couleur == BLANC) && (caseFinale.getPiecePtr()->couleur == NOIR))
//					{
//					PositionValide = true;
//					}*/
//					if ((caseInitiale.getPiecePtr()->getCouleur() != caseFinale.getPiecePtr()->getCouleur()) && (caseInitiale.getPiecePtr()->getCouleur() == joueur))
//					{
//						PositionValide = true;
//					}
//				}
//			}
//		}
//	}
//
//	return PositionValide;
//}

#endif // !UTIL_H

