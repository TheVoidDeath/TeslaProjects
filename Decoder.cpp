#include <iostream>
#include <fstream>
#include <string>
//Unos cesto koriscenih funkcija u korisnu biblioteku;
using std::endl;
using std::cin;
using std::cout;
using std::ios;
using std::string;

class sifra
{
private:
	int password;
public:
	sifra(string u)
	{
		int int_pass = 0; // Sifra ce biti zbir ascii kodova inicijalne reci koja je koriscena kao sifra
		for (int z = 0; z < u.length(); z++) // For koji dodaje ascii kodove da bi dobili finalnu sifru
		{
			int_pass += (int)u.at(z);
		}
		password = int_pass;
	};
	int get_sifru()
	{
		return password;
	};
};


int code(string fname, int password)
{
	//Otvaranje oba fajla, spremanje za dalje algoritme
	std::fstream reader, writer;
	string full_filename = fname + ".txt";
	reader.open(full_filename, ios::in);
	writer.open("coded_file.txt", ios::out);
	if (reader.is_open() == 0 || writer.is_open() == 0)
	{
		return 0;
	}
	char* file_data;//deklarisanje promenjive za ceo tekstualni podatak


	reader.seekg(0, ios::end);     // Dolazi na kraj fajla
	int file_lenght = reader.tellg();  // Javlja tu lokaciju kao kraj ili ti duzinu
	reader.seekg(0, ios::beg);     // Vraca se na pocetak fajla

	file_data = (char*)malloc(file_lenght * sizeof(char));


	reader.read(file_data, file_lenght);//Cita fajl
	reader.close();                //Zatvara fajl
	string file_data_string(file_data, file_lenght); //Prebacuje char odozgo u string
	free(file_data); // Oslobadja memoriju gornje promenjive word
	// Kraj "shared dela"

	for (int i=0;i<file_lenght;i++)
	{
		unsigned temp = (unsigned)file_data_string.at(i); // Uzima kod slova koje se trenutno obradjuje
		int temp_pass = password; // Postavlja privremenu promenjivu za sifru da bi mogli da menjamo vrednost bez gubljenja originalne vrednosti . Default vrednost ove promenjive je na pocetku ista kao i normalna
		while (temp_pass > 0) // Vrti petlju sve dok se sifra ne smanji na 0
		{
			//Ovaj deo koda  je objasnjen u decode sekciji (ovde je samo obrnut)
			if ( (temp + 1) < 127)
			{
				temp += 1;
				temp_pass -= 1;
			}
			else
			{
				temp = 1;
				temp_pass -= 1;
			}
		}
		file_data_string.at(i) = (char)temp;
	}

	writer << file_data_string;
	writer.close();
	cout << "Datoteka je uspesno iskodirana" << endl;
	return 1;
}

int decode(int password)
{
	//Otvaranje oba fajla, spremanje za dalje algoritme
	std::fstream reader, writer;


	reader.open("coded_file.txt", ios::in);
	writer.open("decoded_file.txt", ios::out);


	if (reader.is_open() == 0 || writer.is_open() == 0)
	{
		return 0; //Vraca 0 za nas upit koji se nalazi dole
	}
	char* file_data;//deklarisanje promenjive za ceo tekstualni podatak


	reader.seekg(0, ios::end);     // Dolazi na kraj fajla
	int file_lenght = reader.tellg();  // Javlja tu lokaciju kao kraj ili ti duzinu
	reader.seekg(0, ios::beg);     // Vraca se na pocetak fajla

	file_data = (char*)malloc(file_lenght * sizeof(char));


	reader.read(file_data, file_lenght);//Cita fajl
	reader.close();                //Zatvara fajl
	string file_data_string(file_data, file_lenght); //Prebacuje char odozgo u string
	free(file_data); // Oslobadja memoriju gornje promenjive word
	// Kraj "shared dela"

	for (int i = 0; i < file_lenght; i++)
	{
		unsigned temp = (unsigned)file_data_string.at(i); // Uzima kod slova koje se trenutno obradjuje
		int temp_pass = 0; // Postavlja privremenu promenjivu za sifru da bi mogli da menjamo vrednost bez gubljenja originalne vrednosti . Default vrednost ove promenjive je obrnuta normalnoj
		while (temp_pass <password) // Vrti petlju sve dok se ne izjednace 2 sifre
		{
			if ( (temp - 1) > 0) // Ukoliko je vrednost ascii koda unutar normalnog opsega pali se ovaj if
			{
				temp -= 1; // Smanjuje ascii vrednost za 1
				temp_pass += 1; // priblizava vrednost sifre za 1 originalu
			}
			else
			{
				temp = 126; // Ukoliko vrednost tezi tome da izadje iz opsega vraca ga u obrntu stranu da bi vrteli kao petlju
				temp_pass += 1; //idalje se vrednost priblizava
			}
		}
		file_data_string.at(i) = (char)temp; // Menja originalni karakter sa nasim kodovanim
	}

	writer << file_data_string; // Upisuje kodovani string u datoteku
	writer.close(); //zatvara datoteku
	cout << "Datoteka je uspesno dekodirana" << endl; // Javlja korisniku da je uspesno izvrsena radnja
	return 1;
}

//Opseg asci karakrera je od 33 do 255 (ukljucujuci njih)
int main()
{

	//NAPOMENE ZA KORISNIIKA
	cout << "NAPOMENA" << endl << "Za koriscenje programa je neophodna prethodno pripremljena datoteka" << endl << endl;
	cout << "Unesite sifru koji zelite da koristite za operaciju programa (bez razmaka)" << endl;



	string pass;//INICIJALIZOVANJE SIFRE KOJU KORISTIMO ZA KODOVANJE
	cin >> pass; // UNOS GORESPOMENUTE



	sifra s(pass);

	while (1)// Petlja koja se vrti dok ne resimo ono sto zelimo da radimo
	{
		//NAPOMENE ZA KORISNIKE
		cout << "Ukoliko zelite da sifrujete datoteku unesite code razmak ime vase datoteke" << endl
			<< "Ukoliko zelite da desifrujete datoteku unesite decode" << endl
			<< "Ime vase datoteke ne sme imati razmake!!!" << endl; 
		string mode, filename; //deklarisanje promenjivi osnove
		cin >> mode ; //unos moda (za decode cemo pretpostaviti da je koriscena nasa aplikacija da ne bi zamarali korisnika sa redundantnim informacijama)

		if (mode.compare("code") == 0)
		{
			cin >> filename; // Ukoliko se datoteka kodira neophodno nam je da znamo i ime fajla
			if (code(filename, s.get_sifru() )) // Provera uspeha operacije
				return 1;
			else
			{
				cout << "Doslo je do problema molimo vas obratite se ovlascenom tehnicaru" << endl; // Ukoliko je doslo do greske javlja korisniku da vrati aplikacijiju
				return 0;
			}
		}
		else if (mode.compare("decode") == 0)
		{
			if (decode(s.get_sifru() )) // Isti sistem kao za code
				return 1;
			else
			{
				cout << "Doslo je do problema molimo vas obratite se ovlascenom tehnicaru" << endl;
				return 0;
			}
		}
		else
		{
			cout << endl << "Rezim nije tacno unesen molim vas probajte ponovo" << endl;
		}
	}
}
