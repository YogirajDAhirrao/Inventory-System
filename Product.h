#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

// Base class: common data + behaviour for every product.
class Product {
protected:                       // Encapsulation: data hidden from outside,
    int id;                      // visible only to this class and its children
    std::string name;
    double price;
    int quantity;

public:
    Product(int id, const std::string& name, double price, int quantity);
    virtual ~Product() = default;    // virtual destructor: needed for polymorphic delete

    // Getters
    int getId() const;
    const std::string& getName() const;
    double getPrice() const;
    int getQuantity() const;

    // Setter with validation
    void setQuantity(int newQuantity);

    // Polymorphic interface: each child class provides its own version
    virtual std::string getType() const = 0;      // "Physical" / "Digital"
    virtual std::string getDetails() const = 0;   // type-specific info
    virtual std::string serialize() const;        // one line for file storage
};

// Child class 1: has a weight
class PhysicalProduct : public Product {
    double weightKg;

public:
    PhysicalProduct(int id, const std::string& name, double price,
                    int quantity, double weightKg);

    std::string getType() const override;
    std::string getDetails() const override;
    std::string serialize() const override;
};

// Child class 2: has a download size
class DigitalProduct : public Product {
    double fileSizeMB;

public:
    DigitalProduct(int id, const std::string& name, double price,
                   int quantity, double fileSizeMB);

    std::string getType() const override;
    std::string getDetails() const override;
    std::string serialize() const override;
};

#endif
