# Contact Management System

## Overview
The Contact Management System is a simple C program that allows users to manage a list of contacts. It provides functionality to add, search, delete, and list contacts, with all data being stored persistently in a text file. The system features a user-friendly terminal-based interface.

## Features
- **Add Contact**: Add a new contact with a name, phone number, and email address.
- **Search Contact**: Search for a contact by name.
- **Delete Contact**: Remove a contact by name.
- **List Contacts**: List all stored contacts.
- **Persistent Storage**: Contacts are saved to a file and loaded upon program start.

## Project Structure
```
.
├── CMakeLists.txt
├── include
│   └── contacts.h
├── src
│   ├── contacts.c
│   └── main.c
├── tests
│   └── test_contacts.cpp
└── README.md
```

## Requirements
- CMake 3.22.1 or higher
- A C compiler (e.g., GCC)
- A C++ compiler for running tests

## Building the Project
1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/contact_management_c.git
    cd contact_management_c
    ```

2. Create a build directory and navigate to it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    make
    ```
   or if you don't have make:
    ```sh
    cmake --build .
    ```

5. Run the executable:
    ```sh
    ./contact_management_c
    ```

## Running Tests
Tests are written using the Catch2 testing framework.

1. Build the tests:
    ```sh
    make tests
    ```
   or if you don't have make:
    ```sh
    cmake --build . --target tests
    ```

2. Run the tests:
    ```sh
    ./tests
    ```

## Usage
Upon running the program, you will be presented with a menu of options:

1. Add Contact
2. Search Contact
3. Delete Contact
4. List Contacts
5. Save and Exit

Follow the prompts to interact with the contact management system. Contact information is validated and stored in a file named `contact_db.txt`. The file name is stored as a global constant in main.c, so it can be easily changed.

## Example
Here is a brief example of how to use the system:

1. **Add Contact**:
    ```
    New contact's name: John Doe
    New contact's phone: 123-456-7890
    New contact's email: johndoe@example.com
    Successfully added John Doe to the database!
    ```

2. **Search Contact**:
    ```
    Name of the contact to search: John Doe
    Contact found successfully!
    Name: John Doe
    Phone: 123-456-7890
    Email: johndoe@example.com
    ```

3. **Delete Contact**:
    ```
    Name of the contact to delete: John Doe
    Contact with the name John Doe was deleted successfully!
    ```

4. **List Contacts**:
    ```
    Contact #1:
    Name: John Doe
    Phone: 123-456-7890
    Email: johndoe@example.com
   
    Contact #2:
    Name: John Doe Jr.
    Phone: 0987-654-321
    Email: johndoejr@example.com
   
    Total number of contacts: 2
    ```

## Contributing
If you wish to contribute to the project, please fork the repository and submit a pull request. Issues and feature requests are also welcome.

## License
This project is licensed under the MIT License.