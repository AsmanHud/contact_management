#ifndef CONTACT_MANAGEMENT_C_CONTACTS_H
#define CONTACT_MANAGEMENT_C_CONTACTS_H

/**
 * @file contacts.h
 * @brief Defines the structures and functions for managing contacts in the contact management system.
 */

#include <stddef.h>

#define MAX_NAMELEN 100
#define MAX_PHONELEN 15
#define MAX_EMAILLEN 100

/**
 * @struct Contact
 * @brief Represents a contact in the contact management system.
 *
 * @var name The name of the contact.
 * @var phone The phone number of the contact.
 * @var email The email address of the contact.
 */
typedef struct {
    char name[MAX_NAMELEN+1];
    char phone[MAX_PHONELEN+1];
    char email[MAX_EMAILLEN+1];
} Contact;

/**
 * @brief Adds a new contact to the database.
 *
 * @param name The name of the contact.
 * @param phone The phone number of the contact.
 * @param email The email address of the contact.
 * @param database The current contact database.
 * @param contact_count Pointer to the number of contacts in the database.
 * @return A pointer to the updated contact database.
 */
Contact *add_contact(char *name, char *phone, char *email, Contact *database, int *contact_count);

/**
 * @brief Searches for a contact by name.
 *
 * @param name The name of the contact to search for.
 * @param database The current contact database.
 * @param contact_count The number of contacts in the database.
 * @return A pointer to the found contact, or NULL if not found.
 */
Contact *search_contact(char *name, Contact *database, int contact_count);

/**
 * @brief Deletes a contact by name.
 *
 * @param name The name of the contact to delete.
 * @param database The current contact database.
 * @param contact_count Pointer to the number of contacts in the database.
 * @return A pointer to the updated contact database.
 */
Contact *delete_contact(char *name, Contact *database, int *contact_count);

/**
 * @brief Lists all contacts in the database.
 *
 * @param database The current contact database.
 * @param contact_count The number of contacts in the database.
 */
void list_contacts(const Contact *database, int contact_count);

/**
 * @brief Saves the contacts to a file.
 *
 * @param database The current contact database.
 * @param contact_count The number of contacts in the database.
 * @param output_file The file to save the contacts to.
 */
void save_contacts_to_file(Contact *database, int contact_count, const char *output_file);

/**
 * @brief Loads contacts from a file.
 *
 * @param database The current contact database.
 * @param contact_count Pointer to the number of contacts in the database.
 * @param input_file The file to load the contacts from.
 * @return A pointer to the updated contact database.
 */
Contact *load_contacts_from_file(Contact *database, int *contact_count, const char *input_file);

/**
 * @brief Prints the details of a contact.
 *
 * @param contact The contact to print.
 */
void print_contact(Contact contact);

#endif //CONTACT_MANAGEMENT_C_CONTACTS_H
