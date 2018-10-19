#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <glad/glad.h>

#include "util.h"
#include "Case.h"

using namespace std;

Case echequier[NB_LIGNE][NB_COL];

class Piece
{
public:
	//void setUpAndConfigureObjects(const float verticesPieces[], unsigned int& VertexBufferObject, unsigned int& VertexArrayObject);
	void setUpAndConfigureObjects(const float verticesPieces[], unsigned short verticesPiecesSize, unsigned int& VertexBufferObject, unsigned int& VertexArrayObject);

	//void UpdateScren(float verticesPieces[], GLFWwindow *window, int shaderProg)
	void UpdateScren(unsigned short verticesSize, GLFWwindow *window, int shaderProg);

	//virtual void dessinerPiece(unsigned char rang, char col) = 0;
	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col) = 0;

	bool sousValiderDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);
	//virtual bool validerCase(unsigned char rang, char col) = 0;
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2) = 0;

	unsigned short getCouleur()
	{
		return couleur;
	}

	void setCouleur(unsigned short laCouleur)
	{
		couleur = laCouleur;
	}

	unsigned short getType()
	{
		return type;
	}

	void setType(unsigned short leType)
	{
		type = leType;
	}

	void setNom(string nomDeLaPiece)
	{
		nom = nomDeLaPiece;
	}

	string getNom()
	{
		return nom;
	}

private:
	unsigned short couleur;
	unsigned short type;
	string nom;
};

void Piece::UpdateScren(unsigned short verticesSize, GLFWwindow *window, int shaderProg)
{
	// input
	// -----
	processInput(window);

	// draw our first triangle
	glUseProgram(shaderProg);

	// PAS BESOIN DE "BINDER" LE VAO
	if (verticesSize == 9 * sizeof(float))
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else if (verticesSize == 18 * sizeof(float))
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else if (verticesSize == 36 * sizeof(float)) // LE ROI
	{
		glDrawArrays(GL_TRIANGLES, 0, 12);
	}
	else
	{
		cout << "ERREUR: NOMBRE DE VERTICES INVALIDE ??????????????????????????" << endl << endl;
	}
	
	// INITIALISER LE BUFFER ET LE ARRAY ...
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/*
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();*/
}

// VertexBufferObject et VertexArrayObject ne sont pas nécessaires
void Piece::setUpAndConfigureObjects( const float verticesPieces[], unsigned short verticesPiecesSize, unsigned int& VertexBufferObject, unsigned int& VertexArrayObject )
{
	/*glGenVertexArrays(1, &VertexArrayObject);
	glGenBuffers(1, &VertexBufferObject);*/

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, verticesPiecesSize, verticesPieces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);
}

// Compare les positions initiales et finales en tenant
// compte du joueur qui joue...
bool Piece::sousValiderDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	bool PositionValide = false;

	if ((rang1 >= 1) && (rang1 <= 8) && (col1 >= 'A') && (col1 <= 'H'))
	{
		if ((rang2 >= 1) && (rang2 <= 8) && (col2 >= 'A') && (col2 <= 'H'))
		{
			if ((rang1 != rang2) || (col1 != col2))
			{
				Case caseInitiale(echequier[rang1 - 1][col1 - 'A']),
					caseFinale(echequier[rang2 - 1][col2 - 'A']);

				if (!caseFinale.getEtat())
				{
					PositionValide = true;
				}
				else
				{
					if ((caseInitiale.getPiecePtr()->couleur == NOIR) && (caseFinale.getPiecePtr()->couleur == BLANC))
					{
					PositionValide = true;
					}
					else if ((caseInitiale.getPiecePtr()->couleur == BLANC) && (caseFinale.getPiecePtr()->couleur == NOIR))
					{
					PositionValide = true;
					}
					/*//if ((caseInitiale.getPiecePtr()->getCouleur() != caseFinale.getPiecePtr()->getCouleur()) && (caseInitiale.getPiecePtr()->getCouleur() == joueur))
					//if ((caseInitiale.getPiecePtr()->getCouleur() != caseFinale.getPiecePtr()->getCouleur()) && (caseInitiale.getPiecePtr()->getCouleur() == couleurDuRoiAdverse))
					if ((caseInitiale.getPiecePtr()->getCouleur() != caseFinale.getPiecePtr()->getCouleur()) && (caseInitiale.getPiecePtr()->getCouleur() == couleurDuRoi))
					{
						PositionValide = true;
					}*/
				}
			}
		}
	}

	return PositionValide;
}

#endif // !PIECE_H

