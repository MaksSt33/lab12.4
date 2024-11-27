#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Структура для зберігання даних про людину
struct Zodiac {
    string surname;      // Прізвище
    string name;         // Ім'я
    string zodiac_sign;  // Знак Зодіаку
    int birthday[3];     // День народження: [0] - день, [1] - місяць, [2] - рік
};

// Функція для запису об'єкта Zodiac у файл
void writeToBinary(ofstream& file, const Zodiac& person) {
    size_t length;

    // Записуємо довжину прізвища та саме прізвище
    length = person.surname.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(person.surname.c_str(), length);

    // Записуємо довжину імені та саме ім'я
    length = person.name.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(person.name.c_str(), length);

    // Записуємо довжину знаку Зодіаку та сам знак
    length = person.zodiac_sign.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(person.zodiac_sign.c_str(), length);

    // Записуємо масив з днем, місяцем і роком народження
    file.write(reinterpret_cast<const char*>(person.birthday), sizeof(person.birthday));
}

// Функція для зчитування об'єкта Zodiac із файлу
void readFromBinary(ifstream& file, Zodiac& person) {
    size_t length;
    char buffer[256];

    // Зчитуємо довжину та саме прізвище
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    file.read(buffer, length);
    buffer[length] = '\0';  // Додаємо термінуючий нуль для рядка
    person.surname = buffer;

    // Зчитуємо довжину та саме ім'я
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    file.read(buffer, length);
    buffer[length] = '\0';
    person.name = buffer;

    // Зчитуємо довжину та знак Зодіаку
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    file.read(buffer, length);
    buffer[length] = '\0';
    person.zodiac_sign = buffer;

    // Зчитуємо масив із днем, місяцем і роком народження
    file.read(reinterpret_cast<char*>(person.birthday), sizeof(person.birthday));
}

// Функція для додавання нової людини у файл
void addPersonToFile(const string& filename) {
    // Відкриваємо файл для додавання (режим `app` означає, що нові записи додаються в кінець файлу)
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cout << "Не вдалося відкрити файл для запису.\n";
        return;
    }

    Zodiac person;
    // Вводимо дані від користувача
    cout << "Введіть прізвище: ";
    cin >> person.surname;
    cout << "Введіть ім'я: ";
    cin >> person.name;
    cout << "Введіть знак Зодіаку: ";
    cin >> person.zodiac_sign;
    cout << "Введіть день народження (день, місяць, рік): ";
    cin >> person.birthday[0] >> person.birthday[1] >> person.birthday[2];

    // Записуємо ці дані у файл
    writeToBinary(file, person);
    file.close();
    cout << "Дані додано у файл.\n";
}

// Функція для сортування записів у файлі за датами днів народження
void sortFileByBirthday(const string& filename) {
    // Відкриваємо файл для читання
    ifstream file_in(filename, ios::binary);
    if (!file_in) {
        cout << "Не вдалося відкрити файл для читання.\n";
        return;
    }

    // Визначаємо кількість записів у файлі
    file_in.seekg(0, ios::end);
    size_t fileSize = file_in.tellg();
    file_in.seekg(0, ios::beg);

    size_t recordCount = fileSize / sizeof(Zodiac);  // Загальна кількість записів
    Zodiac* people = new Zodiac[recordCount];       // Динамічний масив для зберігання записів

    // Зчитуємо всі записи у масив
    for (size_t i = 0; i < recordCount; ++i) {
        readFromBinary(file_in, people[i]);
    }
    file_in.close();

    // Сортуємо записи за датою народження (порівнюємо рік, місяць, день)
    for (size_t i = 0; i < recordCount - 1; ++i) {
        for (size_t j = i + 1; j < recordCount; ++j) {
            if (people[i].birthday[2] > people[j].birthday[2] ||
                (people[i].birthday[2] == people[j].birthday[2] && people[i].birthday[1] > people[j].birthday[1]) ||
                (people[i].birthday[2] == people[j].birthday[2] && people[i].birthday[1] == people[j].birthday[1] && people[i].birthday[0] > people[j].birthday[0])) {
                swap(people[i], people[j]);  // Міняємо записи місцями
            }
        }
    }

    // Перезаписуємо файл з відсортованими даними
    ofstream file_out(filename, ios::binary | ios::trunc);
    for (size_t i = 0; i < recordCount; ++i) {
        writeToBinary(file_out, people[i]);
    }
    file_out.close();
    delete[] people;  // Звільняємо пам'ять

    cout << "Файл відсортовано за датами народження.\n";
}

// Функція для пошуку людини за прізвищем
void findPersonBySurname(const string& filename, const string& surname) {
    // Відкриваємо файл для читання
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Не вдалося відкрити файл для читання.\n";
        return;
    }

    bool found = false;  // Змінна для збереження статусу пошуку
    Zodiac person;

    // Переглядаємо всі записи у файлі
    while (file.peek() != EOF) {  // Перевіряємо, чи не досягли кінця файлу
        readFromBinary(file, person);
        if (person.surname == surname) {  // Якщо прізвище збігається
            cout << "Прізвище: " << person.surname
                << ", Ім'я: " << person.name
                << ", Знак Зодіаку: " << person.zodiac_sign
                << ", День народження: " << person.birthday[0] << "."
                << person.birthday[1] << "." << person.birthday[2] << "\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Людина з таким прізвищем не знайдена.\n";
    }
    file.close();
}

// Функція для виведення меню
void displayMenu() {
    cout << "\n--- Меню ---\n";
    cout << "1. Додати дані у файл\n";
    cout << "2. Відсортувати записи за датами народження\n";
    cout << "3. Знайти людину за прізвищем\n";
    cout << "4. Вийти\n";
    cout << "Оберіть пункт меню: ";
}

// Головна функція
int main() {
    string filename;
    cout << "Введіть ім'я файлу: ";
    cin >> filename;

    bool running = true;
    while (running) {
        displayMenu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            addPersonToFile(filename);  // Додати людину
            break;
        case 2:
            sortFileByBirthday(filename);  // Сортувати файл
            break;
        case 3: {
            string surname;
            cout << "Введіть прізвище для пошуку: ";
            cin >> surname;
            findPersonBySurname(filename, surname);  // Знайти людину за прізвищем
            break;
        }
        case 4:
            running = false;  // Завершити програму
            break;
        default:
            cout << "Невірний вибір. Спробуйте ще раз.\n";
        }
    }

    return 0;
}
