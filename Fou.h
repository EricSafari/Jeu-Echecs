#ifndef FOU_H
#define FOU_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"

#include <cmath>

class Fou : public Piecee
{
public:
	Fou(couleur_t laCouleur = BLANC, type_t leType = PION);
	~Fou();

	virtual void dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerCase(unsigned char rang, char col);

private:
	// ...
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

Fou::Fou( couleur_t laCouleur, type_t leType )
{
	setCouleur(laCouleur);
	setType(leType);
}

Fou::~Fou()
{
}

// TRIANGLE VERS LA GAUCHE
void Fou::dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float base  = DX / 4,
		hauteur = DX / 4;

	Case laCase(echequier[rang - 1][col - 1]);

	unsigned int VAO, VBO;

	position coinGauche = { laCase.getCentre().x - hauteur, laCase.getCentre().y, 0.0 };
	//position coinDroit = { laCase.getCentre().x + (base / 2), laCase.getCentre().y, 0.0 };
	position coinBas  = { laCase.getCentre().x, laCase.getCentre().y - (base / 2), 0.0 };
	position coinHaut = { laCase.getCentre().x, laCase.getCentre().y + (base / 2), 0.0 };

	float vertices[] = {
		coinGauche.x, coinGauche.y, 0.0,
		coinHaut.x, coinHaut.y, 0.0,
		coinBas.x, coinBas.y, 0.0
	};

	setUpAndConfigureObjects(vertices, VBO, VAO);
	UpdateScren(sizeof(vertices), window, shaderProgram);
}

bool Fou::validerCase(unsigned char rang, char col)
{
	bool caseValide = false;

	//Case caseInitiale(echequier[rangeeTemp - 1][colonneTemp - 1]),
	//	caseFinale(echequier[rang - 1][col - 1]);

	if ((rang >= 1) && (rang <= 8) && (col >= 'A') && (col <= 'H'))
	{
		// CASE INOCCUPEE OU PIÈCE ADVERSE
		if( validerPositionFinale(rang, col) )
		{
			// VÉRIFIER LA DIAGONALE
			if( abs(rangeeTemp - rang) == abs(colonneTemp - col) )
			{
				if ((rang > rangeeTemp) && (col > colonneTemp))
				{
					char colonne = col + 1;
					unsigned char rangee = rangeeTemp + 1;

					while ( rangee < rang )
					{
						if( echequier[rangee - 1][col - 1].getEtat() )
						{
							return false;
						}

						// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
						rangee++;
						colonne++;
					}
				}
				else if ((rang < rangeeTemp) && (col < colonneTemp))
				{
					char colonne = col - 1;
					unsigned char rangee = rangeeTemp - 1;

					while (rangee > rang)
					{
						if (echequier[rangee - 1][col - 1].getEtat())
						{
							return false;
						}

						// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
						rangee--;
						colonne--;
					}
				}
				else if ((rang > rangeeTemp) && (col < colonneTemp))
				{
					char colonne = col - 1;
					unsigned char rangee = rangeeTemp + 1;

					while (rangee < rang)
					{
						if (echequier[rangee - 1][col - 1].getEtat())
						{
							return false;
						}

						// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
						rangee++;
						colonne--;
					}
				}
				else // (rang < rangeeTemp) && (col > colonneTemp)
				{
					char colonne = col + 1;
					unsigned char rangee = rangeeTemp - 1;

					while (rangee > rang)
					{
						if (echequier[rangee - 1][col - 1].getEtat())
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
		}
	}

	return caseValide;
}

#endif // !FOU_H