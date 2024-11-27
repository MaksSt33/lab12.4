#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ��������� ��� ��������� ����� ��� ������
struct Zodiac {
    string surname;      // �������
    string name;         // ��'�
    string zodiac_sign;  // ���� ������
    int birthday[3];     // ���� ����������: [0] - ����, [1] - �����, [2] - ��
};

// ������� ��� ������ ��'���� Zodiac � ����
void writeToBinary(ofstream& file, const Zodiac& person) {
    size_t length;

    // �������� ������� ������� �� ���� �������
    length = person.surname.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(person.surname.c_str(), length);

    // �������� ������� ���� �� ���� ��'�
    length = person.name.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(person.name.c_str(), length);

    // �������� ������� ����� ������ �� ��� ����
    length = person.zodiac_sign.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(person.zodiac_sign.c_str(), length);

    // �������� ����� � ����, ������ � ����� ����������
    file.write(reinterpret_cast<const char*>(person.birthday), sizeof(person.birthday));
}

// ������� ��� ���������� ��'���� Zodiac �� �����
void readFromBinary(ifstream& file, Zodiac& person) {
    size_t length;
    char buffer[256];

    // ������� ������� �� ���� �������
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    file.read(buffer, length);
    buffer[length] = '\0';  // ������ ���������� ���� ��� �����
    person.surname = buffer;

    // ������� ������� �� ���� ��'�
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    file.read(buffer, length);
    buffer[length] = '\0';
    person.name = buffer;

    // ������� ������� �� ���� ������
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    file.read(buffer, length);
    buffer[length] = '\0';
    person.zodiac_sign = buffer;

    // ������� ����� �� ����, ������ � ����� ����������
    file.read(reinterpret_cast<char*>(person.birthday), sizeof(person.birthday));
}

// ������� ��� ��������� ���� ������ � ����
void addPersonToFile(const string& filename) {
    // ³�������� ���� ��� ��������� (����� `app` ������, �� ��� ������ ��������� � ����� �����)
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cout << "�� ������� ������� ���� ��� ������.\n";
        return;
    }

    Zodiac person;
    // ������� ��� �� �����������
    cout << "������ �������: ";
    cin >> person.surname;
    cout << "������ ��'�: ";
    cin >> person.name;
    cout << "������ ���� ������: ";
    cin >> person.zodiac_sign;
    cout << "������ ���� ���������� (����, �����, ��): ";
    cin >> person.birthday[0] >> person.birthday[1] >> person.birthday[2];

    // �������� �� ��� � ����
    writeToBinary(file, person);
    file.close();
    cout << "��� ������ � ����.\n";
}

// ������� ��� ���������� ������ � ���� �� ������ ��� ����������
void sortFileByBirthday(const string& filename) {
    // ³�������� ���� ��� �������
    ifstream file_in(filename, ios::binary);
    if (!file_in) {
        cout << "�� ������� ������� ���� ��� �������.\n";
        return;
    }

    // ��������� ������� ������ � ����
    file_in.seekg(0, ios::end);
    size_t fileSize = file_in.tellg();
    file_in.seekg(0, ios::beg);

    size_t recordCount = fileSize / sizeof(Zodiac);  // �������� ������� ������
    Zodiac* people = new Zodiac[recordCount];       // ��������� ����� ��� ��������� ������

    // ������� �� ������ � �����
    for (size_t i = 0; i < recordCount; ++i) {
        readFromBinary(file_in, people[i]);
    }
    file_in.close();

    // ������� ������ �� ����� ���������� (��������� ��, �����, ����)
    for (size_t i = 0; i < recordCount - 1; ++i) {
        for (size_t j = i + 1; j < recordCount; ++j) {
            if (people[i].birthday[2] > people[j].birthday[2] ||
                (people[i].birthday[2] == people[j].birthday[2] && people[i].birthday[1] > people[j].birthday[1]) ||
                (people[i].birthday[2] == people[j].birthday[2] && people[i].birthday[1] == people[j].birthday[1] && people[i].birthday[0] > people[j].birthday[0])) {
                swap(people[i], people[j]);  // ̳����� ������ ������
            }
        }
    }

    // ������������ ���� � ������������� ������
    ofstream file_out(filename, ios::binary | ios::trunc);
    for (size_t i = 0; i < recordCount; ++i) {
        writeToBinary(file_out, people[i]);
    }
    file_out.close();
    delete[] people;  // ��������� ���'���

    cout << "���� ����������� �� ������ ����������.\n";
}

// ������� ��� ������ ������ �� ��������
void findPersonBySurname(const string& filename, const string& surname) {
    // ³�������� ���� ��� �������
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "�� ������� ������� ���� ��� �������.\n";
        return;
    }

    bool found = false;  // ����� ��� ���������� ������� ������
    Zodiac person;

    // ����������� �� ������ � ����
    while (file.peek() != EOF) {  // ����������, �� �� ������� ���� �����
        readFromBinary(file, person);
        if (person.surname == surname) {  // ���� ������� ��������
            cout << "�������: " << person.surname
                << ", ��'�: " << person.name
                << ", ���� ������: " << person.zodiac_sign
                << ", ���� ����������: " << person.birthday[0] << "."
                << person.birthday[1] << "." << person.birthday[2] << "\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "������ � ����� �������� �� ��������.\n";
    }
    file.close();
}

// ������� ��� ��������� ����
void displayMenu() {
    cout << "\n--- ���� ---\n";
    cout << "1. ������ ��� � ����\n";
    cout << "2. ³���������� ������ �� ������ ����������\n";
    cout << "3. ������ ������ �� ��������\n";
    cout << "4. �����\n";
    cout << "������ ����� ����: ";
}

// ������� �������
int main() {
    string filename;
    cout << "������ ��'� �����: ";
    cin >> filename;

    bool running = true;
    while (running) {
        displayMenu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            addPersonToFile(filename);  // ������ ������
            break;
        case 2:
            sortFileByBirthday(filename);  // ��������� ����
            break;
        case 3: {
            string surname;
            cout << "������ ������� ��� ������: ";
            cin >> surname;
            findPersonBySurname(filename, surname);  // ������ ������ �� ��������
            break;
        }
        case 4:
            running = false;  // ��������� ��������
            break;
        default:
            cout << "������� ����. ��������� �� ���.\n";
        }
    }

    return 0;
}
