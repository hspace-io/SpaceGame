//gcc -o uaf uaf.c -no-pie
#include <stdio.h>
#include <stdlib.h>
typedef struct {
    char name[32];
    unsigned long size;
} Dog;

typedef struct {
    char name[32];
    void (*fp)();
} Cat;

void init_buffer() {
    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
}

void exploit() {
    char *cmd = "/bin/sh";
    char *args[] = {cmd, NULL};
    execve(cmd, args, NULL);
}

void print_menu() {
    puts("1. create dog");
    puts("2. free dog");
    puts("3. create cat");
    puts("4. free cat");
    puts("5. exit");
    printf("> ");
}

Dog* create_dog() {
    char c;
    int n;
    Dog* dog = (Dog*)malloc(sizeof(Dog));
    puts("Create dog!");
    printf("Enter name: ");
    n = read(0, dog->name, sizeof(dog->name) - 1);
    dog->name[n - 1] = '\0';
    printf("Enter size: ");
    scanf("%u", &(dog->size));
    while((c = getchar()) != EOF && c != '\n');
    return dog;
}

void free_dog(Dog* dog) {
    puts("Free dog!");
    free(dog);
}

Cat* create_cat() {
    char c;
    int n;
    Cat* cat = (Cat*)malloc(sizeof(Cat));
    puts("Create cat!");
    printf("Enter name: ");
    n = read(0, cat->name, sizeof(cat->name) - 1);
    cat->name[n - 1] = '\0';
    if (cat->fp) cat->fp();
    return cat;
}

void free_cat(Cat* cat) {
    puts("Free cat!");
    free(cat);
}

int main() {
    char c;
    int choice = 0;
    Dog* dog;
    Cat* cat;
    init_buffer();
    while (1) {
        print_menu();
        scanf("%d", &choice);
        while((c = getchar()) != EOF && c != '\n');
        switch (choice) {
            case 1: dog = create_dog(); break;
            case 2: free_dog(dog); break;
            case 3: cat = create_cat(); break;
            case 4: free_cat(cat); break;
            case 5: puts("Bye"); return 0;
            default: puts("Wrong input");
        }
    }
    return 0;
}