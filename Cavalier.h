#ifndef CAVALIER_H
#define CAVALIER_H

#include "util.h"
#include <GLFW/glfw3.h>
#include "Piece.h"
#include "Case.h"

#include <cmath>

class Cavalier : public Piece
{
public:
	Cavalier(unsigned short laCouleur = BLANC, unsigned short leType = CAVALIER, unsigned int laTexture = 0);
	~Cavalier();

	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);

	string getNom();
	void setNom(string nom);
private:
	string nom;
};

Cavalier::Cavalier( unsigned short laCouleur, unsigned short leType, unsigned int laTexture )
:Piece(laCouleur, leType, laTexture)
{
	/*setCouleur(laCouleur);
	setType(leType);*/
	setNom("CAVALIER");
}

Cavalier::~Cavalier()
{
}

void Cavalier::setNom(string nomDeLaPiece)
{
	nom = nomDeLaPiece;
}

string Cavalier::getNom()
{
	return nom;
}

// TRIANGLE VERS LA DROITE
void Cavalier::dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col)
{
	float base = DX / 2,
	   hauteur = DX / 2;

	Case laCase(echequier[rang - 1][col - 'A']);

	//unsigned int VAO, VBO, EBO;
    const unsigned int indices[] =
	{  
        0, 1, 2, // first triangle
        0, 3, 2  // second triangle
    };

	position coinDroit = { laCase.getCentre().x + (hauteur / 2), laCase.getCentre().y, 0.0 };
	position coinBas = { laCase.getCentre().x - (hauteur / 2), laCase.getCentre().y - (base / 2), 0.0 };
	position coinHaut = { laCase.getCentre().x - (hauteur / 2), laCase.getCentre().y + (base / 2), 0.0 };

	float vertices[] = {
		coinDroit.x, coinDroit.y, 0.0,
		coinHaut.x, coinHaut.y, 0.0,
		coinBas.x, coinBas.y, 0.0
	};

	setUpAndConfigureObjects(vertices, sizeof(vertices), indices, VBOCavalier, VAO1, EBO1, false);
	//UpdateScren(sizeof(vertices), window, shaderProgram, false);
	UpdateScren(sizeof(vertices), window, shaderProgram);
}

bool Cavalier::validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool caseValide = false;

	if (sousValiderDeplacement(rang1, col1, rang2, col2))
	{
		// VÉRIFIER SI LE DÉPLACEMENT EST PERMIS POUR LE "CAVALIER"
		if ((abs(rang1 - rang2) == 2) && (abs(col1 - col2) == 1))
		{
			caseValide = true;
		}
		else if ((abs(rang1 - rang2) == 1) && (abs(col1 - col2) == 2))
		{
			caseValide = true;
		}
	}

	return caseValide;
}

#endif // !CAVALIER_H
