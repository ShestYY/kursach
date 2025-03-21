#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class Computer {
private:
    int number;
    string specs;
    bool isOccupied;

public:
    Computer(int num, string spec)
        : number(num), specs(spec), isOccupied(false) {
    }

    void occupy() {
        isOccupied = true;
    }

    void release() {
        isOccupied = false;
    }

    void displayInfo() const {
        cout << "Компьютер #" << number
            << "\nХарактеристики: " << specs
            << "\nСтатус: " << (isOccupied ? "Занят" : "Свободен")
            << "\n\n";
    }

    bool isAvailable() const { return !isOccupied; }
    int getNumber() const { return number; }
};

class Customer {
private:
    string name;
    Computer* computer;

public:
    Customer(string n) : name(n), computer(nullptr) {}

    void assignComputer(Computer& comp) {
        computer = &comp;
        comp.occupy();
    }

    void releaseComputer() {
        if (computer) {
            computer->release();
            computer = nullptr;
        }
    }

    string getName() const { return name; }
    Computer* getComputer() const { return computer; }
};

class Admin {
private:
    vector<Computer> computers;
    vector<Customer> customers;

public:
    Admin() {
        computers.emplace_back(1, "Intel i7, 16GB RAM, RTX 3080");
        computers.emplace_back(2, "Intel i5, 8GB RAM, GTX 1660");
        computers.emplace_back(3, "AMD Ryzen 5, 16GB RAM, RX 6700");
        computers.emplace_back(4, "Intel i3, 4GB RAM, Integrated GPU");
    }

    void displayAllComputers() const {
        cout << "\nСписок всех компьютеров:\n";
        for (const auto& comp : computers) {
            comp.displayInfo();
        }
    }

    bool addCustomerToComputer(int compNum, const string& name) {
        for (auto& comp : computers) {
            if (comp.getNumber() == compNum) {
                if (comp.isAvailable()) {
                    customers.emplace_back(name);
                    customers.back().assignComputer(comp);
                    cout << name << " добавлен к компьютеру #" << compNum << "\n";
                    return true;
                }
                cout << "Компьютер #" << compNum << " уже занят!\n";
                return false;
            }
        }
        cout << "Компьютер #" << compNum << " не найден!\n";
        return false;
    }

    bool removeCustomerFromComputer(int compNum) {
        for (auto& comp : computers) {
            if (comp.getNumber() == compNum) {
                if (!comp.isAvailable()) {
                    for (auto it = customers.begin(); it != customers.end(); ++it) {
                        if (it->getComputer() == &comp) {
                            it->releaseComputer();
                            customers.erase(it);
                            cout << "Клиент удален с компьютера #" << compNum << "\n";
                            return true;
                        }
                    }
                }
                cout << "Компьютер #" << compNum << " не занят!\n";
                return false;
            }
        }
        cout << "Компьютер #" << compNum << " не найден!\n";
        return false;
    }

    void showStatistics() const {
        int occupied = 0;
        for (const auto& comp : computers) {
            if (!comp.isAvailable()) occupied++;
        }
        cout << "\nСтатистика:\n"
            << "Всего компьютеров: " << computers.size() << "\n"
            << "Занято: " << occupied << "\n"
            << "Свободно: " << (computers.size() - occupied) << "\n";
    }
};

void adminInterface(Admin& admin) {
    int choice;
    do {
        cout << "\n=== Меню администратора ==="
            << "\n1. Добавить клиента к компьютеру"
            << "\n2. Удалить клиента с компьютера"
            << "\n3. Показать все компьютеры"
            << "\n4. Показать статистику"
            << "\n5. Выход"
            << "\nВыберите действие: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка ввода! Введите число от 1 до 5.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            int compNum;
            string name;
            cout << "Введите номер компьютера: ";
            cin >> compNum;
            cin.ignore();
            cout << "Введите имя клиента: ";
            getline(cin, name);
            admin.addCustomerToComputer(compNum, name);
            break;
        }
        case 2: {
            int compNum;
            cout << "Введите номер компьютера: ";
            cin >> compNum;
            admin.removeCustomerFromComputer(compNum);
            break;
        }
        case 3:
            admin.displayAllComputers();
            break;
        case 4:
            admin.showStatistics();
            break;
        case 5:
            cout << "Выход...\n";
            break;
        default:
            cout << "Неверный выбор!\n";
        }
    } while (choice != 5);
}

int main() {
    setlocale(LC_ALL, "Russian");
    Admin admin;
    adminInterface(admin);
    return 0;
}