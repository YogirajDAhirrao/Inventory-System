#include "Product.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>

// ---------- Product ----------

Product::Product(int id, const std::string& name, double price, int quantity)
    : id(id), name(name), price(price), quantity(quantity) {
    if (id <= 0)          throw std::invalid_argument("ID must be positive.");
    if (name.empty())     throw std::invalid_argument("Name cannot be empty.");
    if (name.find('|') != std::string::npos)
                          throw std::invalid_argument("Name cannot contain '|'.");
    if (price < 0)        throw std::invalid_argument("Price cannot be negative.");
    if (quantity < 0)     throw std::invalid_argument("Quantity cannot be negative.");
}

int Product::getId() const { return id; }
const std::string& Product::getName() const { return name; }
double Product::getPrice() const { return price; }
int Product::getQuantity() const { return quantity; }

void Product::setQuantity(int newQuantity) {
    if (newQuantity < 0)
        throw std::invalid_argument("Quantity cannot be negative.");
    quantity = newQuantity;
}

std::string Product::serialize() const {
    std::ostringstream out;
    out << id << '|' << name << '|' << price << '|' << quantity;
    return out.str();
}

// ---------- PhysicalProduct ----------

PhysicalProduct::PhysicalProduct(int id, const std::string& name, double price,
                                 int quantity, double weightKg)
    : Product(id, name, price, quantity), weightKg(weightKg) {
    if (weightKg < 0) throw std::invalid_argument("Weight cannot be negative.");
}

std::string PhysicalProduct::getType() const { return "Physical"; }

std::string PhysicalProduct::getDetails() const {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << weightKg << " kg";
    return out.str();
}

std::string PhysicalProduct::serialize() const {
    std::ostringstream out;
    out << "P|" << Product::serialize() << '|' << weightKg;
    return out.str();
}

// ---------- DigitalProduct ----------

DigitalProduct::DigitalProduct(int id, const std::string& name, double price,
                               int quantity, double fileSizeMB)
    : Product(id, name, price, quantity), fileSizeMB(fileSizeMB) {
    if (fileSizeMB < 0) throw std::invalid_argument("File size cannot be negative.");
}

std::string DigitalProduct::getType() const { return "Digital"; }

std::string DigitalProduct::getDetails() const {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << fileSizeMB << " MB";
    return out.str();
}

std::string DigitalProduct::serialize() const {
    std::ostringstream out;
    out << "D|" << Product::serialize() << '|' << fileSizeMB;
    return out.str();
}
