#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct user {
    char username[50];
    char password[256];
    char firstname[50];
    char lastname[50];
    int admin;
};

char* cs621Hash(const char* password) {
    return crypt(password, "00");
}

struct user* createUsers(int* count) {
    //Your code goes here
    FILE* f = fopen("credential_file", "rb");
    if (f == NULL) {
        exit(EXIT_FAILURE);
    }
    fseek (f, 0, SEEK_END);
    int length = ftell (f);
    fseek (f, 0, SEEK_SET);
    char* b = malloc (length);
    char* buffer = b;
    if (buffer) {
        fread (buffer, 1, length, f);
    }
    fclose (f);
    for (int i = 0; i < strlen(buffer); i ++) {
        if (*(buffer + i) == '\n') {
            *count += 1;
        }
    }
    struct user* res = malloc(sizeof(struct user)*(*count));
    
    free(b);
    return res;
}

void populateUsers(void* users) {
    struct user* res = users;
    FILE* f = fopen("credential_file", "rb");
    if (f == NULL) {
        exit(EXIT_FAILURE);
    }
    fseek (f, 0, SEEK_END);
    int length = ftell (f);
    fseek (f, 0, SEEK_SET);
    char* b = malloc (length);
    char* buffer = b;
    if (buffer) {
        fread (buffer, 1, length, f);
    }
    fclose (f);
    //Your code goes here
    char* l = strtok(buffer, "\n");
    int i = 0;
    while (l != NULL) {
        struct user u;
        char* p = malloc(500);
        char* line = p;
        strcpy(line, l) ;
        char* item = strtok(line, "\t");
        strcpy(u.firstname, item);
        line += strlen(item) + 1;
        item = strtok(line, "\t");
        strcpy(u.lastname, item);
        line += strlen(item) + 1;
        item = strtok(line, "\t");
        strcpy(u.username, item);
        line += strlen(item) + 1;
        item = strtok(line, "\t");
        strcpy(u.password, item);
        line += strlen(item) + 1;
        item = strtok(line, "\t");
        u.admin = atoi(item);
        memcpy(res + i, &u, sizeof(struct user));
        i += 1;
        buffer += strlen(l)+1;
        l = strtok(buffer, "\n");
        free(p);
    }
    free(b);
}

int checkAdminPassword(char* password, struct user* users, int count) {
    printf("hash: %s\n", cs621Hash(password));
    for (int i = 0; i < count; ++i) {
        if (strcmp((users + i)->username, "admin") == 0) {
            if (strcmp(cs621Hash(password), (users + i)->password) == 0) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }
    return 0;
}

struct user* addUser(struct user* users, int* count, char* username, char* password, char* firstname, char* lastname, int admin) {
    //Your code goes here
    struct user* new = (struct user*)realloc(users, sizeof(struct user)*(*count + 1));
    struct user* tmp = new + *count;
    strcpy(tmp->username, username);
    strcpy(tmp->password, cs621Hash(password));
    strcpy(tmp->firstname, firstname);
    strcpy(tmp->lastname, lastname);
    tmp->admin = admin;
    *count += 1;
    return new;
}

void saveUsers(struct user* users, int count) {
    //Your code goes here
    FILE* f = fopen("credential_file", "w");
    if (f == NULL) {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < count; i ++) {
        fprintf(f, "%s\t", users->firstname);
        fprintf(f, "%s\t", users->lastname);
        fprintf(f, "%s\t", users->username);
        fprintf(f, "%s\t", users->password);
        fprintf(f, "%d\n", users->admin);
        users += 1;
    }
}

int main(void) {
    int user_count = 0;
    struct user* users = createUsers(&user_count);
    if (users == NULL) {
        return EXIT_FAILURE;
    }
    populateUsers(users);
    printf("Enter admin password to add new users:");
    char entered_admin_password[50];
    scanf("%s", entered_admin_password);
    if (checkAdminPassword(entered_admin_password, users, user_count)) {
        struct user new_user;
        printf("Enter username:");
        scanf("%s", new_user.username);
        printf("Enter first name:");
        scanf("%s", new_user.firstname);
        printf("Enter last name:");
        scanf("%s", new_user.lastname);
        printf("Enter password:");
        scanf("%s", new_user.password);
        printf("Enter admin level:");
        scanf("%d", &(new_user.admin));
        users = addUser(users, &user_count, new_user.username, new_user.password, new_user.firstname, new_user.lastname, new_user.admin);
        if (users == NULL) {
            return EXIT_FAILURE;
        }
    }
    saveUsers(users, user_count);
    free(users);
    return EXIT_SUCCESS;
}