#ifndef CONTACT_MANAGEMENT_C_CONTACTS_H
#define CONTACT_MANAGEMENT_C_CONTACTS_H

// each limit represents the maximum number of "visible characters" allowed (i.e. without \0)
#define MAX_NAMELEN 100
#define MAX_PHONELEN 15
#define MAX_EMAILLEN 100

// Adding +1 to account for \0 character
typedef struct {
    char name[MAX_NAMELEN+1];
    char phone[MAX_PHONELEN+1];
    char email[MAX_EMAILLEN+1];
} Contact;

Contact *add_contact(char *name, char *phone, char *email, Contact *database, int *contact_count);

Contact *search_contact(char *name, Contact *database, int contact_count);

Contact *delete_contact(char *name, Contact *database, int *contact_count);

void list_contacts(const Contact *database, int contact_count);

void save_contacts_to_file(Contact *database, int contact_count, const char *output_file);

Contact *load_contacts_from_file(Contact *database, int *contact_count, const char *input_file);

void print_contact(Contact contact);

#endif //CONTACT_MANAGEMENT_C_CONTACTS_H
