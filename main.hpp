#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
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
	string type; //react prod null + melange liquide etc

	double masse; //g
	double ks;
	int donne;				  //nb de donne sur ctte molecule
	vector<string> typedonne; //  type de donee genre typedonne[0] est le type de la donnee 0
							  // concentration becher (c0 * v0) /vtot
							  // ca * va = cb * vb (/ coef stoechi)

	// faire en sorte que ca garde le nb de atom etc
	map<string, double> atom; // nb atomes et leurs noms
	// atom["C"] = 2

};

class Fomurles
{
public:
};

Molecules Molecules::set(string a, char b, int c, int d, string e)
{
	Molecules mol = {};
	mol.brut = a;
	mol.positif = b;
	mol.nbpositif = c;
	mol.nbmol = d;
	mol.type = e;
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
			for (u; mol.brut[u] >= '0' && mol.brut[u] <= '9'; u++)
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
						if (string(1, c) == ATOM[j].name) {
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
						if (c == ATOM[j].name) {
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
		cout << "brut, positif, nb positif, nb molecules (dans la reaction), type(reactif, produit, null(pas important)" << endl;
		cin >> name >> pos >> nbpos >> nbmol >> type;
		mol.push_back(chimie::Molecules::set(name, pos, nbpos, nbmol, type));
		mol[i].mmol = Molecules::masseMolaire(mol[i]);
		mol[i].typedonne.push_back("M");
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
			string type;
			cin >> type;
			if (type == "volume")
			{
				cout << "inserer volume en L :" << endl;
				cin >> MaMol[i].vol;
				MaMol[i].typedonne.push_back("vol");
			}
			else if (type == "mol")
			{
				cout << "inserer nombre de mol en mol :" << endl;
				cin >> MaMol[i].n;
				MaMol[i].typedonne.push_back("n");
			}
			else if (type == "masse")
			{
				cout << "inserer masse en g :" << endl;
				cin >> MaMol[i].masse;
				MaMol[i].typedonne.push_back("masse");
			}
			else if (type == "concentration")
			{
				cout << "inserer concentratrion en mol/L :" << endl;
				cin >> MaMol[i].conc;
				MaMol[i].typedonne.push_back("conc");
			}
			else if (type == "ks")
			{
				cout << "inserer k sans unite :" << endl;
				cin >> MaMol[i].ks;
				MaMol[i].typedonne.push_back("ks");
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
		for (size_t k = 0; k < MaMol[i].typedonne.size(); k++)
		{
			//calcul la concentration c1 c2 etc
			if (MaMol[i].typedonne[k] == "masse")
				MaMol[i].conc = (MaMol[i].masse / MaMol[i].vol) / MaMol[i].mmol;
			else if (MaMol[i].typedonne[k] == "n")
				MaMol[i].conc = MaMol[i].n / MaMol[i].vol;
		}
		MaMol[i].conc = (MaMol[i].conc * MaMol[i].vol) / vtot;
		MaMol[i].typedonne.push_back("conc");
	}
	return MaMol;
}
bool Molecules::reaction(vector<Molecules> &MaMol)
{
	vector<Molecules> reactif;
	vector<Molecules> produit;
	for(size_t i = 0; i < MaMol.size(); i++)
	{
		if (MaMol[i].type == "react")
			reactif.push_back(MaMol[i]);
		else if (MaMol[i].type == "prod")
			produit.push_back(MaMol[i]);		
	}
	map<string, double> satom_rea; // tt les atomes dans tt les reactifs et leurs somme
	map<string, double> satom_pro; // tt les atomes dans tt les produits et leurs somme
	for (size_t i = 0; i < reactif.size(); i++)
	{
		for (map<string, double>::iterator it = reactif[i].atom.begin(); it != reactif[i].atom.end(); it++)
		{
			//faire que si cest un"C" ca aille dans satom["C"] etc
			if (satom_rea.find(it->first) != satom_rea.end())
				satom_rea[it->first] += it->second;
			else
				satom_rea[it->first] = it->second;
		}		
	}
	for (size_t i = 0; i < produit.size(); i++)
	{
		for (map<string, double>::iterator it = produit[i].atom.begin(); it != produit[i].atom.end(); it++)
		{
			//faire que si cest un"C" ca aille dans satom["C"] etc
			if (satom_pro.find(it->first) != satom_pro.end())
				satom_pro[it->first] += it->second;
			else
				satom_pro[it->first] = it->second;
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
}; // namespace chimie