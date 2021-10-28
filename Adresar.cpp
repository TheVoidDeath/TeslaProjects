#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::fstream;
using std::ios;
using std::vector;

static struct adresa
{
public:
	string naziv;
	string broj_telefona;
	string e_mail;
};

vector<adresa> citaj_fajl()
{
	vector<adresa> out;
	adresa temp_a;
	fstream f;
	f.open("adresar.txt", ios::in);
	string temp;
	int helper = 0;
	while (std::getline(f, temp))
	{
		if (helper == 0)
		{
			temp_a.naziv = temp;
			helper++;
		}
		else if (helper == 1)
		{
			temp_a.broj_telefona = temp;
			helper++;
		}
		else if (helper == 2)
		{
			temp_a.e_mail = temp;
			helper = 0;
			out.push_back(temp_a);
		}
	}
	f.close();
	return out;
}

void pisi_fajl(vector<adresa> a)
{
	fstream f;
	f.open("adresar.txt", ios::out);
	for (int i = 0; i < a.size(); i++)
	{
		f << a[i].naziv;
		f << endl;
		f << a[i].broj_telefona;
		f << endl;
		f << a[i].e_mail;
		f << endl;
	}
	f.close();
}

void unos_adr(vector<adresa> a)
{
	adresa temp_a;
	system("cls");
	cout << "Unesite naziv kontakta koji dodajemo:" << endl;
	cin >> temp_a.naziv;
	system("cls");
	cout << "Unesite njegov broj:" << endl;
	cin >> temp_a.broj_telefona;
	system("cls");
	cout << "Unesite njegov mail" << endl;
	cin >> temp_a.e_mail;
	if (temp_a.e_mail.empty())
	{
		temp_a.e_mail = "     ";
	}
	a.push_back(temp_a);
	pisi_fajl(a);
	system("cls");
	cout << "Uspesno ste uneli kontakt u svoj adresar" << endl;
	system("pause");
	system("cls");

}

void pregled_adr(vector<adresa> &a_pregled)
{
	system("cls");
	vector<adresa> temp = a_pregled;
	// Sto je manja od deset vise razmaka dodati
	for (int i = 0; i < temp.size(); i++)
	{
		string help_n, help_b, help_e;
		int dif_n= (15-temp[i].naziv.size() ), dif_b= (15 - temp[i].broj_telefona.size()), dif_e= (15 - temp[i].e_mail.size());
		for (int j_1 = 0; j_1 < dif_n; j_1++)
		{
			help_n += " ";
		}
		for (int j_2 = 0; j_2 < dif_b; j_2++)
		{
			help_b += " ";
		}
		for (int j_3 = 0; j_3 < dif_e; j_3++)
		{
			help_e += " ";
		}
		cout << temp[i].naziv << help_n << temp[i].broj_telefona << help_b << temp[i].e_mail << endl;
	}
	system("pause");
	system("cls");
}

int main()
{
	vector<adresa> adr;
	while (1)
	{
		cout << "Ukoliko zelite da unesete broj u adresar unesite 1" << endl
			<< "Ukoliko zelite da pregledate svoj adresar unesite 2" << endl
			<< "Ukoliko zelite da izadjete iz aplikacije unesite 0" << endl;

		adr = citaj_fajl();
		int chooser;
		cin >> chooser;
		if (chooser == 1)
		{
			unos_adr(adr);
		}
		else if (chooser == 2)
		{
			pregled_adr(adr);
		}
		else if (!chooser)
		{
			return 1;
		}
	}

}