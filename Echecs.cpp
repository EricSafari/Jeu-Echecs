
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "Case.h"
/////////////////
#include "Piece.h"
#include "Pion.h"
#include "Tour.h"
#include "Cavalier.h"
#include "Fou.h"
//////////////////
#include "Reine.h"
#include "Roi.h"

#include "init.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//Define this somewhere in your header file
//#define BUFFER_OFFSET(i) ((void*)(i))

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
/*
const unsigned int NB_VERTICES = 3 * (NB_COL + 1) * (NB_LIGNE + 1),
					NB_INDICES = 2 * 3 * NB_LIGNE * NB_COL;
*/
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    ourColor = aColor;\n"
"    TexCoord = aTexCoord;\n"
"}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform int counter;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"   if( counter == 0 )\n"						   // TEXTURE...
"     FragColor = texture(ourTexture, TexCoord);\n" // IMAGE
"   else if( counter == 1 )\n"
"     FragColor = texture(ourTexture, TexCoord);\n" // IMAGE
"   else if( counter == 2 )\n"
"     FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n" // VERT
"   else if( counter == 3 )\n"
"     FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n" // BLEU
"}\n\0";

//struct Joueur
//{
//	unsigned short couleur;
//	unsigned short nbPiecesGagnees;
//	unsigned short nbPiecesPerdues;
//};

unsigned int texture1, texture2, texture3,
			 texture4, texture5, texture6;

int BuildAndCompileShaderProgram(const char * vertexShaderSrc, const char * fragmentShaderSrc);

void setUpAndConfigureVertex(unsigned int& VertexBufferObject, unsigned int& VertexArrayObject, unsigned int& ElementBufferObject);

void DessinerEchequier(int shaderProgram, GLFWwindow* window, unsigned int VertexArrayObject);

void DessinerLesPieces( GLFWwindow* window, int shaderProgram )
{
	GLint counterLocationPieces = glGetUniformLocation(shaderProgram, "counter");

	// DESSINER LES PIECES SUR LES CASES OCCUP�ES
	for (unsigned short k = 0; k < 8; k++)
	{
		for (unsigned short l = 0; l < 8; l++)
		{
			Case laCase( echequier[k][l] );

			// CASE OCCUP�E
			if( laCase.getEtat() )
			{
				if (echequier[k][l].getPiecePtr()->getCouleur() == BLANC)
				{
					glUniform1i(counterLocationPieces, 1);
				}
				else
				{
					glUniform1i(counterLocationPieces, 0);
				}

				laCase.getPiecePtr()->dessinerPiece( window, shaderProgram, k + 1, l + 'A' );
				//if( (k == 0) && (l == 0 || l == 7) ) laCase.getPiecePtr()->dessinerPiece( window, shaderProgram, k + 1, l + 'A' );
				//if( k == 0 ) laCase.getPiecePtr()->dessinerPiece( window, shaderProgram, k + 1, l + 'A' );
			}
		}
	}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();
}

enum etat_jeu_t { INIT, ATT_JOUEUR_BLANC, CHOISIR_CASE_DEST, REDESSINER, ALTERNER_JOUEUR, ATT_JOUEUR_NOIR };

int main(int argc, int * argv[])
{
	// INITIALISATION
	etat_jeu_t etatDuJeu = INIT;
	bool fin = false;

	// OP�RATIONS DE GLFW.........
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // comment this line in a release build! 

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback( window, mouse_button_callback );

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	int shaderProgram = BuildAndCompileShaderProgram(vertexShaderSource, fragmentShaderSource);

	//// set up vertex data (and buffer(s)) and configure vertex attributes
	//// ------------------------------------------------------------------
	unsigned int VBO, VAO, EBO;
	setUpAndConfigureVertex( VBO, VAO, EBO );
	
	//...
	InitialiserLesCases();

	/*****************************/
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &EBO1);
	glGenBuffers(1, &VBOPion);
	glGenBuffers(1, &VBOTour);
	glGenBuffers(1, &VBOCavalier);
	glGenBuffers(1, &VBOFou);
	glGenBuffers(1, &VBOReine);
	glGenBuffers(1, &VBORoi);
	/*
	// TOUR BLANCHE
	unsigned int textureID = 0xFFFFFFFF;
	unsigned char * data1 = echequier[1][0].getPiecePtr()->initTextures(textureID, "Pion blanc.PNG");
	echequier[0][0].getPiecePtr()->setTexture(textureID);
	echequier[0][0].getPiecePtr()->setImageData(data1);
	
	// PION BLANC
	unsigned int textureID = 0xFFFFFFFF;
	unsigned char * data1 = echequier[1][0].getPiecePtr()->initTextures(textureID, "Pion blanc.PNG");
	echequier[1][0].getPiecePtr()->setTexture(textureID);
	echequier[1][0].getPiecePtr()->setImageData(data1);

	// REINE BLANCHE
	//textureID = 0xFFFFFFFF;
	unsigned char * data5 = echequier[0][3].getPiecePtr()->initTextures(textureID, "Reine blanche.PNG");
	echequier[0][3].getPiecePtr()->setTexture(textureID);
	echequier[0][3].getPiecePtr()->setImageData(data5);
	*/
	// ROI BLANC
	unsigned int textureID = 0xFFFFFFFF;
	unsigned char * data6 = echequier[0][4].getPiecePtr()->initTextures(textureID, "Roi blanc.PNG");
	echequier[0][4].getPiecePtr()->setTexture(textureID);
	echequier[0][4].getPiecePtr()->setImageData(data6);
	
	while ( !glfwWindowShouldClose(window) )
	{
		processInput(window);

		//////  � LA SORTIE DU SWITCH...  /////////
		while( (!fin) && !glfwWindowShouldClose(window) )
		{
			// PERMET DE D'INT�RAGIR AVEC LA FEN�TRE...
			// FERMER LA FEN�TRE, CHANGER SA TAILLE,...
			processInput(window);

			switch (etatDuJeu)
			{
			case INIT:	// DESSINER L'�CHEQUIER ET LES PI�CES
				// render
				// ------

				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				DessinerEchequier(shaderProgram, window, VAO);
				DessinerLesPieces(window, shaderProgram );

				etatDuJeu = ATT_JOUEUR_BLANC;
				break;

			case ATT_JOUEUR_BLANC:
				// Le joueur a cliqu� sur la souris
				// Et Nous sommes sur l'�chequier

				if (positionValide && echequier[rangeeDepart - 1][colonneDepart - 'A'].getEtat())
				{
					if (echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr()->getCouleur() == BLANC)
					{
						etatDuJeu = CHOISIR_CASE_DEST;
						//positionValide = false;
					}
					else
					{
						positionValide = false;
					}
				}
				else
				{
					// render
					// ------

					glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);
					DessinerEchequier(shaderProgram, window, VAO);
					DessinerLesPieces(window, shaderProgram);

					if (!echequier[rangeeDepart - 1][colonneDepart - 'A'].getEtat())
					{
						positionValide = false;
					}
				}

				break;

			case CHOISIR_CASE_DEST:	//
				// LORSQUE LE JOUEUR CHOISIT UNE "CASE FINALE"
				// LA "FONCTION CALLBACK" VALIDE LA CASE

				if (caseValide)
				{
					// 1 - ...
					if (joueur == JOUEUR_BLANC)
					{
						couleurDuRoiAdverse = NOIR;
					}
					else
					{
						couleurDuRoiAdverse = BLANC;
					}

					// 2 - ...

					if (!echequier[rangeeArrivee - 1][colonneArrivee - 'A'].getEtat())
					{
						if (echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr()->validerDeplacement(rangeeDepart, colonneDepart, rangeeArrivee, colonneArrivee))
						{
							//caseValide = true;
						}
						else
						{
							caseValide = false;
							break;
						}
					}
					else
					{
						if (echequier[rangeeArrivee - 1][colonneArrivee - 'A'].getPiecePtr()->getCouleur() == couleurDuRoiAdverse)
						{
							if (echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr()->validerDeplacement(rangeeDepart, colonneDepart, rangeeArrivee, colonneArrivee))
							{
								//caseValide = true;
							}
							else
							{
								caseValide = false;
								break;
							}
						}
						else
						{
							rangeeDepart = rangeeArrivee;
							colonneDepart = colonneArrivee;

							// SORTIR DU SWITCH... ET ATTENDRE QUE LE 
							// JOUEUR CHOISISSE UNE NOUVELLE CASE FINALE
							caseValide = false;
							break;
						}
					}

					// METTRE � JOUR L'�CHEQUIER...
					// ------------------------------ //
					if (echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr()->getType() != ROI)
					{
						deplacerPiece(rangeeDepart, colonneDepart, rangeeArrivee, colonneArrivee);
					}
					else
					{
						// D�PLACER LE ROI
						deplacerPiece(rangeeDepart, colonneDepart, rangeeArrivee, colonneArrivee);

						Roi * RoiPtr = (Roi *)echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr();

						// D�placer la Tour Associ�e au Roi
						if (RoiPtr->getCouleur() == BLANC)
						{
							if (RoiPtr->getPetitRoque())
							{
								deplacerPiece(1, 'H', 1, 'F'); // D�PLACER LA TOUR 1H
							}
							else if (RoiPtr->getGrandRoque())
							{
								deplacerPiece(1, 'A', 1, 'D'); // D�PLACER LA TOUR 1A
							}
						}
						else
						{
							if (RoiPtr->getPetitRoque())
							{
								deplacerPiece(8, 'H', 8, 'F'); // D�PLACER LA TOUR 8H
							}
							else if (RoiPtr->getGrandRoque())
							{
								deplacerPiece(8, 'A', 8, 'D'); // D�PLACER LA TOUR 8A
							}
						}
					}
					// ------------------------------ //
					verifierSiEchec();

					if (echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr()->getType() != ROI)
					{
						if (Echec)
						{
							annulerDeplacement(rangeeDepart, colonneDepart, rangeeArrivee, colonneArrivee);

							//Roi * RoiPtr = (Roi *)echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr();

							//// D�placer la Tour Associ�e au Roi
							//if (RoiPtr->getCouleur() == BLANC)
							//{
							//	if (RoiPtr->getPetitRoque())
							//	{
							//		annulerDeplacement(1, 'H', 1, 'F'); // D�PLACER LA TOUR 1H
							//		RoiPtr->setPetitRoque(false);
							//	}
							//	else if (RoiPtr->getGrandRoque())
							//	{
							//		annulerDeplacement(1, 'A', 1, 'D'); // D�PLACER LA TOUR 1A
							//		RoiPtr->setGrandRoque(false);
							//	}
							//}
							//else
							//{
							//	if (RoiPtr->getPetitRoque())
							//	{
							//		annulerDeplacement(8, 'H', 8, 'F'); // D�PLACER LA TOUR 1H
							//		RoiPtr->setPetitRoque(false);
							//	}
							//	else if (RoiPtr->getGrandRoque())
							//	{
							//		annulerDeplacement(8, 'A', 8, 'D'); // D�PLACER LA TOUR 1A
							//		RoiPtr->setGrandRoque(false);
							//	}
							//}

							caseValide = false;
						}
						else
						{
							//cout << "EN TRAIN DE METTRE A JOUR L'�CHEQUIER..." << endl;

							// LIB�RER LA M�MOIRE
							if (pieceArriveePtrTemp != NULL)
							{
								delete pieceArriveePtrTemp;
							}

							etatDuJeu = REDESSINER;
						}
					}
					else // LE ROI : IL FAUT V�RIFIER TOUTES LES POSITIONS INTERM�DIAIRES
					{
						// Annuler le D�placement du Roi
						annulerDeplacement(rangeeDepart, colonneDepart, rangeeArrivee, colonneArrivee);	
						Roi * RoiPtr = (Roi *)echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr();

						// Ramener la Tour � la Case de D�part(1H ou 8H)
						if (RoiPtr->getCouleur() == BLANC)
						{
							if (RoiPtr->getPetitRoque())
							{
								annulerDeplacement(1, 'H', 1, 'F'); // D�PLACER LA TOUR 1H
								//RoiPtr->setPetitRoque(false);
							}
							else if (RoiPtr->getGrandRoque())
							{
								annulerDeplacement(1, 'A', 1, 'D'); // D�PLACER LA TOUR 1A
								//RoiPtr->setGrandRoque(false);
							}
						}
						else
						{
							if (RoiPtr->getPetitRoque())
							{
								annulerDeplacement(8, 'H', 8, 'F'); // D�PLACER LA TOUR 1H
								RoiPtr->setPetitRoque(false);
							}
							else if (RoiPtr->getGrandRoque())
							{
								annulerDeplacement(8, 'A', 8, 'D'); // D�PLACER LA TOUR 1A
								RoiPtr->setGrandRoque(false);
							}
						}

						if (!Echec)
						{
							//Roi * RoiPtr = (Roi *)echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr();
							//
							unsigned char rangee;

							if (RoiPtr->getCouleur() == BLANC)
							{
								rangee = 1;
							}
							else
							{
								rangee = 8;
							}
							
							if( RoiPtr->getPetitRoque() )
							{
								deplacerPiece(rangee, 'E', rangee, 'F');
								verifierSiEchec();

								if( !Echec )
								{
									deplacerPiece(rangee, 'F', rangee, 'G'); // D�PLACER LE ROI...
									deplacerPiece(rangee, 'H', rangee, 'F'); // D�PLACER LA TOUR 1H
								}
								else
								{
									annulerDeplacement(rangee, 'E', rangee, 'F');// ROI RETOUR � LA CASE 1E
								}
							}
							else if (RoiPtr->getGrandRoque())
							{
								deplacerPiece(rangee, 'E', rangee, 'D');
								verifierSiEchec();

								if( !Echec )
								{
									deplacerPiece(rangee, 'D', rangee, 'C'); // D�PLACER LE ROI...
									deplacerPiece(rangee, 'A', rangee, 'D'); // D�PLACER LA TOUR 1A
								}
								else
								{
									annulerDeplacement(rangee, 'E', rangee, 'D');// ROI RETOUR � LA CASE 1E
								}
							}
						}
						else
						{
							//
						}
					}
				}
				else
				{
					// render
					// ------

					glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);
					DessinerEchequier(shaderProgram, window, VAO);
					DessinerLesPieces(window, shaderProgram);
				}

				break;

			case REDESSINER:	//

				/*************************************/
				// REDESSINER "MAINTENANT" L'�CHEQUIER,
				// IL A �T� MIS � JOUR...
				/*************************************/
				DessinerEchequier(shaderProgram, window, VAO);
				DessinerLesPieces(window, shaderProgram);

				//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
				//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
				// C'EST ICI QUE...
				joueur = !joueur; // C'EST N�CESSAIRE PCQ... ON VEUT
				verifierSiEchecEtMat();
				joueur = !joueur; // SAVOIR QUEL JOUEUR EST EN �CHEC
				//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
				//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

				if (!EchecEtMat && !Pat)
				{
					etatDuJeu = ALTERNER_JOUEUR;
				}
				else
				{
					fin = true;
				}

				break;

			case ALTERNER_JOUEUR:	//

				// PASSER � L'AUTRE JOUEUR
				positionValide = false;
				caseValide = false;

				if (joueur == JOUEUR_BLANC)
				{
					joueur = JOUEUR_NOIR;
					etatDuJeu = ATT_JOUEUR_NOIR;
				}
				else
				{
					joueur = JOUEUR_BLANC;
					etatDuJeu = ATT_JOUEUR_BLANC;
				}

				break;

			case ATT_JOUEUR_NOIR:
				// Le joueur a cliqu� sur la souris
				// Et Nous sommes sur l'�chequier

				if (positionValide && echequier[rangeeDepart - 1][colonneDepart - 'A'].getEtat())
				{
					if (echequier[rangeeDepart - 1][colonneDepart - 'A'].getPiecePtr()->getCouleur() == NOIR)
					{
						etatDuJeu = CHOISIR_CASE_DEST;
						//positionValide = false;
					}
					else
					{
						positionValide = false;
					}
				}
				else
				{
					// render
					// ------

					glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);
					DessinerEchequier(shaderProgram, window, VAO);
					DessinerLesPieces(window, shaderProgram);

					if (!echequier[rangeeDepart - 1][colonneDepart - 'A'].getEtat())
					{
						positionValide = false;
					}
				}

				break;

			default:
				cout << "BYE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
				cout << "L'�TAT DU JEU" << "((" << etatDuJeu << ")) " << "N'EST PAS VALIDE" << endl;

				/*************************/
				// POUR POUVOIR SORTIR DE
				// LA BOUCLE...........
				fin = true;
				/*************************/
				break;
			}

			//glfwPollEvents();
		}
		///////////////////////////////////////

		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &EBO1);
	glDeleteBuffers(1, &VBOPion);
	glDeleteBuffers(1, &VBOTour);
	glDeleteBuffers(1, &VBOCavalier);
	glDeleteBuffers(1, &VBOFou);
	glDeleteBuffers(1, &VBOReine);
	glDeleteBuffers(1, &VBORoi);

	glDeleteTextures(1, &textureID);

	LibererLaMemoire();
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void DessinerEchequier(int shaderProgram, GLFWwindow* window, unsigned int VertexArrayObject)
{
	//
	enum etatCouleur { BLEU, VERT };
	enum etatCouleur  couleur = VERT;

	unsigned short ligne = 0,
		colonne = 0;

	enum etatCase_t
	{
		INIT0,
		INIT1,
		CHANGER_LIGNE,
		CASE_BLEU,
		CASE_VERT,
		INIT2
	};

	etatCase_t etatCase = INIT0;

	// input
	// -----
	processInput(window);
	/*
	// render
	// ------

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	*/
	// draw our first triangle
	glUseProgram(shaderProgram);

	GLint counterLocation = glGetUniformLocation(shaderProgram, "counter");

	glBindVertexArray(VertexArrayObject); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	/********************************************************************************/
	bool fin = false;

	while (!fin)
	{
		bool dessiner = true;

		switch (etatCase)
		{
		case INIT0: // CASE BLEU
			glUniform1i(counterLocation, 3);
			etatCase = INIT1;

			break;

		case INIT1: // CASE VERT
			glUniform1i(counterLocation, 2);
			colonne++;

			if (colonne != NB_COL - 1)
			{
				etatCase = CASE_BLEU;
			}
			else
			{
				etatCase = CHANGER_LIGNE;
				//ligne++;
			}

			break;

		case CASE_BLEU: // CASE BLEU
			glUniform1i(counterLocation, 3);
			colonne++;

			if (colonne != NB_COL - 1)
			{
				etatCase = CASE_VERT;
			}
			else
			{
				etatCase = CHANGER_LIGNE;
			}

			break;

		case CASE_VERT: // CASE VERT
			glUniform1i(counterLocation, 2);
			colonne++;

			if (colonne != NB_COL - 1)
			{
				etatCase = CASE_BLEU;
			}
			else
			{
				etatCase = CHANGER_LIGNE;
			}

			break;

		case CHANGER_LIGNE: //
			ligne++;
			colonne = 0;

			// NE PAS DESSINER POUR LE MOMENT...
			dessiner = false;

			if (ligne != NB_LIGNE)
			{
				if (ligne % 2 == 1)
				{
					etatCase = INIT2;
				}
				else
				{
					etatCase = INIT0;
				}
			}
			else
			{
				// FIN DE LA BOUCLE
				fin = true;
				ligne = 0;
				etatCase = INIT0;
			}

			break;

		case INIT2: // CASE VERT
			glUniform1i(counterLocation, 2);
			etatCase = CASE_BLEU;

			break;

		default: // ERREUR ERREUR ERREUR ERREUR ERREUR!!!!!!!!!!!!!!!!
			std::cout << "ERREUR!!!!!!!!!!!!!!!!!!!!" << std::endl;
			return;

			// NE PAS DESSINER POUR LE MOMENT...
			dessiner = false;
			break;
		}

		if (dessiner)
		{
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(12 * (ligne * NB_COL + colonne) * 2));
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(12 * ((ligne * NB_COL + colonne) * 2 + 1)));

			if (glGetError() == GL_INVALID_ENUM)
			{
				std::cout << "FONCTION [[glDrawElements]] FAILED" << std::endl;
				return;
			}
		}
	}
	//

	// D'AUTRES VBO SERONT UTILIS�S
	glBindVertexArray(0); // no need to unbind it every time 
	/*
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();*/
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//// Obtenir la position de la souris
	double xPos,
		   yPos;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xPos, &yPos);

		int longueur,
			largeur;

		float xRatio,
			  yRatio;

		glfwGetFramebufferSize(window, &longueur, &largeur);

		// 3 - Souris sur l'�chequier ?????
		xRatio = float(xPos / longueur);
		yRatio = 1.0f -  float(yPos / largeur);

		float xMarge = 1 - X_2;
		float yMarge = 1 - Y_2;

		if ((xRatio >= xMarge / 2) && (xRatio <= (1 - (xMarge / 2))))
		{
			if ((yRatio >= yMarge / 2) && (yRatio <= (1 - (yMarge / 2))))
			{
				// CES COORDONN�ES VARIENT DE 0 � 7
				unsigned char rangeeTemp2 = (unsigned char)(2 * (yRatio - (DY/2)) / DY) + 1;
				char colonneTemp2		  = (char)(2 * (xRatio - (DX/2)) / DX) + 'A';

				if ((rangeeTemp2 >= 1) && (rangeeTemp2 <= 8) && (colonneTemp2 >= 'A') && (colonneTemp2 <= 'H'))
				{
					// LE JOUEUR CHOISIT LA "PI�CE" � BOUGER
					if( !positionValide )
					{
						rangeeDepart  = rangeeTemp2;
						colonneDepart = colonneTemp2;

						///**************************************************************/
						//cout << "LA RANG�E DE D�PART EST : " << rangeeDepart << endl;
						//cout << "LA COLONNE DE D�PART EST : " << colonneDepart << endl;
						///**************************************************************/

						positionValide = true;
					}
					else // LE JOUEUR CHOISIT LA "CASE FINALE"
					{
						rangeeArrivee  = rangeeTemp2;
						colonneArrivee = colonneTemp2;

						///***************************************************************/
						//cout << "LA RANG�E D'ARRIV�E EST : " << rangeeArrivee << endl;
						//cout << "LA COLONNE D'ARRIV�E EST : " << colonneArrivee << endl;
						///***************************************************************/

						caseValide = true;
					}
				}
				else
				{
					cout << "ERREUR: CASE INVALIDE CASE INVALIDE CASE INVALIDE!!!!!!!!!!!!!!!!!!!" << endl;
				}
			}
		}
	}
}

int BuildAndCompileShaderProgram(const char * vertexShaderSrc, const char * fragmentShaderSrc)
{
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	// ----------------------------------------
	int shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertexShader);
	glAttachShader(shaderProg, fragmentShader);
	glLinkProgram(shaderProg);
	// check for linking errors
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProg, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProg;
}

void setUpAndConfigureVertex(unsigned int& VertexBufferObject, unsigned int& VertexArrayObject, unsigned int& ElementBufferObject)
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//float vertices[NB_VERTICES]; // NB_VERTICES

	for (unsigned int i = 0; i < NB_VERTICES; i++)
	{
		if (i % 3 == 0)		// COORD X( i = 0, 3, 6, 9, ..., NB_VERTICES-2)
		{
			if ((i / 3) % (NB_COL + 1) > 0)
			{
				vertices[i] = X_1 + ((i / 3) % (NB_COL + 1)) * DX;
				//vertices[i] = X_1 + ((i / (NB_COL + 1)) % NB_VERTICES) * DX;
			}
			else
			{
				vertices[i] = X_1;
			}
		}
		else if (i % 3 == 1) // COORD Y( i = 1, 4, 7, 10, ..., NB_VERTICES-2)
		{
			vertices[i] = Y_2 - ((i / 3) / (NB_COL + 1)) * DX;
		}
		else //(i % 3 == 2)    COORD Z
		{
			vertices[i] = 0.0f;
		}
	}

	//unsigned int indices[NB_INDICES];

	for (unsigned int j = 0; j < NB_LIGNE; j++)
	{
		// POUR CHAQUE CARR�
		for (unsigned int l = 0; l < NB_COL * 6; l += 6)
		{
			indices[(j * 6 * NB_COL) + l] = j * (NB_COL + 1) + (l / 6);
			//indices[(j * 6 * NB_COL) + l] = l / 6;
			indices[(j * 6 * NB_COL) + l + 1] = indices[(j * 6 * NB_COL) + l] + 1;
			indices[(j * 6 * NB_COL) + l + 2] = indices[(j * 6 * NB_COL) + l + 1] + (NB_COL + 1);

			indices[(j * 6 * NB_COL) + l + 3] = indices[(j * 6 * NB_COL) + l];
			indices[(j * 6 * NB_COL) + l + 4] = indices[(j * 6 * NB_COL) + l] + (NB_COL + 1);
			indices[(j * 6 * NB_COL) + l + 5] = indices[(j * 6 * NB_COL) + l + 2];
		}
	}

	//
	glGenVertexArrays(1, &VertexArrayObject);
	glGenBuffers(1, &VertexBufferObject);
	glGenBuffers(1, &ElementBufferObject);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}