#ifndef INVENTORY_H
#define INVENTORY_H

#include <memory>
#include <string>
#include <vector>

#include "Product.h"

class Inventory {
    // Base-class pointers let one vector hold both product types (polymorphism)
    std::vector<std::unique_ptr<Product>> products;
    std::string fileName;

    // Returns index of product with this id, or -1 if not found
    int findIndexById(int id) const;

public:
    explicit Inventory(const std::string& fileName);

    void addProduct(std::unique_ptr<Product> product);   // throws on duplicate ID
    void displayAll() const;
    void searchById(int id) const;                       // throws if not found
    void searchByName(const std::string& keyword) const; // case-insensitive
    void updateStock(int id, int newQuantity);           // throws if not found
    void removeProduct(int id);                          // throws if not found

    void loadFromFile();
    void saveToFile() const;
};

#endif
