#ifndef REINE_H
#define REINE_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"

#include <cmath>

class Reine : public Piece
{
public:
	Reine(unsigned short laCouleur = BLANC, unsigned short leType = REINE);
	~Reine();

	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);

private:
	//.....
	bool validerLaRangee(unsigned char rang1, char col1, unsigned char rang2, char col2);
	bool validerLaColonne(unsigned char rang1, char col1, unsigned char rang2, char col2);
	bool validerLesDiagonales(unsigned char rang1, char col1, unsigned char rang2, char col2);
};

Reine::Reine(unsigned short laCouleur, unsigned short leType)
{
	setCouleur(laCouleur);
	setType(leType);
	setNom("REINE");
}

Reine::~Reine()
{
}

// TRIANGLE VERS LA GAUCHE
void Reine::dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float L = DX / 2,
		  l = DX / 4;

	Case laCase(echequier[rang - 1][col - 'A']);

	//unsigned int VAO, VBO;

	position coinGaucheBas  = { laCase.getCentre().x - (L / 2), laCase.getCentre().y - (l / 2), 0.0 };
	position coinGaucheHaut = { laCase.getCentre().x - (L / 2), laCase.getCentre().y + (l / 2), 0.0 };
	position coinDroiteBas  = { laCase.getCentre().x + (L / 2), laCase.getCentre().y - (l / 2), 0.0 };
	position coinDroiteHaut = { laCase.getCentre().x + (L / 2), laCase.getCentre().y + (l / 2), 0.0 };

	float vertices[] = {	coinGaucheHaut.x, coinGaucheHaut.y, 0.0,
							coinGaucheBas.x, coinGaucheBas.y, 0.0,
							coinDroiteBas.x, coinDroiteBas.y, 0.0,

							coinGaucheHaut.x, coinGaucheHaut.y, 0.0,
							coinDroiteHaut.x, coinDroiteHaut.y, 0.0,
							coinDroiteBas.x, coinDroiteBas.y, 0.0
						};

	setUpAndConfigureObjects(vertices, sizeof(vertices), VBOReine, VAO1);
	UpdateScren(sizeof(vertices), window, shaderProgram);
}

bool Reine::validerLaRangee(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	if (sousValiderDeplacement(rang1, col1, rang2, col2))
	{
		if ((rang2 == rang1) && (col2 != col1))
		{
			if (col2 > col1)
			{
				for (char colonne = col1 + 1; colonne < col2; colonne++)
				{
					// INOCCUPEE
					if (!echequier[rang2 - 1][colonne - 'A'].getEtat())
					{
						//return true;
					}
					else // OCCUPEE
					{
						return false;
					}
				}
				// TOUTES LES CASES INTERMEDIAIRES SONT VIDES
				return true;
			}
			else
			{
				for (char colonne = col1 - 1; colonne > col2; colonne--)
				{
					// INOCCUPEE
					if (!echequier[rang2 - 1][colonne - 'A'].getEtat())
					{
						//return true;
					}
					else // OCCUPEE
					{
						return false;
					}
				}
				// TOUTES LES CASES INTERMEDIAIRES SONT VIDES
				return true;
			}
		}
	}

	// POUR LE CAS O� LE PREMIER "IF"
	// NE RETOURNE PAS DE VALEUR
	return false;
}

bool Reine::validerLaColonne(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool colonneValide = false;

	if (sousValiderDeplacement(rang1, col1, rang2, col2))
	{
		if ((col2 == col1) && (rang2 != rang1))
		{
			if (rang2 > rang1)
			{
				for (unsigned char rangee = rang1 + 1; rangee < rang2; rangee++)
				{
					// INOCCUPEE
					if (!echequier[rangee - 1][col1 - 'A'].getEtat())
					{
						//return true;
					}
					else // OCCUPEE
					{
						return false;
					}
				}
				//...
				return true;
			}
			else
			{
				for (unsigned char rangee = rang1 - 1; rangee > rang2; rangee--)
				{
					// INOCCUPEE
					if (!echequier[rangee - 1][col1 - 'A'].getEtat())
					{
						//return true;
					}
					else // OCCUPEE
					{
						return false;
					}
				}
				//...
				return true;
			}
		}
	}

	// POUR LE CAS O� LE PREMIER "IF"
	// NE RETOURNE PAS DE VALEUR
	return colonneValide;
}

bool Reine::validerLesDiagonales(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide = false;

	// .....
	if (sousValiderDeplacement(rang1, col1, rang2, col2))
	{
		char colonne;
		unsigned char rangee;

		// V�RIFIER LA DIAGONALE
		if (abs(rang1 - rang2) == abs(col1 - col2))
		{
			if ((rang2 > rang1) && (col2 > col1))
			{
				colonne = col1 + 1;
				rangee = rang1 + 1;

				while (rangee < rang2)
				{
					if (echequier[rangee - 1][colonne - 'A'].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANG�E CHANGENT SIMULTAN�MENT
					rangee++;
					colonne++;
				}
			}
			else if ((rang2 < rang1) && (col2 < col1))
			{
				colonne = col1 - 1;
				rangee = rang1 - 1;

				while (rangee > rang2)
				{
					if (echequier[rangee - 1][colonne - 'A'].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANG�E CHANGENT SIMULTAN�MENT
					rangee--;
					colonne--;
				}
			}
			else if ((rang2 > rang1) && (col2 < col1))
			{
				//colonne = col2 + 1;
				colonne = col1 - 1;
				rangee = rang1 + 1;

				while (rangee < rang2)
				{
					if (echequier[rangee - 1][colonne - 'A'].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANG�E CHANGENT SIMULTAN�MENT
					rangee++;
					colonne--;
				}
			}
			else // (rang2 < rang1) && (col2 > col1)
			{
				colonne = col1 + 1;
				rangee = rang1 - 1;

				while (rangee > rang2)
				{
					if (echequier[rangee - 1][colonne - 'A'].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANG�E CHANGENT SIMULTAN�MENT
					rangee--;
					colonne++;
				}
			}

			caseValide = true;
		}
	}

	return caseValide;
}

bool Reine::validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide;

	caseValide = validerLaRangee(rang1, col1, rang2, col2) || validerLaColonne(rang1, col1, rang2, col2) || validerLesDiagonales(rang1, col1, rang2, col2);
	//caseValide = validerLaRangee(rang1, col1, rang2, col2) || validerLaColonne(rang1, col1, rang2, col2) || validerLaColonne(rang1, col1, rang2, col2);

	return caseValide;
}




#endif // !REINE_H
