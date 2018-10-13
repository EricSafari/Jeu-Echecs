
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "Case.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//Define this somewhere in your header file
#define BUFFER_OFFSET(i) ((void*)(i))

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//const unsigned int NB_COL = 8,
//				   NB_LIGNE = 8;

const unsigned int NB_VERTICES = 3 * (NB_COL + 1) * (NB_LIGNE + 1),
					NB_INDICES = 2 * 3 * NB_LIGNE * NB_COL;

//const float X_1 = -0.5,
//			X_2 = 0.5,
//			Y_1 = -0.5,
//			Y_2 = 0.5;
//
//const float DX = (X_2 - X_1) / NB_COL,
//			DY = (Y_2 - Y_1) / NB_LIGNE;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform bool counter;\n"
"void main()\n"
"{\n"
"   if( counter )\n"
"     FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"   else\n"
"     FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";



struct Joueur
{
	couleur_t couleur;
	unsigned short nbPiecesGagnees;
	unsigned short nbPiecesPerdues;
};


enum etat_jeu_t { INIT0, INIT1, ATT_JOUEUR_BLANC, JOUER, REDESSINER, ATT_JOUEUR_NOIR };
enum joueur_tour_t {JOUEUR_BLANC, JOUEUR_NOIR};
//bool EchecEtMat = false;
//Case echequier[8][8];


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
	float vertices[NB_VERTICES]; // NB_VERTICES

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

	unsigned int indices[NB_INDICES];

	for (unsigned int j = 0; j < NB_LIGNE; j++)
	{
		// POUR CHAQUE CARRÉ
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

	// render
	// ------

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw our first triangle
	glUseProgram(shaderProgram);

	unsigned short counterLocation = glGetUniformLocation(shaderProgram, "counter");

	glBindVertexArray(VertexArrayObject); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	/********************************************************************************/
	bool fin = false;
	while( !fin )
	{
		bool dessiner = true;
		switch (etatCase)
		{
		case INIT0: // CASE BLEU
			glUniform1ui(counterLocation, false);
			etatCase = INIT1;

			break;

		case INIT1: // CASE VERT
			glUniform1ui(counterLocation, true);
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
			glUniform1ui(counterLocation, false);
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
			glUniform1ui(counterLocation, true);
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
			glUniform1ui(counterLocation, true);
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

			////////////////////////////////////////////////////////
			/*glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);*/
			/*glDrawElements(GL_LINE_STRIP, NB_INDICES, GL_UNSIGNED_INT, 0);*/
			////////////////////////////////////////////////////////
			if (glGetError() == GL_INVALID_ENUM)
			{
				std::cout << "FONCTION [[glDrawElements]] FAILED" << std::endl;
				return;
			}
		}
	}
	//

	// D'AUTRES VBO SERONT UTILISÉS
	glBindVertexArray(0); // no need to unbind it every time 

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void DessinerUnePiece(Case laCase, struct Piece piece)
{
	//// CENTRE
	//struct position centre;

	//centre.x = X_1 + DX * (laCase.getColonne() - 'A' + 1);
	//centre.y = X_2 + DY * (laCase.getRangee() )
	float cote = DX / 4,
		  base = DX / 4,
		  L = DX / 4,
		  l = DX / 8;

	switch( piece.type )
	{
	case PION: //
		//float cote = DX / 4;
		break;

	case TOUR: //
		//float base1 = DX / 4;
		break;

	case CAVALIER: //
		//float base2 = DX / 4;
		break;

	case FOU: //
		//float base3 = DX / 4;
		break;

	case REINE: //
		//float L1 = DX / 4,
					//l1 = DX / 8;
		break;

	case ROI: //
		//float L2 = DX / 4,
					//l2 = DX / 8;
		break;

	default: //
		cout << "ERREUR!!!!!!!!!!!!!!!!!!!" << endl << endl;
		break;
	}
}

void DessinerLesPieces()
{
	// DESSINER LES PIECES SUR LES CASES OCCUPÉES
	for (unsigned short k = 0; k < 8; k++)
	{
		for (unsigned short l = 0; l < 8; l++)
		{
			// CASE OCCUPÉE
			if( echequier[k][l].getEtat() )
			{
				Case uneCase( echequier[k][l].getRangee(), echequier[k][l].getColonne() );
				DessinerUnePiece( uneCase, echequier[k][l].getPiece());
			}
		}
	}
}

// Vérifier si la case(la position finale) est valide.
// Selon chaque pièce(déplacement permis, case inoccupée/pièce adverse),
// passage non bloqué,.....
bool verifierCase(unsigned char rangee, char colonne);

//using namespace std;
bool positionValide = false,
	 caseValide		= false;

//unsigned char rangeeTemp;
//char colonneTemp;

int main(int argc, int * argv[])
{
	bool EchecEtMat = false;

	etat_jeu_t etatDuJeu = INIT0;
	joueur_tour_t joueur = JOUEUR_BLANC;

	cout << "ALLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << endl << endl;

	// INITIALISATION DES CASES
	for (char k = 0; k < 8; k++)
	{
		for (char l = 0; l < 8; l++)
		{
			// ASSIGNATION DES RANGÉES ET COLONNES
			echequier[k][l] = Case( k + 1, 'A' + l);

			// LE CENTRE( CASES INOCCUPPÉES )
			if ((k >= 2) && (k <= 5))
			{
				echequier[k][l].setEtat(false);
			}
			else //LES BORDS( CASES OCCUPPÉES )
			{
				echequier[k][l].setEtat(true);
			}

			// 1er Rangée
			if (k == 0)
			{
				Piece pieceTemp;
				pieceTemp.couleur = BLANC;

				switch (l)
				{
					case 0: // TOUR
					case 7: pieceTemp.type = TOUR;
							break;

					case 1: // CAVALIER
					case 6: pieceTemp.type = CAVALIER;
							break;

					case 2: // FOU
					case 5: pieceTemp.type = FOU;
						break;

					case 3: pieceTemp.type = REINE;
						break;

					case 4: pieceTemp.type = ROI;
						break;

					default: cout << "ERREUR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
						break;
				}

				echequier[k][l].setPiece(pieceTemp);
			}

			// 2e Rangée
			if (k == 1)
			{
				Piece PionBlanc;
				PionBlanc.couleur = BLANC;
				PionBlanc.type = PION;

				echequier[k][l].setPiece(PionBlanc);
			}

			// 7e Rangée
			if (k == 6)
			{
				Piece PionNoir;
				PionNoir.couleur = NOIR;
				PionNoir.type = PION;

				echequier[k][l].setPiece(PionNoir);
			}

			// 8e Rangée
			if (k == 7)
			{
				Piece pieceTemp;
				pieceTemp.couleur = NOIR;

				switch (l)
				{
				case 0: // TOUR
				case 7: pieceTemp.type = TOUR;
					break;

				case 1: // CAVALIER
				case 6: pieceTemp.type = CAVALIER;
					break;

				case 2: // FOU
				case 5: pieceTemp.type = FOU;
					break;

				case 3: pieceTemp.type = REINE;
					break;

				case 4: pieceTemp.type = ROI;
					break;

				default: cout << "ERREUR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
					break;
				}

				echequier[k][l].setPiece(pieceTemp);
			}
		}
	}

	// OPÉRATIONS DE GLFW.........
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
	//glfwSetMouseButtonCallback( window, mouse_button_callback );

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//// enable OpenGL debug context if context allows for debug context
	//GLint flags;
	//glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	//if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	//{
	//	glEnable(GL_DEBUG_OUTPUT);
	//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
	//	glDebugMessageCallback(glDebugOutput, nullptr);
	//	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	//}

	// build and compile our shader program
	// ------------------------------------
	int shaderProgram = BuildAndCompileShaderProgram(vertexShaderSource, fragmentShaderSource);

	//// set up vertex data (and buffer(s)) and configure vertex attributes
	//// ------------------------------------------------------------------

	/*unsigned int VBO = 0,
				 VAO = 0,
				 EBO = 0;*/
	unsigned int VBO, VAO, EBO;
	setUpAndConfigureVertex( VBO, VAO, EBO );

	while ( !glfwWindowShouldClose(window) )
	{
		///////////////////////////////////////
		///////////////////////////////////////
		while( !EchecEtMat )
		{
			switch (etatDuJeu)
			{
				case INIT0:	// DESSINER L'ÉCHEQUIER
					DessinerEchequier(shaderProgram, window, VAO);
					//etatDuJeu = INIT1;

					break;

				case INIT1:	// DESSINER LES PIÈCES
					
					etatDuJeu = ATT_JOUEUR_BLANC;
					break;

				case ATT_JOUEUR_BLANC:
					// Le joueur a cliqué sur la souris
					// Et Nous sommes sur l'Échequier
					if (positionValide)
					{
						// 1 - Case occupée ?????
						// 2 - Pièce blanche
						if((echequier[rangeeTemp - 1][colonneTemp - 1].getEtat()) && (echequier[rangeeTemp - 1][colonneTemp - 1].getPiece().couleur == BLANC) )
						{
							etatDuJeu = JOUER;
							//positionValide = false;
						}
					}
					break;

				case JOUER:	//
					//if( !EchecEtMat )
					if (caseValide)
					{
						if (joueur == JOUEUR_BLANC)
						{
							//
							joueur = JOUEUR_NOIR;
							//etatDuJeu = ATT_JOUEUR_NOIR;
						}
						else
						{
							//
							joueur = JOUEUR_BLANC;
						}

						etatDuJeu = REDESSINER;
					}
					/*else
					{
						etatDuJeu = INIT0;
					}*/
					break;

				case REDESSINER:	//
					// METTRE À JOUR L'ÉCHEQUIER
					//.....

					// PASSER À L'AUTRE JOUEUR
					positionValide = false;
					caseValide	   = false;

					if (joueur == JOUEUR_BLANC)
					{
						etatDuJeu = ATT_JOUEUR_NOIR;
					}
					else
					{
						etatDuJeu = ATT_JOUEUR_BLANC;
					}
					break;

				case ATT_JOUEUR_NOIR:
					// Le joueur a cliqué sur la souris
					// Et Nous sommes sur l'Échequier
					if (positionValide)
					{
						// 1 - Case occupée ?????
						// 2 - Pièce Noire
						if ((echequier[rangeeTemp - 1][colonneTemp - 1].getEtat()) && (echequier[rangeeTemp - 1][colonneTemp - 1].getPiece().couleur == NOIR))
						{
							etatDuJeu = JOUER;
						}
					}
					break;

				default:	cout << "BYE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
					return -1;
			}
		}
		///////////////////////////////////////
		///////////////////////////////////////

		//// glBindVertexArray(0); // no need to unbind it every time 

		//// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		//// -------------------------------------------------------------------------------
		//glfwSwapBuffers(window);
		//glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
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

//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, float xpos, float ypos)
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

		// 3 - Souris sur l'échequier ?????
		if (((float)xPos >= X_1) && ((float)xPos <= X_2))
		{
			if (((float)yPos >= Y_1) && ((float)yPos <= Y_2))
			{
				unsigned char rangeeTemp2 = (unsigned char)((xPos + X_2) / DX) + 1;
				char colonneTemp2		  = (char)((yPos + Y_2) / DY) + 'A';

				//positionValide = true;
				if ((rangeeTemp >= 0) && (rangeeTemp <= 3) && (colonneTemp >= 0) && (colonneTemp <= 3))
				{
					if( !positionValide )
					{
						rangeeTemp  = rangeeTemp2;
						colonneTemp = colonneTemp2;

						positionValide = true;
					}
					else
					{
						caseValide = verifierCase( rangeeTemp2, colonneTemp2 );
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

bool verifierCase(unsigned char rangee, char colonne)
{
	//
	return false;
}