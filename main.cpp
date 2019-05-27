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
			if (react == false)
			{
				cout << "la reaction n'est pas valide continuer ? (non = 0, oui = 1)" << endl;
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
			else
				cout << "reaction valide";
			//pour linstant calcul lie a tte les autres molecules
			for (size_t i = 0; i < MaMol.size(); i++)
			{
				//if (MaMol[i].select) {
				chimie::Molecules &mol = MaMol[i];
				if (mol.typedonne.size() == 1)
				{
					/*
					// pas sur plutot faire un cout de tt
					//for() if typedonne ==" " then cout conc = ...
					if (mol.typedonne[0] != "conc")
						cout << "relancer le prog avec plus de donnees" << endl;
					else
						cout << "la concentration de " << mol.brut << " est : " << mol.conc << " mol.L-1" << endl;
						//test si ka ?
						k a la fin apres le plus de calcul possible
						*/
				}
				if (mol.typedonne.size() >= 2)
				{
					for (auto k : mol.typedonne)
					{
						for (auto j : mol.typedonne)
						{
							if (k == "n")
							{
								if (j == "M")
								{
									mol.masse = chimie::Formules::masse(mol.n, mol.mmol);
									mol.typedonne.insert("masse");
								}
								if (j == "vol")
								{
									mol.conc = chimie::Formules::concentration(mol.n, mol.vol);
									mol.typedonne.insert("conc");
								}
							}
							else if (k == "conc")
							{
								if (j == "vol")
								{
									mol.n = chimie::Formules::calnconc(mol.conc, mol.vol);
									mol.typedonne.insert("n");
								}
								else if (j == "n")
								{
									mol.vol = chimie::Formules::volconc(mol.conc, mol.n);
									mol.typedonne.insert("vol");
								}
							}
						}
					}
					if (mol.typedonne.size() >= 3)
					{
					}
					//else
					//cout << "manque de donnes relancer le programme" << endl;
				}
			}
		}
		else if (application == "Ca")
		{
			MaMol = chimie::Molecules::demandeMol();
			MaMol = chimie::Molecules::enterdata(MaMol);
			MaMol = chimie::Molecules::cv(MaMol);
		}
		for (size_t i = 0; i < MaMol.size(); i++)
		{
			chimie::Molecules &mol = MaMol[i];
			for (auto k : mol.typedonne)
			{
				if (k == "vol")
					cout << "Molecule : " << mol.brut << " a un volume de : " << mol.vol << " L" << endl;
				else if (k == "masse")
					cout << "Molecule : " << mol.brut << " a une masse de : " << mol.masse << " g" << endl;
				else if (k == "n")
					cout << "Molecule : " << mol.brut << " a un nb de mol de : " << mol.n << " mol" << endl;
				else if (k == "conc")
					cout << "Molecule : " << mol.brut << " a une concentration de : " << mol.conc << " mol.L-1" << endl;
				else if (k == "M")
					cout << "Molecule : " << mol.brut << " a une masse molaire de : " << mol.mmol << " g.mol-1" << endl;
			}
		}
		cout << "recommncer ? (non = 0, oui = 1)" << endl;
		cin >> Continue;
	}
	return 0;
}