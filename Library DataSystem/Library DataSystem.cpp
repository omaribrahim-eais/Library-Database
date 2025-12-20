// Library DataSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//  OMAR'S PART 
const int MAX_BOOKS = 200;
const int MAX_LOANS = 200;

//  STRUCTS 
struct Book {
    int id;
    char title[50];
    char author[50];
};

struct Loan {
    int id;
    int book_id;
    char patron[50];
    char checkout_date[11];// YYYY-MM-DD
    char due_date[11];// YYYY-MM-DD
};

//  FILE OPERATIONS
void loadBooks(Book books[], int* count) {
    ifstream file("books.txt");
    *count = 0;
    if (!file) return;   //1|sksksa

    while (file >> books[*count].id) {
        file.ignore();
        file.getline(books[*count].title, 50, '|');
        file.getline(books[*count].author, 50);
        (*count)++;
        if (*count >= MAX_BOOKS) break;
    }
    file.close();
}

void saveBooks(Book books[], int count) {
    ofstream file("books.txt");
    for (int i = 0; i < count; i++)
        file << books[i].id << "|" << books[i].title << "|" << books[i].author << endl;
    file.close();
}

void loadLoans(Loan loans[], int* count) {
    ifstream file("loans.txt");
    *count = 0;
    if (!file) return;

    while (file >> loans[*count].id) {
        file.ignore();
        file >> loans[*count].book_id;
        file.ignore();
        file.getline(loans[*count].patron, 50, '|');
        file.getline(loans[*count].checkout_date, 11, '|');
        file.getline(loans[*count].due_date, 11);
        (*count)++;
        if (*count >= MAX_LOANS) break;
    }
    file.close();
}

void saveLoans(Loan loans[], int count) {
    ofstream file("loans.txt");
    for (int i = 0; i < count; i++)
        file << loans[i].id << "|" << loans[i].book_id << "|"
        << loans[i].patron << "|" << loans[i].checkout_date << "|"
        << loans[i].due_date << endl;
    file.close();
}

//  BOOK OPERATIONS 
void addBook(Book books[], int* count) {
    if (*count >= MAX_BOOKS) {
        cout << "Book array full.\n";
        return;
    }

    if (*count == 0)
        books[*count].id = 1;
    else
        books[*count].id = books[*count - 1].id + 1;

    cin.ignore();
    cout << "Enter title: ";
    cin.getline(books[*count].title, 50);
    cout << "Enter author: ";
    cin.getline(books[*count].author, 50);

    (*count)++;
    saveBooks(books, *count);
    cout << "Book added!\n";
}

void updateBook(Book books[], int count) {
    int id;
    cout << "Enter book ID to update: ";
    cin >> id;
    cin.ignore();

    bool found = false;
    for (int i = 0; i < count; i++) {
        if (books[i].id == id) {
            found = true;
            cout << "Enter new title: ";
            cin.getline(books[i].title, 50);
            cout << "Enter new author: ";
            cin.getline(books[i].author, 50);
            saveBooks(books, count);
            cout << "Book updated.\n";
            break;
        }
    }
    if (!found) cout << "Book not found.\n";
}

void deleteBook(Book books[], int* count, Loan loans[], int loan_count) {
    int id;
    cout << "Enter book ID to delete: ";
    cin >> id;

    for (int i = 0; i < loan_count; i++) {
        if (loans[i].book_id == id) {
            cout << "Cannot delete: Book is currently checked out.\n";
            return;
        }
    }

    bool found = false;
    for (int i = 0; i < *count; i++) {
        if (books[i].id == id) {
            found = true;
            for (int j = i; j < *count - 1; j++)
                books[j] = books[j + 1];
            (*count)--;
            saveBooks(books, *count);
            cout << "Book deleted.\n";
            break;
        }
    }
    if (!found) cout << "Book not found.\n";
}

int compareStrings(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i])
            return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}
// KAREEM'S PART 


//SEARCHING 
void searchBookByTitle(Book books[], int count) {
    char search[50];
    cin.ignore();
    cout << "Enter title to search: ";
    cin.getline(search, 50);

    bool found = false;
    for (int i = 0; i < count; i++) {
        if (compareStrings(books[i].title, search) == 0) {
            cout << "ID: " << books[i].id
                << " | Title: " << books[i].title
                << " | Author: " << books[i].author << endl;
            found = true;
        }
    }
    if (!found)
        cout << "No book with this title.\n";
}

//SORTING 
void sortBooksByTitle(Book books[], int count) {
    Book temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (compareStrings(books[i].title, books[j].title) > 0) {
                temp = books[i];
                books[i] = books[j];
                books[j] = temp;
            }
        }
    }
    saveBooks(books, count);
    cout << "Books sorted by title.\n";
}


void listBooks(Book books[], int count) {
    if (count == 0)
        cout << "No books to display.\n";
    else {
        for (int i = 0; i < count; i++)
            cout << "ID: " << books[i].id
            << " | Title: " << books[i].title
            << " | Author: " << books[i].author << endl;
    }
}

 
int nextLoanID(Loan loans[], int loan_count) {
    int max = 0;
    for (int i = 0; i < loan_count; i++)
        if (loans[i].id > max)
            max = loans[i].id;
    return max + 1;
}

void checkoutBook(Book books[], int book_count, Loan loans[], int* loan_count) {
    int book_id;
    cout << "Enter book ID to checkout: ";
    cin >> book_id;

    bool exists = false;
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            exists = true;
            break;
        }
    }
    if (!exists) {
        cout << "Book not found.\n";
        return;
    }

    for (int i = 0; i < *loan_count; i++) {
        if (loans[i].book_id == book_id) {
            cout << "This book is already checked out.\n";
            return;
        }
    }

    int index = *loan_count;
    loans[index].id = nextLoanID(loans, *loan_count);
    loans[index].book_id = book_id;

    cin.ignore();
    cout << "Enter patron name: ";
    cin.getline(loans[index].patron, 50);
    cout << "Enter checkout date: ";
    cin.getline(loans[index].checkout_date, 11);
    cout << "Enter due date: ";
    cin.getline(loans[index].due_date, 11);

    (*loan_count)++;
    saveLoans(loans, *loan_count);
    cout << "Checked out successfully.\n";
}

void returnBook(Loan loans[], int* loan_count) {
    int loan_id;
    cout << "Enter loan ID to return: ";
    cin >> loan_id;

    for (int i = 0; i < *loan_count; i++) {
        if (loans[i].id == loan_id) {
            for (int j = i; j < *loan_count - 1; j++)
                loans[j] = loans[j + 1];
            (*loan_count)--;
            saveLoans(loans, *loan_count);
            cout << "Book returned.\n";
            return;
        }
    }
    cout << "Loan not found.\n";
}


void showMenu() {
    cout << "\n1) Load  2) Save  3) Add  4) Update  5) Delete\n";
    cout << "6) List  7) Search Title  8) Sort Title\n";
    cout << "9) Checkout  10) Return  0) Exit\nChoose: ";
}


int main() {
    Book books[MAX_BOOKS];
    Loan loans[MAX_LOANS];
    int book_count = 0, loan_count = 0;

    loadBooks(books, &book_count);
    loadLoans(loans, &loan_count);

    int choice;
    do {
        showMenu();
        cin >> choice;

        if (choice == 1) loadBooks(books, &book_count);
        else if (choice == 2) saveBooks(books, book_count);
        else if (choice == 3) addBook(books, &book_count);
        else if (choice == 4) updateBook(books, book_count);
        else if (choice == 5) deleteBook(books, &book_count, loans, loan_count);
        else if (choice == 6) listBooks(books, book_count);
        else if (choice == 7) searchBookByTitle(books, book_count);
        else if (choice == 8) sortBooksByTitle(books, book_count);
        else if (choice == 9) checkoutBook(books, book_count, loans, &loan_count);
        else if (choice == 10) returnBook(loans, &loan_count);
        else if (choice == 0) cout << "Goodbye.\n";
        else cout << "Invalid option.\n";

    } while (choice != 0);

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
