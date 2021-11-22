#include <iostream>
#include <functional>
#include <optional>
#include <vector>
#include <algorithm>
#include <variant>
#include <fstream>
#include <string>

using namespace std;
//firstTask
class Person {
private:
    string _lastName;
    string _name;
    optional<string> _surName;
public:
    Person(const string& lastName, const string& name, optional<string> surName = nullopt) : _lastName(lastName), _name(name), _surName(surName) {};

    friend bool operator<(const Person& left, const Person& right) {
        return tie(left._lastName, left._name, left._surName) < tie(right._lastName, right._name, right._surName);
    };

    friend bool operator==(const Person& left, const Person& right) {
        return tie(left._lastName, left._name, left._surName) == tie(right._lastName, right._name, right._surName);
    };

    friend ostream& operator<< (ostream& out, const Person& person) {
        out.width(12);
        out.fill(' ');
        out << right << person._lastName;
        out.width(15);
        out.fill(' ');
        out << right << person._name;
        out.width(20);
        out.fill(' ');
        if (person._surName.has_value())
            out << right << *person._surName;
        else
            out << right << " ";
        out.width(5);
        out.fill(' ');
        out << " ";
        return out;
    };

    const string& getLastName() const { return _lastName; };
};

class PhoneNumber {
private:
    size_t _countryCode;
    size_t _cityCode;
    string _number;
    optional<size_t> _additinalNumber;
public:
    PhoneNumber(const size_t& countryCode, const size_t& cityCode, const string& number, optional<size_t> additionalNumber = nullopt) : _countryCode(countryCode), _cityCode(cityCode), _number(number), _additinalNumber(additionalNumber) {};

    friend ostream& operator<< (ostream& out, const PhoneNumber& phoneNumber) {
        out << "+" << phoneNumber._countryCode << "(" << phoneNumber._cityCode << ")" << phoneNumber._number;
        if (phoneNumber._additinalNumber.has_value())
            out << " " << *phoneNumber._additinalNumber;
        return out;
    };

    friend bool operator<(const PhoneNumber& left, const PhoneNumber& right) {
        return tie(left._countryCode, left._cityCode, left._number, left._additinalNumber) < tie(right._countryCode, right._cityCode, right._number, right._additinalNumber);
    };

    friend bool operator==(const PhoneNumber& left, const PhoneNumber& right) {
        return tie(left._countryCode, left._cityCode, left._number, left._additinalNumber) == tie(right._countryCode, right._cityCode, right._number, right._additinalNumber);
    };
};

class PhoneBook {
private:
    vector<pair<Person, PhoneNumber>> _phoneBook;
public:
    explicit PhoneBook(ifstream& in) {
        if (!in.is_open())
            throw std::runtime_error("File not found");
        while (!in.eof()) {
            string name;
            string lastName;
            string surName;
            int countryCode;
            int cityCode;
            string number;
            string additionalNumber;
            in >> lastName >> name >> surName >> countryCode >> cityCode >> number >> additionalNumber;
            _phoneBook.push_back(pair<Person, PhoneNumber>(Person(lastName, name, surName == "-" ? nullopt : optional<string>(surName)), PhoneNumber(countryCode, cityCode, number, additionalNumber == "-" ? nullopt : optional<size_t>(atoi(additionalNumber.c_str())))));
        }
        in.close();
    };

    friend ostream& operator<< (ostream& out, const PhoneBook& phoneBook) {
        for (const auto& [person, phoneNumber] : phoneBook._phoneBook)
            out << person << " " << phoneNumber << endl;
        return out;
    };

    void SortByName() {
        sort(_phoneBook.begin(), _phoneBook.end(), [](const auto& left, const auto& right) { return left.first < right.first; });
    }

    void SortByPhone() {
        sort(_phoneBook.begin(), _phoneBook.end(), [](const auto& left, const auto& right) { return left.second < right.second; });
    }

    const auto GetPhoneNumber(const string& lastName) const {
        PhoneNumber resultPhone(0, 0, "", nullopt);
        string resultString = "";
        int _count = 0;
        for_each(_phoneBook.begin(), _phoneBook.end(), [&_count, lastName, &resultPhone](const auto& row) {
            if (row.first.getLastName()._Equal(lastName)) {
                resultPhone = row.second;
                _count++;
            }
            });
        resultString = _count == 0 ? "not found" : (_count == 1 ? "" : "found more than 1");
        return make_tuple(resultString, resultPhone);
    };

    void ChangePhoneNumber(const Person& person, const PhoneNumber& phoneNumber) {
        auto found = find_if(_phoneBook.begin(), _phoneBook.end(), [person](const auto& row) { return row.first == person; });
        if (found != _phoneBook.end())
            found->second = phoneNumber;
    }

    auto& operator[] (int index) {
        if (index >= _phoneBook.size())
            throw std::out_of_range("Index out of range");
        return _phoneBook[index];
    };
};

void firstTask()
{
    ifstream file("data.txt");
    PhoneBook book(file);
    cout << book;

    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;

    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    cout << "-----GetPhoneNumber-----" << endl;
    // лямбда функция, которая принимает фамилию и выводит номер телефона этого    	человека, либо строку с ошибкой
    auto print_phone_number = [&book](const string& surname) {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            cout << get<1>(answer);
        else
            cout << get<0>(answer);
        cout << endl;
    };

    // вызовы лямбды
    print_phone_number("Ivanov");
    print_phone_number("Petrov");

    cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;
}
//firstTask
//testing
#define ASSERT_TRUE(expression)                   \
    testing::assert_true((expression), __func__)  \

#define ASSERT_FALSE(expression)                  \
    testing::assert_false((expression), __func__) \

#define ASSERT_EQUALS(lhs, rhs)                       \
    testing::assert_equals((lhs), (rhs), __func__)    \

#define EXCEPTION_WAITER(code, exception)                        \
    do{                                                          \
        try{                                                     \
            code;                                                \
            cerr << "Test [" << __func__ << "] failed" << endl;  \
        }                                                        \
        catch(const exception&){                                 \
            cout << "Test [" << __func__ << "] passed" << endl;  \
        }                                                        \
    }while(0)                                                    \



namespace testing {
    void assert_true(bool expression, const string& function) {
        if (expression)
            cout << "Test [" << function << "] passed" << endl;
        else
            cerr << "Test [" << function << "] failed" << endl;
    };

    void assert_false(bool expression, const string& function) {
        if (!expression)
            cout << "Test [" << function << "] passed" << endl;
        else
            cerr << "Test [" << function << "] failed" << endl;
    };

    template <typename T>
    void assert_equals(const T& lhs, const T& rhs, const string& function) {
        if (lhs == rhs)
            cout << "Test [" << function << "] passed" << endl;
        else
            cerr << "Test [" << function << "] failed" << endl;
    };
};

void unit_test_phonebook_file_wrong() {
    EXCEPTION_WAITER(
        {
            ifstream file("nofile.txt");
            PhoneBook book(file);
        },
        runtime_error
            );
};

void unit_test_sort_by_phone() {
    ifstream file("test_phones.txt");
    PhoneBook book(file);
    book.SortByPhone();
    ASSERT_EQUALS(book[0].second, PhoneNumber(7, 17, "4559767"));
};

void unit_test_sort_by_name() {
    ifstream file("test_phones.txt");
    PhoneBook book(file);
    book.SortByPhone();
    ASSERT_EQUALS(book[0].first, Person("Ilin", "Petr", "Artemovich"));
};

void unit_test_phonebook_index_operator() {
    EXCEPTION_WAITER(
        {
            ifstream file("test_phones.txt");
            PhoneBook book(file);
            book[2];
        },
        out_of_range
            );
};

void unit_test_phonebook_equal_operator() {
    ifstream file("test_phones.txt");
    PhoneBook book(file);
    ASSERT_TRUE(book[0] == book[0]);
};

void unit_test_phonebook_nonequal_operator() {
    ifstream file("test_phones.txt");
    PhoneBook book(file);
    ASSERT_FALSE(book[0] < book[0]);
};

void all_test() {
    unit_test_phonebook_file_wrong();
    unit_test_sort_by_phone();
    unit_test_sort_by_name();
    unit_test_phonebook_index_operator();
    unit_test_phonebook_equal_operator();
    unit_test_phonebook_nonequal_operator();
}
//testing
int main()
{
    //firstTask();
    all_test();
}