#ifndef ROI_H
#define ROI_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"
#include "Tour.h"

#include <cmath>

class Roi: public Piece
{
public:
	Roi(unsigned short laCouleur = BLANC, unsigned short leType = ROI, bool premierDepl = false);
	~Roi();

	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);

	bool getPremierDeplacement();
	void setPremierDeplacement(bool premierDepl);

private:
	bool premierDeplacement;
};

Roi::Roi(unsigned short laCouleur, unsigned short leType, bool premierDepl)
{
	setCouleur(laCouleur);
	setType(leType);
	setPremierDeplacement(premierDepl);
	setNom("ROI");
}

Roi::~Roi()
{
}

bool Roi::getPremierDeplacement()
{
	return premierDeplacement;
}

void Roi::setPremierDeplacement(bool premierDepl)
{
	premierDeplacement = premierDepl;
}

// TRIANGLE VERS LA GAUCHE
void Roi::dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float L = DX / 2,
		  l = DX / 4;

	Case laCase(echequier[rang - 1][col - 'A']);

	//unsigned int VAO, VBO;

	position coinGaucheBas1  = { laCase.getCentre().x - (l / 2), laCase.getCentre().y - (L / 2), 0.0 };
	position coinGaucheHaut1 = { laCase.getCentre().x - (l / 2), laCase.getCentre().y + (L / 2), 0.0 };
	position coinDroiteBas1  = { laCase.getCentre().x + (l / 2), laCase.getCentre().y - (L / 2), 0.0 };
	position coinDroiteHaut1 = { laCase.getCentre().x + (l / 2), laCase.getCentre().y + (L / 2), 0.0 };

	position coinGaucheBas2  = { laCase.getCentre().x - (L / 2), laCase.getCentre().y - (l / 2), 0.0 };
	position coinGaucheHaut2 = { laCase.getCentre().x - (L / 2), laCase.getCentre().y + (l / 2), 0.0 };
	position coinDroiteBas2  = { laCase.getCentre().x + (L / 2), laCase.getCentre().y - (l / 2), 0.0 };
	position coinDroiteHaut2 = { laCase.getCentre().x + (L / 2), laCase.getCentre().y + (l / 2), 0.0 };

	float vertices[] = {	coinGaucheHaut1.x, coinGaucheHaut1.y, 0.0,
							coinGaucheBas1.x, coinGaucheBas1.y, 0.0,
							coinDroiteBas1.x, coinDroiteBas1.y, 0.0,

							coinGaucheHaut1.x, coinGaucheHaut1.y, 0.0,
							coinDroiteHaut1.x, coinDroiteHaut1.y, 0.0,
							coinDroiteBas1.x, coinDroiteBas1.y, 0.0,

							// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ //
							coinGaucheHaut2.x, coinGaucheHaut2.y, 0.0,
							coinGaucheBas2.x, coinGaucheBas2.y, 0.0,
							coinDroiteBas2.x, coinDroiteBas2.y, 0.0,

							coinGaucheHaut2.x, coinGaucheHaut2.y, 0.0,
							coinDroiteHaut2.x, coinDroiteHaut2.y, 0.0,
							coinDroiteBas2.x, coinDroiteBas2.y, 0.0
						};

	setUpAndConfigureObjects(vertices, sizeof(vertices), VBORoi, VAO1);
	UpdateScren(sizeof(vertices), window, shaderProgram);
}

bool Roi::validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide = false;
	Tour * TourPtr;

	// ....
	if( sousValiderDeplacement(rang1, col1, rang2, col2) )
	{
		if( !getPremierDeplacement() )
		{
			/*******************	ROI BLANC	*******************/
			// DEVRAIT ÊTRE DANS LA CASE INITIALE... LA TOUR AUSSI
			if( (rang1 == 1) && (col1 == 'E') )
			{
				if ((echequier[0][4].getPiecePtr()->getType() == ROI) && (echequier[0][4].getPiecePtr()->getCouleur() == BLANC))
				{
					if ((rang2 == 1) && (col2 == 'G'))
					{
						// Déplacer ROI(G) et Tour(F)
						if (!echequier[0][5].getEtat() && !echequier[0][6].getEtat())
						{
							//......
							if ((echequier[0][7].getPiecePtr()->getType() == TOUR) && (echequier[0][7].getPiecePtr()->getCouleur() == BLANC))
							{
								TourPtr = (Tour *)echequier[0][7].getPiecePtr();

								if (!TourPtr->getPremierDeplacement())
								{
									caseValide = true;
									return true;
								}
							}
						}
					}
					else if ((rang2 == 1) && (col2 == 'C'))
					{
						//.......
						// Déplacer ROI(C) et Tour(D)
						if (!echequier[0][1].getEtat() && !echequier[0][2].getEtat() && !echequier[0][3].getEtat())
						{
							Tour * TourPtr = (Tour *)echequier[0][0].getPiecePtr();

							if (!TourPtr->getPremierDeplacement())
							{
								caseValide = true;
								return true;
							}
						}
					}
				}
			}
			/*******************	ROI BLANC	*******************/
			// ROI NOIR......................................//
			else if( (rang1 == 8) && (col1 == 'E') )
			{
				if ((echequier[7][4].getPiecePtr()->getType() == ROI) && (echequier[7][4].getPiecePtr()->getCouleur() == NOIR))
				{
					if ((rang2 == 8) && (col2 == 'G'))
					{
						// Déplacer ROI(G) et Tour(F)
						if (!echequier[7][5].getEtat() && !echequier[7][6].getEtat())
						{
							//......
							if ((echequier[7][7].getPiecePtr()->getType() == TOUR) && (echequier[7][7].getPiecePtr()->getCouleur() == NOIR))
							{
								TourPtr = (Tour *)echequier[0][7].getPiecePtr();

								if (!TourPtr->getPremierDeplacement())
								{
									caseValide = true;
									return true;
								}
							}
						}
					}
					else if ((rang2 == 8) && (col2 == 'C'))
					{
						//.......
						// Déplacer ROI(C) et Tour(D)
						if (!echequier[7][1].getEtat() && !echequier[7][2].getEtat() && !echequier[7][3].getEtat())
						{
							Tour * TourPtr = (Tour *)echequier[7][0].getPiecePtr();

							if (!TourPtr->getPremierDeplacement())
							{
								caseValide = true;
								return true;
							}
						}
					}
				}
			}
			// ROI NOIR......................................//
		}

		if ((abs(rang2 - rang1) == 1) && (abs(col2 - col1) == 0))
		{
			setPremierDeplacement(true);
			caseValide = true;
		}
		else if ((abs(rang2 - rang1) == 0) && (abs(col2 - col1) == 1))
		{
			setPremierDeplacement(true);
			caseValide = true;
		}
		else if( (abs(rang2 - rang1) == 1) && (abs(col2 - col1) == 1) )
		{
			setPremierDeplacement(true);
			caseValide = true;
		}
	}

	return caseValide;
}

#endif // !ROI_H
