#ifndef PION_H
#define PION_H

#include "util.h"

#include <GLFW/glfw3.h>

#include "Piece.h"
#include "Case.h"

using namespace std;

class Pion: public Piece
{
public:
	Pion( unsigned short laCouleur = BLANC, unsigned short leType = PION );
	~Pion();

	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	//virtual bool validerCase(unsigned char rang, char col);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);
private:
	//
};

Pion::Pion( unsigned short laCouleur, unsigned short leType )
{
	//piecePointer = new Pion(laCouleur,  leType);
	
	setCouleur(laCouleur);
	setType(leType);
}

Pion::~Pion()
{
}

// CARRÉ
void Pion::dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float cote = DX / 2;
	Case laCase( echequier[rang - 1][col - 'A'] );

	//unsigned int VAO, VBO;

	position coinGaucheBas = { laCase.getCentre().x - (cote / 2), laCase.getCentre().y - (cote / 2), 0.0 };
	position coinGaucheHaut = { laCase.getCentre().x - (cote / 2), laCase.getCentre().y + (cote / 2), 0.0 };
	position coinDroiteBas = { laCase.getCentre().x + (cote / 2), laCase.getCentre().y - (cote / 2), 0.0 };
	position coinDroiteHaut = { laCase.getCentre().x + (cote / 2), laCase.getCentre().y + (cote / 2), 0.0 };

	float vertices[] = {  coinGaucheHaut.x, coinGaucheHaut.y, 0.0,
					coinGaucheBas.x, coinGaucheBas.y, 0.0,
					coinDroiteBas.x, coinDroiteBas.y, 0.0,
	
					coinGaucheHaut.x, coinGaucheHaut.y, 0.0,
					coinDroiteHaut.x, coinDroiteHaut.y, 0.0,
					coinDroiteBas.x, coinDroiteBas.y, 0.0
				 };

	setUpAndConfigureObjects( vertices, sizeof(vertices), VBOPion, VAO1 );
	//setUpAndConfigureObjects( vertices, sizeof(vertices), VBO, VAO );
	UpdateScren( sizeof(vertices), window, shaderProgram);
}

// AVANCE DEVANT ET MANGE EN DIAGONALE-AVANT
// "rang1" et "col1" représentent la Case de la pièce(DANS CE CAS-CI... LE PION)
bool Pion::validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide = false,
		 rangeeValide = false;
	Case caseInitiale( echequier[rang1 - 1][col1 - 'A'] ),
		 caseFinale(echequier[rang2 - 1][col2 - 'A']);

	if( sousValiderDeplacement(rang1, col1, rang2, col2) )
	{
		// PREMIER DÉPLACEMENT DU PION
		if (caseInitiale.getPiecePtr()->getCouleur() == BLANC)
		{
			if ((rang1 == 2) && (rang2 == 4))
			{
				rangeeValide = true;
			}
			else if ((rang2 == rang1 + 1)) // ???????????????????????????????????? "rang1 + 1"
			{
				rangeeValide = true;
			}
			else
			{
				return false;
			}
		}
		else // PIONS NOIRS
		{
			if ((rang1 == 7) && (rang2 == 5))
			{
				rangeeValide = true;
			}
			else if ((rang2 == rang1 - 1)) // "rang1 - 1"
			{
				rangeeValide = true;
			}
			else
			{
				return false;
			}
		}

		// LA RANGÉE EST VALIDE, IL FAUT
		// MAINTENANT VÉRIFIER LA COLONNE
		if (rangeeValide) // SAUT DE 1 CASE OU DE 2 CASES
		{
			if ((rang2 == rang1 - 1) || (rang2 == rang1 + 1)) // SAUT DE 1 CASE EN DIAG.
			{
				if ((col2 == col1) && (!caseFinale.getEtat())) // CASE EN AVANT LIBRE
				{
					caseValide = true;
				}
				else if ((col2 == col1 + 1) || (col2 == col1 - 1)) // CASES EN DIAGONALE
				{
					if (caseFinale.getEtat()) // CASE OCCUPÉE
					{
						if (caseFinale.getPiecePtr()->getCouleur() != caseInitiale.getPiecePtr()->getCouleur())
						{
							caseValide = true;
						}
					}
				}
			}
			else if ((col2 == col1)) //  SAUT DE 2 CASES EN AVANT
			{
				if (!caseFinale.getEtat()) // CASE EN AVANT LIBRE
				{
					caseValide = true;
				}
			}
		}
	}

	return caseValide;
}

#endif // !PION_H
