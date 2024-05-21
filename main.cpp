#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <cctype>

using namespace std;

int main() {
    void countCharacters(const string &);
    vector<int> findSubstring(const string &filename, const string &substring);
    void processFile(const string& inputFile, const string& outputFile);

    char ch, name[50];
    string filename = name;
    FILE *in;
    printf("Введите имя файла для просмотра: ");
    scanf("%s", name);
    if ((in = fopen(name, "r")) == NULL)
        printf("Файл %s не открыт", name);
    else {
        string substring = "example";
        string outputFile = "output.txt";
        countCharacters(filename);
        vector<int> indices = findSubstring(filename, substring);
        if (indices.empty()) {
            cout << "Подстрока не найдена" << endl;
        } else {
            cout << "Индексы вхождений подстроки: ";
            for (int index : indices) {
                cout << index << " ";
            }
            cout << endl;
        }

        processFile(filename, outputFile);

        while (!feof(in)) {
            ch = getc(in);
            putchar(ch);
        }
    }
    return 0;
}

//Функция findSubstring принимает имя файла и искомую подстроку в качестве параметров.
//Открывается файл с помощью ifstream.
//Создается вектор indices для хранения индексов вхождений подстроки.
//Читаются строки из файла в цикле while с помощью getline.
//Для каждой строки ищутся все вхождения подстроки с помощью line.find(substring, pos). Индекс каждого вхождения вычисляется как сумма текущего индекса строки (lineIndex) и позиции вхождения внутри строки (pos). Индексы добавляются в вектор indices.
//После выхода из цикла файл закрывается с помощью file.close().
//Функция возвращает вектор indices с индексами вхождений подстроки.
//В main функции указываются имя файла и искомая подстрока, вызывается функция findSubstring, и выводятся результаты поиска.

vector<int> findSubstring(const string& filename, const string& substring) {
    ifstream file(filename);
    if (!file) {
        cout << "Невозможно открыть файл " << filename << endl;
        return {};
    }

    vector<int> indices;
    string line;
    int lineIndex = 0;

    while (getline(file, line)) {
        size_t pos = 0;
        while ((pos = line.find(substring, pos)) != string::npos) {
            indices.push_back(lineIndex + pos);
            pos += substring.length();
        }
        lineIndex += line.length() + 1; // + 1 для учета символа новой строки
    }

    file.close();
    return indices;
}

//Функция countCharacters принимает имя файла в виде строки.
//Открывается файл с помощью ifstream.
//Внутри цикла while происходит чтение символов из файла по одному с помощью file.get(c).
//Для каждого символа проверяется, является ли он пробельным символом с помощью isspace(c). Если да, увеличивается счетчик emptyCount, иначе увеличивается счетчик nonEmptyCount.
//После выхода из цикла файл закрывается с помощью file.close().
//Выводятся количества пустых и непустых символов.
//В main функции указывается имя файла, и вызывается функция countCharacters с этим именем.

void countCharacters(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Невозможно открыть файл " << filename << endl;
        return;
    }

    int emptyCount = 0;
    int nonEmptyCount = 0;

    char c;
    while (file.get(c)) {
        if (isspace(c)) {
            emptyCount++;
        } else {
            nonEmptyCount++;
        }
    }

    file.close();

    cout << "Количество пустых символов: " << emptyCount << endl;
    cout << "Количество непустых символов: " << nonEmptyCount << endl;
}

//Функция processFile принимает имена входного и выходного файлов.
//Открываются входной и выходной файлы с помощью ifstream и ofstream.
//Создаются переменные pageNumber для хранения номера текущей страницы и isNewPage для отслеживания начала новой страницы.
//В цикле while читаются строки из входного файла с помощью getline.
//Если строка пустая, то она пропускается.
//Если первый символ строки равен '\f', это признак перехода на новую страницу. Если pageNumber не пустой, то он записывается в выходной файл, и переменная pageNumber очищается.
//Если это первая строка новой страницы (isNewPage равно true), то из нее извлекается номер страницы с помощью функций find_first_not_of и find_last_not_of.
//Если это не первая строка страницы, то строка записывается в выходной файл.
//После выхода из цикла, если pageNumber не пустой, он записывается в выходной файл.
//Входной и выходной файлы закрываются.

void processFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile || !outFile) {
        cout << "Ошибка при открытии файла" << endl;
        return;
    }

    string line;
    string pageNumber;
    bool isNewPage = true;

    while (getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }

        if (line[0] == '\f') {  // Признак перехода на новую страницу
            isNewPage = true;
            if (!pageNumber.empty()) {
                outFile << pageNumber << endl;
                pageNumber.clear();
            }
            continue;
        }

        if (isNewPage) {
            // Извлечение номера страницы из первой строки
            size_t start = line.find_first_not_of(" -");
            size_t end = line.find_last_not_of(" -") + 1;
            if (start != string::npos && end != string::npos && start < end) {
                pageNumber = line.substr(start, end - start);
            }
            isNewPage = false;
        } else {
            outFile << line << endl;
        }
    }

    if (!pageNumber.empty()) {
        outFile << pageNumber << endl;
    }

    inFile.close();
    outFile.close();
}