/* vector.h
 * Gideon Stephenson
 * 10/8/25
 * A declaration for vector.c
 */
#include <stdbool.h>

struct vector{
    char name;
    double x;
    double y;
    double z;
};

struct vector addVect(struct vector a, struct vector b);
struct vector subtractVect(struct vector a, struct vector b);
struct vector scalarMultVect(struct vector a, double b);
void run();
int findVect(char vectName);
void help();
int add(char *token, char name1, char newName, bool stored);
int subtract(char *token, char name1, char newName, bool stored);
int multiply(char *token, char name1, char newName, int order, double scale, bool stored);
void list();
int assign(char *token, char name);
