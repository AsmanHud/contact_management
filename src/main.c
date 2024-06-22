#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contacts.h"

#define ZERO_ASCII 48
#define NUM_OF_ACTIONS 5

const char *contact_list_file = "contact_db.txt";

typedef enum {
    START_SCREEN,
    ADD_CONTACT,
    SEARCH_CONTACT,
    DELETE_CONTACT,
    LIST_CONTACTS,
    SAVE_AND_EXIT
} ActionState;

static void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void clear_input_buffer() {
    int temp;
    while ((temp = getchar()) != '\n' && temp != EOF);
}

static int handle_str_input(const char *input_msg, char *output_str, int max_size) {
    printf("%s\n: ", input_msg);
    if (fgets(output_str, max_size, stdin) != NULL) {
        clear_screen();

        // Input was empty (i.e. a newline was entered without any preceding characters)
        if (strcmp(output_str, "\n") == 0) {
            printf("Input cannot be empty!\n\n");
        }
            // Input was larger than the limit of characters
        else if (output_str[strlen(output_str) - 1] != '\n') {
            // maximum number of characters is the size of the output_str minus two for \n and \0
            printf("Maximum number of characters: %d, please try again.\n\n", max_size - 2);
            clear_input_buffer();
        } else {
            output_str[strlen(output_str) - 1] = '\0'; // remove the trailing \n
            return 0;
        }

        return 1;
    }

    return 1;
}

static void wait_for_enter() {
    printf("Press ENTER to return to the main menu...\n");

    char buffer[2];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strcmp(buffer, "\n") != 0) {
            clear_input_buffer(); // something was typed before the ENTER press
        }
    }
}

static void display_menu() {
    printf("Contact Management System. Available actions:\n");
    printf("1. Add Contact\n");
    printf("2. Search Contact\n");
    printf("3. Delete Contact\n");
    printf("4. List Contacts\n");
    printf("5. Save and Exit\n");
}

static int handle_start_screen() {
    display_menu();
    printf("Choose an option: ");

    int input = getchar() - ZERO_ASCII;
    clear_input_buffer();
    clear_screen();

    if (input < 1 || input > NUM_OF_ACTIONS) {
        printf("Invalid action! Choose an action from 1 to %d\n\n", NUM_OF_ACTIONS);
        return -1;
    }

    return input;
}

int main(void) {
    Contact *database = NULL;
    int contact_count = 0;

    database = load_contacts_from_file(database, &contact_count, contact_list_file);

    ActionState action_state = START_SCREEN;
    int exit_flag = 0;

    // Adding +2 to the limits to account for the trailing \n and \0.
    // \n will be removed from the resulting string (matching the length specified in the header),
    // but it is a good indicator of whether the number of inputted characters (from fgets) exceeded the limit.
    // If \n is not there, it means that it got overwritten (by characters not fitting into limit)
    char name[MAX_NAMELEN + 2] = "";
    char phone[MAX_PHONELEN + 2] = "";
    char email[MAX_EMAILLEN + 2] = "";

    while (!exit_flag) {
        switch (action_state) {
            case START_SCREEN: {
                int input = handle_start_screen();
                if (input != -1) {
                    action_state = input;
                }
                break;
            }
            case ADD_CONTACT: {
                if (strlen(name) == 0 &&
                    handle_str_input("New contact's name", name, MAX_NAMELEN + 2)) {
                    strcpy(name, "");
                    break;
                }
                // Check if the inputted name is unique in the database
                if (search_contact(name, database, contact_count) != NULL) {
                    printf("Contact with such name already exists in the database!\n\n");
                    strcpy(name, "");
                    break;
                }
                if (strlen(phone) == 0 &&
                    handle_str_input("New contact's phone", phone, MAX_PHONELEN + 2)) {
                    strcpy(phone, "");
                    break;
                }
                if (strlen(email) == 0 &&
                    handle_str_input("New contact's email", email, MAX_EMAILLEN + 2)) {
                    strcpy(email, "");
                    break;
                }

                Contact *updated_database = add_contact(name, phone, email, database, &contact_count);
                if (updated_database == NULL) {
                    printf("Failed to add %s to database! Try rechecking the input data.\n\n", name);
                } else {
                    printf("Successfully added %s to database!\n\n", name);
                    database = updated_database;
                }

                strcpy(name, "");
                strcpy(phone, "");
                strcpy(email, "");
                action_state = START_SCREEN;
                break;
            }
            case SEARCH_CONTACT: {
                if (strlen(name) == 0 &&
                    handle_str_input("Name of the contact to search", name, MAX_NAMELEN + 2)) {
                    strcpy(name, "");
                    break;
                }

                Contact *found_contact = search_contact(name, database, contact_count);
                if (found_contact == NULL) {
                    printf("Contact with the name %s was not found!\n\n", name);
                } else {
                    printf("Contact found successfully!\n");
                    print_contact(*found_contact);
                    printf("\n");
                    wait_for_enter();
                    clear_screen();
                }

                strcpy(name, "");
                action_state = START_SCREEN;
                break;
            }
            case DELETE_CONTACT: {
                if (contact_count == 0) {
                    printf("Contact list is empty, nothing to delete!\n\n");
                    action_state = START_SCREEN;
                    break;
                }

                if (strlen(name) == 0 &&
                    handle_str_input("Name of the contact to delete", name, MAX_NAMELEN + 2)) {
                    strcpy(name, "");
                    break;
                }

                Contact *updated_database = delete_contact(name, database, &contact_count);
                // For the error, we additionally check if contact_count > 0,
                // as the delete_contact will return NULL if the contact count was 1 before deletion,
                // but such NULL is not an error - it is expected as the database becomes empty
                if (updated_database == NULL && contact_count > 0) {
                    printf("There is no contact with name %s in the contact list!\n\n", name);
                } else {
                    printf("Contact with the name %s was deleted successfully!\n\n", name);
                }

                strcpy(name, "");
                action_state = START_SCREEN;
                break;
            }
            case LIST_CONTACTS: {
                list_contacts(database, contact_count);
                printf("Total number of contacts: %d\n\n", contact_count);
                wait_for_enter();
                clear_screen();

                action_state = START_SCREEN;
                break;
            }
            case SAVE_AND_EXIT: {
                save_contacts_to_file(database, contact_count, contact_list_file);

                exit_flag = 1;
                break;
            }
        }
    }

    free(database);
    return 0;
}
