#ifndef TOUR_H
#define TOUR_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"
#include <cmath>

class Tour : public Piecee
{
public:
	Tour(couleur_t laCouleur = BLANC, type_t leType = PION);
	~Tour();

	virtual void dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerCase(unsigned char rang, char col);

private:
	// DÉPEND DE LA PIÈCE
	bool validerLesDroites(unsigned char rang1, char col1, unsigned char rang2, char col2)
	{
		bool positionValide = false;

		if ((rang2 == rang1) && (col2 != col1))
		{
			if (col2 > col1)
			{
				for (char colonne = col1 + 1; colonne++; colonne < col2)
				{
					if (echequier[rang2 - 1][colonne - 1].getEtat())
					{
						return false;
					}
				}
			}
			else
			{
				for (char colonne = col2 + 1; colonne++; colonne < col1)
				{
					if (echequier[rang2 - 1][colonne - 1].getEtat())
					{
						return false;
					}
				}
			}
		}
		else if ((col2 == col1) && (rang2 != rang1))
		{
			if (rang2 > rang1)
			{
				for (unsigned char rangee = rang1 + 1; rangee < rang2; rangee++)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}
				}
			}
			else
			{
				for (unsigned char rangee = rang2 + 1; rangee < rang1; rangee++)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}
				}
			}
		}

		return positionValide;
	}
};

Tour::Tour(couleur_t laCouleur, type_t leType)
{
	setCouleur(laCouleur);
	setType(leType);
}

Tour::~Tour()
{
}

// LOSANGE
void Tour::dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float base	  = DX / 4,
		  hauteur = DX / 4;
	
	Case laCase( echequier[rang - 1][col - 1] );

	unsigned int VAO, VBO;

	position coinGauche = { laCase.getCentre().x - (base / 2), laCase.getCentre().y, 0.0 };
	position coinDroit  = { laCase.getCentre().x + (base / 2), laCase.getCentre().y, 0.0 };
	position coinBas	= { laCase.getCentre().x, laCase.getCentre().y - hauteur, 0.0 };
	position coinHaut   = { laCase.getCentre().x, laCase.getCentre().y + hauteur, 0.0 };

	float vertices[] = {
						coinGauche.x, coinGauche.y, 0.0,
						coinHaut.x, coinHaut.y, 0.0,
						coinDroit.x, coinDroit.y, 0.0,

						coinGauche.x, coinGauche.y, 0.0,
						coinBas.x, coinBas.y, 0.0,
						coinDroit.x, coinDroit.y, 0.0,
					   };

	setUpAndConfigureObjects(vertices, VBO, VAO);
	UpdateScren(sizeof(vertices), window, shaderProgram);
}

bool Tour::validerCase(unsigned char rang, char col)
{
	bool caseValide = false;
	/*Case caseInitiale(echequier[rangeeTemp - 1][colonneTemp - 1]),
		caseFinale(echequier[rang - 1][col - 1]);*/

	if ((rang >= 1) && (rang <= 8) && (col >= 'A') && (col <= 'H'))
	{
		if( validerPositionFinale(rang, col) )
		{
			if ((rang == rangeeTemp) && (col != colonneTemp))
			{
				if (col > colonneTemp)
				{
					for (char colonne = colonneTemp + 1; colonne++; colonne < col)
					{
						if (echequier[rang - 1][colonne - 1].getEtat())
						{
							return false;
						}
					}
				}
				else
				{
					for (char colonne = col + 1; colonne++; colonne < colonneTemp)
					{
						if (echequier[rang - 1][colonne - 1].getEtat())
						{
							return false;
						}
					}
				}

				//if (!echequier[rang - 1][col - 1].getEtat()) // CASE FINALE VIDE
				//{
				//	caseValide = true;
				//}
				//else // CASE OCCUPEE
				//{
				//	if ((caseInitiale.getPiece().couleur == NOIR) && (caseFinale.getPiece().couleur == BLANC))
				//	{
				//		caseValide = true;
				//	}
				//	else if ((caseInitiale.getPiece().couleur == BLANC) && (caseFinale.getPiece().couleur == NOIR))
				//	{
				//		caseValide = true;
				//	}
				//}
			}
			else if ((col == colonneTemp) && (rang != rangeeTemp))
			{
				if (rang > rangeeTemp)
				{
					for (unsigned char rangee = rangeeTemp + 1; rangee < rang; rangee++)
					{
						if (echequier[rangee - 1][col - 1].getEtat())
						{
							return false;
						}
					}
				}
				else
				{
					for (unsigned char rangee = rang + 1; rangee < rangeeTemp; rangee++)
					{
						if (echequier[rangee - 1][col - 1].getEtat())
						{
							return false;
						}
					}
				}

				//if (!echequier[rang - 1][col - 1].getEtat()) // CASE FINALE VIDE
				//{
				//	caseValide = true;
				//}
				//else // CASE OCCUPEE
				//{
				//	if ((caseInitiale.getPiece().couleur == NOIR) && (caseFinale.getPiece().couleur == BLANC))
				//	{
				//		caseValide = true;
				//	}
				//	else if ((caseInitiale.getPiece().couleur == BLANC) && (caseFinale.getPiece().couleur == NOIR))
				//	{
				//		caseValide = true;
				//	}
				//}
			}
		}
	}

	return caseValide;
}

#endif // !TOUR_H
