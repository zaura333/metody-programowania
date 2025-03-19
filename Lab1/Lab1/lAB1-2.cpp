#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <cstring> // Required for strtok

// SPRAWOZDANIE I TESTY DO CZWARTKU

using namespace std;

// Funkcja wypisuj�ca kombinacj� i zapisuj�ca j� do pliku
void printCombination(const vector<int>& comb, ofstream& ofs) {
    for (int num : comb)
        ofs << num << " ";
    ofs << "\n";
}

// Funkcja generuj�ca wszystkie kombinacje wg algorytmu klasycznego
void generateCombinations(int n, int k, ofstream& ofs) {
    vector<int> comb(k);
    // Inicjalizacja pierwszej kombinacji: [1, 2, ..., k]
    for (int i = 0; i < k; i++)
        comb[i] = i + 1;

    printCombination(comb, ofs);  // Zapisujemy pocz�tkow� kombinacj�

    while (true) {
        int i = k - 1;  // Zaczynamy od ostatniego elementu
        // Szukamy elementu, kt�ry mo�na zwi�kszy�
        while (i >= 0 && comb[i] == n - k + i + 1)
            i--;
        if (i < 0) break;  // Je�li nie mo�na ju� zwi�kszy� �adnego elementu, ko�czymy
        comb[i]++;  // Zwi�kszamy wybrany element o 1
        // Resetujemy wszystkie elementy po pozycji i tak, aby tworzy�y ci�g rosn�cy
        for (int j = i + 1; j < k; j++)
            comb[j] = comb[i] + (j - i);
        printCombination(comb, ofs);  // Zapisujemy now� kombinacj�
    }
}



// Funkcja generuj�ca kombinacje wg algorytmu Semby
void generateCombinationsSemby(int n, int k, ofstream& ofs) {
    vector<int> comb(k);
    // Inicjalizacja pierwszej kombinacji: [1, 2, ..., k]
    for (int i = 0; i < k; i++)
        comb[i] = i + 1;
    printCombination(comb, ofs);  // Zapisujemy pocz�tkow� kombinacj�

    while (true) {
        int i = k - 1;  // Rozpoczynamy od ostatniego elementu
        // Szukamy elementu, kt�ry mo�na zwi�kszy�, sprawdzaj�c "odchylenie"
        while (i >= 0 && (comb[i] - (i + 1)) == (n - k))
            i--;
        if (i < 0) break;  // Je�li nie mo�na zwi�kszy� �adnego elementu, ko�czymy
        comb[i]++;  // Zwi�kszamy wybrany element o 1
        // Ustawiamy kolejne elementy jako ci�g liczb nast�puj�cych po poprzednim
        for (int j = i + 1; j < k; j++)
            comb[j] = comb[j - 1] + 1;
        printCombination(comb, ofs);  // Zapisujemy now� kombinacj�
    }
}

int main()
{
    ifstream input("input.txt");
    vector<vector<int>> testy;
    int i = 0;

    if (!input.is_open()) {
        cout << "Nie udało się otworzyć pliku z danymi wejściowymi";
        return -1;
    }

    char curr[100];
    while (input.getline(curr, 100)) {
		vector<int> nk;
		char* token = strtok(curr, " ");
		while (token != NULL) {
			nk.push_back(atoi(token));
			token = strtok(NULL, " ");
		}
        i++;
        testy.push_back(nk);
    }

    input.close();

    ofstream output("output.txt");
    if (!output.is_open()) {
        cout << "Nie udało się otworzyć pliku z danymi wejściowymi";
        return -1;
    }

    for (int t = 0; t < testy.size(); t++) {
        output << "TEST " << t + 1 << ":\n";
        output << "Dane wejsciowe:";

        vector<int> nk = testy[t];

        for (int data : nk) {
            output << " " << data;
        }

        auto begin = std::chrono::high_resolution_clock::now();

        output << "\n\nKombinacje - algorytm klasyczny:\n";
        generateCombinations(nk[0], nk[1], output);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - begin;
        output << "Czas generowania kombinacji algorytmu klasycznego: " << elapsed.count() << " s\n";

        begin = std::chrono::high_resolution_clock::now();

        output << "\n\nKombinacje - algorytm Semby:\n";
        generateCombinationsSemby(nk[0], nk[1], output);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - begin;
        output << "Czas generowania kombinacji algorytmu Semby: " << elapsed.count() << " s\n";
    }

    output.close();

    return 0;
}
