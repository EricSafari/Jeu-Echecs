#ifndef CASE_H
#define CASE_H

//#include "Piece.h"
//#include "util.h"

const unsigned int NB_COL = 8,
NB_LIGNE = 8;

const float X_1 = -0.5,
X_2 = 0.5,
Y_1 = -0.5,
Y_2 = 0.5;

const float DX = (X_2 - X_1) / NB_COL,
DY = (Y_2 - Y_1) / NB_LIGNE;

//enum couleur_t { BLANC, NOIR };

struct position
{
	float x;
	float y;
	float z;
};

//enum type_t { PION, TOUR, CAVALIER, FOU, REINE, ROI };

// DOIT ÊTRE MODIFIÉ EN CLASSE POUR INTÉGRER
// LE POLYMORPHISME( DIFFÉRENTES PIÈCES )
struct Piece
{
	couleur_t couleur;
	type_t type;
};

class Case
{
public:
	Case(unsigned char rang = 0, char col = '@')
	{
		/*rangee = rang;
		colonne = col;*/
		setRangee(rang);
		setColonne(col);
		setEtat(false);

		float xCentre = X_1 + DX * (col - 'A' + 1),
			  yCentre = X_2 + DY * rang;

		setCentre(xCentre, yCentre); // z = 0.0

		// L'INITIALISATION DES PIÈCES SE
		// FAIT DANS LA FONCTION "main"
		piecePtr = new struct Piece;
		//piecePtr->couleur = BLANC;
		//setPiece(*piecePtr);
	}

	Case(Case& uneCase)
	{
		//Case( uneCase.getRangee(), uneCase.getColonne() );
		setRangee( uneCase.getRangee() );
		setColonne( uneCase.getColonne() );
		setEtat( uneCase.getEtat() );

		setCentre( uneCase.getCentre().x, uneCase.getCentre().y );
		setPiece( *(uneCase.getPiece()) );
	}

	~Case()
	{
		delete piecePtr;
	}

	unsigned char getRangee()
	{
		return rangee;
	}

	void setRangee(unsigned char NumRangee)
	{
		rangee = NumRangee;
	}

	char getColonne()
	{
		return colonne;
	}

	void setColonne(char NumColonne)
	{
		colonne = NumColonne;
	}

	struct position getCentre()
	{
		return centre;
	}

	void setCentre(float x, float y, float z = 0.0)
	{
		centre.x = x;
		centre.y = y;
		centre.z = z;
	}

	bool getEtat()
	{
		return occupee;
	}

	void setEtat(bool caseOccuppee)
	{
		occupee = caseOccuppee;
	}

	struct Piece * getPiece()
	{
		return piecePtr;
	};

	void setPiece(struct Piece nouvPiece)
	{
		piecePtr->couleur = nouvPiece.couleur;
		piecePtr->type = nouvPiece.type;
	}

private:
	bool occupee;
	struct Piece * piecePtr;
	unsigned char rangee;
	char colonne;
	position centre;

	position coinGaucheBas;
	position coinGaucheHaut;
	position coinDroiteBas;
	position coinDroiteHaut;
};
#endif // !CASE_H

