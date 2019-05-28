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
		vector<chimie::Molecules> MaMol;
		//string application = "Cs";
		//if (application == "Cs")
		//{
		MaMol = chimie::Molecules::demandeMol();
		MaMol = chimie::Molecules::enterdata(MaMol);
		bool react = chimie::Molecules::reaction(MaMol);
		if (react == false)
		{
			cout << "la reaction n'est pas valide continuer ? (non = 0, oui = 1)" << endl;
			bool suite;
			cin >> suite;
			if (suite == false)
			{
				MaMol.clear();
				MaMol = chimie::Molecules::demandeMol();
				MaMol = chimie::Molecules::enterdata(MaMol);
			}
		}
		else
			cout << "reaction valide" << endl;
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
					if (mol.typedonne.count("n"))
					{
						if (mol.typedonne.count("M"))
						{
							mol.masse = chimie::Formules::masse(mol.n, mol.mmol);
							mol.typedonne.insert("masse");
						}
						if (mol.typedonne.count("vol"))
						{
							mol.conc = chimie::Formules::concentration(mol.n, mol.vol);
							mol.typedonne.insert("conc");
						}
					}
					if (mol.typedonne.count("masse"))
					{
						if (mol.typedonne.count("M"))
						{
							mol.n = chimie::Formules::calnmasse(mol.masse, mol.mmol);
							mol.typedonne.insert("n");
						}
					}
					if (mol.typedonne.count("conc"))
					{
						if (mol.typedonne.count("vol"))
						{
							mol.n = chimie::Formules::calnconc(mol.conc, mol.vol);
							mol.typedonne.insert("n");
						}
						else if (mol.typedonne.count("n"))
						{
							mol.vol = chimie::Formules::volconc(mol.conc, mol.n);
							mol.typedonne.insert("vol");
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
		MaMol = chimie::Molecules::cv(MaMol);
		for (size_t i = 0; i < MaMol.size(); i++)
		{
			chimie::Molecules &mol = MaMol[i];
			if (mol.typedonne.count("vol"))
				cout << "Molecule : " << mol.brut << " a un volume de : " << mol.vol << " L" << endl;
			if (mol.typedonne.count("masse"))
				cout << "Molecule : " << mol.brut << " a une masse de : " << mol.masse << " g" << endl;
			if (mol.typedonne.count("n"))
				cout << "Molecule : " << mol.brut << " a un nb de mol de : " << mol.n << " mol" << endl;
			if (mol.typedonne.count("conc"))
				cout << "Molecule : " << mol.brut << " a une concentration de : " << mol.conc << " mol.L-1" << endl;
			if (mol.typedonne.count("M"))
				cout << "Molecule : " << mol.brut << " a une masse molaire de : " << mol.mmol << " g.mol-1" << endl;
		}
		cout << "recommencer ? (non = 0, oui = 1)" << endl;
		cin >> Continue;
	}
	return 0;
}