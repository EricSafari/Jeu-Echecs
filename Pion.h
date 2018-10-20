#ifndef PION_H
#define PION_H

#include "util.h"

#include <GLFW/glfw3.h>

#include "Piece.h"
#include "Case.h"

using namespace std;

class Pion: public Piece
{
public:
	Pion( unsigned short laCouleur = BLANC, unsigned short leType = PION, bool saut2Cases = false, unsigned int laTexture = 0 );
	~Pion();

	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	//virtual bool validerCase(unsigned char rang, char col);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);

	string getNom();
	void setNom(string nom);
	bool getEtatSaut2Cases() { return saut2Cases; }
	void setEtatSaut2Cases(bool saut) { saut2Cases = saut; }
private:
	//
	string nom;
	bool saut2Cases;
};

Pion::Pion( unsigned short laCouleur, unsigned short leType, bool saut, unsigned int laTexture )
:Piece(laCouleur, leType, laTexture)
{
	// LA TEXTURE ET L'IMAGE SONT DÉFINIES DANS LA
	// FONCTION "main"
	setEtatSaut2Cases(saut);
	setNom("PION");
}

Pion::~Pion()
{
}

string Pion::getNom()
{
	return nom;
}

void Pion::setNom(string nomDeLaPiece)
{
	nom = nomDeLaPiece;
}

// CARRÉ
void Pion::dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float cote = DX / 2;
	Case laCase( echequier[rang - 1][col - 'A'] );

	//unsigned int VAO, VBO, EBO;
	unsigned char * dataImg = laCase.getPiecePtr()->getImageData();

//if (dataImg)
//{
position coinGaucheBas = { laCase.getCentre().x - (cote / 2), laCase.getCentre().y - (cote / 2), 0.0 };
position coinGaucheHaut = { laCase.getCentre().x - (cote / 2), laCase.getCentre().y + (cote / 2), 0.0 };
position coinDroiteBas = { laCase.getCentre().x + (cote / 2), laCase.getCentre().y - (cote / 2), 0.0 };
position coinDroiteHaut = { laCase.getCentre().x + (cote / 2), laCase.getCentre().y + (cote / 2), 0.0 };

float vertices[] = {
					coinGaucheHaut.x, coinGaucheHaut.y, 0.0,
					coinGaucheBas.x, coinGaucheBas.y, 0.0,
					coinDroiteBas.x, coinDroiteBas.y, 0.0,

					coinGaucheHaut.x, coinGaucheHaut.y, 0.0,
					coinDroiteHaut.x, coinDroiteHaut.y, 0.0,
					coinDroiteBas.x, coinDroiteBas.y, 0.0
};

/*float vertices[] = {	coinGaucheHaut.x, coinGaucheHaut.y, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
						coinGaucheBas.x,  coinGaucheBas.y,  0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
						coinDroiteBas.x,  coinDroiteBas.y,  0.0, 0.0, 0.0, 1.0, 1.0, 0.0,
						coinDroiteHaut.x, coinDroiteHaut.y, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
};*/

setUpAndConfigureObjects(vertices, sizeof(vertices), indicesTextures, VBOPion, VAO1, EBO1, dataImg);
//UpdateScren( sizeof(vertices), window, shaderProgram, dataImg);
UpdateScren(sizeof(vertices), window, shaderProgram);
//}
}

// AVANCE DEVANT ET MANGE EN DIAGONALE-AVANT + LA PRISE EN PASSANT
// "rang1" et "col1" représentent la Case de la pièce(DANS CE CAS-CI... LE PION)
bool Pion::validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide = false,
		rangeeValide = false;
	Case caseInitiale(echequier[rang1 - 1][col1 - 'A']),
		caseFinale(echequier[rang2 - 1][col2 - 'A']);

	if (sousValiderDeplacement(rang1, col1, rang2, col2))
	{
		// PREMIER DÉPLACEMENT DU PION
		if (caseInitiale.getPiecePtr()->getCouleur() == BLANC)
		{
			if ((rang1 == 2) && (rang2 == 4))
			{
				rangeeValide = true;
			}
			else if ((rang2 == rang1 + 1)) // ???????????????????????????????????? "rang1 + 1"
			{
				rangeeValide = true;
			}
			else
			{
				return false;
			}
		}
		else // PIONS NOIRS
		{
			if ((rang1 == 7) && (rang2 == 5))
			{
				rangeeValide = true;
			}
			else if ((rang2 == rang1 - 1)) // "rang1 - 1"
			{
				rangeeValide = true;
			}
			else
			{
				return false;
			}
		}

		// LA RANGÉE EST VALIDE, IL FAUT
		// MAINTENANT VÉRIFIER LA COLONNE
		if (rangeeValide) // SAUT DE 1 CASE OU DE 2 CASES
		{
			// Pions Blancs...
			if( (!caseFinale.getEtat()) || (caseFinale.getPiecePtr()->getCouleur() == NOIR) )
			{
				if (rang2 == rang1 + 1)
				{
					if ((col2 == col1) && (!caseFinale.getEtat())) // CASE EN AVANT LIBRE
					{
						caseValide = true;
					}
					else if ((col2 == col1 + 1) || (col2 == col1 - 1)) // CASES EN DIAGONALE
					{
						if (caseFinale.getEtat()) // CASE OCCUPÉE
						{
							if (caseFinale.getPiecePtr()->getCouleur() == NOIR)
							{
								caseValide = true;
							}
						}
						else
						{
							if (rang2 == 6)
							{
								if( (echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr()->getType() == PION) &&
									(echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr()->getCouleur() == NOIR) )
								{
									if( !caseFinale.getEtat() )
									{
										Pion * pionPtrTemp = (Pion *)echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr();

										if( pionPtrTemp->getEtatSaut2Cases() )
										{
											caseValide = true;
											return true;
										}
									}
								}
								else if( (echequier[rang1 - 1][(col1 - 1) - 'A'].getPiecePtr()->getType() == PION) &&
									(echequier[rang1 - 1][(col1 - 1) - 'A'].getPiecePtr()->getCouleur() == NOIR) )
								{
									if( !caseFinale.getEtat() )
									{
										Pion * pionPtrTemp = (Pion *)echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr();

										if( pionPtrTemp->getEtatSaut2Cases() )
										{
											caseValide = true;
											return true;
										}
									}
								}
							}
						}
					}
				}
				else if ((col2 == col1)) //  SAUT DE 2 CASES EN AVANT
				{
					if( !caseFinale.getEtat() ) // CASE EN AVANT LIBRE
					{
						caseValide = true;
						Case CasePriseEnPassant1(echequier[rang1 - 1][(col1 + 1) - 'A']),
							 CasePriseEnPassant2(echequier[rang1 - 1][(col1 - 1) - 'A']);

						if( (CasePriseEnPassant1.getEtat()) && // Case Occupée
							(CasePriseEnPassant1.getPiecePtr()->getType() == PION) &&
							(CasePriseEnPassant1.getPiecePtr()->getCouleur() == NOIR))
						{
							setEtatSaut2Cases(true);
						}

						if( (CasePriseEnPassant2.getEtat()) && // Case Occupée
							(CasePriseEnPassant2.getPiecePtr()->getType() == PION) &&
							(CasePriseEnPassant2.getPiecePtr()->getCouleur() == NOIR))
						{
							setEtatSaut2Cases(true);
						}
					}
				}
			}
			else // Pions Noirs...
			{
				if (rang2 == rang1 - 1)
				{
					if ((col2 == col1) && (!caseFinale.getEtat())) // CASE EN AVANT LIBRE
					{
						caseValide = true;
					}
					else if ((col2 == col1 + 1) || (col2 == col1 - 1)) // CASES EN DIAGONALE
					{
						if (caseFinale.getEtat()) // CASE OCCUPÉE
						{
							if (caseFinale.getPiecePtr()->getCouleur() == BLANC)
							{
								caseValide = true;
							}
						}
						else
						{
							if (rang2 == 3)
							{
								if( (echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr()->getType() == PION) &&
									(echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr()->getCouleur() == BLANC) )
								{
									if( !caseFinale.getEtat() )
									{
										Pion * pionPtrTemp = (Pion *)echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr();

										if( pionPtrTemp->getEtatSaut2Cases() )
										{
											caseValide = true;
											return true;
										}
									}
								}
								else if( (echequier[rang1 - 1][(col1 - 1) - 'A'].getPiecePtr()->getType() == PION) &&
									(echequier[rang1 - 1][(col1 - 1) - 'A'].getPiecePtr()->getCouleur() == BLANC) )
								{
									if( !caseFinale.getEtat() )
									{
										Pion * pionPtrTemp = (Pion *)echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr();

										if( pionPtrTemp->getEtatSaut2Cases() )
										{
											caseValide = true;
											return true;
										}
									}
								}
							}
						}
					}
				}
				else if ((col2 == col1)) //  SAUT DE 2 CASES EN AVANT
				{
					if (!caseFinale.getEtat()) // CASE EN AVANT LIBRE
					{
						caseValide = true;

						if ((echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr()->getType() == PION) &&
							(echequier[rang1 - 1][(col1 + 1) - 'A'].getPiecePtr()->getCouleur() == BLANC))
						{
							setEtatSaut2Cases(true);
						}
						else if ((echequier[rang1 - 1][(col1 - 1) - 'A'].getPiecePtr()->getType() == PION) &&
							(echequier[rang1 - 1][(col1 - 1) - 'A'].getPiecePtr()->getCouleur() == BLANC))
						{
							setEtatSaut2Cases(true);
						}
					}
				}
			}
		}
	}

	return caseValide;
}

#endif // !PION_H
