#include <iostream>
#include <string> //Stringovi cpp bibliotteka
#include <fstream> //Biblioteka za fajlove...duh
#include <stdlib.h>
#include<vector>
#include<cstdlib>
#include<time.h>//Potrebna nam je da bi dali seed random funkciji

//naredne 4 linije koristimo da uvezemo sve stvari koje cesto koristimo iz std biblioteke
using std::ios;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


class reci
{
private:
	vector<string> words; // Deklarisanje promenjive koja ce sadrzati sve nase reci
	int word_count;//Glavna promenjiva za broj reci
public:	
	
	reci() // Konstruktor nase klase zahteva tip citanja koji nam treba (2 out,1 in)
	{
		word_count = 0;
		std::fstream f;
		f.open("zadata_rec.txt", ios::in);
		if (!f)
		{
			cout<< "Nije moguce procitati datoteku";
		}
		string temp;
		while (std::getline(f, temp))
		{
			if (temp.size() > 0)
			{
				words.push_back(temp);
				word_count++;
			}
		}
		f.close();
	}
	string rand_Rec()
	{
		srand(time(NULL));
		int temp = rand() % word_count; //Daje nam random broj od 0 to vrednosti count
		return words[temp]; // Vraca rec na poziciji gornjeg inta
	}

	void add_Rec()
	{
		std::fstream f; //Deklarisanje fajla
		f.open("zadata_rec.txt", ios::out); //Otvaranje fajla u rezimu za ispis u njega
		f.clear();
		if (f.is_open()) //Provera da li je fajl uspesno otvoren
		{
			char temp[100]; // Deklarisanje privremene promenjive za cuvanje inputa igraca
			cin >> temp; // Input igraca
			words.push_back(temp); // Upisivanje u nas vektr
			word_count++;
			for (int i = 0; i < word_count; i++)
			{
				f << words[i] << "\n";
			}
		}
		else throw "Doslo je do error-a pri ucitavanju fajla";
		f.close();
		cout << "Uspesno je upisana rec u datoteku i mozete zapoceti igru" << endl; //Poruka o uspehu
	}
};


//Program koji kontrolise upis reci koje zelimo da se trazi u igri vesala
void upis()
{
	reci r_i;
	cout << "Unesite rec koju zelite da dodate u recnik koriscen za igru" << endl; //Instrukcije za igraca
	r_i.add_Rec();

}




int igra()
{
	reci baza_reci;
	string word_string = baza_reci.rand_Rec();//Pozivamo metodu klase da bi dobili random rec iz fajla

	if (word_string.length() == 0)//Ukoliko je duzina reci 0 znaci da rec nepostoji ili je datoteka pogresno iscitana
	{
		cout << "Doslo je do problema sa citanjem fajla/fajl nije napravljen" << endl; // Daje korisniku poruku o gresci
		return 0; //Vraca nas na pocetak programa
	}
	else
	{
		bool beaten = false; //Ucitavanje stanja igra (pobedjeno ili ne)

		string crtice_string = "";


		// KREIRANJE CHAR* ZA CRTICE
		for (int i = 0; i < word_string.length(); i++)
		{
			crtice_string = crtice_string + "-";

			//Konvertovanje stringa word u donji font	
			word_string.at(i) = tolower(word_string.at(i));
		}


		int lives_counter = 5; //UCITAVAMO POCETNI BROJ ZIVOTA
		while (!beaten && lives_counter > 0)
		{
			system("cls");

			bool matcher = false;//Deklarisanje promenjivo koju koristimo za proveru da li se zivot smanjuje

			char x; //Deklarisanje unetog slova
			cout << crtice_string << endl << "Unesite 1 slovo koje ce biti testirano u odnosu na rec" << " broj zivota je: " << lives_counter << endl; //Instrukcije za igraca i osnovne informacije		

			cin >> x;//Unos slova

			//Konvertovanje slova u donji font
			if (isupper(x))
			{
				x = tolower(x);
			}
			//Provera da li se char nalazi u stringu
			for (int i = 0; i < word_string.length(); i++)
			{
				//If koji pita pitanje odozgore
				if (word_string[i] == x)
				{
					crtice_string.at(i) = x; // Zamena crtice sa slovom
					matcher = true; //Ukoliko je zamenjeno stavljamo igri do znanja da netreba smanjiti zivot
				}
			}
			if (!matcher) //Ukoliko zastavica za smanjenje zivota nije postavljena smanjujemo zivote igracu
			{
				lives_counter -= 1;
			}
			if (word_string.compare(crtice_string) == 0)//Ukoliko je original rec ista reci koja je napravljena iz stringa crtice igra je pobedjena
			{
				beaten = true;
			}
		}

		if (beaten)//Ukoliko je pobedjena igra stavljamo to do znanja igracu
		{
			system("cls");
			cout << "SVAKA CAST POBEDILI STE" << endl
				<< "Vasa rec je bila: "
				<< word_string << endl;
		}
		if (lives_counter == 0)//Ako ste izgubili, desava se
		{
			cout << "Niste uspeli da pobedite" << endl;
		}

		return 1;
	}
}


void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}


//MAIN PROGRAM
int main()
{
	while (1)//Quazi beskonacna petlja koja ima ugradjen izlaz dole u kodu izlaz
	{
		clear();
		cout << "Ukoliko zelite da dodate rec u recnik igre pritisnite 1," << endl
			<<"ukoliko zelite da igrate unesite 2" << endl
			<< "I naravno posle toga lupite enter!!!" << endl
			<< "Ukoliko zelite da izadjete iskucajte EXIT" << endl; // Inicijalne instrukcije o izabiru rezima
			
		string x;
		fflush(stdin);
		cin >> x;
		if (x[0] == '1') //Provera pritiska tastera za rezim upisa
		{
			upis();
			fflush(stdin);
			system("pause");
			system("cls");
		}
		if (x[0] == '2') //Provera pritiska tastera za rezim igre
		{
			igra();
			fflush(stdin);
			system("pause");
			system("cls");
		}
		if (x.compare("EXIT") == 0)
		{
			return 0;
		}
	}
	return 1;
}
