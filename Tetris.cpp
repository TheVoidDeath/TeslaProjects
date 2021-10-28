#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;


//V0.3 Update Klasa
class scores
{
private:
	int high_score = 0;
public:
	int score = 0;
	scores()
	{
		score = 0;
		high_score = 0;
		fstream f;
		f.open("highscore.txt", ios::in);
		string high_s;
		getline(f, high_s);
		if (!high_s.empty())
		{
			high_score = stoi(high_s);
		}
		else
		{
			high_score = 0;
		}
		f.close();
	}
	bool check_hs()
	{
		if (score > high_score)
		{
			fstream f;
			high_score = score;
			remove("highscore.txt");
			f.open("highscore.txt", ios::out);
			f << to_string(high_score);
			f.close();
			return true;
		}
		else return false;
	}
	int print_hs()
	{
		return high_score;
	}
};
//Deklarisanje globalnih promenjivih

//TIpovi
wstring tipovi_blokova[7];//Ovo je promenjiva koju koristim za smestanje tipova blokova za igru. Kada sam smisljao nacin da resim rotiranje koje postoji u tetrisu nisam mogao da resim rotaciju sa 2d array-em pa sam se konsultovao sa drugom i dosli smo do sledeceg 1d resenja


//Boundry box za igru
int field_width = 12; // Zadajemo sirinu koju ce playable area imati (naravno -1 posto se mora isrtati i border)
int field_height = 18;
unsigned char* pField = nullptr; // Deklarisanje promenjive koju cu koristiti za igrivi deo (Kocka u kojoj ce se tetris igrati)

//Deklarisanje velicine konzolnog prozora u kome ce se crtati
int screen_width = 80;
int screen_height = 30;

void set_console() // Postavlja konzolu na odgovarajucu velicinu
{
	HWND console = GetConsoleWindow(); // Uzimamo konzolu
	RECT ConsoleRect; // Deklarisemo rectangle za konzolu
	GetWindowRect(console, &ConsoleRect); // Bindujemo gornje dve stvari
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 675, 400, TRUE); // Postavljamo velicinu konzole

}
//Funkcija za smestanje tipova blokova u jedan niz goredeklarisan
void pravljenje_tipova()
{
	// RAVAN
	tipovi_blokova[0].append(L"..X.");
	tipovi_blokova[0].append(L"..X.");
	tipovi_blokova[0].append(L"..X.");
	tipovi_blokova[0].append(L"..X.");

	//L
	tipovi_blokova[1].append(L"..X.");
	tipovi_blokova[1].append(L"..X.");
	tipovi_blokova[1].append(L".XX.");
	tipovi_blokova[1].append(L"....");

	//Obrnuto L
	tipovi_blokova[2].append(L".X..");
	tipovi_blokova[2].append(L".X..");
	tipovi_blokova[2].append(L".XX.");
	tipovi_blokova[2].append(L"....");

	// -|-
	tipovi_blokova[3].append(L"..X.");
	tipovi_blokova[3].append(L".XX.");
	tipovi_blokova[3].append(L"..X.");
	tipovi_blokova[3].append(L"....");

	//2x2
	tipovi_blokova[4].append(L".XX.");
	tipovi_blokova[4].append(L".XX.");
	tipovi_blokova[4].append(L"....");
	tipovi_blokova[4].append(L"....");

	//Skretnica 1
	tipovi_blokova[5].append(L".X..");
	tipovi_blokova[5].append(L".XX.");
	tipovi_blokova[5].append(L"..X.");
	tipovi_blokova[5].append(L"....");

	//Skretnica 2
	tipovi_blokova[6].append(L"..X.");
	tipovi_blokova[6].append(L".XX.");
	tipovi_blokova[6].append(L".X..");
	tipovi_blokova[6].append(L"....");
}

//Funkcija koja resava rotiranje nasih blokova
int rotiranje(int px, int py, int tip_rotacije)
{
	switch (tip_rotacije % 4)
	{
	case 0:
		return py * 4 + px;//Ne rotira
	case 1:
		return 12 + py - (px * 4); //rotacija 90
	case 2:
		return 15 - (py * 4) - px; //rotacija 180
	case 3:
		return 3 - py + (px * 4); //rotacija 270
	}
	return 0;
}

bool uklapa_li_se(int tip_bloka, int rotacija_bloka, int pos_X, int pos_Y) // U sustini logika za kolizion detektion
{
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			int p_index = rotiranje(px, py, rotacija_bloka); // dobijamo odgovarajucu rotaciju u kojoj je blok da bi ispravno testirali

			int field_index = (pos_Y + py) * field_width + (pos_X + px); //Dobijamo index(na ekranu) specificnog "pixela" bloka koji testiramo

			if (pos_X + px >= 0 && pos_X + px < field_width)
			{
				if (pos_Y + py >= 0 && pos_Y + py < field_height)
				{
					if (tipovi_blokova[tip_bloka][p_index] != L'.' && pField[field_index] != 0)
					{
						return false;//Pada
					}
				}
			}
		}
	}

	return true;//Ako je prosao kroz sve provere javljamo da moze da nastavi sa akcijom koja je sledeca
}

int main()
{
	set_console();

	pravljenje_tipova();

	pField = new unsigned char[field_width * field_height]; // Alociranje memorije playable field pokazivaca
	for (int i = 0; i < field_width; i++)
	{
		for (int j = 0; j < field_height; j++)
		{
			pField[j * field_width + i] = (i == 0 || i == (field_width - 1) || j == (field_height - 1)) ? 9 : 0; // Ukoliko smo na ivicama igrivog terena zelimo da oznacimo to drugacije nego ako, pa nismo da bih mogli lakse da iscrtamo border
		}
	}

	wchar_t* screen = new wchar_t[screen_width*screen_height]; // Deklarisemo bufer za nasu konzolu, i alociramo memorijski prostor velicine naseg ekrana (u kome cemo crtati)
	for (int i = 0; i < screen_width * screen_height; i++)//Prolazimo kroz promenjivu i stavljamo karaktere na mesto gde treba da stoje
	{
		screen[i] = L' ';
	}
	                                  //Nacini na koje cemo koristiti konzolu , Da li zelimo da je podelimo , Security opcije konzole,Rezim u kojem ce da radi, i za kraj podatci kojima je punimo
	HANDLE hconsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);// Pravimo novi buffer za konzolu sa parametrima koji su objasnjeni u liniji 104 (linija iznad)
	SetConsoleActiveScreenBuffer(hconsole); // Postavljamo bafer koji smo mi napravili za aktivan bafer koji koristi konzola
	DWORD dwBytesWritten = 0;


	bool game_over = false;//deklarisemo bool za gamestate (pobedjen:nepobedjen)

	//Deklarisemo inicijalne podatke igre
	
	int current_piece = 2;
	int current_rotation = 0;
	int current_location_X = field_width / 2;
	int current_location_Y = 0;

	//Tastatura
	bool dugmad[4];

	// Promenjiva za zaustavljanje rotacije na 1 tik (spiner efekat)
	bool stop_rotate = false;

	//Promenjive za jos logike
	int speed = 20;
	int speed_counter = 0;
	bool spustaj = false;
	int broj_odigranih = 0;

	vector<int> linije;

	//Stvari vezane za score
	scores s;


	while (!game_over) // vrtimo petlju sve dok se ne zavrsi igra
	{
		// Da ne bi tajmer igre zavisio od racunara na kome se pokrece neophodan nam je tajming kod:
		this_thread::sleep_for(50ms);
		speed_counter++;
		spustaj = (speed_counter == speed);
		//Unos korisnika (hvatanje tastera
		for (int k = 0; k < 4; k++)
		{
			dugmad[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0; // Proverava da li je neki od nasih tastera pritisnut i stavlja true ako jeste
		}

		//Logika za unos
		{
			current_location_X += (dugmad[0] && uklapa_li_se(current_piece, current_rotation, current_location_X + 1, current_location_Y)) ? 1 : 0;
			current_location_X -= (dugmad[1] && uklapa_li_se(current_piece, current_rotation, current_location_X - 1, current_location_Y)) ? 1 : 0;
			current_location_Y += (dugmad[2] && uklapa_li_se(current_piece, current_rotation, current_location_X, current_location_Y + 1)) ? 1 : 0;
			if (dugmad[3])
			{
				current_rotation += (stop_rotate && uklapa_li_se(current_piece, current_rotation + 1, current_location_X, current_location_Y)) ? 1 : 0;
				stop_rotate = true;
			}
			else
			{
				stop_rotate = false;
			}
		}

		//logika igre
		{
			if (spustaj) // Ako je vreme da se spusti za 1 proveravamo da li moze
			{
				//Povecavanje tezine u intervalima
				speed_counter = 0;
				broj_odigranih++;
				if (broj_odigranih % 50 == 0)
				{
					if (speed >= 10)
					{
						speed--; // 
					}
				}

				//Spustanje
				if (uklapa_li_se(current_piece, current_rotation, current_location_X, current_location_Y+1))
				{
					current_location_Y += 1; // ako mozze da se spusti spustaj
				}
				else
				{

					//Zakljucava blok kada pipne "pod" (Dodaje ga kao deo bg ili ti border boxa)
					for (int px = 0; px < 4; px++)
					{
						for (int py = 0; py < 4; py++)
						{
							if (tipovi_blokova[current_piece][rotiranje(px, py, current_rotation)] != L'.')
							{
								pField[(current_location_Y + py) * field_width + (current_location_X + px)] = current_piece + 1;
							}
						}
					}

					//Formira "liniju" = ako smo uspesno "zatvorili" ceo red i zatim ga brise;
					for (int py = 0; py < 4; py++)
					{
						if (current_location_Y + py < field_height - 1)
						{
							bool linija = true;//za pocetak pretpostavljamo da linija postoji

							for (int px = 1; px < field_width - 1; px++)
							{
								linija &= (pField[(current_location_Y + py) * field_width + px]) != 0; // Koristimo operator & da ako pfield...je jednako nuli stavimo bool linija na false a ako jeste !0 onda linija zadrzava inicijalno stanje true
							}
							if (linija)
							{
								for (int px = 1; px < field_width - 1; px++)
								{
									pField[(current_location_Y + py) * field_width + px] = 8; //ispunjavamo red int 8 ( ili ti u nasem trasnlatoru koji se nalazi znak to je znak "=");
								}
								linije.push_back(current_location_Y + py);
							}
						}
					}
					//Povecavanje skora
					s.score += 25;
					if (!linije.empty())
					{
						s.score += (1 << linije.size()) * 100;
					}


					//Sledeci tip bloka koji ce se prikazati
					{
						current_location_X = field_width / 2;
						current_location_Y = 0;
						current_piece = rand() % 7;
						current_rotation = 0;
					}


					//Zavrsi igru ako se ne moze ubaciti sledeci i upisuje highscore
					if (!uklapa_li_se(current_piece, current_rotation, current_location_X, current_location_Y))
					{
						game_over = true;
						//HISHCORE UBACITI OVDE
						cout << "Vas konacan rezultat je bio: "
							<< s.score << endl;
						if (s.check_hs())
						{
							cout << "USPELI STE DA POBEDITE HIGHSCORE!!!" << endl;
						}
						else cout << "Niste uspeli da pobedite highscore (" << s.print_hs() << ") ovaj put," << endl						
						<< " ali mozda cete imati vise srece sledeci put!!!" << endl;

					}
				}
			}
		}


		//crtanje
		{

			// Iteratujemo kroz polje gde nam se crtaju simboli da bi uneli odgovarajuce podatke
			for (int i = 0; i < field_width; i++)
			{
				for (int j = 0; j < field_height; j++)
				{
					screen[(j + 2) * screen_width + (i + 2)] = L" ABCDEFG=#"[pField[j * field_width + i]];//popunjava screen(bufer) sa odgovarajucim simbolima za boundry box (ivice)
				}
			}

			//Iteratujemo kroz "pixele" naseg bloga da bi ga iscrtali
			for (int px = 0; px < 4; px++)
			{
				for (int py = 0; py < 4; py++)
				{
					if (tipovi_blokova[current_piece][rotiranje(px, py, current_rotation)] == L'X') // Ako je na trenutnoj poziciji naseg bloka x treba da se iscrta u protivnom ne
					{
						screen[(current_location_Y + py + 2) * screen_width + (current_location_X + px + 2)] = current_piece + 65; //  crta karakter (pod gornjim uslovom). Zasto +65? 0+65 je ascii "A" 1+65 je "B" itd itd (da bi dobili razliku koju imamo u gornjoj for iteraciji za crtanje boundry boxa
					}
				}
			}
			
			//Crtanje rezultata
			//Posto se svakako ne crta nista van fielda samo cemo koristiti swprint funkciju da iscrtamo skorove da bi pratili sta se desava
			swprintf_s(&screen[2 * screen_width + field_width + 6], 21, L"SCORE: %13d", s.score);
			swprintf_s(&screen[3 * screen_width + field_width + 6], 21, L"HIGH-SCORE: %8d", s.print_hs());
			swprintf_s(&screen[4 * screen_width + field_width + 6], 33, L"KRETANJE LEVO DESNO: NA STRELICE");
			swprintf_s(&screen[5 * screen_width + field_width + 6], 31, L"BRZO PADANJE NA DONJU STRELICU");
			swprintf_s(&screen[6 * screen_width + field_width + 6], 14, L"ROTACIJA NA Z");

			//Animacija nestajanja linija
			if (!linije.empty())
			{
				//Komanda objasnjena u segmentu ispod 
				WriteConsoleOutputCharacter(hconsole, screen, screen_width * screen_height, { 0,0 }, &dwBytesWritten);
				this_thread::sleep_for(400ms); // Ne zelimo da odmah izbrise linije tako da cekamo 0.4sec

				for (auto& v : linije) //U sustini foreach iterator iz c#
					for (int px = 1; px < field_width - 1; px++)
					{
						for (int py = v; py > 0; py--)
						{
							pField[py * field_width + px] = pField[(py - 1) * field_width + px];//Premesta linije za jedan nadole
						}
						pField[px] = 0;// Brise "="
					}

				linije.clear();
			}

			//Komanda Setconsoleactivescreenbufer efektivno menjaja nacin na koji mozemo da koristimo konzolu, tako da cemo od sada za ispisivanje stvari koristiti sledecu komandu umesto klasicne cout seme
			WriteConsoleOutputCharacter(hconsole, screen, (screen_height * screen_width), { 0,0 }, &dwBytesWritten);
			//Ova komanda prihvata parametre (u nasem slucaju) 1:bufer u koji se pise ,2: Source koji prebacujemo u taj bafer,3:Koliko kolicinski zelimo da iskoristimo (Koliko karaktera bufera menjamo),4: odakle pocinjemo sa menjanjem bafera
		}
	}
	CloseHandle(hconsole);
	return 1;
}