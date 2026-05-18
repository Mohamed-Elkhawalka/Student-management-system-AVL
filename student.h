#ifndef STUDENT_H
#define STUDENT_H

typedef struct student 
{
    int id;
    char name[50];
    float gpa;

} student;
typedef struct node 
{
    student data;
    int height;
    struct node *right;
    struct node *left;

} node;

int AddStudent();
int countStudents(node *root);
int countAboveAverage(node *root, float avg);
int countBelowAverage(node *root, float avg);
node *createNode(student s);
node *deleteStudent(node *root, int id);
int displayGPAStatistics();
int displayTreeStatistics();
int DisplayAllStudents(node *root);
void deleteAllStudents(node *root);
void findPredecessorSuccessor(node *root, int id);
int getBalance(node *node);
node *insert (node *node, student s);
int inputint(const char* msg);
float inputfloat(const char* msg);
node *leftRotate (node *x);
node *Loadfromfile(node *root);
int max(int a, int b);
float maxGPA(node *root);
float minGPA(node *root);
int nodeHeight(node *node);
node *rightRotate (node *y);
node *searchByID(node *root, int id);
node *searchByName(node *root, char *name);
void saveNode(node *n, FILE *f);
int Savetofile();
float totalGPA(node *root);
int UpdateStudent();
#endif