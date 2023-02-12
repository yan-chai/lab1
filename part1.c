#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct user{
    char username[50];
    char password[50];
    int admin;
};

struct user* createUsers(int max_number_of_users) {
    //Your code goes here
    struct user* res = malloc(sizeof(struct user)*max_number_of_users);
    return res;
}

void initUsers(void* users, int max_number_of_users) {
    //Your code goes here
    struct user* tmp = users;
    for (int i = 0; i < max_number_of_users; i++) {
        tmp->admin=0;
        strcpy(tmp->username, "default");
        strcpy(tmp->password, "default");
        tmp += sizeof(struct user);
    }
}

void addUser1(struct user* users, char* username, char* password,
    int admin, int* count) {
    //Your code goes here
    struct user u;
    u.admin = admin;
    strcpy(u.username, username);
    strcpy(u.password, password);
    memcpy(users+(*count)*sizeof(struct user), &u, sizeof(struct user));
    *count += 1;
}

void addUser2(struct user* users, struct user* newUser, int* count) {
    //Your code goes here
    memcpy(users+(*count)*sizeof(struct user), newUser, sizeof(struct user));
    *count += 1;
}

void printUser(struct user* users, int number_of_users) {
    //Your code goes here
    struct user* tmp = users;
    for (int i = 0; i < number_of_users; i ++) {
        printf("username%d: %s\n", i, tmp->username);
        printf("password%d: %s\n", i, tmp->password);
        tmp += sizeof(struct user);
    }
}

int main(void) {
    struct user* users = createUsers(10);
    if(users == NULL) {
        return 0;
    }
    int userCount = 0;
    initUsers(users, 10);
    addUser1(users, "admin", "s#1Pa5", 1, &userCount);
    addUser1(users, "vahab", "fgH!12", 0, &userCount);
    struct user u1;
    u1.admin = 2;
    strcpy(u1.username, "usf");
    strcpy(u1.password, "gA24?m");
    addUser2(users, &u1, &userCount);
    printUser(users, userCount);
    free(users);
    return 0;
}