#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "util.h"
#include "Case.h"

#define BUFFER_OFFSET(i) ((void*)(i))

using namespace std;

Case echequier[NB_LIGNE][NB_COL];

const unsigned int NB_VERTICES = 3 * (NB_COL + 1) * (NB_LIGNE + 1),
					NB_INDICES = 2 * 3 * NB_LIGNE * NB_COL;

float vertices[NB_VERTICES]; // NB_VERTICES
unsigned int indices[NB_INDICES];

const unsigned int indicesTextures[6] =
{  
    0, 1, 2, // 1er Triangle
    0, 3, 2  // 2e  Triangle
};

class Piece
{
public:
	Piece(unsigned short couleur, unsigned short type, unsigned int texture);
	~Piece();
	void setUpAndConfigureObjects(const float verticesPieces[], unsigned short verticesPiecesSize, const unsigned int indicesPieces[], unsigned int& VertexBufferObject, unsigned int& VertexArrayObject, unsigned int& ElementBufferObject, unsigned char * data);
	
	void UpdateScren(unsigned short verticesSize, GLFWwindow *window, int shaderProg);
	virtual void dessinerPiece(GLFWwindow* window, int shaderProgram, unsigned char rang, char col) = 0;
	
	bool sousValiderDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2);
	virtual bool validerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2) = 0;

	unsigned short getCouleur();
	void setCouleur(unsigned short laCouleur);

	unsigned short getType();
	void setType(unsigned short leType);

	/*string getNom();
	void setNom(string nomDeLaPiece);*/

	unsigned int getTexture();
	void setTexture(unsigned int uneTexture);

	unsigned char * initTextures(unsigned int& laTexture, char * NomFichier);

	void setImageData(unsigned char * data);
	unsigned char * getImageData();

private:
	unsigned short couleur;
	unsigned short type;
	unsigned int texture;
	//string nom;
	unsigned char * imageData;
};

Piece::Piece(unsigned short couleur, unsigned short type, unsigned int texture)
{
	setCouleur(couleur);
	setType(type);
	setTexture(texture);
	setImageData(NULL);
}

Piece::~Piece()
{}

unsigned short Piece::getCouleur()
{
	return couleur;
}

void Piece::setCouleur(unsigned short laCouleur)
{
	couleur = laCouleur;
}

unsigned short Piece::getType()
{
	return type;
}

void Piece::setType(unsigned short leType)
{
	type = leType;
}

/*void Piece::setNom(string nomDeLaPiece)
{
	nom = nomDeLaPiece;
}

string Piece::getNom()
{
	return nom;
}*/

unsigned int Piece::getTexture()
{
	return texture;
}

void Piece::setTexture(unsigned int uneTexture)
{
	texture = uneTexture;
}

void Piece::setImageData(unsigned char * data)
{
	imageData = data;
}

unsigned char * Piece::getImageData()
{
	return imageData;
}

unsigned char * Piece::initTextures(unsigned int& laTexture, char * NomFichier)
{
	// load and create a texture 
	// -------------------------
	//unsigned int texture;
	glGenTextures(1, &laTexture);
	glBindTexture(GL_TEXTURE_2D, laTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load(NomFichier, &width, &height, &nrChannels, 0);
	//unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		const char * Erreur = stbi_failure_reason();
		std::cout << "Failed to load texture..." << std::endl;

		return NULL;
	}

	stbi_image_free(data);

	// IL FAUT LIBÉRER LA TEXTURE PCQ TOUS LES OPÉRATIONS
	// SUR TEXTURES AFFECTENT LA TEXTURE EN COURS(Binded)...
	// IL FAUT LA LIBÉRER POUR POUVOIR CONFIGURER LES AUTRES
	// LES AUTRES TEXTURES, LES AUTRES PIÈCES...
	glBindTexture(GL_TEXTURE_2D, 0);

	return data;
}

void Piece::UpdateScren(unsigned short verticesSize, GLFWwindow *window, int shaderProg)
{
	// input
	// -----
	processInput(window);
	
	// draw our first triangle
	//glUseProgram(shaderProg);

	unsigned char * data = getImageData();

	if (data)
	{
		/*glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(12 * (0 * NB_COL + 0) * 2));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(12 * ((0 * NB_COL + 0) * 2 + 1)));*/
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indicesTextures);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(12));
	}
	else
	{
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
			cout << "ERREUR: NOMBRE DE VERTICES INVALIDE(" << verticesSize << ")  ??????????????????????????" << endl << endl;
		}
	}
	
	// REINITIALISER LE BUFFER ET LE ARRAY ...
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/*
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();*/
}

// VertexBufferObject et VertexArrayObject ne sont pas nécessaires
void Piece::setUpAndConfigureObjects( const float verticesPieces[], unsigned short verticesPiecesSize, const unsigned int indicesPieces[], unsigned int& VertexBufferObject, unsigned int& VertexArrayObject, unsigned int& ElementBufferObject, unsigned char * data )
{
	if (texture) // data = 0
	{
		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	//glUseProgram(shaderProg);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, verticesPiecesSize, verticesPieces, GL_STATIC_DRAW);

	if (data)
	{
		//glBindBuffer(GL_ARRAY_BUFFER, ElementBufferObject);
		//glBufferData(GL_ARRAY_BUFFER, 6, indicesPieces, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicesPieces, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicesPieces, GL_STATIC_DRAW);

		// VERTEX
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// COULEURS
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// TEXTURES(Coordonnees)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	else
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);
	
	//// bind Texture
	//glBindTexture(GL_TEXTURE_2D, 0);
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
				}
			}
		}
	}

	return PositionValide;
}

#endif // !PIECE_H

