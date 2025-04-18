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
vector<vector<int>> generateCombinations(int n, int k) {
    vector<vector<int>> combinations;
    vector<int> comb(k);
    // Inicjalizacja pierwszej kombinacji: [1, 2, ..., k]
    for (int i = 0; i < k; i++)
        comb[i] = i + 1;

    combinations.push_back(comb);  // Zapisujemy początkową kombinację

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
        combinations.push_back(comb);  // Zapisujemy nową kombinację
    }

    return combinations;
}



// Funkcja generująca kombinacje wg algorytmu Semby
vector<vector<int>> generateCombinationsSemby(int n, int k) {
    vector<vector<int>> combinations;
    vector<int> comb(k);
    // Inicjalizacja pierwszej kombinacji: [1, 2, ..., k]
    for (int i = 0; i < k; i++)
        comb[i] = i + 1;
    combinations.push_back(comb);  // Zapisujemy początkową kombinację

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
        combinations.push_back(comb);  // Zapisujemy nową kombinację
    }

    return combinations;
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
        cout << "Nie udało się otworzyć pliku z danymi wejściowymi";
        return -1;
    }

    // Sprawdzanie błędów pisania do pliku
    if (output.fail()) {
        cout << "Error writing to output file. Exiting program.\n";
        return -1;
    }

    output << "ALGORYTMY GENERUJĄCE KOMBINACJE\n";

    if (testy.empty()) {
        output << "Brak danych wejściowych w pliku lub dane są niepoprawne.";
        output.close();
        return 0;
    }

    output << std::fixed << std::setprecision(4); // Dla formatu upłyniętego czasu w sekundach

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

        // Mierzenie czasu generowania kombinacji
        auto begin = std::chrono::high_resolution_clock::now();

        output << "\n\nKombinacje - algorytm klasyczny:\n";
        vector<vector<int>> combinationsClassic = generateCombinations(nk[0], nk[1]);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - begin;

        for (auto& comb : combinationsClassic) {
            printCombination(comb, output);
        }

        output << "Czas generowania kombinacji algorytmu klasycznego: " << (elapsed.count() * 1000.0) << " ms\n";

        begin = std::chrono::high_resolution_clock::now();

        output << "\n\nKombinacje - algorytm Semby:\n";
        vector<vector<int>> combinationsSemba = generateCombinationsSemby(nk[0], nk[1]);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - begin;

        for (auto& combSemba : combinationsSemba) {
            printCombination(combSemba, output);
        }

        output << "Czas generowania kombinacji algorytmu Semby: " << (elapsed.count() * 1000.0) << " ms\n";
    }

    output.close();

    return 0;
}
