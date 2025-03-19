#include <fstream> // Do obsługi plików
#include <iostream> // Do obsługi strumieni wejścia/wyjścia
#include <vector>
#include <chrono> // Do pomiaru czasu
#include <iomanip> // Do formatowania upłyniętych sekund w outpucie
#include <cstring> // Do funkcji strtok przy wczytywaniu danych z pliku

using namespace std;

// Funkcja wypisująca kombinację i zapisująca ją do pliku
void printCombination(const vector<int>& comb, ofstream& ofs) {
    for (int num : comb)
        ofs << num << " ";
    ofs << "\n";
}

// Funkcja generująca wszystkie kombinacje wg algorytmu klasycznego
void generateCombinations(int n, int k, ofstream& ofs) {
    vector<int> comb(k);
    // Inicjalizacja pierwszej kombinacji: [1, 2, ..., k]
    for (int i = 0; i < k; i++)
        comb[i] = i + 1;

    printCombination(comb, ofs);  // Zapisujemy początkową kombinację

    while (true) {
        int i = k - 1;  // Zaczynamy od ostatniego elementu
        // Szukamy elementu, który można zwiększyć
        while (i >= 0 && comb[i] == n - k + i + 1)
            i--;
        if (i < 0) break;  // Jeżli nie można już zwiększyć żadnego elementu, kończymy
        comb[i]++;  // Zwiększamy wybrany element o 1
        // Resetujemy wszystkie elementy po pozycji i tak, aby tworzyły ciąg rosnący
        for (int j = i + 1; j < k; j++)
            comb[j] = comb[i] + (j - i);
        printCombination(comb, ofs);  // Zapisujemy nową kombinację
    }
}



// Funkcja generująca kombinacje wg algorytmu Semby
void generateCombinationsSemby(int n, int k, ofstream& ofs) {
    vector<int> comb(k);
    // Inicjalizacja pierwszej kombinacji: [1, 2, ..., k]
    for (int i = 0; i < k; i++)
        comb[i] = i + 1;
    printCombination(comb, ofs);  // Zapisujemy początkową kombinację

    while (true) {
        int i = k - 1;  // Rozpoczynamy od ostatniego elementu
        // Szukamy elementu, który można zwiększyć, sprawdzając "odchylenie"
        while (i >= 0 && (comb[i] - (i + 1)) == (n - k))
            i--;
        if (i < 0) break;  // Jeśli nie można zwiększyć żadnego elementu, kończymy
        comb[i]++;  // Zwiększamy wybrany element o 1
        // Ustawiamy kolejne elementy jako ciąg liczb następujących po poprzednim
        for (int j = i + 1; j < k; j++)
            comb[j] = comb[j - 1] + 1;
        printCombination(comb, ofs);  // Zapisujemy nową kombinację
    }
}

int main()
{
    // Stałe z maksymalnymi wartościami n i k
    const int MAX_N = 20; 
    const int MAX_K = 10;

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

    if (testy.empty()) {
        output << "Brak danych wejściowych w pliku";
        output.close();
        return 0;
    }

    output << std::fixed << std::setprecision(6); // Dla formatu upłyniętego czasu w sekundach

    for (int t = 0; t < testy.size(); t++) {
        output << "\nTEST " << t + 1 << ":\n";
        output << "Dane wejsciowe:";

        vector<int> nk = testy[t];

        for (int data : nk) {
            output << " " << data;
        }

        if (nk[0] > MAX_N || nk[1] > MAX_K) {
            output << "\nZa duże wartości n lub k. Maksymalne wartości to: n = " << MAX_N << ", k = " << MAX_K;
            output << "\n Test zostanie pominięty.";
            continue;
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
