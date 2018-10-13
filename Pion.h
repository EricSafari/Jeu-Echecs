#ifndef PION_H
#define PION_H

#include "util.h"
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <iostream>
#include "Piece.h"
#include "Case.h"

using namespace std;

class Pion: public Piecee
{
public:
	Pion( couleur_t laCouleur = BLANC, type_t leType = PION );
	~Pion();

	virtual void dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerCase(unsigned char rang, char col);
private:

};

Pion::Pion( couleur_t laCouleur, type_t leType )
{
	setCouleur(laCouleur);
	setType(leType);
}

Pion::~Pion()
{
}

// CARRÉ
void Pion::dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float cote = DX / 4;
	//Case laCase(rang, col);
	Case laCase( echequier[rang - 1][col - 1] );

	unsigned int VAO, VBO;

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

	setUpAndConfigureObjects( vertices, VBO, VAO );
	UpdateScren( sizeof(vertices), window, shaderProgram);
}

// AVANCE DEVANT ET MANGE EN DIAGONALE-AVANT
bool Pion::validerCase(unsigned char rang, char col)
{
	bool caseValide = false;
	Case caseInitiale( echequier[rangeeTemp - 1][colonneTemp - 1] ),
		 caseFinale(echequier[rang - 1][col - 1]);

	if( (rang >= 1) && (rang <= 8) && (col >= 'A') && (col <= 'H'))
	{
		if( (rang == rangeeTemp + 1) )
		{
			if ((col == colonneTemp) && (!caseFinale.getEtat()) ) // CASE EN AVANT LIBRE
			{
				caseValide = true;
			}
			else if ((col == colonneTemp + 1)  || (col == colonneTemp - 1)) // CASES EN DIAGONALE
			{
				if( caseFinale.getEtat() ) // CASE OCCUPÉE
				{
					if ((caseInitiale.getPiece().couleur == NOIR) && (caseFinale.getPiece().couleur == BLANC))
					{
						caseValide = true;
					}
					else if ((caseInitiale.getPiece().couleur == BLANC) && (caseFinale.getPiece().couleur == NOIR))
					{
						caseValide = true;
					}
				}
			}
		}
	}

	return caseValide;
}

#endif // !PION_H
