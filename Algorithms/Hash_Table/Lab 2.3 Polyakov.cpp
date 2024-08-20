#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

// Динамическая хеш-таблица с открытой адресацией
// Статус ячейки: 0 - пусто, 1 - занято, 2 - было занято ранее
// Хеш-функция: метод середины квадрата
// Линейное пробирование с шагом k > 1

using namespace std;

struct Key {
    string direction_name; // Наименование направления
    string full_name;      // Полное имя
    string direction_code; // Код направления
    int status;            // Статус ячейки
    int request_number;    // Номер заявки

    // Конструкторы
    Key(string dirName, string fullName, string dirCode, int stat, int reqNumber)
        : direction_name(dirName), full_name(fullName), direction_code(dirCode), status(stat), request_number(reqNumber) {}
    Key() : status(0) {}

    // Операторы равенства
    bool operator==(const Key& other) const {
        return direction_name == other.direction_name && full_name == other.full_name &&
            request_number == other.request_number && direction_code == other.direction_code;
    }

    bool operator!=(const Key& other) const {
        return !(*this == other);
    }
};

class HashTable {
public:
    int initial_size;
    int current_size;
    int element_count;
    int collision_count; // Для отслеживания количества попыток разрешения коллизий
    vector<Key> table;

    // Конструкторы
    HashTable() : initial_size(0), current_size(0), element_count(0), collision_count(0) {}
    HashTable(int size) {
        table.resize(size);
        current_size = size;
        initial_size = size;
        element_count = 0;
        collision_count = 0;
    }

    // Хеш-функция, использующая метод середины квадрата
    int hash_function(string fio, int request) {
        int sum = 0;
        for (char ch : fio) {
            sum += ch;
        }
        sum += request;
        string str_sum = to_string(sum);

        int mod = 10;
        int count = 1;
        while (int(current_size / mod) != 0) {
            mod *= 10;
            count++;
        }

        int result = 0;
        size_t str_sum_size = str_sum.size();
        for (int i = 0; i < int(str_sum_size / count); i++) {
            result += stoi(str_sum.substr(i * count, count));
        }

        result *= result;
        string result_str = to_string(result);
        return stoi(result_str.substr(result_str.size() / 2, count));
    }

    // Получение индекса с использованием линейного пробирования
    int get_index(string fio, int request, int probe_count, int step = 2) {
        return (hash_function(fio, request) + probe_count * step) % current_size;
    }

    // Загрузка элементов из файла в хеш-таблицу
    void load_from_file(const string& path, int count) {
        ifstream file;
        string buffer;
        int elements_loaded = 0;

        file.open(path);
        if (file.is_open()) {
            while (!file.eof() && (elements_loaded <= count)) {
                Key new_key;
                file >> buffer;
                new_key.direction_code = buffer;

                file >> buffer;
                new_key.full_name = buffer;
                file >> buffer;
                new_key.full_name.append(" ").append(buffer);
                file >> buffer;
                new_key.full_name.append(" ").append(buffer);

                file >> buffer;
                new_key.direction_name = buffer;
                file >> buffer;
                new_key.request_number = stoi(buffer);

                new_key.status = 0;
                elements_loaded++;

                insert(new_key);
            }
        }
        file.close();
    }

    // Вставка элемента в хеш-таблицу
    void insert(const Key& new_key) {
        if (element_count >= 0.8 * current_size) {
            resize(current_size * 2);
        }

        int index = hash_function(new_key.full_name, new_key.request_number) % current_size;

        if (table[index].status == 0) {
            table[index] = new_key;
            table[index].status = 1;
            element_count++;
        } else {
            open_addressing_insert(new_key, index);
        }
    }

    // Разрешение коллизий методом открытой адресации
    void open_addressing_insert(const Key& new_key, int index) {
        int probe_count = 0;

        while (table[index].status == 1) {
            if (table[index] == new_key) {
                return; // Если элемент уже существует, не вставляем его
            }
            probe_count++;
            index = get_index(new_key.full_name, new_key.request_number, probe_count);

            if (probe_count >= current_size) {
                resize(current_size * 2);
                insert(new_key);
                return;
            }
        }

        // Проверка на уникальность при статусе 2
        while (table[index].status == 2) {
            int check_index = index;
            int check_probe_count = probe_count;

            while (table[check_index].status != 0) {
                if (table[check_index].status == 1 && table[check_index] == new_key) {
                    return; // Повторяющийся
                }
                check_probe_count++;
                check_index = get_index(new_key.full_name, new_key.request_number, check_probe_count);

                if (check_probe_count >= current_size) {
                    resize(current_size * 2);
                    insert(new_key);
                    return;
                }
            }
            break;
        }

        table[index] = new_key;
        table[index].status = 1;
        element_count++;
    }

    // Удаление элемента
    void remove(const Key& key) {
        int index = hash_function(key.full_name, key.request_number) % current_size;

        if (table[index].status == 1 && table[index] == key) {
            table[index].status = 2;
            element_count--;
        } else {
            open_addressing_remove(key, index);
        }

        if (element_count <= 0.2 * current_size && current_size / 2 >= initial_size) {
            resize(current_size / 2);
        }
    }

    void open_addressing_remove(const Key& key, int index) {
        int probe_count = 0;

        while (table[index].status != 0) {
            if (table[index] == key && table[index].status == 1) {
                table[index].status = 2;
                element_count--;
                return;
            }
            probe_count++;
            index = get_index(key.full_name, key.request_number, probe_count);

            if (probe_count >= current_size) {
                return; // Элемент не найден
            }
        }
    }

    // Поиск элемента
    int find(const string& fio, int request) {
        int probe_count = 0;
        int index = hash_function(fio, request) % current_size;

        if (table[index].status == 1 && table[index].full_name == fio && table[index].request_number == request) {
            return probe_count;
        } else {
            return open_addressing_find(fio, request, index);
        }
    }

    int open_addressing_find(const string& fio, int request, int index) {
        int probe_count = 0;

        while (table[index].status != 0) {
            if (table[index].full_name == fio && table[index].request_number == request && table[index].status == 1) {
                return probe_count;
            }
            probe_count++;
            index = get_index(fio, request, probe_count);

            if (probe_count >= current_size) {
                return -1; // Элемент не найден
            }
        }

        return -1; // Элемент не найден
    }


    // Изменение размера хеш-таблицы
    void resize(int new_size) {
        vector<Key> old_table = table;
        table.clear();
        table.resize(new_size);
        current_size = new_size;
        element_count = 0;

        for (const auto& key : old_table) {
            if (key.status == 1) {
                insert(key);
            }
        }
    }

    // Вывод хеш-таблицы
    void print() {
        int count = 0;
        for (int i = 0; i < current_size; i++) {
            if (table[i].status != 0) {
                count++;
                cout << setw(2) << i << setw(10) << table[i].request_number
                    << setw(35) << table[i].full_name << setw(14) << table[i].direction_code
                    << setw(30) << table[i].direction_name << setw(5) << get_index(table[i].full_name, table[i].request_number, 0)
                    << setw(5) << get_index(table[i].full_name, table[i].request_number, 1)
                    << setw(15) << "Статус: " << table[i].status << "\n";
            }
        }
        cout << element_count << "               " << current_size << "\n";
    }
};


int main() {
    HashTable hashTable(13);
    hashTable.load_from_file("C:/Users/User/Desktop/FEFU/Second_course/FDSA/Hash_Table/test1.txt", 1);
    hashTable.print();
    int index;
    //while (hashTable.element_count != 0) {
    //    cin >> index;
    //    if (index == -1) {
    //        break;
    //    }
    //    if (hashTable.find(hashTable.table[index].full_name, hashTable.table[index].request_number) == -1) {
    //        cout << "Student not found." << "\n";
    //    } else {
    //        cout << "Student found. Probe count: " << hashTable.find(hashTable.table[index].full_name, hashTable.table[index].request_number) << "\n";
    //    }
    //    hashTable.print();
    //}

    hashTable.print();
    while (hashTable.element_count != 0) {
        cin >> index;
        if (index == -1) {
            break;
        }
        hashTable.remove(hashTable.table[index]);
        hashTable.print();
    }
    hashTable.print();

    while (hashTable.element_count != 0) {
        cin >> index;
        if (index == -1) {
            break;
        }
        if (hashTable.find(hashTable.table[index].full_name, hashTable.table[index].request_number) == -1) {
            cout << "Student not found." << "\n";
        } else {
            cout << "Student found. Probe count: " << hashTable.find(hashTable.table[index].full_name, hashTable.table[index].request_number) << "\n";
        }
        hashTable.print();
    }
    // B7890 Wilson Pavel Vladimirovich himbio 1101
    //Key newElement1("himbio", "Wilson Pavel Vladimirovich", "B7890", 1, 1101);
    //hashTable.insert(newElement1);
    hashTable.print();
    Key newElement("menegment", "Lee Maxim Alexandrovich", "B2253", 1, 2757);
    hashTable.insert(newElement);



    //Key newElement10("menegment", "Lee Maxim Alexandrovichq", "B2253", 1, 2757);
    //Key newElement2("menegment", "Lee Maxim Alexandrovichw", "B2253", 1, 2757);
    //Key newElement3("menegment", "Lee Maxim Alexandroviche", "B2253", 1, 2757);
    //Key newElement4("menegment", "Lee Maxim Alexandrovichr", "B2253", 1, 2757);
    //Key newElement5("menegment", "Lee Maxim Alexandrovicht", "B2253", 1, 2757);
    //Key newElement6("menegment", "Lee Maxim Alexandrovichy", "B2253", 1, 2757);
    //Key newElement7("menegment", "Lee Maxim Alexandrovichu", "B2253", 1, 2757);
    //Key newElement8("menegment", "Lee Maxim Alexandrovichi", "B2253", 1, 2757);
    //hashTable.insert(newElement10);
    //hashTable.insert(newElement2);
   //
    //hashTable.insert(newElement3);
    //hashTable.insert(newElement4);
    //
    //hashTable.insert(newElement5);
    //hashTable.print();
    //hashTable.insert(newElement6);
    //hashTable.insert(newElement7);
    //hashTable.insert(newElement8);

    hashTable.print();
    while (hashTable.element_count != 0) {
        cin >> index;
        if (index == -1) {
            break;
        }
        hashTable.remove(hashTable.table[index]);
        hashTable.print();
    }
    hashTable.print();

    return 0;
}