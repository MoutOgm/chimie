#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <unordered_set>
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
	double mmol; // masse molaire
	double elec; // electronegativite
};

const vector<atom> ATOM = {
	//1 carac
	{"C", 12, 2.55},
	{"N", 14, 3.04},
	{"H", 1, 2.2},
	{"O", 16, 3.44},
	//2 carac
	{"Na", 23, 0.93},
	{"Mg", 24.3, 1.31},
	{"Ca", 40, 1},
	{"Cr", 52, 1.66},
	{"Cl", 35.5, 3.16},
	{"Fe", 56, 1.83},
	{"Ag", 108, 1.93},
};

class Molecules
{
public:
	static Molecules set(string a, char b, int c, int d, string e); // set les bases de chaque molecules
	static double masseMolaire(Molecules &mol);						//calcul la masse molair de chaque molecule a partir des cstes ATOM
	static vector<Molecules> demandeMol();							// demande le nb de molecules et utilise set pour les mettre
	static vector<Molecules> enterdata(vector<Molecules> &MaMol);   //entre chaque donnees de lutilisateur
	static vector<Molecules> cv(vector<Molecules> &MaMol);			// calcul la conc dun melange
	static bool reaction(vector<Molecules> &MaMol);

	string brut;  //H2O1
	char positif; //'-' '+' '~'
	int nbpositif;
	int nbmol;
	//calcul mol

	double mmol; // masse molaire de la molecules
	double vol;  //l ou ml a voir
	double n;	// nb de mol
	double conc; //concentration mol.l-1
	unordered_set<string> type; //react prod null + melange liquide etc

	double masse; //g
	double ks;
	int donne;				  //nb de donne sur ctte molecule typedonnee.size
	unordered_set<string> typedonne; //  type de donee genre typedonne[0] est le type de la donnee 0
							  // concentration becher (c0 * v0) /vtot
							  // ca * va = cb * vb (/ coef stoechi)
							  // n  vol masse conc M

	// faire en sorte que ca garde le nb de atom etc
	map<string, double> atom; // nb atomes et leurs noms
							  // atom["C"] = 2

};
class Formules
{
public:
	static double calnmasse(double masse, double mmol); // avoir n avec masse
	static double calnconc(double conc, double vol); 	// avoir n avec conc 
	static double concentration(double n, double vol);  // avoir concentration avec n et vol
	static double masse(double n, double mmol);		    // avoir la masse avec n et mmol
	static double volconc(double conc, double n); 		// avoir le volume avec n et conc
	static double conccv(double conc, double vol, double vtot); // avoir la concentration avec c0v0 /vtot
};



Molecules Molecules::set(string a, char b, int c, int d, string e)
{
	Molecules mol = {};
	mol.brut = a;
	mol.positif = b;
	mol.nbpositif = c;
	mol.nbmol = d;
	mol.type.insert(e);
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
	vector<Molecules> mol;
	int nbT = 0;
	while (nbT == 0)
	{
		cout << "nombre de molecules a faire ?";
		cin >> nbT;
	}
	for (int i = 0; i < nbT; i++)
	{
		string name = "H1Cl1";
		char pos = '~';
		int nbpos = 1;
		int nbmol = 1;
		string type;
		cout << "brut, positif, nb positif, nb molecules (dans la reaction), type(react, prod, null(pas important)" << endl;
		cin >> name >> pos >> nbpos >> nbmol >> type;
		if (type != "react" && type != "prod" && type != "null")
		{
			cout << "type(react, prod, null(pas important) ?" << endl;
			cin >> type;
		}
		mol.push_back(chimie::Molecules::set(name, pos, nbpos, nbmol, type));
		mol[i].mmol = Molecules::masseMolaire(mol[i]);
		mol[i].typedonne.insert("M");
	}
	return mol;
}
vector<Molecules> Molecules::enterdata(vector<Molecules> &MaMol)
{
	for (size_t i = 0; i < MaMol.size(); i++)
	{
		cout << "nb de donnees sur cette molecule ? " << MaMol[i].brut << endl;
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
	for (size_t i = 0; i < MaMol.size(); i++)
	{
		double vtot = 0; // initialise vtot (v0 +v1 +v2 +..)
		for (size_t k = 0; k < MaMol.size(); k++)
			// fait la somme des volumes
			vtot += MaMol[k].vol;
		for (auto k : MaMol[i].typedonne)
		{
			//calcul la concentration c1 c2 etc
			if (k == "masse")
			{
				MaMol[i].n = chimie::Formules::calnmasse(MaMol[i].masse, MaMol[i].mmol);
				MaMol[i].conc = chimie::Formules::concentration(MaMol[i].n, MaMol[i].vol);
				MaMol[i].typedonne.insert("n");
			}
				
			else if (k == "n")
				MaMol[i].conc = MaMol[i].n / MaMol[i].vol;
		}
		MaMol[i].conc = Formules::conccv(MaMol[i].conc, MaMol[i].vol, vtot);
		MaMol[i].typedonne.insert("conc");
	}
	return MaMol;
}
bool Molecules::reaction(vector<Molecules> &MaMol)
{
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
			for (map<string, double>::iterator itsatom = itmol[i].atom.begin(); itsatom != itmol[i].atom.end(); itsatom++)
			{
				//faire que si cest un"C" ca aille dans satom["C"] etc
				if (satom_rea.find(itsatom->first) != satom_rea.end())
					satom_rea[itsatom->first] += itsatom->second;
				else
					satom_rea[itsatom->first] = itsatom->second;
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


double Formules::calnmasse(double masse, double mmol)
{
	return masse / mmol;
}
double Formules::calnconc(double conc, double vol)
{
	return conc * vol;
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
}; // namespace chimie