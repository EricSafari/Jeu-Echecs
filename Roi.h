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
	Roi(unsigned short laCouleur = BLANC, unsigned short leType = ROI, bool premierDepl = false, bool petitRoque = false, bool grandRoque = false, unsigned int laTexture = 0);
	~Roi();

	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);

	bool getPremierDeplacement();
	void setPremierDeplacement(bool premierDepl);
	bool getPetitRoque();
	void setPetitRoque(bool petitRoq);
	bool getGrandRoque();
	void setGrandRoque(bool grandRoq);

	string getNom();
	void setNom(string nom);
private:
	bool premierDeplacement;
	bool PetitRoque;
	bool GrandRoque;
	string nom;
};

Roi::Roi(unsigned short laCouleur, unsigned short leType, bool premierDepl, bool petitRoque, bool grandRoque, unsigned int laTexture)
:Piece(laCouleur, leType, laTexture)
{
	/*setCouleur(laCouleur);
	setType(leType);*/
	setPremierDeplacement(premierDepl);
	setPetitRoque(petitRoque);
	setGrandRoque(grandRoque);
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

bool Roi::getPetitRoque()
{
	return PetitRoque;
}
void Roi::setPetitRoque(bool petitRoq)
{
	PetitRoque = petitRoq;
}

bool Roi::getGrandRoque()
{
	return GrandRoque;
}

void Roi::setGrandRoque(bool grandRoq)
{
	GrandRoque = grandRoq;
}

void Roi::setNom(string nomDeLaPiece)
{
	nom = nomDeLaPiece;
}

string Roi::getNom()
{
	return nom;
}

// TRIANGLE VERS LA GAUCHE
void Roi::dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float L = DX / 2,
		  l = DX / 2;

	Case laCase(echequier[rang - 1][col - 'A']);

	//unsigned int VAO, VBO, EBO;
	unsigned char * dataImg = laCase.getPiecePtr()->getImageData();

	position coinGaucheBas1  = { laCase.getCentre().x - l, laCase.getCentre().y - (L / 2), 0.0 };
	position coinGaucheHaut1 = { laCase.getCentre().x - l, laCase.getCentre().y + (L / 2), 0.0 };
	position coinDroiteBas1  = { laCase.getCentre().x + l, laCase.getCentre().y - (L / 2), 0.0 };
	position coinDroiteHaut1 = { laCase.getCentre().x + l, laCase.getCentre().y + (L / 2), 0.0 };

	position coinGaucheBas2  = { laCase.getCentre().x - (L / 2), laCase.getCentre().y - l, 0.0 };
	position coinGaucheHaut2 = { laCase.getCentre().x - (L / 2), laCase.getCentre().y + l, 0.0 };
	position coinDroiteBas2  = { laCase.getCentre().x + (L / 2), laCase.getCentre().y - l, 0.0 };
	position coinDroiteHaut2 = { laCase.getCentre().x + (L / 2), laCase.getCentre().y + l, 0.0 };

	if (!false)
	{
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

		//setUpAndConfigureObjects(vertices, sizeof(vertices), indicesTextures, VBORoi, VAO1, EBO1, (unsigned char *)NULL);
		setImageData(NULL);
		setUpAndConfigureObjects(vertices, sizeof(vertices), indicesTextures, VBORoi, VAO1, EBO1, dataImg);
		UpdateScren(sizeof(vertices), window, shaderProgram);
	}
	else
	{
		position coinGaucheBas  = { laCase.getCentre().x - L, laCase.getCentre().y - l, 0.0 };
		position coinGaucheHaut = { laCase.getCentre().x - L, laCase.getCentre().y + l, 0.0 };
		position coinDroiteBas  = { laCase.getCentre().x + L, laCase.getCentre().y - l, 0.0 };
		position coinDroiteHaut = { laCase.getCentre().x + L, laCase.getCentre().y + l, 0.0 };

		/*float vertices[] = {	coinGaucheHaut.x, coinGaucheHaut.y, coinGaucheHaut.z, 1.0, 0.0, 0.0, 0.0, 1.0,
								coinGaucheBas.x,  coinGaucheBas.y,  coinGaucheBas.z,  0.0, 1.0, 0.0, 0.0, 0.0,
								coinDroiteBas.x,  coinDroiteBas.y,  coinDroiteBas.z,  0.0, 0.0, 1.0, 1.0, 0.0,
								coinDroiteHaut.x, coinDroiteHaut.y, coinDroiteHaut.z, 1.0, 1.0, 1.0, 1.0, 1.0
							};

		float vertices[] = {	coinGaucheHaut.x, coinGaucheHaut.y, coinGaucheHaut.z, 1.0, 0.0, 0.0, 0.4f, 0.6f,
								coinGaucheBas.x,  coinGaucheBas.y,  coinGaucheBas.z,  0.0, 1.0, 0.0, 0.4f, 0.4f,
								coinDroiteBas.x,  coinDroiteBas.y,  coinDroiteBas.z,  0.0, 0.0, 1.0, 0.6f, 0.4f,
								coinDroiteHaut.x, coinDroiteHaut.y, coinDroiteHaut.z, 1.0, 1.0, 1.0, 0.6f, 0.6f
							};

		float vertices[] = {	-0.8f, 0.8f, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
								-0.8f, -0.8f, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
								0.8f, -0.8f, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,
								0.8f, 0.8f, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
							};*/
		
		setUpAndConfigureObjects( vertices, sizeof(vertices), indicesTextures, VBORoi, VAO1, EBO1, dataImg );
		UpdateScren( sizeof(vertices), window, shaderProgram );
	}
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
									setPetitRoque(true); // GrandRoque = false;
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
								setGrandRoque(true); // PetitRoque = false
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
									setPetitRoque(true); // GrandRoque = ...
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
								setGrandRoque(true); // PetitRoque = ...
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
