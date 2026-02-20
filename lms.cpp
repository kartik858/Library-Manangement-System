#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

class Book {
public:
    string bno;
    string bname;
    string aname;
    bool issued = false;

    void create() {
        cout << "\nNEW BOOK ENTRY\n";
        cout << "Enter Book No: ";
        cin >> bno;
        cin.ignore();

        cout << "Enter Book Name: ";
        getline(cin, bname);

        cout << "Enter Author Name: ";
        getline(cin, aname);
    }

    void display() const {
        cout << left << setw(10) << bno
             << setw(25) << bname
             << setw(20) << aname
             << setw(10) << (issued ? "Yes" : "No")
             << endl;
    }

    void save(ofstream& out) const {
        out << bno << '\n'
            << bname << '\n'
            << aname << '\n'
            << issued << '\n';
    }

    bool load(ifstream& in) {
        if (!getline(in, bno)) return false;
        getline(in, bname);
        getline(in, aname);
        in >> issued;
        in.ignore();
        return true;
    }
};

class Student {
public:
    string admno;
    string name;
    string issuedBook;

    void create() {
        cout << "\nNEW STUDENT ENTRY\n";
        cout << "Enter Admission No: ";
        cin >> admno;
        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, name);

        issuedBook = "";
    }

    void display() const {
        cout << left << setw(15) << admno
             << setw(25) << name
             << setw(15) << (issuedBook.empty() ? "None" : issuedBook)
             << endl;
    }

    void save(ofstream& out) const {
        out << admno << '\n'
            << name << '\n'
            << issuedBook << '\n';
    }

    bool load(ifstream& in) {
        if (!getline(in, admno)) return false;
        getline(in, name);
        getline(in, issuedBook);
        return true;
    }
};

vector<Book> loadBooks() {
    vector<Book> books;
    ifstream in("books.txt");
    while (in) {
        Book b;
        if (b.load(in))
            books.push_back(b);
    }
    return books;
}

void saveBooks(const vector<Book>& books) {
    ofstream out("books.txt");
    for (const auto& b : books)
        b.save(out);
}

vector<Student> loadStudents() {
    vector<Student> students;
    ifstream in("students.txt");
    while (in) {
        Student s;
        if (s.load(in))
            students.push_back(s);
    }
    return students;
}

void saveStudents(const vector<Student>& students) {
    ofstream out("students.txt");
    for (const auto& s : students)
        s.save(out);
}

void addBook() {
    auto books = loadBooks();
    Book b;
    b.create();

    for (const auto& bk : books) {
        if (bk.bno == b.bno) {
            cout << "Book number already exists.\n";
            return;
        }
    }

    books.push_back(b);
    saveBooks(books);
}

void addStudent() {
    auto students = loadStudents();
    Student s;
    s.create();

    for (const auto& st : students) {
        if (st.admno == s.admno) {
            cout << "Admission number already exists.\n";
            return;
        }
    }

    students.push_back(s);
    saveStudents(students);
}

void displayBooks() {
    auto books = loadBooks();

    cout << "\nBOOK LIST\n";
    cout << left << setw(10) << "No"
         << setw(25) << "Name"
         << setw(20) << "Author"
         << setw(10) << "Issued"
         << endl;

    for (const auto& b : books)
        b.display();
}

void displayStudents() {
    auto students = loadStudents();

    cout << "\nSTUDENT LIST\n";
    cout << left << setw(15) << "Adm No"
         << setw(25) << "Name"
         << setw(15) << "Book"
         << endl;

    for (const auto& s : students)
        s.display();
}

void issueBook() {
    auto books = loadBooks();
    auto students = loadStudents();

    string admno, bno;

    cout << "Enter Admission No: ";
    cin >> admno;

    Student* studentPtr = nullptr;
    for (auto& s : students) {
        if (s.admno == admno) {
            studentPtr = &s;
            break;
        }
    }

    if (!studentPtr) {
        cout << "Student not found.\n";
        return;
    }

    if (!studentPtr->issuedBook.empty()) {
        cout << "Student already has a book.\n";
        return;
    }

    cout << "Enter Book No: ";
    cin >> bno;

    Book* bookPtr = nullptr;
    for (auto& b : books) {
        if (b.bno == bno) {
            bookPtr = &b;
            break;
        }
    }

    if (!bookPtr) {
        cout << "Book not found.\n";
        return;
    }

    if (bookPtr->issued) {
        cout << "Book already issued.\n";
        return;
    }

    bookPtr->issued = true;
    studentPtr->issuedBook = bno;

    saveBooks(books);
    saveStudents(students);

    cout << "Book issued successfully.\n";
}

void returnBook() {
    auto books = loadBooks();
    auto students = loadStudents();

    string admno;
    cout << "Enter Admission No: ";
    cin >> admno;

    for (auto& s : students) {
        if (s.admno == admno) {
            if (s.issuedBook.empty()) {
                cout << "No book issued.\n";
                return;
            }

            for (auto& b : books) {
                if (b.bno == s.issuedBook) {
                    b.issued = false;
                    break;
                }
            }

            s.issuedBook = "";
            saveBooks(books);
            saveStudents(students);

            cout << "Book returned successfully.\n";
            return;
        }
    }

    cout << "Student not found.\n";
}

int main() {
    int choice;

    do {
        cout << "\nLIBRARY MANAGEMENT SYSTEM\n";
        cout << "1. Add Student\n";
        cout << "2. Add Book\n";
        cout << "3. Display Students\n";
        cout << "4. Display Books\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: addBook(); break;
            case 3: displayStudents(); break;
            case 4: displayBooks(); break;
            case 5: issueBook(); break;
            case 6: returnBook(); break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice\n";
        }

    } while (choice != 7);

    return 0;
}