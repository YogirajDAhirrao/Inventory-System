#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

#include "Inventory.h"

// ---------- Safe input helpers ----------

int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) throw std::runtime_error("Input closed.");
        try {
            size_t pos;
            int value = std::stoi(line, &pos);
            if (pos == line.size()) return value;
        } catch (const std::exception&) {}
        std::cout << "Please enter a valid whole number.\n";
    }
}

double readDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) throw std::runtime_error("Input closed.");
        try {
            size_t pos;
            double value = std::stod(line, &pos);
            if (pos == line.size()) return value;
        } catch (const std::exception&) {}
        std::cout << "Please enter a valid number.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw std::runtime_error("Input closed.");
    return line;
}

// ---------- Menu actions ----------

void addProductMenu(Inventory& inventory) {
    std::cout << "\nProduct Type:\n1. Physical\n2. Digital\n";
    int type = readInt("Enter: ");
    if (type != 1 && type != 2) {
        std::cout << "Invalid product type.\n";
        return;
    }

    int id = readInt("Product ID: ");
    std::string name = readLine("Name: ");
    double price = readDouble("Price: ");
    int quantity = readInt("Quantity: ");

    if (type == 1) {
        double weight = readDouble("Weight (kg): ");
        inventory.addProduct(std::make_unique<PhysicalProduct>(id, name, price, quantity, weight));
    } else {
        double size = readDouble("File size (MB): ");
        inventory.addProduct(std::make_unique<DigitalProduct>(id, name, price, quantity, size));
    }
    inventory.saveToFile();
    std::cout << "Product added successfully.\n";
}

void searchMenu(const Inventory& inventory) {
    std::cout << "\n1. Search by ID\n2. Search by Name\n";
    int choice = readInt("Enter: ");
    if (choice == 1) {
        inventory.searchById(readInt("Product ID: "));
    } else if (choice == 2) {
        inventory.searchByName(readLine("Name (or part of it): "));
    } else {
        std::cout << "Invalid choice.\n";
    }
}

void printMenu() {
    std::cout << "\n========== INVENTORY MANAGEMENT ==========\n\n"
              << "1. Add Product\n"
              << "2. View Products\n"
              << "3. Search Product\n"
              << "4. Update Stock\n"
              << "5. Remove Product\n"
              << "6. Exit\n\n";
}

int main() {
    Inventory inventory("inventory.txt");
    inventory.loadFromFile();

    try {
        while (true) {
            printMenu();
            int choice = readInt("Enter choice: ");

            // Each action is wrapped so one error never crashes the program
            try {
                switch (choice) {
                    case 1:
                        addProductMenu(inventory);
                        break;
                    case 2:
                        std::cout << "\n========== PRODUCTS ==========\n\n";
                        inventory.displayAll();
                        break;
                    case 3:
                        searchMenu(inventory);
                        break;
                    case 4: {
                        int id = readInt("Product ID: ");
                        int qty = readInt("New quantity: ");
                        inventory.updateStock(id, qty);
                        inventory.saveToFile();
                        std::cout << "Stock updated.\n";
                        break;
                    }
                    case 5: {
                        int id = readInt("Product ID: ");
                        inventory.removeProduct(id);
                        inventory.saveToFile();
                        std::cout << "Product removed.\n";
                        break;
                    }
                    case 6:
                        inventory.saveToFile();
                        std::cout << "Goodbye!\n";
                        return 0;
                    default:
                        std::cout << "Invalid choice. Enter 1-6.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << '\n';
            }
        }
    } catch (const std::exception&) {
        // Input stream closed (e.g. Ctrl+D): save and leave quietly
        inventory.saveToFile();
    }
    return 0;
}
