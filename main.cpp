#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// 1. Book Class Definition
class Book {
private:
    std::string title;
    std::string author;
    double price;

public:
    // Constructor to initialize members
    Book(std::string t, std::string a, double p) 
        : title(t), author(a), price(p) {}

    // Display book information
    void displayInfo() const {
        std::cout << "Title: " << title 
                  << " | Author: " << author 
                  << " | Price: $" << std::fixed << std::setprecision(2) << price << "\n";
    }

    // Optional getter if needed
    std::string getTitle() const {
        return title;
    }
};

// 2. Inventory Class Definition (RAII Pattern)
class Inventory {
private:
    std::vector<Book*> books; // Holds pointers to dynamically allocated Book objects

public:
    // Destructor: CRITICAL for RAII. Frees all dynamically allocated memory.
    ~Inventory() {
        std::cout << "\n[RAII Cleanup] Inventory is being destroyed. Deallocating all books...\n";
        for (Book* bookPtr : books) {
            delete bookPtr; // Deallocate heap memory
        }
        books.clear(); // Clear the vector
    }

    // Add a dynamically allocated Book pointer
    void addBook(Book* bookPtr) {
        if (bookPtr != nullptr) {
            books.push_back(bookPtr);
            std::cout << "Success: Book added to inventory.\n";
        }
    }

    // Display all books in the inventory
    void displayAll() const {
        if (books.empty()) {
            std::cout << "\nThe inventory is currently empty.\n";
            return;
        }

        std::cout << "\n--- Current Book Inventory ---\n";
        for (size_t i = 0; i < books.size(); ++i) {
            std::cout << "[" << i << "] ";
            books[i]->displayInfo(); // Using arrow operator for pointers
        }
        std::cout << "------------------------------\n";
    }

    // Remove a book at a given index and ensure memory is deleted first
    void removeBook(int index) {
        if (index < 0 || static_cast<size_t>(index) >= books.size()) {
            std::cout << "Error: Invalid index.\n";
            return;
        }

        // CRITICAL: Delete the pointer to prevent memory leaks before erasing from vector
        delete books[index];
        books.erase(books.begin() + index);
        std::cout << "Success: Book at index " << index << " removed and memory freed.\n";
    }
};

// 3. Main Program Flow with Menu-Driven Interface
int main() {
    // Declare a single Inventory store object on the stack
    Inventory store;
    int choice = 0;

    do {
        std::cout << "\n=== Book Inventory Menu ===\n";
        std::cout << "1. Add New Book\n";
        std::cout << "2. Display All Books\n";
        std::cout << "3. Remove Book by Index\n";
        std::cout << "4. Quit\n";
        std::cout << "Enter your choice (1-4): ";
        
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(); // Clear newline buffer

        switch (choice) {
            case 1: {
                std::string title, author;
                double price;

                std::cout << "Enter book title: ";
                std::getline(std::cin, title);
                std::cout << "Enter author name: ";
                std::getline(std::cin, author);
                std::cout << "Enter price: $";
                while (!(std::cin >> price)) {
                    std::cout << "Invalid price. Enter price: $";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                }
                std::cin.ignore();

                // Dynamically allocate a new Book object using 'new'
                Book* newBook = new Book(title, author, price);
                store.addBook(newBook);
                break;
            }
            case 2:
                store.displayAll();
                print:
                break;
            case 3: {
                store.displayAll();
                int index;
                std::cout << "Enter the index of the book to remove: ";
                if (std::cin >> index) {
                    store.removeBook(index);
                } else {
                    std::cout << "Invalid index format.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                }
                break;
            }
            case 4:
                std::cout << "Exiting program...\n";
                break;
            default:
                std::cout << "Invalid choice. Please select between 1 and 4.\n";
        }
    } while (choice != 4);

    // When main exits, 'store' goes out of scope, triggering its destructor automatically!
    return 0;
}
