#ifndef PIECE_H
#define PIECE_H

enum couleur_t { BLANC, NOIR };
enum type_t { PION, TOUR, CAVALIER, FOU, REINE, ROI };

Case echequier[8][8];
unsigned char rangeeTemp;
char colonneTemp;


class Piecee
{
public:
	Piecee(couleur_t laCouleur = BLANC, type_t leType = PION)
	{
		setCouleur(laCouleur);
		setType(leType);
	}

	void setUpAndConfigureObjects(const float verticesPieces[], unsigned int& VertexBufferObject, unsigned int& VertexArrayObject);

	//void UpdateScren(float verticesPieces[], GLFWwindow *window, int shaderProg)
	void UpdateScren(unsigned short verticesSize, GLFWwindow *window, int shaderProg)
	{
		// input
		// -----
		processInput(window);

		// draw our first triangle
		glUseProgram(shaderProg);

		// PAS BESOIN DE "BINDER" LE VAO
		if(verticesSize == 9 * sizeof(float) )
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else if(verticesSize == 18 * sizeof(float) )
		{
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else
		{
			cout << "ERREUR: NOMBRE DE VERTICES INVALIDE ??????????????????????????" << endl << endl;
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	virtual void dessiner(unsigned char rang, char col) = 0;

	virtual bool validerCase(unsigned char rang, char col) = 0;

	bool validerPositionFinale(unsigned char rang, char col)
	{
		bool PositionValide = false;

		Case caseInitiale(echequier[rangeeTemp - 1][colonneTemp - 1]),
			caseFinale(echequier[rang - 1][col - 1]);

		if( !caseFinale.getEtat() )
		{
			PositionValide = true;
		}
		else
		{
			if ((caseInitiale.getPiece().couleur == NOIR) && (caseFinale.getPiece().couleur == BLANC))
			{
				PositionValide = true;
			}
			else if ((caseInitiale.getPiece().couleur == BLANC) && (caseFinale.getPiece().couleur == NOIR))
			{
				PositionValide = true;
			}
		}

		return PositionValide;
	}

	couleur_t getCouleur()
	{
		return couleur;
	}

	void setCouleur(couleur_t laCouleur)
	{
		couleur = laCouleur;
	}

	type_t getType()
	{
		return type;
	}

	void setType(type_t leType)
	{
		type = leType;
	}

private:
	couleur_t couleur;
	type_t type;
};

// VertexBufferObject et VertexArrayObject ne sont pas nécessaires
void Piecee::setUpAndConfigureObjects( const float verticesPieces[], unsigned int& VertexBufferObject, unsigned int& VertexArrayObject )
{
	//
	glGenVertexArrays(1, &VertexArrayObject);
	glGenBuffers(1, &VertexBufferObject);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPieces), verticesPieces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);
}

#endif // !PIECE_H

