#ifndef CAVALIER_H
#define CAVALIER_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"

#include <cmath>

class Cavalier : public Piecee
{
public:
	Cavalier(couleur_t laCouleur = BLANC, type_t leType = PION);
	~Cavalier();

	virtual void dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerCase(unsigned char rang, char col);

private:
};

Cavalier::Cavalier( couleur_t laCouleur, type_t leType )
{
	setCouleur(laCouleur);
	setType(leType);
}

Cavalier::~Cavalier()
{
}

// TRIANGLE VERS LA DROITE
void Cavalier::dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float base = DX / 4,
	   hauteur = DX / 4;

	Case laCase(echequier[rang - 1][col - 1]);

	unsigned int VAO, VBO;

	position coinDroit = { laCase.getCentre().x + hauteur, laCase.getCentre().y, 0.0 };
	position coinBas = { laCase.getCentre().x, laCase.getCentre().y - (base / 2), 0.0 };
	position coinHaut = { laCase.getCentre().x, laCase.getCentre().y + (base / 2), 0.0 };

	float vertices[] = {
		coinDroit.x, coinDroit.y, 0.0,
		coinHaut.x, coinHaut.y, 0.0,
		coinBas.x, coinBas.y, 0.0
	};

	setUpAndConfigureObjects(vertices, VBO, VAO);
	UpdateScren(sizeof(vertices), window, shaderProgram);
}

bool Cavalier::validerCase(unsigned char rang, char col)
{
	bool caseValide = false;

	//Case caseInitiale(echequier[rangeeTemp - 1][colonneTemp - 1]),
	//	 caseFinale(echequier[rang - 1][col - 1]);

	if ((rang >= 1) && (rang <= 8) && (col >= 'A') && (col <= 'H'))
	{
		if( validerPositionFinale(rang, col) )
		{
			// VÉRIFIER SI LE DÉPLACEMENT EST PERMIS POUR LE "CAVALIER"
			if( (abs(rangeeTemp - rang) == 2) && (abs(colonneTemp - col) == 1))
			{
				caseValide = true;
			}
			else if( (abs(rangeeTemp - rang) == 1) && (abs(colonneTemp - col) == 2) )
			{
				caseValide = true;
			}
		}
	}

	return caseValide;
}

#endif // !CAVALIER_H
