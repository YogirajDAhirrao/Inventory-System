#include "Inventory.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

void printHeader() {
    std::cout << std::left
              << std::setw(8)  << "ID"
              << std::setw(20) << "Name"
              << std::setw(10) << "Type"
              << std::setw(12) << "Price"
              << std::setw(10) << "Quantity"
              << "Details\n"
              << std::string(70, '-') << '\n';
}

void printRow(const Product& p) {
    std::cout << std::left << std::fixed << std::setprecision(2)
              << std::setw(8)  << p.getId()
              << std::setw(20) << p.getName()
              << std::setw(10) << p.getType()
              << std::setw(12) << p.getPrice()
              << std::setw(10) << p.getQuantity()
              << p.getDetails() << '\n';
}
}  // namespace

Inventory::Inventory(const std::string& fileName) : fileName(fileName) {}

int Inventory::findIndexById(int id) const {
    for (size_t i = 0; i < products.size(); ++i)
        if (products[i]->getId() == id) return static_cast<int>(i);
    return -1;
}

void Inventory::addProduct(std::unique_ptr<Product> product) {
    if (findIndexById(product->getId()) != -1)
        throw std::runtime_error("A product with this ID already exists.");
    products.push_back(std::move(product));
}

void Inventory::displayAll() const {
    if (products.empty()) {
        std::cout << "Inventory is empty.\n";
        return;
    }
    printHeader();
    for (const auto& p : products) printRow(*p);   // polymorphic calls inside
}

void Inventory::searchById(int id) const {
    int index = findIndexById(id);
    if (index == -1) throw std::runtime_error("Product not found.");
    printHeader();
    printRow(*products[index]);
}

void Inventory::searchByName(const std::string& keyword) const {
    std::string key = toLower(keyword);
    bool found = false;
    for (const auto& p : products) {
        if (toLower(p->getName()).find(key) != std::string::npos) {
            if (!found) printHeader();
            printRow(*p);
            found = true;
        }
    }
    if (!found) throw std::runtime_error("No product matches that name.");
}

void Inventory::updateStock(int id, int newQuantity) {
    int index = findIndexById(id);
    if (index == -1) throw std::runtime_error("Product not found.");
    products[index]->setQuantity(newQuantity);   // may throw if negative
}

void Inventory::removeProduct(int id) {
    int index = findIndexById(id);
    if (index == -1) throw std::runtime_error("Product not found.");
    products.erase(products.begin() + index);
}

void Inventory::saveToFile() const {
    std::ofstream file(fileName);
    if (!file) throw std::runtime_error("Could not open file for writing.");
    for (const auto& p : products) file << p->serialize() << '\n';
}

void Inventory::loadFromFile() {
    std::ifstream file(fileName);
    if (!file) return;   // first run: no file yet, start empty

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            // Format: T|id|name|price|quantity|extra
            std::stringstream ss(line);
            std::string type, idStr, name, priceStr, qtyStr, extraStr;
            std::getline(ss, type, '|');
            std::getline(ss, idStr, '|');
            std::getline(ss, name, '|');
            std::getline(ss, priceStr, '|');
            std::getline(ss, qtyStr, '|');
            std::getline(ss, extraStr, '|');

            int id = std::stoi(idStr);
            double price = std::stod(priceStr);
            int qty = std::stoi(qtyStr);
            double extra = std::stod(extraStr);

            if (type == "P")
                addProduct(std::make_unique<PhysicalProduct>(id, name, price, qty, extra));
            else if (type == "D")
                addProduct(std::make_unique<DigitalProduct>(id, name, price, qty, extra));
            else
                throw std::runtime_error("Unknown product type.");
        } catch (const std::exception& e) {
            std::cerr << "Skipping bad line in file: " << line
                      << " (" << e.what() << ")\n";
        }
    }
}
