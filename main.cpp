#include "main.hpp"


int main()
{
	bool Continue = true;
	while ((bool)Continue)
	{
		// calcul masse molaire
	// calcul ka / pka
	// -> reactif ou produit
	// calcul concentration
	// -> volume gramme masse molaire mol 
		cout << "que voulez vous faire ? (M, Concentration : (sans c0*v0 'Cs', avec 'Ca' ))" << endl;
		string application;
		cin >> application;
		vector<chimie::Molecules> MaMol;
		if (application == "M")
		{
			MaMol = chimie::Molecules::demandeMol();
		}
		else if (application == "Cs")
		{
			MaMol = chimie::Molecules::demandeMol();
			MaMol = chimie::Molecules::enterdata(MaMol);
			bool react = chimie::Molecules::reaction(MaMol);
			if (react == false) {
				cout << "la reaction n'est pas valide continuer ? (0, 1)" << endl;
				bool suite;
				cin >> suite;
				if (suite)
					break;
				else
				{
					MaMol.clear();
					MaMol = chimie::Molecules::demandeMol();
					MaMol = chimie::Molecules::enterdata(MaMol);
				}
			}
			//pour linstant calcul lie a tte les autres molecules
			for (string::size_type i = 0; i < MaMol.size(); i++) {
				//if (MaMol[i].select) {
				chimie::Molecules& mol = MaMol[i];

				if (mol.donne == 1)
				{
					/*
					// pas sur plutot faire un cout de tt
					//for() if typedonne ==" " then cout conc = ...
					if (mol.typedonne[0] != "conc")
						cout << "relancer le prog avec plus de donnees" << endl;
					else
						cout << "la concentration de " << mol.brut << " est : " << mol.conc << " mol.L-1" << endl;
						//test si ka ?
						*/
					if (mol.typedonne[0] == "ks") {

					}
				}
				else if (mol.donne >= 2)
				{
					bool test = false;
					for (int k = 0; k < mol.donne; k++) {
						if (mol.typedonne[k] == "volume")
							test = true; break;
					}
					if (test)
					{
						for (int k = 0; k < mol.donne; k++) {
							if (mol.typedonne[k] == "n")
							{
								mol.conc = (mol.n / mol.vol);
								mol.typedonne.push_back("conc");
								//cout << "la concentration de " << mol.brut << " est : " << mol.conc << " mol.L-1" << endl;
							}
							else if (mol.typedonne[k] == "masse")
							{
								mol.n = mol.masse / mol.mmol;
								mol.conc = mol.n / mol.vol;
								mol.typedonne.push_back("conc");
								mol.typedonne.push_back("n");
								//cout << "la concentration de " << mol.brut << " est : " << mol.conc << " mol.L-1" << endl;
							}
						}
					}
					if (mol.donne >= 3)
					{

					}
					else
						cout << "manque de donnes relancer le programme" << endl;
				}
				//}
			}
		}
		else if (application == "Ca")
		{
			MaMol = chimie::Molecules::demandeMol();
			MaMol = chimie::Molecules::enterdata(MaMol);
			MaMol = chimie::Molecules::cv(MaMol);
		}
		for (string::size_type i = 0; i < MaMol.size(); i++) {
			chimie::Molecules& mol = MaMol[i];
			for (string::size_type k = 0; k < mol.typedonne.size(); k++) {
				if (mol.typedonne[k] == "vol")
					cout << "Molecule : " << mol.brut << " a un volume de : " << mol.vol << " L" << endl;
				else if (mol.typedonne[k] == "masse")
					cout << "Molecule : " << mol.brut << " a une masse de : " << mol.masse << " g" << endl;
				else if (mol.typedonne[k] == "n")
					cout << "Molecule : " << mol.brut << " a un nb de mol de : " << mol.n << " mol" << endl;
				else if (mol.typedonne[k] == "conc")
					cout << "Molecule : " << mol.brut << " a une concentration de : " << mol.conc << " mol.L-1" << endl;
				else if (mol.typedonne[k] == "M")
					cout << "Molecule : " << mol.brut << " a une masse molaire de : " << mol.mmol << " g.mol-1" << endl;
			}
		}
		cout << "recommncer ? (0, 1)" << endl;
		cin >> Continue;
	}
	return 0;
}