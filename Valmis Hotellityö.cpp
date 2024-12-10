// Leevi Koskimaa Hotellityö

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

// Huonevarauksen aliohjelma
bool huonevaraus() {
    // Asetetaan huoneiden hinnat ja alkuperäiset määrät
    const int yhdenHengenHinta = 80;
    const int kahdenHengenHinta = 100;
    int yhdenHengenHuoneet = 25;
    int kahdenHengenHuoneet = 25;

    // Luetaan huoneiden nykyinen tilanne tiedostosta
    ifstream huonetilasto("huonetilanne.txt");
    if (huonetilasto.is_open()) {
        huonetilasto >> yhdenHengenHuoneet >> kahdenHengenHuoneet;
        huonetilasto.close();
    }

    cout << "\n--- HOTELLIHUONEVARAUS ---" << endl;

    // Looppi varauksen tekemiseen
    while (true) {
        cout << "Millaisen huoneen haluat varata?" << endl;
        cout << "1. Yhden hengen huone (80€/huone)" << endl;
        cout << "2. Kahden hengen huone (100€/huone)" << endl;
        cout << "Valitse vaihtoehto (1 tai 2): ";
        int huoneTyyppi;
        cin >> huoneTyyppi;

        // Tarkistetaan syöte
        if (huoneTyyppi != 1 && huoneTyyppi != 2) {
            cout << "Virheellinen valinta. Yritä uudelleen." << endl;
            continue;
        }

        cout << "Kuinka monta huonetta haluat varata? ";
        int maara;
        cin >> maara;

        // Tarkistetaan syötteen oikeellisuus
        if (cin.fail() || maara <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Virheellinen määrä. Yritä uudelleen." << endl;
            continue;
        }

        // Tarkistetaan huineiden saatavuus
        if (huoneTyyppi == 1 && maara > yhdenHengenHuoneet) {
            cout << "Valitettavasti yhden hengen huoneet ovat loppu!" << endl;
            cout << "Haluatko varata kahden hengen huoneita? (k/e): ";
            char vastaus;
            cin >> vastaus;
            if (vastaus == 'e' || vastaus == 'E') {
                return false;
            }
            continue;
        } else if (huoneTyyppi == 2 && maara > kahdenHengenHuoneet) {
            cout << "Valitettavasti kahden hengen huoneet ovat loppu!" << endl;
            cout << "Haluatko varata yhden hengen huoneita? (k/e): ";
            char vastaus;
            cin >> vastaus;
            if (vastaus == 'e' || vastaus == 'E') {
                return false;
            }
            continue;
        }

        // Lasketaan loppusumma
        int hinta = (huoneTyyppi == 1 ? yhdenHengenHinta : kahdenHengenHinta) * maara;
        cout << "Huoneiden loppusumma: " << hinta << "€" << endl;

        // Pyydetään varaajan nimi
        cout << "Syötä varaajan nimi: ";
        cin.ignore();
        string nimi;
        getline(cin, nimi);

        // Luodaan satunnainen varausnumero
        srand(time(0));
        int varausnumero = 10000 + rand() % 90000;

        // Päivitetään huoneiden tilanne
        if (huoneTyyppi == 1) {
            yhdenHengenHuoneet -= maara;
        } else {
            kahdenHengenHuoneet -= maara;
        }

        // Tallennetaan uusi huonetilanne tiedostoon
        ofstream huoneKirjaus("huonetilanne.txt");
        if (huoneKirjaus.is_open()) {
            huoneKirjaus << yhdenHengenHuoneet << " " << kahdenHengenHuoneet << endl;
            huoneKirjaus.close();
        }

        // Tallennetaan varauksen tiedot tiedostoon
        ofstream varausKirjaus("varaukset.txt", ios::app);
        if (varausKirjaus.is_open()) {
            varausKirjaus << "Varaajan nimi: " << nimi << endl;
            varausKirjaus << "Varausnumero: " << varausnumero << endl;
            varausKirjaus << "Huone: " << (huoneTyyppi == 1 ? "Yhden hengen" : "Kahden hengen") << endl;
            varausKirjaus << "Huoneiden määrä: " << maara << endl;
            varausKirjaus << "-------------------------" << endl;
            varausKirjaus.close();
        }

        cout << "Huoneet varattu onnistuneesti! Varausnumerosi on " << varausnumero << "." << endl;
        return true;
    }
}

// Tyhjentää varaukset ja palauttaa huoneiden määrät alkuperäiseksi
void tyhjennaVaraukset() {
    // Tyhjennetään varauksetiedosto
    ofstream varauksetTyhjenna("varaukset.txt", ios::trunc);
    if (varauksetTyhjenna.is_open()) {
        varauksetTyhjenna.close();
        cout << "Varaukset on tyhjennetty." << endl;
    } else {
        cout << "Virhe tyhjentäessä varauksia." << endl;
    }

    // Palautetaan alkuperäiset huonemäärät
    ofstream huoneTilanne("huonetilanne.txt");
    if (huoneTilanne.is_open()) {
        huoneTilanne << "25 25" << endl;  // Alkuperäinen huonemäärä
        huoneTilanne.close();
        cout << "Kaikki huoneet ovat nyt vapaana." << endl;
    } else {
        cout << "Virhe huonemäärien palautuksessa." << endl;
    }
}

// Näyttää varaus tilanteen ja mahdollisuus tarkistella varattuja huoneita
void naytaVarauksetJaVarasto() {
    int yhdenHengenHuoneet = 25;
    int kahdenHengenHuoneet = 25;

    // Luetaan huonetilanne tiedostosta
    ifstream huonetilasto("huonetilanne.txt");
    if (huonetilasto.is_open()) {
        huonetilasto >> yhdenHengenHuoneet >> kahdenHengenHuoneet;
        huonetilasto.close();
    }

    // Lasketaan tehtyjen varausten määrä
    int varauksiaYhteensa = 0;
    ifstream varauksetTiedosto("varaukset.txt");
    string rivi;
    while (getline(varauksetTiedosto, rivi)) {
        if (rivi.find("Varaajan nimi:") != string::npos) {
            varauksiaYhteensa++;
        }
    }
    varauksetTiedosto.close();

    cout << "\n--- NÄYTÄ VARAUKSET ---" << endl;
    cout << "Yhden hengen huoneita vapaana: " << yhdenHengenHuoneet << endl;
    cout << "Kahden hengen huoneita vapaana: " << kahdenHengenHuoneet << endl;
    cout << "Varauksia tehty yhteensä: " << varauksiaYhteensa << endl;

    // Kaikki tehdyt varaukset yhteensä
    cout << "\n--- VARAUKSET ---" << endl;
    ifstream varaukset("varaukset.txt");
    if (varaukset.is_open()) {
        while (getline(varaukset, rivi)) {
            cout << rivi << endl;
        }
        varaukset.close();
    } else {
        cout << "Varauksia ei löytynyt." << endl;
    }

    // Mahdollisuus hakea tiettyä varausta, joko nimellä tai varausnumerolla
    cout << "Haluatko hakea varausta? (kyllä/ei): ";
    string vastaus;
    cin >> vastaus;

    if (vastaus == "kyllä" || vastaus == "Kyllä") {
        while (true) {
            cout << "\nKirjoita varaajan nimi tai varausnumero: ";
            cin.ignore();
            string hakusana;
            getline(cin, hakusana);

            bool loytyi = false;
            ifstream varauksetHaku("varaukset.txt");
            if (varauksetHaku.is_open()) {
                vector<string> varausTiedot;
                while (getline(varauksetHaku, rivi)) {
                    if (rivi.find(hakusana) != string::npos) {
                        loytyi = true;
                        varausTiedot.push_back(rivi);
                        for (int i = 0; i < 3; i++) {
                            getline(varauksetHaku, rivi);
                            varausTiedot.push_back(rivi);
                        }
                        break;
                    }
                }
                varauksetHaku.close();

                if (loytyi) {
                    cout << "\nVaraus löytyi:" << endl;
                    for (const auto &tieto : varausTiedot) {
                        cout << tieto << endl;
                    }
                } else {
                    cout << "Varausta ei löytynyt." << endl;
                }
            }

            cout << "Haluatko jatkaa hakua? (kyllä/ei): ";
            string jatko;
            cin >> jatko;
            if (jatko != "kyllä" && jatko != "Kyllä") {
                break;
            }
        }
    }
    cout << "Palataan päävalikkoon!" << endl;
}

int main() {

    // Ohjelman käynnistys
    cout << "Paina mitä tahansa näppäintä ja sitten enter avataksesi ohjelman: " << endl;
    string aloitus;
    getline(cin, aloitus);

    // Päävalikko
    while (true) {
        cout << "\n--- PÄÄVALIKKO ---" << endl;
        cout << "1. HOTELLIHUONEVARAUS" << endl;
        cout << "2. NÄYTÄ VARAUKSET" << endl;
        cout << "3. TYHJENNÄ VARAUKSET" << endl;
        cout << "4. EXIT" << endl;
        cout << "Valitse toiminto (1-4): ";

        int valinta;
        cin >> valinta;

        // Tarkistetaan syötteen oikeellisuus
        if (cin.fail() || valinta < 1 || valinta > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Virheellinen valinta. Yritä uudelleen." << endl;
            continue;
        }

        // Suoritetaan valittu toiminto
        switch (valinta) {
            case 1:
                if (!huonevaraus()) {
                    cout << "Palataan päävalikkoon." << endl;
                }
                break;
            case 2:
                naytaVarauksetJaVarasto();
                break;
            case 3:
                tyhjennaVaraukset();
                break;
            case 4:
                cout << "Ohjelma suljetaan. Kiitos!" << endl;
                return 0;
        }
    }

    return 0;
}
