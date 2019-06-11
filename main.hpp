#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <unordered_set>
#include <algorithm>
//using std::cout;
//using std::string;
//using std::endl;
//using std::vector;
//using std::map;
//using std::unordered_set;
using namespace std;

namespace chimie
{
struct atom
{
	string name;
	double mmol;  // masse molaire
	double elec;  // electronegativite
	int liaisons; // nb liaisons de la mol
};

const vector<atom> ATOM = {
	//1 carac
	{"H", 1, 2.2, 1},
	{"C", 12, 2.55, 4},
	{"N", 14, 3.04, 3},
	{"O", 16, 3.44, 2},
	{"K", 39, 0.82, 1},
	{"B", 10.8, 2.04, 3},
	{"F", 19.9, 3.98, 1},
	//2 carac
	{"Li", 6.94, 0.98, 1},
	{"Be", 9, 1.57, 2},
	{"Na", 23, 0.93, 1},
	{"Mg", 24.3, 1.31, 2},
	{"Ca", 40, 1, 2},
	{"Cr", 52, 1.66, 2},
	{"Cl", 35.5, 3.16, 1},
	{"Fe", 56, 1.83, 2},
	{"Ag", 108, 1.93, 1},
	{"Al", 26.9, 1.61, 3},
};

class Molecules
{
public:
	static Molecules set(string a, char b, int c, int d, vector<string> e, int f, int g); // set les bases de chaque molecules
	static double masseMolaire(Molecules &mol);											  //calcul la masse molair de chaque molecule a partir des cstes ATOM
	static vector<Molecules> demandeMol();												  // demande le nb de molecules et utilise set pour les mettre
	static vector<Molecules> enterdata(vector<Molecules> &MaMol);						  //entre chaque donnees de lutilisateur
	static vector<Molecules> cv(vector<Molecules> &MaMol);								  // calcul la conc dun melange
	static bool reaction(vector<Molecules> &MaMol);										  // verifi que la reaction est valide
	static bool exist(Molecules &mol);													  // calcul si la molecule existe
	static vector<Molecules> tabadvance(vector<Molecules> &MaMol);						  //fait les calculs de tableau d'avancement

	string brut; //H2O1
	//char positif; //'-' '+' '~'
	//int nbpositif;
	map<char, int> positivite;
	int nbmol; // nb de mol
	//calcul mol

	double mmol;				// masse molaire de la molecules
	double vol;					//l ou ml a voir
	double n;					// nb de mol
	double conc;				//concentration mol.l-1
	unordered_set<string> type; //react prod null + melange liquide etc

	double masse; //g
	double ks;
	int donne;						 //nb de donne sur ctte molecule typedonnee.size
	unordered_set<string> typedonne; //  type de donee genre typedonne[0] est le type de la donnee 0
									 // concentration becher (c0 * v0) /vtot
									 // ca * va = cb * vb (/ coef stoechi)
									 // n  vol masse conc M

	// faire en sorte que ca garde le nb de atom etc
	map<string, int> atom; // nb atomes et leurs noms
						   // atom["C"] = 2

	map<string, int> liaison; //"double", "triple"
};
class Formules
{
public:
	static double calnmasse(double masse, double mmol);			// avoir n avec masse
	static double calnconc(double conc, double vol);			// avoir n avec conc
	static double calcnxmax(double xmax, int nbmol);			// avoir n avec xmax
	static double concentration(double n, double vol);			// avoir concentration avec n et vol
	static double masse(double n, double mmol);					// avoir la masse avec n et mmol
	static double volconc(double conc, double n);				// avoir le volume avec n et conc
	static double conccv(double conc, double vol, double vtot); // avoir la concentration avec c0v0 /vtot
	static double xmax(double n, int nbmol);					// calcul le xmax de la molecule
};

Molecules Molecules::set(string a, char b, int c, int d, vector<string> e, int f, int g)
{
	Molecules mol = {};
	mol.brut = a;
	mol.positivite[b] = c;
	mol.nbmol = d;
	for (size_t i = 0; i < e.size(); i++)
		mol.type.insert(e[i]);
	mol.liaison["double"] = f;
	mol.liaison["triple"] = g;
	return mol;
}

double Molecules::masseMolaire(Molecules &mol)
{
	//trouve chaque atom et le nb stoechio pour savoir quels atom constitu la molecules
	mol.mmol = 0;
	for (size_t i = 1; i < mol.brut.size(); i++)
	{
		if (mol.brut[i] >= '0' && mol.brut[i] <= '9')
		{
			int u = i;
			double somme = 0;
			int compt = -1;
			for (int m = u; mol.brut[m] >= '0' && mol.brut[m] <= '9'; m++)
			{
				compt++;
			}
			for (; mol.brut[u] >= '0' && mol.brut[u] <= '9'; u++)
			{
				double c = mol.brut[u] - (double)48;
				somme = somme + c * pow(10, compt);
				compt--;
			}
			for (int k = i; (mol.brut[k] >= '0' && mol.brut[k] <= '9') || k == 1; k--)
			{
				// si l'atom n'a que un charactere
				if (mol.brut[k - 1] == toupper(mol.brut[k - 1]) && mol.brut[k] == toupper(mol.brut[k]))
				{
					char c = mol.brut[k - 1];
					for (size_t j = 0; j < ATOM.size(); j++)
					{
						if (string(1, c) == ATOM[j].name)
						{
							mol.mmol += ATOM[j].mmol * somme;
							if (mol.atom.find(string(1, c)) != mol.atom.end())
								mol.atom[string(1, c)] += somme;
							else
								mol.atom[string(1, c)] = somme;
						}
					}
				}
				// si l'atom en a 2
				else
				{
					string c = string(1, mol.brut[k - 2]) + string(1, mol.brut[k - 1]);
					for (size_t j = 0; j < ATOM.size(); j++)
					{
						if (c == ATOM[j].name)
						{
							mol.mmol += ATOM[j].mmol * somme;
							if (mol.atom.find(c) != mol.atom.end())
								mol.atom[c] += somme;
							else
								mol.atom[c] = somme;
						}
					}
					k--;
				}
			}
			i = u;
		}
	}
	return mol.mmol;
}
vector<Molecules> Molecules::demandeMol()
{
	vector<Molecules> mol = {};
	int nbM = 0;
	while (nbM == 0)
	{
		cout << "nombre de molecules a faire ?";
		cin >> nbM;
	}
	for (int i = 0; i < nbM; i++)
	{
		string name = "C1";
		char pos = '~';
		int nbpos = 1;
		int nbmol = 1;
		int nbdouble = 1;
		int nbtriple = 1;
		bool exist = false;
		while (exist == false)
		{
			cout << "brut, positif, nb positif, nb molecules (dans la reaction), nb double liaisons, nb triple liaisons" << endl;
			cin >> name >> pos >> nbpos >> nbmol >> nbdouble >> nbtriple;
			cout << "nb de type sur la molecule" << endl;
			size_t nbtype;
			cin >> nbtype;
			if (nbtype == 0)
				nbtype = 1;
			vector<string> type = {};
			for (int k = 0; type.size() != nbtype; k++)
			{
				if (k == 0)
					cout << "type: (react, prod, null(pas important)" << endl;
				else
					cout << "type: (melange, gazeux, liquide, solide, dissolution, )" << endl;
				string types; // pour enregistreer la donne et la stocker
				cin >> types;
				type.push_back(types);
			}
			mol.push_back(Molecules::set(name, pos, nbpos, nbmol, type, nbdouble, nbtriple));
			mol[i].mmol = Molecules::masseMolaire(mol[i]);
			exist = Molecules::exist(mol[i]);
			if (!exist)
			{
				cout << "la molecule nexiste pas mais continuer ? (non=0,oui=1)" << endl;
				cin >> exist;
				if (!exist)
					mol.erase(mol.begin() + i);
			}
		}

		mol[i].typedonne.insert("M");
	}
	return mol;
}
vector<Molecules> Molecules::enterdata(vector<Molecules> &MaMol)
{
	for (size_t i = 0; i < MaMol.size(); i++)
	{
		cout << "nb de donnees sur cette molecule ? " << MaMol[i].brut;
		if (MaMol[i].type.count("react"))
			cout << " react" << endl;
		else if (MaMol[i].type.count("prod"))
			cout << " prod" << endl;
		else if (MaMol[i].type.count("null"))
			cout << " null" << endl;
		cin >> MaMol[i].donne;
		for (int k = 0; k < MaMol[i].donne; k++)
		{
			cout << "molecule : " << MaMol[i].brut << " inserer donne : " << k;
			cout << " volume, mol, masse, k, concentration" << endl;
			string type; // type de donnes a add sur la molecule
			cin >> type;
			if (type == "volume")
			{
				cout << "inserer volume en L :" << endl;
				cin >> MaMol[i].vol;
				MaMol[i].typedonne.insert("vol");
			}
			else if (type == "mol")
			{
				cout << "inserer nombre de mol en mol :" << endl;
				cin >> MaMol[i].n;
				MaMol[i].typedonne.insert("n");
			}
			else if (type == "masse")
			{
				cout << "inserer masse en g :" << endl;
				cin >> MaMol[i].masse;
				MaMol[i].typedonne.insert("masse");
			}
			else if (type == "concentration")
			{
				cout << "inserer concentratrion en mol/L :" << endl;
				cin >> MaMol[i].conc;
				MaMol[i].typedonne.insert("conc");
			}
			else if (type == "ks")
			{
				cout << "inserer k sans unite :" << endl;
				cin >> MaMol[i].ks;
				MaMol[i].typedonne.insert("ks");
			}
		}
	}
	return MaMol;
}
vector<Molecules> Molecules::cv(vector<Molecules> &MaMol)
{
	double vtot = 0; // initialise vtot (v0 +v1 +v2 +..)
	for (size_t i = 0; i < MaMol.size(); i++)
		// fait la somme des volumes
		if (MaMol[i].type.count("melange") && MaMol[i].typedonne.count("vol"))
			vtot += MaMol[i].vol;
	for (size_t i = 0; i < MaMol.size(); i++)
		if (MaMol[i].type.count("melange"))
			if (MaMol[i].typedonne.count("conc") && MaMol[i].typedonne.count("vol"))
				MaMol[i].conc = Formules::conccv(MaMol[i].conc, MaMol[i].vol, vtot);
	return MaMol;
}
bool Molecules::reaction(vector<Molecules> &MaMol)
{
	//faire avec les - et + aussi pas seulement les atomes
	map<string, vector<Molecules>> reaction;
	reaction["reactif"] = {};
	reaction["produit"] = {};
	for (size_t i = 0; i < MaMol.size(); i++)
	{
		if (MaMol[i].type.count("react"))
			reaction["reactif"].push_back(MaMol[i]);
		else if (MaMol[i].type.count("prod"))
			reaction["produit"].push_back(MaMol[i]);
	}
	map<string, double> satom_rea; // tt les atomes dans tt les reactifs et leurs somme
	map<string, double> satom_pro; // tt les atomes dans tt les produits et leurs somme
	for (map<string, vector<Molecules>>::iterator it = reaction.begin(); it != reaction.end(); it++)
	{
		vector<Molecules> itmol = it->second;
		for (size_t i = 0; i < itmol.size(); i++)
		{
			for (map<string, int>::iterator itsatom = itmol[i].atom.begin(); itsatom != itmol[i].atom.end(); itsatom++)
			{
				//faire que si cest un"C" ca aille dans satom["C"] etc
				if (itmol[i].type.count("react"))						   // verif si cest un reactif pour le mettre dans les reactifs
					if (satom_rea.find(itsatom->first) != satom_rea.end()) // si on a deja un nb de cet atom
						satom_rea[itsatom->first] += itsatom->second;
					else
						satom_rea[itsatom->first] = itsatom->second;
				else if (itmol[i].type.count("prod"))					   // verif que cest un produit pour le mettre dans les produit
					if (satom_pro.find(itsatom->first) != satom_pro.end()) // si on a deja un nb de cet atom
						satom_pro[itsatom->first] += itsatom->second;
					else
						satom_pro[itsatom->first] = itsatom->second;
			}
		}
	}
	for (map<string, double>::iterator it = satom_rea.begin(); it != satom_rea.end(); it++)
	{
		if (satom_pro.find(it->first) != satom_pro.end())
		{
			if (it->second != satom_pro[it->first])
				return false;
			else
				continue;
		}
		else
			return false;
	}
	return true;
}

bool Molecules::exist(Molecules &mol)
{
	double liaison = 0; // somme liaison possible
	int atoms = 0;		// somme tout les atomes
	for (map<string, int>::iterator it = mol.atom.begin(); it != mol.atom.end(); it++)
	{
		atoms += it->second;
		for (int i = 0; i < ATOM.size(); i++)
			if (it->first == ATOM[i].name)
				liaison += ATOM[i].liaisons * it->second;
	}
	atoms = atoms + mol.liaison["double"] + (2 * mol.liaison["triple"]) - 1;
	if (ceil(liaison / 2) == atoms)
		return true;
	else
		return false;
}

vector<Molecules> Molecules::tabadvance(vector<Molecules> &MaMol)
{
	map<string, vector<Molecules>> stockmol;
	for (size_t i = 0; i < MaMol.size(); i++)
	{
		if (MaMol[i].type.count("react"))
			stockmol["react"].push_back(MaMol[i]);
		else if (MaMol[i].type.count("prod"))
			stockmol["prod"].push_back(MaMol[i]);
	}
	for (map<string, vector<Molecules>>::iterator it = stockmol.begin(); it != stockmol.end(); it++)
	{
		cout << it->first << " de la reactions : " << endl;
		for (size_t i = 0; i < it->second.size(); i++)
			cout << i << " : " << it->second[i].brut << endl;
	}
	map<string, double> xmax;
	for (size_t i = 0; i < stockmol["react"].size(); i++)
		xmax[stockmol["react"][i].brut] = Formules::xmax(stockmol["react"][i].n, stockmol["react"][i].nbmol);
	//xmax molecule react MaMol[i] .brut
	double xmaxfinal;
	for (map<string, double>::iterator it1 = xmax.begin(); it1 != xmax.end(); it1++)
		for (map<string, double>::iterator it2 = xmax.begin(); it2 != xmax.end(); it2++)
			if (it1->second <= it2->second)
				xmaxfinal = it1->second;
			else
				xmaxfinal = it2->second;
	for (size_t i = 0; i < stockmol["prod"].size(); i++)
	{
		stockmol["prod"][i].n += Formules::calcnxmax(xmaxfinal, stockmol["prod"][i].nbmol);
		stockmol["prod"][i].typedonne.insert("n");
	}
	for (size_t i = 0; i < stockmol["prod"].size(); i++)
		for (size_t k = 0; k < MaMol.size(); k++)
			if (stockmol["prod"][i].brut == MaMol[k].brut && MaMol[k].type.count("prod"))
				MaMol[k] = stockmol["prod"][i];
	return MaMol;
}

double Formules::calnmasse(double masse, double mmol)
{
	return masse / mmol;
}
double Formules::calnconc(double conc, double vol)
{
	return conc * vol;
}
double Formules::calcnxmax(double xmax, int nbmol)
{
	return nbmol * xmax;
}
double Formules::concentration(double n, double vol)
{
	return n / vol;
}
double Formules::masse(double n, double mmol)
{
	return n * mmol;
}
double Formules::volconc(double conc, double n)
{
	return n / conc;
}
double Formules::conccv(double conc, double vol, double vtot)
{
	return (conc * vol) / vtot;
}
double Formules::xmax(double n, int nbmol)
{
	return n / nbmol;
}
}; // namespace chimie