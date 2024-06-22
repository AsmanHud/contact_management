#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contacts.h"

// Validation rules:
// - data is not longer than max specification
// - data is not empty
// - data does not contain newlines
static int validate_info(char *data, int maxlen) {
    if ((data == NULL) ||
        (strlen(data) > maxlen) ||
        (strlen(data) == 0) ||
        (strchr(data, '\n') != NULL)) {
        return 1;
    }
    return 0;
}

void print_contact(Contact contact) {
    printf("Name: %s\n", contact.name);
    printf("Phone: %s\n", contact.phone);
    printf("Email: %s\n", contact.email);
}

Contact *add_contact(char *name, char *phone, char *email, Contact *database, int *contact_count) {
    if (validate_info(name, MAX_NAMELEN) ||
        validate_info(phone, MAX_PHONELEN) ||
        validate_info(email, MAX_EMAILLEN) ||
        contact_count == NULL) {
        return NULL;
    }

    Contact new_contact;
    strcpy(new_contact.name, name);
    strcpy(new_contact.phone, phone);
    strcpy(new_contact.email, email);

    Contact *updated_database = realloc(database, sizeof(Contact) * (*contact_count + 1));
    if (updated_database == NULL) {
        fprintf(stderr, "Failed to reallocate memory for a new contact:\n");
        print_contact(new_contact);
        free(database);
        exit(EXIT_FAILURE);
    }

    updated_database[*contact_count] = new_contact;
    (*contact_count)++;

    return updated_database;
}

Contact *search_contact(char *name, Contact *database, int contact_count) {
    if (validate_info(name, MAX_NAMELEN) ||
        database == NULL) {
        return NULL;
    }
    for (int i = 0; i < contact_count; ++i) {
        if (strcmp(database[i].name, name) == 0) {
            return &database[i];
        }
    }
    return NULL; // if the contact is not found
}

Contact *delete_contact(char *name, Contact *database, int *contact_count) {
    if (validate_info(name, MAX_NAMELEN) ||
        database == NULL ||
        contact_count == NULL) {
        return NULL;
    }

    // Shift all the contacts after the contact to be deleted by 1 to the left
    int found_flag = 0;
    Contact contact_to_delete;
    for (int i = 0; i < *contact_count; ++i) {
        if (found_flag) {
            database[i - 1] = database[i];
        } else if (strcmp(database[i].name, name) == 0) {
            found_flag = 1;
            contact_to_delete = database[i];
        }
    }
    if (!found_flag) {
        return NULL; // if the contact is not found
    }

    // Now we can safely remove the last contact, as it will be redundant
    Contact *updated_database = realloc(database, sizeof(Contact) * (*contact_count - 1));
    // if the database had only 1 contact, reallocation will be called for size 0
    // which effectively frees the memory held by database and sets it to NULL
    // but this behavior is expected, thus for an error we additionally check if contact_count > 1
    if (updated_database == NULL && *contact_count > 1) {
        fprintf(stderr, "Failed to delete the following contact:\n");
        print_contact(contact_to_delete);
        free(database);
        exit(EXIT_FAILURE);
    }

    (*contact_count)--;

    return updated_database;
}

void list_contacts(const Contact *database, int contact_count) {
    if (database == NULL) {
        return;
    }

    for (int i = 0; i < contact_count; ++i) {
        printf("Contact #%d:\n", i + 1);
        print_contact(database[i]);
        printf("\n");
    }
}

void save_contacts_to_file(Contact *database, int contact_count, const char *output_file) {
    FILE *file;
    file = fopen(output_file, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file to save contacts: %s\n", output_file);
        return;
    }

    for (int i = 0; i < contact_count; ++i) {
        fprintf(file, "%s\n", database[i].name);
        fprintf(file, "%s\n", database[i].phone);
        fprintf(file, "%s\n", database[i].email);
    }

    fclose(file);
}

static void print_invalid_contact_msg(int contact_count, const char *invalid_data_label, char *invalid_data) {
    printf("Invalid information on contact #%d, not adding this and all trailing contacts.\n", contact_count + 1);
    printf("Specifically, %s was invalidated: %s\n", invalid_data_label, invalid_data);
}

Contact *load_contacts_from_file(Contact *database, int *contact_count, const char *input_file) {
    FILE *file;
    file = fopen(input_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file, when loading contacts: %s\n", input_file);
        free(database);
        exit(EXIT_FAILURE);
    }

    char name[MAX_NAMELEN + 1] = "";
    char phone[MAX_NAMELEN + 1] = "";
    char email[MAX_NAMELEN + 1] = "";

    char line[256];
    int error_flag = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        for (int i = (int)strlen(line)-1; i >= 0; --i) {
            if (line[i] != '\n' && line[i] != '\r') {
                line[i+1] = '\0';
                break;
            }
        }

        if (strlen(name) == 0) {
            if (!validate_info(line, MAX_NAMELEN)) {
                strcpy(name, line);
                continue;
            }
            print_invalid_contact_msg(*contact_count, "name", line);
            error_flag = 1;
            break;
        }
        if (strlen(phone) == 0) {
            if (!validate_info(line, MAX_PHONELEN)) {
                strcpy(phone, line);
                continue;
            }
            print_invalid_contact_msg(*contact_count, "phone", line);
            error_flag = 1;
            break;
        }
        if (validate_info(line, MAX_EMAILLEN)) {
            print_invalid_contact_msg(*contact_count, "email", line);
            error_flag = 1;
            break;
        }
        strcpy(email, line);

        Contact *updated_database = add_contact(name, phone, email, database, contact_count);
        if (updated_database == NULL) {
            printf("Something went wrong when adding the contact #%d.\n", *contact_count + 1);
            error_flag = 1;
            break;
        }
        database = updated_database;

        strcpy(name, "");
        strcpy(phone, "");
        strcpy(email, "");
    }

    // If the error flag was not raised (i.e. the loop did not end prematurely)
    // and the input strings were cleared properly, then all the data from the text file was read
    if (!error_flag && strlen(name) == 0 && strlen(phone) == 0 && strlen(email) == 0) {
        printf("All contacts were successfully loaded from the text file.\n");
    } else {
        printf("There was an error during loading contacts. Some of the data from the text file was omitted.\n");
    }
    printf("Total number of contacts written to database: %d\n", *contact_count);
    printf("\n");

    fclose(file);
    return database;
}