#ifndef CASE_H
#define CASE_H

class Piece;

const unsigned int NB_COL = 8,
				   NB_LIGNE = 8;

const float X_1 = -0.8f,
			X_2 = 0.8f,
			Y_1 = -0.8f,
			Y_2 = 0.8f;
/*
const float X_1 = -0.5,
X_2 = 0.5,
Y_1 = -0.5,
Y_2 = 0.5;
*/
const float DX = (X_2 - X_1) / NB_COL,
			DY = (Y_2 - Y_1) / NB_LIGNE;

struct position
{
	float x;
	float y;
	float z;
};

class Case
{
public:
	Case(unsigned char rang = 0, char col = '@', bool occupee = false)
	{
		setRangee(rang);
		setColonne(col);
		setEtat(occupee);

		float xCentre = X_1 + (DX/2) + DX * (col - 'A'),
			  yCentre = Y_1 + (DY/2) + DY * (rang - 1);

		setCentre(xCentre, yCentre, 0.0); // z = 0.0

		// L'INITIALISATION DES PIÈCES SE
		// FAIT DANS LA FONCTION "main"...
		piecePtr = NULL;
	}

	//void operator = (Case CaseXY)
	Case(Case& CaseXY)
	{
		setRangee(CaseXY.getRangee());
		setColonne(CaseXY.getColonne());
		setEtat(CaseXY.getEtat());

		setCentre(CaseXY.getCentre().x, CaseXY.getCentre().y, 0.0);
		setPiecePtr(CaseXY.getPiecePtr());
	}

	/********************************************************/
	/*	 ?????????????		RÉCURSIF	 ???????????????	*/
	//Case(const Case& uneCase)
	//{
	//	*this = uneCase;
	//}
	/*	 ?????????????		RÉCURSIF	 ???????????????	*/
	/********************************************************/

	~Case()
	{
		//delete piecePtr;
	}

	unsigned char getRangee();

	void setRangee(unsigned char NumRangee);

	char getColonne();

	void setColonne(char NumColonne);

	struct position getCentre();

	void setCentre(float x, float y, float z);

	bool getEtat();

	void setEtat(bool caseOccuppee);

	Piece * getPiecePtr();

	void setPiecePtr(Piece * nouvPiecePtr);

private:
	bool occupee;
	Piece * piecePtr;
	unsigned char rangee;
	char colonne;
	position centre;
};


unsigned char Case::getRangee()
{
	return rangee;
}

void Case::setRangee(unsigned char NumRangee)
{
	rangee = NumRangee;
}

char Case::getColonne()
{
	return colonne;
}

void Case::setColonne(char NumColonne)
{
	colonne = NumColonne;
}

struct position Case::getCentre()
{
	return centre;
}

void Case::setCentre(float x, float y, float z = 0.0)
{
	centre.x = x;
	centre.y = y;
	centre.z = z;
}

bool Case::getEtat()
{
	return occupee;
}

void Case::setEtat(bool caseOccuppee)
{
	occupee = caseOccuppee;
}

Piece * Case::getPiecePtr()
{
	return piecePtr;
}

void Case::setPiecePtr(Piece * nouvPiecePtr)
{
	piecePtr = nouvPiecePtr;
}
#endif // !CASE_H

