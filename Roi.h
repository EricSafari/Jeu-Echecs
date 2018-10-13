#ifndef ROI_H
#define ROI_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"

#include <cmath>

class Roi: public Piecee
{
public:
	Roi(couleur_t laCouleur = BLANC, type_t leType = PION);
	~Roi();

	virtual void dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerCase(unsigned char rang, char col);

private:

};

Roi::Roi(couleur_t laCouleur, type_t leType)
{
	setCouleur(laCouleur);
	setType(leType);
}

Roi::~Roi()
{
}

// TRIANGLE VERS LA GAUCHE
void Roi::dessiner(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float L = DX / 4,
		  l = DX / 8;

	Case laCase(echequier[rang - 1][col - 1]);

	unsigned int VAO, VBO;

	position coinGaucheBas  = { laCase.getCentre().x - (l / 2), laCase.getCentre().y - (L / 2), 0.0 };
	position coinGaucheHaut = { laCase.getCentre().x - (l / 2), laCase.getCentre().y + (L / 2), 0.0 };
	position coinDroiteBas  = { laCase.getCentre().x + (l / 2), laCase.getCentre().y - (L / 2), 0.0 };
	position coinDroiteHaut = { laCase.getCentre().x + (l / 2), laCase.getCentre().y + (L / 2), 0.0 };

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

bool Roi::validerCase(unsigned char rang, char col)
{
	bool caseValide = false;

	if ((rang >= 1) && (rang <= 8) && (col >= 'A') && (col <= 'H'))
	{
		// CASE INOCCUPEE OU PIÈCE ADVERSE
		if( validerPositionFinale(rang, col) )
		{
			if ((abs(rang - rangeeTemp) == 1) && (abs(col - colonneTemp) == 0))
			{
				caseValide = true;
			}
			else if ((abs(rang - rangeeTemp) == 0) && (abs(col - colonneTemp) == 1))
			{
				caseValide = true;
			}
			else // ( abs(rang - rangeeTemp) == 1) && (abs(col - colonneTemp) == 1 )
			{
				caseValide = true;
			}
		}
	}

	return caseValide;
}

#endif // !ROI_H
