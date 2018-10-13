#ifndef REINE_H
#define REINE_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"

#include <cmath>

class Reine : public Piecee
{
public:
	Reine(couleur_t laCouleur = BLANC, type_t leType = PION);
	~Reine();

	virtual void dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerCase(unsigned char rang, char col);

private:
	//.....
	bool validerLesDiagonales(unsigned char rang1, char col1, unsigned char rang2, char col2)
	{
		bool caseValide = false;

		// VÉRIFIER LA DIAGONALE
		if (abs(rang1 - rang2) == abs(col1 - col2))
		{
			if ((rang2 > rang1) && (col2 > col1))
			{
				char colonne = col2 + 1;
				unsigned char rangee = rang1 + 1;

				while (rangee < rang2)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee++;
					colonne++;
				}
			}
			else if ((rang2 < rang1) && (col2 < col1))
			{
				char colonne = col2 - 1;
				unsigned char rangee = rang1 - 1;

				while (rangee > rang2)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee--;
					colonne--;
				}
			}
			else if ((rang2 > rang1) && (col2 < col1))
			{
				char colonne = col2 - 1;
				unsigned char rangee = rang1 + 1;

				while (rangee < rang2)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee++;
					colonne--;
				}
			}
			else // (rang2 < rang1) && (col2 > col1)
			{
				char colonne = col2 + 1;
				unsigned char rangee = rang1 - 1;

				while (rangee > rang2)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee--;
					colonne++;
				}
			}

			caseValide = true;
		}

		return caseValide;
	}
	bool validerLesDiagonales(unsigned char rang1, char col1, unsigned char rang2, char col2)
	{
		bool caseValide = false;

		// VÉRIFIER LA DIAGONALE
		if (abs(rang1 - rang2) == abs(col1 - col2))
		{
			if ((rang2 > rang1) && (col2 > col1))
			{
				char colonne = col2 + 1;
				unsigned char rangee = rang1 + 1;

				while (rangee < rang2)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee++;
					colonne++;
				}
			}
			else if ((rang2 < rang1) && (col2 < col1))
			{
				char colonne = col2 - 1;
				unsigned char rangee = rang1 - 1;

				while (rangee > rang2)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee--;
					colonne--;
				}
			}
			else if ((rang2 > rang1) && (col2 < col1))
			{
				char colonne = col2 - 1;
				unsigned char rangee = rang1 + 1;

				while (rangee < rang2)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee++;
					colonne--;
				}
			}
			else // (rang2 < rang1) && (col2 > col1)
			{
				char colonne = col2 + 1;
				unsigned char rangee = rang1 - 1;

				while (rangee > rang2)
				{
					if (echequier[rangee - 1][col2 - 1].getEtat())
					{
						return false;
					}

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee--;
					colonne++;
				}
			}

			caseValide = true;
		}

		return caseValide;
	}
};

Reine::Reine(couleur_t laCouleur, type_t leType)
{
	setCouleur(laCouleur);
	setType(leType);
}

Reine::~Reine()
{
}

// TRIANGLE VERS LA GAUCHE
void Reine::dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float L = DX / 4,
		l = DX / 8;

	Case laCase(echequier[rang - 1][col - 1]);

	unsigned int VAO, VBO;

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

	setUpAndConfigureObjects(vertices, VBO, VAO);
	UpdateScren(sizeof(vertices), window, shaderProgram);
}

bool Reine::validerCase(unsigned char rang, char col)
{
	bool caseValide = false;

	if ((rang >= 1) && (rang <= 8) && (col >= 'A') && (col <= 'H'))
	{
		// CASE INOCCUPEE OU PIÈCE ADVERSE
		if (validerPositionFinale(rang, col))
		{
		}
	}

	return caseValide;
}




#endif // !REINE_H
