#ifndef TOUR_H
#define TOUR_H

#include <Windows.h>
#include <gl/GL.h>

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"
#include <cmath>

//#include <gl/GL.h>
//#include <openglut.h>

class Tour : public Piece
{
public:
	Tour(unsigned short laCouleur = BLANC, unsigned short leType = TOUR, unsigned int laTexture = 0);
	~Tour();

	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);

	bool getPremierDeplacement();
	void setPremierDeplacement(bool premierDepl);
	string getNom();
	void setNom(string nom);

private:
	bool premierDeplacement;
	string nom;
	// DÉPEND DE LA PIÈCE
	bool validerLaRangee(unsigned char rang1, char col1, unsigned char rang2, char col2);
	bool validerLaColonne(unsigned char rang1, char col1, unsigned char rang2, char col2);
};

Tour::Tour(unsigned short laCouleur, unsigned short leType, unsigned int laTexture)
:Piece(laCouleur, leType, laTexture)
{
	/*setCouleur(laCouleur);
	setType(leType);
	setPremierDeplacement(false);*/
	setNom("TOUR");
}

Tour::~Tour()
{
}

bool Tour::getPremierDeplacement()
{
	return premierDeplacement;
}

void Tour::setPremierDeplacement(bool premierDepl)
{
	premierDeplacement = premierDepl;
}

void Tour::setNom(string nomDeLaPiece)
{
	nom = nomDeLaPiece;
}

string Tour::getNom()
{
	return nom;
}

// LOSANGE
void Tour::dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float base = DX / 2,
	   hauteur = DX / 2;
	
	Case laCase( echequier[rang - 1][col - 'A'] );

	//unsigned int VAO, VBO, EBO;
    const unsigned int indices[] =
	{  
        0, 1, 2, // first triangle
        0, 3, 2  // second triangle
    };

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
						coinDroit.x, coinDroit.y, 0.0
					   };

	setUpAndConfigureObjects(vertices, sizeof(vertices), indices, VBOTour, VAO1, EBO1, getImageData());
	UpdateScren(sizeof(vertices), window, shaderProgram);
	/*
	//glColor4f(0.0f, 0.0f, 0.0f, 0.0f);	//RGBA values of text color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//RGBA values of text color
	glClear(GL_COLOR_BUFFER_BIT);
	
	glRasterPos2f(100, 200);				//Top left corner of text
	const unsigned char* t = reinterpret_cast<const unsigned char *>("TOUR");
	// Since 2nd argument of glutBitmapString must be const unsigned char*
	glutBitmapString(GLUT_BITMAP_HELVETICA_12,t);*/
}

bool Tour::validerLaRangee(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	if (sousValiderDeplacement(rang1, col1, rang2, col2))
	{
		if ((rang2 == rang1) && (col2 != col1))
		{
			if (col2 > col1)
			{
				for (char colonne = col1 + 1; colonne < col2; colonne++)
				{
					// INOCCUPÉE
					if (!echequier[rang2 - 1][colonne - 'A'].getEtat())
					{
						//return true;
					}
					else // OCCUPÉE
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

	// POUR LE CAS OÙ LE PREMIER "IF"
	// NE RETOURNE PAS DE VALEUR
	return false;
}

bool Tour::validerLaColonne(unsigned char rang1, char col1, unsigned char rang2, char col2)
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

	// POUR LE CAS OÙ LE PREMIER "IF"
	// NE RETOURNE PAS DE VALEUR
	return colonneValide;
}

bool Tour::validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide = false;

	// SI LA "TOUR" PEUT SE DÉPLACER SUR LA "LIGNE" OU LA "COLONNE" :
	// DÉPLACEMENT PERMIS(OK!!!!!!!!!!!!!!!!!!!!!!!)
	caseValide = validerLaRangee(rang1, col1, rang2, col2) || validerLaColonne(rang1, col1, rang2, col2);

	return caseValide;
}

#endif // !TOUR_H
