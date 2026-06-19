#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// 1. IL NOSTRO ESPLORATORE
struct Nodo {
    int x, y;
    int g, h, f;
    Nodo* genitore;

    Nodo(int coordX, int coordY, Nodo* gen = nullptr) {
        x = coordX;
        y = coordY;
        genitore = gen;
        g = 0; h = 0; f = 0;
    }
};

// 2. LA BUSSOLA
int calcolaEuristica(int x, int y, int arrivoX, int arrivoY) {
    return abs(x - arrivoX) + abs(y - arrivoY);
}

// 3. I SENSI (Capire se c'è un muro o se siamo fuori mappa)
bool mossaValida(int x, int y, const vector<string>& mappa) {
    if (y >= 0 && y < mappa.size() && x >= 0 && x < mappa[0].size()) {
        return mappa[y][x] != '#';
    }
    return false;
}

int main() {
    // LA MAPPA
    vector<string> mappa = {
        "S...#...",
        ".#..#.#.",
        ".#..#.#.",
        "......#E",
        "########"
    };

    int startX = 0, startY = 0; // Coordinate di 'S'
    int endX = 7, endY = 3;     // Coordinate di 'E'

    // LE LISTE DELLA MEMORIA DELL'AI
    vector<Nodo*> openList;   
    vector<Nodo*> closedList; 

    openList.push_back(new Nodo(startX, startY));
    
    Nodo* nodoFinale = nullptr;
    bool trovato = false;

    cout << "Avvio calcolo percorso neurale..." << endl;

    // IL MOTORE DI RICERCA A*
    while (!openList.empty()) {
        int indiceMigliore = 0;
        for (int i = 1; i < openList.size(); i++) {
            if (openList[i]->f < openList[indiceMigliore]->f) {
                indiceMigliore = i;
            }
        }

        Nodo* corrente = openList[indiceMigliore];
        openList.erase(openList.begin() + indiceMigliore);
        closedList.push_back(corrente);

        // VITTORIA?
        if (corrente->x == endX && corrente->y == endY) {
            trovato = true;
            nodoFinale = corrente;
            break; 
        }

        // ESPLORAZIONE
        int dirX[] = {1, -1, 0, 0};
        int dirY[] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++) {
            int nuovoX = corrente->x + dirX[i];
            int nuovoY = corrente->y + dirY[i];

            if (!mossaValida(nuovoX, nuovoY, mappa)) continue;

            bool giaEsplorato = false;
            for (Nodo* n : closedList) {
                if (n->x == nuovoX && n->y == nuovoY) giaEsplorato = true;
            }
            if (giaEsplorato) continue;

            Nodo* vicino = new Nodo(nuovoX, nuovoY, corrente);
            
            vicino->g = corrente->g + 1;
            vicino->h = calcolaEuristica(vicino->x, vicino->y, endX, endY);
            vicino->f = vicino->g + vicino->h;

            bool salta = false;
            for (Nodo* n : openList) {
                if (n->x == vicino->x && n->y == vicino->y && n->g <= vicino->g) {
                    salta = true;
                    break;
                }
            }
            if (salta) continue;

            openList.push_back(vicino);
        }
    }

    // DISEGNAMO IL RISULTATO
    if (trovato) {
        cout << "\nPERCORSO TROVATO! Traccio la rotta...\n" << endl;
        
        // ECCO LA RIGA CHE CONTA I PASSI!
        cout << "Numero di passi reali effettuati: " << nodoFinale->g << endl << endl;
        
        Nodo* traccia = nodoFinale->genitore; 
        while (traccia->genitore != nullptr) { 
            mappa[traccia->y][traccia->x] = '*';
            traccia = traccia->genitore;
        }

        for (const string& riga : mappa) {
            cout << riga << endl;
        }
    } else {
        cout << "\nNessun percorso possibile! Il labirinto e' chiuso." << endl;
    }

    return 0;
}