/* vector.c
 * Gideon Stephenson
 * 10/8/25
 * A description for vector.h
 */
#include "vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define DELIMETERS " ,\n"

struct vector *vectors;
int count = 0;
int capacity = 10;
char filename[256];

struct vector addVect(struct vector a, struct vector b){
    struct vector c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

struct vector subtractVect(struct vector a, struct vector b){
    struct vector c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    return c;
}

struct vector scalarMultVect(struct vector a, double b){
    struct vector c;
    c.x = a.x * b;
    c.y = a.y * b;
    c.z = a.z * b;
    return c;
}

void run(){
    
    vectors = (struct vector *)malloc(capacity * sizeof(struct vector));

    char input[100];
    printf("Enter -h for help\n");
    while(1){
        printf("Minimat>");
        struct vector temp;
        if(fgets(input, sizeof(input), stdin)){
            input[strcspn(input, "\n")] = '\0';
        }
        char *token = strtok(input, DELIMETERS);

        if(token == NULL){
            printf("Enter -h for help\n");
            continue;
        }

        if(strcmp(token, "quit") == 0){
            free(vectors);
            printf("Quitting Program...\n");
            break;
        }

        if(strcmp(token, "clear") == 0){
            free(vectors);
            vectors = malloc(10 * sizeof(struct vector));
            count = 0;
            capacity = 10;
            printf("Enter -h for help\n");
            continue;
        }

        if(strcmp(token, "list") == 0){
            list();
            printf("Enter -h for help\n");
            continue;
        }

        if(strcmp(token, "-h") == 0){
            help();
            printf("Enter -h for help\n");
            continue;
        }

        if(strcmp(token, "save") == 0){
            token = strtok(NULL, DELIMETERS);
            if (token != NULL) {
                strncpy(filename, token, sizeof(filename)-1);
                filename[sizeof(filename)-1] = '\0';
                save(filename);
            } else {
                printf("Error: No filename provided\n");
            }
            continue;
        }

        if(strcmp(token, "load") == 0){
            token = strtok(NULL, DELIMETERS);
            if (token != NULL) {
                strncpy(filename, token, sizeof(filename)-1);
                filename[sizeof(filename)-1] = '\0';
                load(filename);
            } else {
                printf("Error: No filename provided\n");
            }
            continue;
        }
        
        char *end;
        double scalar = 0;
        if(token != NULL){
            scalar = strtod(token, &end);
        } else {
            printf("Invalid Format(-h for Help)\n");
            continue;
        }
        if(*end != '\0'){
            if(strlen(token) == 1){
            temp.name = token[0];
            } else{
                printf("Please enter a single character for vector names.(-h for Help)\n");
                printf("Enter -h for help\n");
                continue;
            }
        }

        token = strtok(NULL, DELIMETERS);

        if(*end != '\0'){
            if(token == NULL){
                int i = findVect(temp.name);
                if(i == -1){
                    printf("No vector with that name found\n");
                } else {
                    printf("%c = %f %f %f\n", vectors[i].name,
                    vectors[i].x, vectors[i].y, vectors[i].z);
                }
            } else if(strcmp(token, "=") == 0){
                assign(token, temp.name);
                continue;
            } else if(strcmp(token, "+") == 0){
                add(token, temp.name, 'R', false);
                continue;
            } else if(strcmp(token, "-") == 0){
                subtract(token, temp.name, 'R', false);
                continue;
            } else if(strcmp(token, "*") == 0){
                int order = 1;
                multiply(token, temp.name, 'R', order, 0, false);
            } else{
                printf("Invalid Format(-h for Help)\n");
            }
        } else {
            if(strcmp(token, "*") != 0){
                printf("Invalid Format(-h for Help)\n");
            } else {
                int order = 2;
                multiply(token, 'N', 'R', order, scalar, false);
            }
        }
    }
}

int findVect(char vectName){
    for(int i = 0; i < count; ++i){
        if(vectors[i].name == vectName){
            return i;
        }
    }
    return -1;
}

void help(){
    printf("--------------------Help Window--------------------\n"
            "Enter 'quit' to quit\n"
            "To assign new vector: varname = VALx, VALy, VALz\n"
            "varname should be one character and non integers\n"
            "Only use ',' and ' ' to separate each argument\n"
            "Enter a vector name to get info on that vector\n"
            "To add vectors: var1 + var2\n"
            "To subtract vectors: var1 - var2\n"
            "To multiply by a scalar: var1 * num or num * var1\n"
            "Enter 'clear' to clear all stored vectors\n"
            "Enter 'list' to list all stored vectors\n"
            "----------------File Reading/Writing----------------\n"
            "To input a file: load <filename or path>\n"
            "To save to a file: save <filename or path>\n"
            "Filenames and directories cannot have spaces\n"
            "Inputting a file will clear current vectors\n"
            "Files should include z values\n"
            "----------------------------------------------------\n");
}

int add(char *token, char name1, char newName, bool stored){
    token = strtok(NULL, DELIMETERS);

    if(token == NULL){
        printf("Invalid Format (-h for Help)\n");
        return 0;
    }

    char name2;
    if(strlen(token) == 1){
        name2 = token[0];
    } else{
        printf("Please enter a single character for vector names.(-h for Help)\n");  
        return 0;              
    }
    
    int index1 = findVect(name1);
    int index2 = findVect(name2);

    if(index1 == -1 || index2 == -1){
        printf("Vector does not exist.\n");
        return 0;
    } else {
        struct vector c = addVect(vectors[index1], vectors[index2]);
        c.name = newName;
        if(stored){
            reallocate(c);
        } else {
            printf("%c = %f %f %f\n", c.name,
                    c.x, c.y, c.z);
        }
    }
    return 0;
}

int subtract(char *token, char name1, char newName, bool stored){
    token = strtok(NULL, DELIMETERS);

    if(token == NULL){
        printf("Invalid Format (-h for Help)\n");
        return 0;
    }

    char name2;
    if(strlen(token) == 1){
        name2 = token[0];
    } else{
        printf("Please enter a single character for vector names.(-h for Help)\n");   
        return 0;  
    }
    
    int index1 = findVect(name1);
    int index2 = findVect(name2);

    if(index1 == -1 || index2 == -1){
        printf("Vector does not exist.\n");
        return 0;
    } else {
        struct vector c = subtractVect(vectors[index1], vectors[index2]);
        c.name = newName;
        if(stored){
            reallocate(c);
        } else {
            printf("%c = %f %f %f\n", c.name,
                    c.x, c.y, c.z);
        }
    }
    return 0;
}

int multiply(char *token, char name1, char newName, int order, double scale, bool stored){
    token = strtok(NULL, DELIMETERS);

    if(token == NULL){
        printf("Invalid Format (-h for Help)\n");
        return 0;
    }

    if(order == 1){
        char *end;
        double scalar = strtod(token, &end);
        if(*end != '\0'){
            printf("Please enter a scalar and a vector for multiplication.(-h for Help)\n");
            return 0;
        } else {
            int index = findVect(name1);
            if(index != -1){
                struct vector c = scalarMultVect(vectors[index], scalar);
                c.name = newName;
                if(stored){
                    reallocate(c);
                } else {
                    printf("%c = %f %f %f\n", c.name,
                    c.x, c.y, c.z);
                }
            } else {
                printf("Vector doesn't exist.(-h for Help)\n");
                return 0;
            }
        }
    } else if(order == 2){
        char name2;
        if(strlen(token) == 1){
            name2 = token[0];
        } else {
            printf("Please enter a scalar and a vector for multiplication.(-h for Help)\n");
            return 0;                
        }
        int index = findVect(name2);
        if(index != -1){
            struct vector c = scalarMultVect(vectors[index], scale);
            c.name = newName;
            if(stored){
                reallocate(c);
            } else {
                printf("%c = %f %f %f\n", c.name,
                        c.x, c.y, c.z);
            }
        } else {
            printf("Vector doesn't exist.(-h for Help)\n");
            return 0;
        }
    }
    return 0;
}

void list(){
    for(int i = 0; i < count; ++i){
        printf("%c = %f %f %f\n", vectors[i].name,
                vectors[i].x, vectors[i].y, vectors[i].z);
    }
}

int assign(char *token, char name){
    token = strtok(NULL, DELIMETERS);

    if(token == NULL){
        printf("Invalid Format (-h for Help)\n");
        return 0;
    }

    char *end;
    double scalar = strtod(token, &end);
    char vect1;
    if(*end != '\0'){
        if(strlen(token) == 1){
            vect1 = token[0];
        } else{
            printf("Please enter a single character for vector names.(-h for Help)\n");
            return 0;
        }
    }

    token = strtok(NULL, DELIMETERS);

    if(token == NULL){
        printf("Invalid Format (-h for Help)\n");
        return 0;
    }

    if(*end != '\0'){
        if(token == NULL){
            printf("Invalid Format(-h for Help)\n");  
        } else if(strcmp(token, "+") == 0){
            add(token, vect1, name, true);
        } else if(strcmp(token, "-") == 0){
            subtract(token, vect1, name, true);
        } else if(strcmp(token, "*") == 0){
            int order = 1;
            multiply(token, vect1, name, order, 0, true);
        } else{
            printf("Invalid Format(-h for Help)\n");
        }
    } else {
        if(strcmp(token, "*") == 0){
            int order = 2;
            multiply(token, 'N', name, order, scalar, true);
        } else {
            struct vector c;
            c.name = name;
            c.x = scalar;
            c.y = 0;
            c.z = 0;

            char *end;

            if(token) {
                double val = strtod(token, &end);
                if(*end != '\0') {
                    printf("Invalid numeric value: %s (-h for Help)\n", token);
                    return 0;
                }
                c.y = val;
            }

            token = strtok(NULL, DELIMETERS);
            if(token) {
                double val = strtod(token, &end);
                if(*end != '\0') {
                    printf("Invalid numeric value: %s (-h for Help)\n", token);
                    return 0;
                }
                c.z = val;
            }
            reallocate(c);
        }
    }
    return 0;
}

void reallocate(struct vector c){
    if(count >= capacity){
        capacity += 10;
        vectors = realloc(vectors, capacity * sizeof(struct vector));
    }
    int indx = findVect(c.name);
    if(indx == -1){
        vectors[count] = c;
        count++;
    } else {
        vectors[indx] = c;
    }
}

void save(char *fileName){
    FILE *file_ptr = fopen(fileName, "w");

    if(!file_ptr){
        printf("Error: Invalid File Path/Name\n");
        return;
    }

    for(int i = 0; i < count; ++i){
        fprintf(file_ptr, "%c,%f,%f,%f\n", vectors[i].name, vectors[i].x,
        vectors[i].y, vectors[i].z);
    }

    fclose(file_ptr);
}

void load(char *fileName){
    free(vectors);
    vectors = malloc(10 * sizeof(struct vector));
    count = 0;
    capacity = 10;

    FILE *file_ptr = fopen(fileName, "r");
    if(!file_ptr){
        printf("Error: Couldn't open file\n");
        return;
    }

    char line[256];
    int lineNum = 0;

    while(fgets(line, sizeof(line), file_ptr)){
        lineNum++;
        if ((unsigned char)line[0] == 0xEF &&
        (unsigned char)line[1] == 0xBB &&
        (unsigned char)line[2] == 0xBF) {
            memmove(line, line + 3, strlen(line) - 2);
        }
        line[strcspn(line, "\r\n")] = '\0';

        if(strlen(line) == 0){
            continue;
        }

        char name[32];
        double x;
        double y;
        double z;

        int parsed = sscanf(line, " %31[^,],%lf,%lf,%lf", name, &x, &y, &z);
        if(parsed != 4){
            continue;
        }
        struct vector c;
        c.name = name[0];
        c.x = x;
        c.y = y;
        c.z = z;

        reallocate(c);
    }
    fclose(file_ptr);
}