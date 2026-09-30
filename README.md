# C++ Inventory Management System

A small console application that manages products in an inventory.
Built to demonstrate core C++ and OOP concepts.

## Features
1. Add product (Physical or Digital)
2. View all products
3. Search by ID or by name (case-insensitive, partial match)
4. Update stock
5. Remove product
6. Data is saved to `inventory.txt` and reloaded on the next run

## Build & Run
```bash
g++ -std=c++17 -Wall -Wextra -o inventory main.cpp Product.cpp Inventory.cpp
./inventory
```

## Concepts Demonstrated
| Concept | Where |
|---|---|
| Classes & objects | `Product`, `Inventory` |
| Encapsulation | `protected`/`private` data with getters and a validated setter |
| Constructors | Validating constructors; derived classes call the base constructor |
| Inheritance | `PhysicalProduct` and `DigitalProduct` extend `Product` |
| Polymorphism | `getType()`, `getDetails()`, `serialize()` are `virtual`; one `vector<unique_ptr<Product>>` holds both types |
| STL `vector` | Storage inside `Inventory` |
| File handling | `saveToFile()` / `loadFromFile()` using `ifstream`/`ofstream` |
| Exception handling | `invalid_argument` for bad data, `runtime_error` for not-found / duplicates |
| Searching | Linear search by ID and by name substring |

## Design Notes
- `Product` has pure virtual functions, so it is an abstract class.
- A virtual destructor makes deletion through a base pointer safe.
- `unique_ptr` gives automatic memory management with no manual `delete`.
- File format (one product per line): `T|id|name|price|quantity|extra`, where T is `P` or `D`.

## Structure
```
inventory-management/
├── Product.h / Product.cpp
├── Inventory.h / Inventory.cpp
├── main.cpp
└── README.md
```
