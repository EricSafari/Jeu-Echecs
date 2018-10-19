#ifndef INIT_H
#define INIT_H

#include "util.h"

#include "Piece.h"
#include "Pion.h"
#include "Tour.h"
#include "Cavalier.h"
#include "Fou.h"
#include "Reine.h"
#include "Roi.h"

#include <vector>

bool positionValide = false,
	 caseValide = false;

bool EchecEtMat = false,
	 Echec = false,
	 Pat = false;

unsigned char rangeeMenace;
char colonneMenace;

void InitialiserLesCases()
{
	Piece * piecePtrTemp = NULL;

	// INITIALISATION DES CASES
	for (char k = 0; k < 8; k++)
	{
		for (char l = 0; l < 8; l++)
		{
			// ASSIGNATION DES RANGÉES ET COLONNES
			echequier[k][l] = Case(k + 1, l + 'A');

			// RANGÉES 2 ET 7 !!!!!!!!
			// LES PIONS.........
			if ((k == 1) || (k == 6))
			{
				echequier[k][l].setEtat(true);

				piecePtrTemp = new Pion();
				// LE CONSTRUCTEUR DU PION "SET"
				// LE TYPE À "PION" PAR DÉFAUT

				if (k == 1) // 2e Rangée
				{
					piecePtrTemp->setCouleur(BLANC);
				}
				else // 7e Rangée
				{
					piecePtrTemp->setCouleur(NOIR);
				}
			}
			else
			{
				// 1re et 8e Rangée
				// AUTRES PIÈCES: Tour, Fou, Reine, ...
				if ((k == 0) || (k == 7))
				{
					echequier[k][l].setEtat(true);

					// COLONNES!!!!!!!!!!!!!!!!!!
					// -------------------------------- //
					// C'EST "ICI" QU'ON "ALLOUE LA MÉMOIRE"
					// POUR "LES PIÈCES" PUISQU'ON CONNAIT  
					// "MAINTENANT..." "LEURS TYPES"

					switch (l)
					{
					case 0: // TOUR
					case 7: //......
						piecePtrTemp = new Tour();
						piecePtrTemp->setType(TOUR);
						break;

					case 1: // CAVALIER
					case 6:
						piecePtrTemp = new Cavalier();
						piecePtrTemp->setType(CAVALIER);
						break;

					case 2: // FOU
					case 5:
						piecePtrTemp = new Fou();
						piecePtrTemp->setType(FOU);
						break;

					case 3:
						piecePtrTemp = new Reine();
						piecePtrTemp->setType(REINE);
						break;

					case 4:
						piecePtrTemp = new Roi();
						piecePtrTemp->setType(ROI);
						break;

					default: cout << "ERREUR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
						break;
					}

					if (k == 0) // 1re Rangée
					{
						piecePtrTemp->setCouleur(BLANC);
					}
					else // 8e Rangée
					{
						piecePtrTemp->setCouleur(NOIR);
					}
				}
				else // Les Autres Rangées
				{
					// LE POINTEUR EST DÉJÀ NUL$$$(FAUX FAUX FAUX FAUX FAUX.................)
					piecePtrTemp = NULL;
				}
			}

			echequier[k][l].setPiecePtr(piecePtrTemp);
		}
	}
}

Piece * pieceArriveePtrTemp;
//Piece * pieceArriveePtrTemp = NULL;
void deplacerPiece(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	Case CaseDepart(echequier[rang1 - 1][col1 - 'A']);
	Case CaseArrivee(echequier[rang2 - 1][col2 - 'A']);

	Piece * pieceDepartPtr  = NULL,
		  * pieceArriveePtr = NULL;

	pieceDepartPtr = CaseDepart.getPiecePtr();
	// ...............
	pieceArriveePtrTemp = NULL;

	// ON SAUVEGARDE...
	if (echequier[rang2 - 1][col2 - 'A'].getEtat())
	{
		pieceArriveePtr = CaseArrivee.getPiecePtr();
		pieceArriveePtrTemp = pieceArriveePtr;
	}

	// ON FAIT POINTER LA 2e CASE VERS
	// LA PIÈCE DE LA 1re CASE...
	echequier[rang2 - 1][col2 - 'A'].setPiecePtr(pieceDepartPtr);
	echequier[rang2 - 1][col2 - 'A'].setEtat(true);

	echequier[rang1 - 1][col1 - 'A'].setEtat(false);
}

void annulerDeplacement(unsigned char rang1, char col1, unsigned char rang2, char col2)
{
	echequier[rang1 - 1][col1 - 'A'].setEtat(true);

	if (pieceArriveePtrTemp  == NULL)
	{
		echequier[rang2 - 1][col2 - 'A'].setEtat(false);
	}
	else
	{
		echequier[rang2 - 1][col2 - 'A'].setPiecePtr(pieceArriveePtrTemp);
		echequier[rang2 - 1][col2 - 'A'].setEtat(true);
	}
}

// CETTE FONCTION EST TRÈS IMPORTANTE, C'EST ELLE
// QUI "DÉTERMINE SI IL Y A ÉCHEC OU PAS"
//.........
bool continuer(unsigned char rangee, char colonne)
{
	Case laCase(echequier[rangee - 1][colonne - 'A']);

	//....
	Echec = false;

	// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
	if (laCase.getEtat())
	{
		// BLANC = 0, NOIR = 1    et    JOUEUR_BLANC = 0, JOUEUR_NOIR = 1
		//if (laCase.getPiecePtr()->getCouleur() == couleurDuRoi)
		if (laCase.getPiecePtr()->getCouleur() == couleurDuRoiAdverse) // À MODIFIER......
		{
			// ÉVALUER SI LA PIÈCE REPRESENTE UNE MENACE :
			// $$$$$$$$$$$$		ÉCHEC	$$$$$$$$$$$$$$$$$$
			//if (laCase.getPiecePtr()->validerCase(RangeeDuRoiBlanc, ColonneDuRoiBlanc))
			//if (laCase.getPiecePtr()->validerDeplacement(rangeeTemp, colonneTemp, RangeeDuRoiBlanc, ColonneDuRoiBlanc))
			//if (laCase.getPiecePtr()->validerDeplacement(rangee, colonne, RangeeDuRoiAdverse, ColonneDuRoiNoir))
			if (laCase.getPiecePtr()->validerDeplacement(rangee, colonne, RangeeDuRoi, ColonneDuRoi))
			{
				Echec = true;
			}
			else
			{
				Echec = false;
			}
		}
		else // MÊME COULEUR
		{
			//Echec = false;
		}

		return false;
	}
	else
	{
		return true;
	}
}

bool verifierSiCavalier(Case caseXY, unsigned short couleur)
{
	if (caseXY.getEtat())
	{
		if (caseXY.getPiecePtr()->getCouleur() != couleur)
		{
			if (caseXY.getPiecePtr()->getType() == CAVALIER)
			{
				return true;
			}
		}
	}

	return false;
}

bool verifierSiCavaliersMenacants(unsigned char RangeeDuRoi, char ColonneDuRoi, unsigned short couleurDuRoi)
{
	bool menace = false;

	// VÉRIFIER SI IL Y'A UN CAVALIER... SUR L'UNE DES CASES(8 MAX.) "POTENTIELLES"
	if (((RangeeDuRoi + 1) >= 1) && ((RangeeDuRoi + 1) <= 8) && ((ColonneDuRoi - 2) >= 'A') && ((ColonneDuRoi - 2) <= 'H'))
	{
		Case caseCavalier(echequier[RangeeDuRoi][ColonneDuRoi - 2 - 'A']);

		Echec = verifierSiCavalier(caseCavalier, couleurDuRoi);

		if (Echec)
		{
			return true;
		}
	}

	if (((RangeeDuRoi + 1) >= 1) && ((RangeeDuRoi + 1) <= 8) && ((ColonneDuRoi + 2) >= 'A') && ((ColonneDuRoi + 2) <= 'H'))
	{
		Case caseCavalier(echequier[RangeeDuRoi][ColonneDuRoi + 2 - 'A']);

		Echec = verifierSiCavalier(caseCavalier, couleurDuRoi);

		if (Echec)
		{
			return true;
		}
	}

	if (((RangeeDuRoi - 1) >= 1) && ((RangeeDuRoi - 1) <= 8) && ((ColonneDuRoi - 2) >= 'A') && ((ColonneDuRoi - 2) <= 'H'))
	{
		Case caseCavalier(echequier[RangeeDuRoi - 2][ColonneDuRoi - 2 - 'A']);

		Echec = verifierSiCavalier(caseCavalier, couleurDuRoi);

		if (Echec)
		{
			return true;
		}
	}

	if (((RangeeDuRoi - 1) >= 1) && ((RangeeDuRoi - 1) <= 8) && ((ColonneDuRoi + 2) >= 'A') && ((ColonneDuRoi + 2) <= 'H'))
	{
		Case caseCavalier(echequier[RangeeDuRoi - 2][ColonneDuRoi + 2 - 'A']);

		Echec = verifierSiCavalier(caseCavalier, couleurDuRoi);

		if (Echec)
		{
			return true;
		}
	}

	if (((RangeeDuRoi + 2) >= 1) && ((RangeeDuRoi + 2) <= 8) && ((ColonneDuRoi - 1) >= 'A') && ((ColonneDuRoi - 1) <= 'H'))
	{
		Case caseCavalier(echequier[RangeeDuRoi + 1][ColonneDuRoi - 1 - 'A']);

		Echec = verifierSiCavalier(caseCavalier, couleurDuRoi);

		if (Echec)
		{
			return true;
		}
	}

	if (((RangeeDuRoi + 2) >= 1) && ((RangeeDuRoi + 2) <= 8) && ((ColonneDuRoi + 1) >= 'A') && ((ColonneDuRoi + 1) <= 'H'))
	{
		Case caseCavalier(echequier[RangeeDuRoi + 1][ColonneDuRoi + 1 - 'A']);

		Echec = verifierSiCavalier(caseCavalier, couleurDuRoi);

		if (Echec)
		{
			return true;
		}
	}

	if (((RangeeDuRoi - 2) >= 1) && ((RangeeDuRoi - 2) <= 8) && ((ColonneDuRoi - 1) >= 'A') && ((ColonneDuRoi - 1) <= 'H'))
	{
		Case caseCavalier(echequier[RangeeDuRoi - 3][ColonneDuRoi - 1 - 'A']);

		Echec = verifierSiCavalier(caseCavalier, couleurDuRoi);

		if (Echec)
		{
			return true;
		}
	}

	if (((RangeeDuRoi - 2) >= 1) && ((RangeeDuRoi - 2) <= 8) && ((ColonneDuRoi + 1) >= 'A') && ((ColonneDuRoi + 1) <= 'H'))
	{
		Case caseCavalier(echequier[RangeeDuRoi - 3][ColonneDuRoi + 1 - 'A']);

		Echec = verifierSiCavalier(caseCavalier, couleurDuRoi);

		if (Echec)
		{
			return true;
		}
	}

	return menace;
}

//bool validerCase(unsigned char rangee, char colonne)
bool verifierSiEchec()
{
	bool RoiBarricade = true;

	unsigned char rangeeDuTrou;
	char colonneDuTrou;

	/****************************************************************************************/
	/*			ICI, ON VÉRIFIE Si L'Autre ROI EST EN ÉCHEC.								*/
	/*			DONC... IL Y A INVERSION DANS L'ASSIGNATION DE ___RangeeDuRoiAdverse___ ET ...		*/
	/****************************************************************************************/
	if (joueur == JOUEUR_BLANC)
	{
		RangeeDuRoi  = RangeeDuRoiBlanc;
		ColonneDuRoi = ColonneDuRoiBlanc;
		RangeeDuRoiAdverse = RangeeDuRoiNoir;
		ColonneDuRoiAdverse = ColonneDuRoiNoir;

		couleurDuRoi = BLANC;
		couleurDuRoiAdverse = NOIR;
	}
	else
	{
		RangeeDuRoi  = RangeeDuRoiNoir;
		ColonneDuRoi = ColonneDuRoiNoir;
		RangeeDuRoiAdverse = RangeeDuRoiBlanc;
		ColonneDuRoiAdverse = ColonneDuRoiBlanc;

		couleurDuRoi = NOIR;
		couleurDuRoiAdverse = BLANC;
	}

	// VÉRIFIER SI IL Y'A UN CAVALIER... SUR L'UNE DES CASES(8 MAX.) "POTENTIELLES"
	verifierSiCavaliersMenacants(RangeeDuRoi, ColonneDuRoi, couleurDuRoi);
	//verifierSiCavaliersMenacants(RangeeDuRoiAdverse, ColonneDuRoiAdverse, couleurDuRoiAdverse);

	// L'UN DES CAVALIERS MÉNACE LE ROI!!!
	if (Echec)
	{
		return true;
	}

	// VÉRIFIER SI LE ROI EST EN SÉCURITÉ...
	for (int i = -1; i < 2; i++)
	{
		// {-1, -1}, {-1, 0}, {-1, +1}, {0, -1},..., {+1,0}, {+1,1}
		for (int j = -1; j < 2; j++)
		{
			// {-1, -1}, {-1, 0}, {-1, +1}, {0, -1},..., {+1,0}, {+1,1}
			/*if(sousValiderDeplacement(RangeeDuRoiAdverse + i, ColonneDuRoiAdverse + j))*/
			if ((RangeeDuRoi + i >= 1) && (RangeeDuRoi + i <= 8) && (ColonneDuRoi + j >= 'A') && (ColonneDuRoi + j <= 'H'))
			{
				// CASE VOISINE "DU ROI" INOCCUPÉE
				if (!echequier[RangeeDuRoi + i - 1][ColonneDuRoi + j - 'A'].getEtat())
				{
					RoiBarricade = false;

					// SORTIR ET ÉVALUER LA MENACE
					rangeeDuTrou = RangeeDuRoi + i;
					colonneDuTrou = ColonneDuRoi + j;

					// ÉVALUER LA MENACE:  EST-CE QUE LA PIÈCE CHOISIE PAR LE
					// JOUEUR ADVERSE MET LE ROI EN ÉCHEC ???????????????????

					/********************************************/
					/********************************************/
					/*	  Évaluer... Du Roi vers l'Extérieur	*/
					/********************************************/
					/********************************************/
					unsigned short iOffset = 0,
						jOffset = 0;

					bool allerProchaineCase = true;

					// ON SE DÉPLACE AUTOUR DU ROI, DONC.....
					// __DX__ = -1, 0, +1 ET __DY__ = -1, 0, +1
					if (rangeeDuTrou == RangeeDuRoi)
					{
						// VÉRIFIER LA RANGEE:::::
						// .......................
						// PARCOURIR LA RANGEE => SEUL LA COLONNE CHANGE
						// ( Vers la Gauche ou vers la Droite ???????????????????? )
						if (ColonneDuRoi > colonneDuTrou)
						{
							//...
							while ((colonneDuTrou - jOffset >= 'A') && allerProchaineCase)
							{
								// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
								allerProchaineCase = continuer(rangeeDuTrou /*+ iOffset*/, colonneDuTrou - jOffset);

								if (Echec)
								{
									return true;
								}
								//....
								jOffset++;
							}
						}
						else
						{
							//...
							while ((colonneDuTrou + jOffset <= 'H') && allerProchaineCase)
							{
								// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
								// ICI, ON SE DÉPLACE VERS LA DROITE
								allerProchaineCase = continuer(rangeeDuTrou /*+ iOffset*/, colonneDuTrou + jOffset);

								if (Echec)
								{
									return true;
								}

								//....
								jOffset++;
							}
						}
						jOffset = 0;
					}
					else if (colonneDuTrou == ColonneDuRoi)
					{
						// VÉRIFIER LA COLONNE
						//....................
						// PARCOURIR LA COLONNE => SEUL LA RANGÉE CHANGE
						// ( Vers le haut ou vers le Bas ???????? )
						if (RangeeDuRoi > rangeeDuTrou)
						{
							//...
							while ((rangeeDuTrou - iOffset >= 1) && allerProchaineCase)
							{
								// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
								allerProchaineCase = continuer(rangeeDuTrou - iOffset, colonneDuTrou + jOffset);

								if (Echec)
								{
									return true;
								}

								//....
								iOffset++;
							}
						}
						else
						{
							//...
							while ((rangeeDuTrou + iOffset <= 8) && allerProchaineCase)
							{
								// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
								allerProchaineCase = continuer(rangeeDuTrou + iOffset, colonneDuTrou + jOffset);

								if (Echec)
								{
									return true;
								}

								//....
								iOffset++;
							}
						}
						iOffset = 0;
					}
					else //if ((abs(rangeeDuTrou - RangeeDuRoi) == 1) && ...)
					{
						// VÉRIFIER LES DIAGONALES
						//.........................
						// PARCOURIR UNE DIAGONALE => LA RANGÉE ET LA COLONNE
						// CHANGENT EN MÊME TEMPS !!!!
						if ((RangeeDuRoi > rangeeDuTrou) && (ColonneDuRoi > colonneDuTrou))
						{
							//...
							while ((rangeeDuTrou - iOffset >= 1) && (colonneDuTrou - jOffset >= 'A') && allerProchaineCase)
							{
								// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
								allerProchaineCase = continuer(rangeeDuTrou - iOffset, colonneDuTrou - jOffset);

								if (Echec)
								{
									return true;
								}

								//....
								iOffset++;
								jOffset++;
							}
						}
						else if ((RangeeDuRoi < rangeeDuTrou) && (ColonneDuRoi < colonneDuTrou))
						{
							//...
							while ((rangeeDuTrou - iOffset >= 1) && (colonneDuTrou - jOffset >= 'A') && allerProchaineCase)
							{
								// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
								allerProchaineCase = continuer(rangeeDuTrou + iOffset, colonneDuTrou + jOffset);

								if (Echec)
								{
									return true;
								}

								//....
								iOffset++;
								jOffset++;
							}
						}
						else if ((RangeeDuRoi < rangeeDuTrou) && (ColonneDuRoi > colonneDuTrou))
						{
							//...
							while ((rangeeDuTrou + iOffset <= 8) && (colonneDuTrou - jOffset >= 'A') && allerProchaineCase)
							{
								// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
								allerProchaineCase = continuer(rangeeDuTrou + iOffset, colonneDuTrou - jOffset);

								if (Echec)
								{
									return true;
								}

								//....
								iOffset++;
								jOffset++;
							}
						}
						else if ((RangeeDuRoi > rangeeDuTrou) && (ColonneDuRoi < colonneDuTrou))
						{
							//...
							while ((rangeeDuTrou - iOffset >= 1) && (colonneDuTrou + jOffset <= 'H') && allerProchaineCase)
							{
								// Si il y a Pièce, Vérifier d'abord la COULEUR et puis le TYPE après
								allerProchaineCase = continuer(rangeeDuTrou - iOffset, colonneDuTrou + jOffset);

								if (Echec)
								{
									return true;
								}

								//....
								iOffset++;
								jOffset++;
							}
						}
						else
						{
							std::cout << "PROBLÈME: MAUVAISES CASES!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
						}
						iOffset = jOffset = 0;
					}
				}
				else
				{
					//unsigned short couleurDuRoi = echequier[RangeeDuRoi - 1][ColonneDuRoi - 'A'].getPiecePtr()->getCouleur();
					Case CaseIJ(echequier[RangeeDuRoi + i - 1][ColonneDuRoi + j - 'A']);

					//if (CaseIJ.getPiecePtr()->getCouleur() != couleurDuRoi)
					if (CaseIJ.getPiecePtr()->getCouleur() != couleurDuRoi)
					{
						//Echec = CaseIJ.getPiecePtr()->validerCase(RangeeDuRoi, ColonneDuRoi);
						Echec = CaseIJ.getPiecePtr()->validerDeplacement(RangeeDuRoi + i, ColonneDuRoi + j, RangeeDuRoi, ColonneDuRoi);
						//Echec = CaseIJ.getPiecePtr()->validerDeplacement(rangeeTemp, colonneTemp, RangeeDuRoi, ColonneDuRoi);

						if (Echec)
						{
							return true;
						}
					}
				}
			}
		}
	}

	return Echec;
}

struct CaseInterception
{
	unsigned char rangeeInterception;
	char colonneInterception;
};

bool IntercepterMenace(unsigned char rangeeMenace, char colonneMenace, CaseInterception & CaseDeLaPiece, CaseInterception & CaseDArrivee)
{
	bool menaceEliminee = false,
		 menaceBloquee = false,
		 roiEvade = false;
	/*unsigned char rangeeDuTrou;
	char colonneDuTrou;*/

	unsigned char RangeeTempDuRoi;
	char ColonneTempDuRoi;

	// 1 - Éliminer la ménace
	// ......
	for (int i = 0; i < NB_LIGNE; i++)
	{
		for (int j = 0; j < NB_COL; j++)
		{
			//...
			Case caseTemp(echequier[i][j]);

			//if (caseTemp.getPiecePtr()->getCouleur() == couleurDuRoiAdverse)
			if (caseTemp.getPiecePtr()->getCouleur() == couleurDuRoi)
			{
				// Éliminer la Menace
				menaceEliminee = caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rangeeMenace, colonneMenace);

				if (menaceEliminee)
				{
					CaseDeLaPiece = {(unsigned char)(i + 1), (char)j + 'A'};
					CaseDArrivee = { rangeeMenace, colonneMenace };

					return true;
				}
			}
		}
	}

	// 2 - Bloquer la ménace(IL FAUT PENSER AU ROI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
	// ON NE PEUT PAS BLOQUER LES CAVALIERS
	// .....
	for (int i = 0; i < NB_LIGNE; i++)
	{
		for (int j = 0; j < NB_COL; j++)
		{
			// LE ROI NE PEUT PLOQUER LA MÉNACE!!!
			if ((RangeeDuRoiAdverse != i + 1) || (ColonneDuRoiAdverse != j + 'A'))
			{
				Case caseTemp(echequier[i][j]);

				unsigned char rang;
				char col;

				//if (caseTemp.getPiecePtr()->getCouleur() == couleurDuRoiAdverse)
				if (caseTemp.getPiecePtr()->getCouleur() == couleurDuRoi)
				{
					if (echequier[rangeeMenace - 1][colonneMenace - 'A'].getPiecePtr()->getType() != CAVALIER)
					{
						// BLOQUER LA MÉNACE SUR LA RANGÉE, LA COLONNE ET LES DIAGONALES
						if (rangeeMenace == RangeeDuRoiAdverse)
						{
							// PARCOURIR LA LIGNE...
							if (colonneMenace > ColonneDuRoiAdverse)
							{
								for (char col = ColonneDuRoiAdverse + 1; col < colonneMenace; col++)
								{
									//menaceBloquee = caseTemp.getPiecePtr()->validerCase(rangeeMenace, col);
									menaceBloquee = caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rangeeMenace, col);

									if (menaceBloquee)
									{
										CaseDeLaPiece = { (unsigned char)(i + 1), (char)j + 'A' };
										CaseDArrivee = { rangeeMenace, col };

										return true;
									}
								}
							}
							else
							{
								for (char col = colonneMenace + 1; col < ColonneDuRoiAdverse; col++)
								{
									//menaceBloquee = caseTemp.getPiecePtr()->validerCase(rangeeMenace, col);
									menaceBloquee = caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rangeeMenace, col);

									if (menaceBloquee)
									{
										CaseDeLaPiece = { (unsigned char)(i + 1), (char)j + 'A' };
										CaseDArrivee = { rangeeMenace, col };

										return true;
									}
								}
							}
						}
						else if (colonneMenace == ColonneDuRoiAdverse)
						{
							// PARCOURIR LA COLONNE...
							if (rangeeMenace > RangeeDuRoiAdverse)
							{
								for (unsigned char rang = RangeeDuRoiAdverse + 1; rang < rangeeMenace; rang++)
								{
									//menaceBloquee = caseTemp.getPiecePtr()->validerCase(rang, colonneMenace);
									menaceBloquee = caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rang, colonneMenace);
									//menaceBloquee = caseTemp.getPiecePtr()->validerDeplacement(rangeeTemp, colonneTemp, rang, colonneMenace);

									if (menaceBloquee)
									{
										CaseDeLaPiece = { (unsigned char)(i + 1), (char)j + 'A' };
										CaseDArrivee = { rang, colonneMenace };

										return true;
									}
								}
							}
							else
							{
								for (unsigned char rang = rangeeMenace + 1; rang < RangeeDuRoiAdverse; rang++)
								{
									//menaceBloquee = caseTemp.getPiecePtr()->validerCase(rang, colonneMenace);
									menaceBloquee = caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rang, colonneMenace);
									//menaceBloquee = caseTemp.getPiecePtr()->validerDeplacement(rangeeTemp, colonneTemp, rang, colonneMenace);

									if (menaceBloquee)
									{
										CaseDeLaPiece = { (unsigned char)(i + 1), (char)j + 'A' };
										CaseDArrivee = { rang, colonneMenace };

										return true;
									}
								}
							}
						}
						else if (abs(rangeeMenace - RangeeDuRoiAdverse) == abs(colonneMenace - ColonneDuRoiAdverse))
						{
							// PARCOURIR LA DIAGONALE...
							if ((rangeeMenace > RangeeDuRoiAdverse) && (colonneMenace > ColonneDuRoiAdverse))
							{
								//...
								rang = RangeeDuRoiAdverse + 1;
								col = ColonneDuRoiAdverse + 1;

								while (rang < rangeeMenace)
								{
									//if (caseTemp.getPiecePtr()->validerCase(rang, col))
									//if(caseTemp.getPiecePtr()->validerDeplacement(rangeeTemp, colonneTemp, rang, col))
									if (caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rang, col))
									{
										// ON PEUT BLOQUER LA PIÈCE MENAÇANTE,
										// DONC IL N'Y A PLUS "D'ÉCHEC"
										menaceBloquee = true;

										//return false;
										CaseDeLaPiece = { (unsigned char)(i + 1), (char)j + 'A' };
										CaseDArrivee = { rang, col };

										return true;
									}
									rang++;
									col++;
								}
							}
							else if ((rangeeMenace < RangeeDuRoiAdverse) && (colonneMenace < ColonneDuRoiAdverse))
							{
								//...
								rang = RangeeDuRoiAdverse - 1;
								col = ColonneDuRoiAdverse - 1;

								while (rang > rangeeMenace)
								{
									if (caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rang, col))
									{
										// ON PEUT BLOQUER LA PIÈCE MENAÇANTE,
										// DONC IL N'Y A PLUS "D'ÉCHEC"
										menaceBloquee = true;

										//return false;
										CaseDeLaPiece = { (unsigned char)(i + 1), (char)j + 'A' };
										CaseDArrivee = { rang, col };

										return true;
									}

									rang--;
									col--;
								}
							}
							else if ((rangeeMenace > RangeeDuRoiAdverse) && (colonneMenace < ColonneDuRoiAdverse))
							{
								//...
								rang = RangeeDuRoiAdverse + 1;
								col = ColonneDuRoiAdverse - 1;

								while (rang < rangeeMenace)
								{
									if (caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rang, col))
									{
										// ON PEUT BLOQUER LA PIÈCE MENAÇANTE,
										// DONC IL N'Y A PLUS "D'ÉCHEC"
										menaceBloquee = true;

										//return false;
										CaseDeLaPiece = { (unsigned char)(i + 1), (char)j + 'A' };
										CaseDArrivee = { rang, col };

										return true;
									}

									rang++;
									col--;
								}
							}
							else if ((rangeeMenace < RangeeDuRoiAdverse) && (colonneMenace > ColonneDuRoiAdverse))
							{
								//...
								rang = RangeeDuRoiAdverse - 1;
								col = ColonneDuRoiAdverse + 1;

								while (rang > rangeeMenace)
								{
									if (caseTemp.getPiecePtr()->validerDeplacement(i + 1, j + 'A', rang, col))
									{
										// ON PEUT BLOQUER LA PIÈCE MENAÇANTE,
										// DONC IL N'Y A PLUS "D'ÉCHEC"
										menaceBloquee = true;

										//return false;
										CaseDeLaPiece = { (unsigned char)(i + 1), (char)j + 'A' };
										CaseDArrivee = { rang, col };

										return true;
									}

									rang--;
									col++;
								}
							}
							else
							{
								// ON NE DEVRAIT PAS SE RETROUVER DANS CE CAS
								// ......
								std::cout << "PROBLÈME: MAUVAISES CASES!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
							}
						}
					}
					else
					{
						//............ON NE POURRA JAMAIS AVOIR "menaceBloquee = true" CAR
						//............SI ON SE REND ICI, C'EST QUE IL Y A UN CAVALIER QUI
						//............MÉNACE LE ROI(ÉCHEC!!!!! ÉCHEC!!!!!! ÉCHEC!!!! ÉCHEC)
						/*menaceBloquee = !verifierSiCavaliersMenacants(RangeeDuRoiAdverse, ColonneDuRoiAdverse, couleurDuRoiAdverse);
						
						if (menaceBloquee)
						{
							return true;
						}*/
					}
				}
			}
		}
	}

	// 3 - DÉPLACER LE ROI
	// {i, j} : {-1, -1}, {-1, 0}, {-1, +1}, {0, -1},..., {+1,0}, {+1,1}
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if ((RangeeDuRoiBlanc + i >= 1) && (RangeeDuRoiBlanc + i <= 8) && (ColonneDuRoiBlanc + j >= 'A') && (ColonneDuRoiBlanc + j <= 'H'))
			{
				// CASE VIDE
				// .....................
				if (!echequier[RangeeDuRoiBlanc + i - 1][ColonneDuRoiBlanc + j - 'A'].getEtat())
				{
					RangeeTempDuRoi = RangeeDuRoiBlanc;
					ColonneTempDuRoi = ColonneDuRoiBlanc;
					echequier[RangeeDuRoiBlanc - 1][ColonneDuRoiBlanc - 'A'].setEtat(false);
					/*rangeeDuTrou  = RangeeDuRoiBlanc + i;
					colonneDuTrou = ColonneDuRoiBlanc + j;*/

					// 1 - DÉPLACEMENT TEMPORAIRE DU ROI POUR VÉRIFIER SI IL Y A ÉCHEC.
					RangeeDuRoiBlanc = RangeeDuRoiBlanc + i;
					ColonneDuRoiBlanc = ColonneDuRoiBlanc + j;
					//echequier[RangeeDuRoiBlanc + i - 1][ColonneDuRoiBlanc + j - 'A'].setEtat(true);
					echequier[RangeeDuRoiBlanc - 1][ColonneDuRoiBlanc - 'A'].setEtat(true);

					//.......................//
					verifierSiEchec();
					//.......................//

					// 2 - SI IL Y A ÉCHEC, ON REMET LE ROI À SA CASE DE DÉPART
					//     ET ON ESSAYE AVEC UNE AUTRE CASE...
					if (Echec)
					{
						// REMETTRE LE ROI À "LA CASE DE DÉPART"
						echequier[RangeeDuRoiBlanc - 1][ColonneDuRoiBlanc - 'A'].setEtat(false);

						RangeeDuRoiBlanc = RangeeTempDuRoi;
						ColonneDuRoiBlanc = ColonneTempDuRoi;

						echequier[RangeeDuRoiBlanc - 1][ColonneDuRoiBlanc - 'A'].setEtat(true);

						roiEvade = false;
						return false;
					}
					else
					{
						// OK!!  IL N'Y A PAS D'ÉCHEC DANS LA NOUVELLE CASE...
						// IL FAUT DÉPLACER... OFFICIELLEMENT LE ROI
						/*Piece * RoiPtr = echequier[RangeeTempDuRoi - 1][ColonneTempDuRoi - 'A'].getPiecePtr();
						echequier[RangeeDuRoiBlanc - 1][ColonneDuRoiBlanc - 'A'].setPiecePtr(RoiPtr);*/

						// REMETTRE LE ROI À "LA CASE DE DÉPART"
						echequier[RangeeDuRoiBlanc - 1][ColonneDuRoiBlanc - 'A'].setEtat(false);

						RangeeDuRoiBlanc = RangeeTempDuRoi;
						ColonneDuRoiBlanc = ColonneTempDuRoi;

						echequier[RangeeDuRoiBlanc - 1][ColonneDuRoiBlanc - 'A'].setEtat(true);
						
						// ET....
						//return false;
						roiEvade = true;
						CaseDeLaPiece = { RangeeDuRoiBlanc, ColonneDuRoiBlanc };
						//unsigned char iConv = static_cast<unsigned char>(i);
						//unsigned char * iPtrConv = reinterpret_cast<unsigned char *>(&i);
						//CaseDArrivee.rangeeInterception  = RangeeDuRoiBlanc + *iPtrConv;
						CaseDArrivee.rangeeInterception = RangeeDuRoiBlanc + (unsigned char)(i);
						CaseDArrivee.colonneInterception = ColonneDuRoiBlanc + (char)j;
						//CaseDArrivee = { RangeeDuRoiBlanc + *iPtrConv, ColonneDuRoiBlanc/* + static_cast<char>(j)*/ };
						//CaseDArrivee = { RangeeDuRoiBlanc + (unsigned char)(i), ColonneDuRoiBlanc + (char)j };

						return true;
					}
				}
			}
		}
	}

	return false;
}

bool TrouverCaseMagique(unsigned char rangee, char colonne)
{
	bool CaseTrouvee = false,
		 CaseCandidateTrouvee = false,
		 DeplacementPermis = false;
	Case CaseAuCentre = echequier[rangee - 1][colonne - 'A'];

	//unsigned couleurDeLaCase = echequier[rangee - 1][colonne - 'A'].getPiecePtr()->getCouleur();
	unsigned couleurDeLaCase = CaseAuCentre.getPiecePtr()->getCouleur();

	for (int indexRangee = 0; indexRangee < NB_LIGNE; indexRangee++)
	{
		for (int indexColonne = 0; indexColonne < NB_COL; indexColonne++)
		{
			Case CaseXY( echequier[indexRangee][indexColonne] );

			if (!CaseXY.getEtat())
			{
				//deplacerPiece(rangee, colonne, indexRangee + 1, indexColonne + 'A');
				CaseCandidateTrouvee = true;
			}
			else
			{
				if (CaseXY.getPiecePtr()->getCouleur() != couleurDeLaCase)
				{
					//deplacerPiece(rangee, colonne, indexRangee + 1, indexColonne + 'A');
					CaseCandidateTrouvee = true;
				}
			}
			
			//...
			if (CaseCandidateTrouvee)
			{
				//DeplacementPermis = CaseXY.getPiecePtr()->validerDeplacement(rangee, colonne, indexRangee + 1, indexColonne + 'A');
				DeplacementPermis = CaseAuCentre.getPiecePtr()->validerDeplacement(rangee, colonne, indexRangee + 1, indexColonne + 'A');
				
				if (DeplacementPermis)
				{
					unsigned char RangeeRoiTemp = 0;
					char ColonneRoiTemp = '@';

					/*********************************************************************/
					if ((rangee == RangeeDuRoi) && (colonne == ColonneDuRoi))
					{
						//............
						RangeeRoiTemp  = RangeeDuRoi;
						ColonneRoiTemp = ColonneDuRoi;
						//
						RangeeDuRoi  = indexRangee + 1;
						ColonneDuRoi = indexColonne + 'A';

						if (joueur == JOUEUR_BLANC)
						{
							RangeeDuRoiBlanc = RangeeDuRoi;
							ColonneDuRoiBlanc = ColonneDuRoi;
						}
						else
						{
							RangeeDuRoiNoir = RangeeDuRoi;
							ColonneDuRoiNoir = ColonneDuRoi;
						}
					}
					/*********************************************************************/

					// 1 - ...
					deplacerPiece(rangee, colonne, indexRangee + 1, indexColonne + 'A');

					// 2 - ...
					verifierSiEchec();

					// 3 - ...
					annulerDeplacement(rangee, colonne, indexRangee + 1, indexColonne + 'A');
					//
					//if ((rangee == RangeeRoiTemp) && (colonne == ColonneRoiTemp))
					if ((RangeeRoiTemp != 0) && (ColonneRoiTemp != '@'))
					{
						RangeeDuRoi  = RangeeRoiTemp;
						ColonneDuRoi = ColonneRoiTemp;

						if (joueur == JOUEUR_BLANC)
						{
							RangeeDuRoiBlanc = RangeeRoiTemp;
							ColonneDuRoiBlanc = ColonneRoiTemp;
						}
						else
						{
							RangeeDuRoiNoir = RangeeRoiTemp;
							ColonneDuRoiNoir = ColonneRoiTemp;
						}
					}

					// 4 - ...
					if (Echec)
					{
						//CaseTrouvee = false;
					}
					else
					{
						CaseTrouvee = true;
						return true;
					}
				}
			}
		}
	}

	return CaseTrouvee;
}
// Il y a 3 possibilités lorsque il y a Échec :
// 1 - Éliminer la ménace
// 2 - Bloquer la ménace
// 3 - DÉPLACER LE ROI
bool verifierSiEchecPerpetuel()
{
	//bool EchecPerpetuel = false;
	if (joueur == JOUEUR_BLANC)
	{
		RangeeDuRoi = RangeeDuRoiBlanc;
		ColonneDuRoi = ColonneDuRoiBlanc;
		RangeeDuRoiAdverse = RangeeDuRoiNoir;
		ColonneDuRoiAdverse = ColonneDuRoiNoir;

		couleurDuRoi = BLANC;
		couleurDuRoiAdverse = NOIR;
	}
	else
	{
		RangeeDuRoi = RangeeDuRoiNoir;
		ColonneDuRoi = ColonneDuRoiNoir;
		RangeeDuRoiAdverse = RangeeDuRoiBlanc;
		ColonneDuRoiAdverse = ColonneDuRoiBlanc;

		couleurDuRoi = NOIR;
		couleurDuRoiAdverse = BLANC;
	}

	// 1 - CHERCHER LA ___MENACE___
	for (int i = 0; i < NB_LIGNE; i++)
	{
		for (int j = 0; j < NB_COL; j++)
		{
			Case caseTemp(echequier[i][j]);

			// PIÈCE DE MÊME COULEUR QUE LE ROI...
			if( (caseTemp.getEtat()) && (caseTemp.getPiecePtr()->getCouleur() == couleurDuRoi) )
			//if( (caseTemp.getEtat()) && (caseTemp.getPiecePtr()->getCouleur() == couleurDuRoiAdverse) )
			{
				if (TrouverCaseMagique(i + 1, j + 'A'))
				{
					//unsigned char RangeeMagique = i + 1;
					//char ColonneMagique = j + 'A';

					return false;
				}
			}
		}
	}
	
	return true;
}

bool verifierSiEchecEtMat()
{
	Pat = false;
	EchecEtMat = false;

	// 1 - Vérifier si il y a Échec
	Echec = verifierSiEchec();

	// 2 - Vérifier si ÉCHEC PERPÉTUEL
	if (Echec)
	{
		if (verifierSiEchecPerpetuel())
		{
			EchecEtMat = true;
		}

		//return true;
	}
	else
	{
		Pat = verifierSiEchecPerpetuel();
	}

	//return false;
	return EchecEtMat;
}

#endif