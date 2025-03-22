#include <fstream> // Do obsługi plików
#include <iostream> // Do obsługi strumieni wejścia/wyjścia
#include <vector>
#include <chrono> // Do pomiaru czasu
#include <iomanip> // Do formatowania upłyniętych sekund w outpucie
#include <cstring> // Do funkcji strtok przy wczytywaniu danych z pliku

using namespace std;

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
                cout << "Dane n oraz k muszą być liczbami. Linia zostanie pominięta. Zły format danych: " << curr << "\n";
                nk.clear();
                break;
            }
			nk.push_back(atoi(token));
			token = strtok(NULL, " ");
		}
        // Sprawdzenie czy jest n i k
        if (nk.size() != 2) {
            cout << "Za mało danych w linii. Linia zostanie pominięta: " << curr << "\n";
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

    output << std::fixed << std::setprecision(6); // Dla formatu upłyniętego czasu w sekundach

    // Iteracja po testach
    for (int t = 0; t < testy.size(); t++) {
        output << "\nTEST " << t + 1 << ":\n";
        output << "Dane wejściowe:";

        vector<int> nk = testy[t];

        for (int data : nk) {
            output << " " << data;
        }

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
            output << "\nBrak kombinacji dla k = 0." << endl;
            continue;
        }

        // TUTAJ FUNKCJA
    }

    output.close();

    return 0;
}
