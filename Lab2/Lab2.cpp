#include <fstream> // Do obsługi plików
#include <iostream> // Do obsługi strumieni wejścia/wyjścia
#include <vector>
#include <chrono> // Do pomiaru czasu
#include <iomanip> // Do formatowania upłyniętych sekund w outpucie
#include <cstring> // Do funkcji strtok przy wczytywaniu danych z pliku

using namespace std;

void generatePartitions(int index, int maxGroup, int n, int k, vector<int>& partition, vector<vector<int>>& results) {
    if (index == n) {
        results.push_back(partition);
        return;
    }

    for (int i = 1; i <= min(maxGroup + 1, k); ++i) {
        partition[index] = i;
        generatePartitions(index + 1, max(maxGroup, i), n, k, partition, results);
    }
}

int main()
{
    // Stałe z maksymalnymi wartościami n i k
    const int MAX_N = 20; 
    const int MAX_K = 10;

    ifstream input("input.txt");
    vector<vector<int>> testy;

    if (!input.is_open()) {
        cout << "Nie udało się otworzyć pliku z danymi wejściowymi";
        return -1;
    }

    // Wczytywanie danych z pliku i dodawanie ich do wektora testów
    char curr[100];
    while (input.getline(curr, 100)) {
		vector<int> nk;
		char* token = strtok(curr, " ");
		while (token != NULL) {
            // Sprawdzanie, czy dane są liczbami
            if (!isdigit(token[0]) && token[0] != '-') {
                cout << "\nBŁĄD. Wartość: " << token << endl;
                cout << "Dane n oraz k muszą być liczbami. Dana zostanie pominięta. Zły format danych: " << token[0] << endl;
                nk.clear();
                break;
            }
			nk.push_back(atoi(token));
			token = strtok(NULL, " ");
		}
        // Sprawdzenie czy jest n i k
        if (nk.size() != 2) {
            cout << "\nBŁĄD. Wartość: " << curr << endl;
            cout << "Za mało danych w linii. Linia zostanie pominięta." << endl;
            continue;
        }
        testy.push_back(nk);
    }

    input.close();

    ofstream output("output.txt");
    if (!output.is_open()) {
        cout << "Nie udało się otworzyć pliku z danymi wejściowymi. Program zostanie zakończony.";
        return -1;
    }

    // Sprawdzanie błędów pisania do pliku
    if (output.fail()) {
        cout << "Błąd zapisu do pliku wyjściowego. Program zostanie zakończony.\n";
        return -1;
    }

    output << "ALGORYTM GENERUJĄCY PODZIAŁY CO NAJWYŻEJ N-BLOKOWE\n";

    if (testy.empty()) {
        output << "Brak danych wejściowych w pliku lub dane są niepoprawne.";
        output.close();
        return 0;
    }

    // Iteracja po testach
    for (int t = 0; t < testy.size(); t++) {
        output << "\nTEST " << t + 1 << ":\n";
        output << "Dane wejściowe:";

        vector<int> nk = testy[t];

        for (int data : nk) {
            output << " " << data;
        }

        output << endl;

        // Obsługa nieprawidłowych wartości inputu i edge case'ów
        if (nk[0] < nk[1]) {
            output << "\nWartość n nie może być mniejsza od k. Test zostanie pominięty." << endl;
            continue;
        }

        if (nk[0] > MAX_N || nk[1] > MAX_K) {
            output << "\nZa duże wartości n lub k. Maksymalne wartości to: n = " << MAX_N << ", k = " << MAX_K;
            output << "\n Test zostanie pominięty." << endl;
            continue;
        }

        if (nk[0] < 0 || nk[1] < 0) {
            output << "\nWartości n i k nie mogą być ujemne. Test zostanie pominięty." << endl;
            continue;
        }

        if (nk[0] == 0) {
            output << "\nWartość n nie może być równa 0. Test zostanie pominięty." << endl;
            continue;
        }

        if (nk[1] == 0) {
            output << "\nBrak podziałów dla k = 0." << endl;
            continue;
        }

        vector<vector<int>> results;
        vector<int> partition(nk[0], 1);

        auto begin = std::chrono::high_resolution_clock::now();
        generatePartitions(0, 0, nk[0], nk[1], partition, results);
        auto end = std::chrono::high_resolution_clock::now();

        // Zapisywanie podziałów do pliku wyjściowego
        for (auto& result : results) {
            for (auto& part : result) {
                output << part << " ";
            } 
            output << endl;
        }

        // Wyświetlanie czasu generacji i liczby podziałów
        std::chrono::duration<double> elapsed = end - begin;
        output << "Czas generowania podziałów: " << (elapsed.count() * 1000.0) << " ms\n";
        output << "Łączna liczba wygenerowanych podziałów: " << results.size() << endl;
    }

    output.close();

    return 0;
}
