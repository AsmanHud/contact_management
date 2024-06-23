#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <catch2/catch_test_macros.hpp>

extern "C" {
#include "contacts.h"
}

#define NUM_OF_TEST_CONTACTS 1000
#define NUM_OF_DATABASES_STRESS_TEST 1000

class ContactFixture {
public:
    ContactFixture() {
        for (int i = 0; i < NUM_OF_TEST_CONTACTS; ++i) {
            Contact new_contact;
            std::string i_str = std::to_string(i);
            std::string name = "Name" + i_str;
            std::string phone = "+370123" + i_str;
            std::string email = "testemail" + i_str + "@gmail.com";
            strcpy(new_contact.name, name.c_str());
            strcpy(new_contact.phone, phone.c_str());
            strcpy(new_contact.email, email.c_str());
            test_contacts[i] = new_contact;

            Contact new_contact_large;
            strcpy(new_contact_large.name, (std::string(MAX_NAMELEN - 3, 'n') + i_str).c_str());
            strcpy(new_contact_large.phone, std::string(MAX_PHONELEN, 'p').c_str());
            strcpy(new_contact_large.email, std::string(MAX_EMAILLEN, 'e').c_str());
            test_contacts_large_data[i] = new_contact_large;
        }
    }

    ~ContactFixture() {
    }

    Contact test_contacts[NUM_OF_TEST_CONTACTS];
    Contact test_contacts_large_data[NUM_OF_TEST_CONTACTS];
};

// ===========================
// = UNIT TESTS: add_contact =
// ===========================

// Create a basic database with 1000 contacts
TEST_CASE_METHOD(ContactFixture, "Add contact base test", "[add_contact]") {
    Contact *database = nullptr;
    int contact_count = 0;

    for (Contact test_contact: test_contacts) {
        Contact *updated_database = add_contact(test_contact.name, test_contact.phone, test_contact.email, database,
                                                &contact_count);
        if (updated_database != nullptr) {
            database = updated_database;
        }
    }

    REQUIRE(database != NULL);
    REQUIRE(contact_count == NUM_OF_TEST_CONTACTS);
    for (int i = 0; i < NUM_OF_TEST_CONTACTS; ++i) {
        REQUIRE(strcmp(database[i].name, test_contacts[i].name) == 0);
        REQUIRE(strcmp(database[i].phone, test_contacts[i].phone) == 0);
        REQUIRE(strcmp(database[i].email, test_contacts[i].email) == 0);
    }

    free(database);
}

// Create a database with 1000 contacts, whose data fields are as large as possible
TEST_CASE_METHOD(ContactFixture, "Add contact boundary test", "[add_contact]") {
    Contact *database = nullptr;
    int contact_count = 0;

    for (Contact test_contact: test_contacts_large_data) {
        Contact *updated_database = add_contact(test_contact.name, test_contact.phone, test_contact.email, database,
                                                &contact_count);
        if (updated_database != nullptr) {
            database = updated_database;
        }
    }

    REQUIRE(database != NULL);
    REQUIRE(contact_count == NUM_OF_TEST_CONTACTS);
    for (int i = 0; i < NUM_OF_TEST_CONTACTS; ++i) {
        REQUIRE(strcmp(database[i].name, test_contacts_large_data[i].name) == 0);
        REQUIRE(strcmp(database[i].phone, test_contacts_large_data[i].phone) == 0);
        REQUIRE(strcmp(database[i].email, test_contacts_large_data[i].email) == 0);
    }

    free(database);
}

// Set different values of the input to null
TEST_CASE("Add contact null test", "[add_contact]") {
    Contact *database = nullptr;
    int contact_count = 0;
    Contact *updated_database = nullptr;

    updated_database = add_contact(nullptr, "test", "test", database, &contact_count);
    REQUIRE(updated_database == nullptr);
    updated_database = add_contact("test", nullptr, "test", database, &contact_count);
    REQUIRE(updated_database == nullptr);
    updated_database = add_contact("test", "test", nullptr, database, &contact_count);
    REQUIRE(updated_database == nullptr);
    updated_database = add_contact("test", "test", "test", nullptr, &contact_count);
    database = updated_database;
    REQUIRE(database != nullptr);
    REQUIRE(contact_count == 1);
    REQUIRE(strcmp(database[0].name, "test") == 0);
    REQUIRE(strcmp(database[0].phone, "test") == 0);
    REQUIRE(strcmp(database[0].email, "test") == 0);
    updated_database = add_contact("test", "test", "test", database, nullptr);
    REQUIRE(updated_database == nullptr);

    free(database);
}

// Check the validation rules
TEST_CASE("Add contact input validation", "[add_contact]") {
    Contact *database = nullptr;
    int contact_count = 0;
    Contact *updated_database = nullptr;

    // Empty string
    updated_database = add_contact("", "test", "test", database, &contact_count);
    REQUIRE(updated_database == nullptr);

    // String length exceeds the maximum character length (16 for phone for ex., when max is 15)
    updated_database = add_contact("test", "1234567890123456", "test", database, &contact_count);
    REQUIRE(updated_database == nullptr);

    // Newlines in the string
    updated_database = add_contact("test", "test", "some\nemail@gmail.com", database, &contact_count);
    REQUIRE(updated_database == nullptr);

    // should be nothing to free at this point

    // Try to add a duplicate contact
    // A valid contact
    updated_database = add_contact("test", "test", "test", database, &contact_count);
    REQUIRE(updated_database != nullptr);
    database = updated_database;
    // Adding a new contact with the same name (should return NULL)
    updated_database = add_contact("test", "test2", "test2", database, &contact_count);
    REQUIRE(updated_database == nullptr);

    free(database);
}

// Try creating a bunch of databases in one test case
TEST_CASE_METHOD(ContactFixture, "Add contact stress test", "[add_contact]") {
    Contact *database_arr[NUM_OF_DATABASES_STRESS_TEST];
    int contact_count_arr[NUM_OF_DATABASES_STRESS_TEST];

    for (int i = 0; i < NUM_OF_DATABASES_STRESS_TEST; ++i) {
        database_arr[i] = nullptr;
        contact_count_arr[i] = 0;
        for (Contact test_contact: test_contacts) {
            Contact *updated_database = add_contact(test_contact.name, test_contact.phone, test_contact.email,
                                                    database_arr[i],
                                                    &contact_count_arr[i]);
            if (updated_database != nullptr) {
                database_arr[i] = updated_database;
            }
        }
    }

    for (int i = 0; i < NUM_OF_DATABASES_STRESS_TEST; ++i) {
        REQUIRE(database_arr[i] != nullptr);
        REQUIRE(contact_count_arr[i] == NUM_OF_TEST_CONTACTS);

        free(database_arr[i]);
    }
}

// ==============================
// = UNIT TESTS: search_contact =
// ==============================

// to be added...

// ==============================
// = UNIT TESTS: delete_contact =
// ==============================

// to be added...

// =============================
// = UNIT TESTS: list_contacts =
// =============================

// to be added...

// =====================================
// = UNIT TESTS: save_contacts_to_file =
// =====================================

// to be added...

// =======================================
// = UNIT TESTS: load_contacts_from_file =
// =======================================

// to be added...