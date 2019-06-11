#include "main.hpp"

int main()
{
	ofstream file;
	file.open("historique.txt", ios::out | ios::app);

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
		bool react = chimie::Molecules::reaction(MaMol);
		while (react == false)
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
			else
				react = true;
		}
		MaMol = chimie::Molecules::enterdata(MaMol);
		//pour linstant calcul lie a tte les autres molecules
		for (size_t i = 0; i < MaMol.size(); i++)
		{
			chimie::Molecules &mol = MaMol[i];
			if (mol.typedonne.count("masse"))
				if (mol.typedonne.count("M"))
				{
					mol.n = chimie::Formules::calnmasse(mol.masse, mol.mmol);
					mol.typedonne.insert("n");
				}
			if (mol.typedonne.count("conc"))
				if (mol.typedonne.count("vol"))
				{
					mol.n = chimie::Formules::calnconc(mol.conc, mol.vol);
					mol.typedonne.insert("n");
				}
		}
		MaMol = chimie::Molecules::tabadvance(MaMol);
		for (size_t i = 0; i < MaMol.size(); i++)
		{
			//if (MaMol[i].select) {
			chimie::Molecules &mol = MaMol[i];
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
			// k ?
		}
		MaMol = chimie::Molecules::cv(MaMol);
		for (size_t i = 0; i < MaMol.size(); i++)
		{
			chimie::Molecules &mol = MaMol[i];
			file << mol.brut << ", " << mol.nbmol << ", " << mol.liaison["double"] << ", " << mol.liaison["triple"] << " :" << endl;
			if (mol.typedonne.count("vol"))
			{
				cout << "Molecule : " << mol.brut << " a un volume de : " << mol.vol << " L" << endl;
				file << "	vol : " << mol.vol << endl;
			}
			if (mol.typedonne.count("masse"))
			{
				cout << "Molecule : " << mol.brut << " a une masse de : " << mol.masse << " g" << endl;
				file << "	masse : " << mol.masse << endl;
			}
			if (mol.typedonne.count("n"))
			{
				cout << "Molecule : " << mol.brut << " a un nb de mol de : " << mol.n << " mol" << endl;
				file << "	nb mol : " << mol.n << endl;
			}
			if (mol.typedonne.count("conc"))
			{
				cout << "Molecule : " << mol.brut << " a une concentration de : " << mol.conc << " mol.L-1" << endl;
				file << "	concentration : " << mol.conc << endl;
			}
			if (mol.typedonne.count("M"))
			{
				cout << "Molecule : " << mol.brut << " a une masse molaire de : " << mol.mmol << " g.mol-1" << endl;
				file << "	M : " << mol.mmol << endl;
			}
		}
		cout << "recommencer ? (non = 0, oui = 1)" << endl;
		cin >> Continue;
	}
	file.close();
	return 0;
}