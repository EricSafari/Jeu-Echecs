#ifndef UTIL_H
#define UTIL_H

extern void processInput(GLFWwindow *window);

#include <GLFW/glfw3.h>
#include "Case.h"

#define BLANC 0
#define NOIR  1

#define PION		0
#define TOUR		1
#define CAVALIER	2
#define FOU			3
#define REINE		4
#define ROI			5

#define DEBUG

Case echequier[NB_LIGNE][NB_COL];
//
enum joueur_tour_t { JOUEUR_BLANC = 0, JOUEUR_NOIR = 1 };
unsigned short joueur = JOUEUR_BLANC;

unsigned char rangeeTemp;
char colonneTemp;

unsigned char rangeeDepart;
char colonneDepart;
unsigned char rangeeArrivee;
char colonneArrivee;

unsigned char RangeeDuRoiBlanc = 1;
char ColonneDuRoiBlanc = 'E';

unsigned char RangeeDuRoiNoir = 8;
char ColonneDuRoiNoir = 'E';

//-----------------------------------------//
//extern void processInput(GLFWwindow *window);
//-----------------------------------------//

// Compare les positions initiales et finales en tenant
// compte du joueur qui joue
bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool PositionValide = false;

	Case caseInitiale(echequier[rang1 - 1][col1 - 'A']),
		 caseFinale(echequier[rang2 - 1][col2 - 'A']);

	if (!caseFinale.getEtat())
	{
		PositionValide = true;
	}
	else
	{
		/*if ((caseInitiale.getPiecePtr()->couleur == NOIR) && (caseFinale.getPiecePtr()->couleur == BLANC))
		{
		PositionValide = true;
		}
		else if ((caseInitiale.getPiecePtr()->couleur == BLANC) && (caseFinale.getPiecePtr()->couleur == NOIR))
		{
		PositionValide = true;
		}*/
		if ((caseInitiale.getPiecePtr()->getCouleur() != caseFinale.getPiecePtr()->getCouleur()) && (caseInitiale.getPiecePtr()->getCouleur() == joueur))
		{
			PositionValide = true;
		}
	}

	return PositionValide;
}

#endif // !UTIL_H

