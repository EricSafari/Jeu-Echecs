#ifndef FOU_H
#define FOU_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"

#include <cmath>

class Fou : public Piece
{
public:
	Fou(unsigned short laCouleur = BLANC, unsigned short leType = FOU, unsigned int laTexture = 0);
	~Fou();

	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);

	string getNom();
	void setNom(string nom);
private:
	// ...
	string nom;
	bool validerLesDiagonales(unsigned char rang1, char col1, unsigned char rang2, char col2);
};

Fou::Fou( unsigned short laCouleur, unsigned short leType, unsigned int laTexture )
:Piece(laCouleur, leType, laTexture)
{
	/*setCouleur(laCouleur);
	setType(leType);*/
	setNom("FOU");
}

Fou::~Fou()
{
}

void Fou::setNom(string nomDeLaPiece)
{
	nom = nomDeLaPiece;
}

string Fou::getNom()
{
	return nom;
}

// TRIANGLE VERS LA GAUCHE
void Fou::dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float base = DX / 2,
	   hauteur = DX / 2;

	Case laCase(echequier[rang - 1][col - 'A']);

	//unsigned int VAO, VBO;
	unsigned char * dataImg = getImageData();

	position coinGauche = { laCase.getCentre().x - (hauteur / 2), laCase.getCentre().y, 0.0 };
	position coinBas  = { laCase.getCentre().x + (hauteur / 2), laCase.getCentre().y - (base / 2), 0.0 };
	position coinHaut = { laCase.getCentre().x + (hauteur / 2), laCase.getCentre().y + (base / 2), 0.0 };

	float vertices[] = {
		coinGauche.x, coinGauche.y, 0.0,
		coinHaut.x, coinHaut.y, 0.0,
		coinBas.x, coinBas.y, 0.0
	};

	setUpAndConfigureObjects(vertices, sizeof(vertices), indicesTextures, VBOFou, VAO1, EBO1, dataImg);
	//setUpAndConfigureObjects(vertices, sizeof(vertices), VBOFou, VAO1);
	UpdateScren(sizeof(vertices), window, shaderProgram);
}

bool Fou::validerLesDiagonales(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide = false;

	// .....
	if (sousValiderDeplacement(rang1, col1, rang2, col2))
	{
		char colonne;
		unsigned char rangee;

		// VÉRIFIER LA DIAGONALE
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

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
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

					// LA COLONNE ET LA RANGÉE CHANGENT SIMULTANÉMENT
					rangee--;
					colonne--;
				}
			}
			else if ((rang2 > rang1) && (col2 < col1))
			{
				colonne = col1 - 1;
				rangee = rang1 + 1;

				while (rangee < rang2)
				{
					if (echequier[rangee - 1][colonne - 'A'].getEtat())
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
				colonne = col1 + 1;
				rangee = rang1 - 1;

				while (rangee > rang2)
				{
					if (echequier[rangee - 1][colonne - 'A'].getEtat())
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

	return caseValide;
}

bool Fou::validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide = validerLesDiagonales(rang1, col1, rang2, col2);

	return caseValide;
}

#endif // !FOU_H