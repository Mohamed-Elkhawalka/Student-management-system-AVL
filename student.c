#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

node *root = NULL;
FILE *StudentsFile;

// Tolba
int AddStudent()
{
    if (countStudents(root) >= 100)
    {
        printf("Student list is full. Cannot add more students.\n");
        return 1;
    }
    printf("\n");
    student newStudent;
    newStudent.id = inputint("Enter the new student ID: ");
    if (searchByID(root, newStudent.id))
    {
        printf("Student already exists, cannot add duplicates.\n");
        return 2;
    }
    printf("Student name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';
    newStudent.gpa = inputfloat("Student GPA: ");
    root = insert(root, newStudent);
    printf("Student added successfully.\n");
    Savetofile();
    return 0;
}
node *deleteStudent(node *root, int id)
{
    if (root == NULL)
    {
        printf("Student with ID %d not found.\n", id);
        return root;
    }
    if (id < root->data.id)
        root->left = deleteStudent(root->left, id);
    else if (id > root->data.id)
        root->right = deleteStudent(root->right, id);
    else
    {
        if (root->left == NULL || root->right == NULL)
        {
            node *temp = root->left ? root->left : root->right;
            if (temp == NULL)
            {
                free(root);
                return NULL;
            }
            *root = *temp;
            free(temp);
        }
        else
        {
            node *temp = root->right;
            while (temp->left)
                temp = temp->left;
            root->data = temp->data;
            root->right = deleteStudent(root->right, temp->data.id);
        }
    }
    root->height = 1 + max(nodeHeight(root->left), nodeHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}
int UpdateStudent()
{
    int id = inputint("Enter Student ID to update: ");
    node *temp = searchByID(root, id);
    if (!temp)
    {
        printf("Student with ID %d not found.\n", id);
        return 1;
    }
    while (1)
    {
        printf("____________________________________________\n"
               "1. Change ID\n2. Change Name\n3. Change GPA\n4. Done\n");
        switch (inputint("Enter Your choice: "))
        {
        case 1:
        {
            student updated = temp->data;
            root = deleteStudent(root, temp->data.id);
            updated.id = inputint("Enter new ID: ");
            root = insert(root, updated);
            temp = searchByID(root, updated.id);
            break;
        }
        case 2:
            printf("Enter the new name: ");
            fgets(temp->data.name, sizeof(temp->data.name), stdin);
            temp->data.name[strcspn(temp->data.name, "\n")] = '\0';
            break;
        case 3:
            temp->data.gpa = inputfloat("Enter the new GPA: ");
            break;
        case 4:
            printf("Student updated successfully.\n");
            Savetofile();
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}
int DisplayAllStudents(node *root)
{
    if (!root)
        return 0;
    printf("Name: %s\nID: %d\nGPA: %.2f\n--------------------------\n",
           root->data.name, root->data.id, root->data.gpa);
    return 1 + DisplayAllStudents(root->left) + DisplayAllStudents(root->right);
}
node *searchByName(node *root, char *name)
{
    if (!root)
        return NULL;
    if (!strcmp(name, root->data.name))
        return root;
    node *found = searchByName(root->left, name);
    return found ? found : searchByName(root->right, name);
}
node *searchByID(node *root, int id)
{
    if (!root)
        return NULL;
    if (id == root->data.id)
        return root;
    return id < root->data.id ? searchByID(root->left, id) : searchByID(root->right, id);
}
// Marwan
int countStudents(node *root)
{
    if (!root)
        return 0;
    return 1 + countStudents(root->left) + countStudents(root->right);
}
void saveNode(node *n, FILE *f)
{
    if (!n)
        return;
    saveNode(n->left, f);
    fprintf(f, "%s|%d|%.2f\n", n->data.name, n->data.id, n->data.gpa);
    saveNode(n->right, f);
}
int Savetofile()
{
    StudentsFile = fopen("students.txt", "w");
    if (!StudentsFile)
    {
        printf("Error opening file for writing.\n");
        return 1;
    }
    saveNode(root, StudentsFile);
    fclose(StudentsFile);
    printf("Students saved to file successfully.\n");
    return 0;
}
node *Loadfromfile(node *root)
{
    StudentsFile = fopen("students.txt", "r");
    if (!StudentsFile)
    {
        printf("No saved file found.\n");
        return NULL;
    }
    char line[200];
    while (fgets(line, sizeof(line), StudentsFile))
    {
        char *name = strtok(line, "|");
        char *idStr = strtok(NULL, "|");
        char *gpaStr = strtok(NULL, "|");
        if (!name || !idStr || !gpaStr)
        {
            printf("Error reading student data from file.\n");
            continue;
        }
        name[strcspn(name, "\n")] = '\0';
        idStr[strcspn(idStr, "\n")] = '\0';
        gpaStr[strcspn(gpaStr, "\n")] = '\0';

        student s;
        strcpy(s.name, name);
        s.id = atoi(idStr);
        s.gpa = atof(gpaStr);
        root = insert(root, s);
    }
    fclose(StudentsFile);
    printf("Students loaded from file successfully.\n");
    return root;
}
int displayTreeStatistics()
{
    if (!root)
    {
        printf("Tree is empty.\n");
        return 1;
    }
    printf("____________ Tree Statistics ____________\n"
           "Total Students: %d\n"
           "Tree Height: %d\n"
           "Root Student ID: %d\n"
           "Root Balance Factor: %d\n"
           "_________________________________________\n",
           countStudents(root), nodeHeight(root), root->data.id, getBalance(root));
    return 0;
}
float totalGPA(node *root)
{
    if (!root)
        return 0;
    return root->data.gpa + totalGPA(root->right) + totalGPA(root->left);
}
void findPredecessorSuccessor(node *root, int id)
{
    if (!searchByID(root, id))
    {
        printf("Student with ID %d not found.\n", id);
        return;
    }
    node *pred = NULL;
    node *succ = NULL;
    node *curr = root;
    while (curr)
    {
        if (id < curr->data.id)
        {
            succ = curr;
            curr = curr->left;
        }
        else if (id > curr->data.id)
        {
            pred = curr;
            curr = curr->right;
        }
        else
        {
            if (curr->left)
            {
                node *temp = curr->left;
                while (temp->right)
                    temp = temp->right;
                pred = temp;
            }
            if (curr->right)
            {
                node *temp = curr->right;
                while (temp->left)
                    temp = temp->left;
                succ = temp;
            }
            break;
        }
    }

    printf("____________ Predecessor & Successor ____________\n");
    if (pred)
        printf("Predecessor -> Name: %s | ID: %d | GPA: %.2f\n", pred->data.name, pred->data.id, pred->data.gpa);
    else
        printf("Predecessor -> None (this is the smallest ID)\n");

    if (succ)
        printf("Successor   -> Name: %s | ID: %d | GPA: %.2f\n", succ->data.name, succ->data.id, succ->data.gpa);
    else
        printf("Successor   -> None (this is the largest ID)\n");
    printf("_________________________________________________\n");
}
int countAboveAverage(node *root, float avg)
{
    if (!root)
        return 0;
    return (root->data.gpa > avg) + countAboveAverage(root->left, avg) + countAboveAverage(root->right, avg);
}
int countBelowAverage(node *root, float avg)
{
    if (!root)
        return 0;
    return (root->data.gpa < avg) + countBelowAverage(root->left, avg) + countBelowAverage(root->right, avg);
}
float maxGPA(node *root)
{
    if (!root)
        return -999999;
    float left = maxGPA(root->left);
    float right = maxGPA(root->right);
    float m = root->data.gpa;
    if (left > m)
        m = left;
    if (right > m)
        m = right;
    return m;
}
float minGPA(node *root)
{
    if (!root)
        return 999999;
    float left = minGPA(root->left);
    float right = minGPA(root->right);
    float m = root->data.gpa;
    if (left < m)
        m = left;
    if (right < m)
        m = right;
    return m;
}
int displayGPAStatistics()
{
    if (!root)
    {
        printf("No students available.\n");
        return 1;
    }
    float average = totalGPA(root) / countStudents(root);
    printf("____________ GPA Statistics ____________\n"
           "Minimum GPA: %.2f\n"
           "Maximum GPA: %.2f\n"
           "Average GPA: %.2f\n"
           "Students Above Average: %d\n"
           "Students Below Average: %d\n"
           "________________________________________\n",
           minGPA(root), maxGPA(root), average,
           countAboveAverage(root, average),
           countBelowAverage(root, average));
    return 0;
}
void deleteAllStudents(node *root)
{
    if (!root)
        return;
    deleteAllStudents(root->left);
    deleteAllStudents(root->right);
    free(root);
}
int max(int a, int b)
{
    return a > b ? a : b;
}
node *createNode(student s)
{
    node *n = malloc(sizeof(struct node));
    n->data = s;
    n->left = NULL;
    n->right = NULL;
    n->height = 1;
    return n;
}
int inputint(const char *msg)
{
    int result, C, value;
    while (1)
    {
        printf("%s", msg);
        result = scanf("%d", &value);
        while ((C = getchar()) != '\n' && C != EOF)
            ;
        if (result)
            return value;
        printf("Invalid input. Please enter an integer value.\n");
    }
}
float inputfloat(const char *msg)
{
    int result, C;
    float value;
    while (1)
    {
        printf("%s", msg);
        result = scanf("%f", &value);
        while ((C = getchar()) != '\n' && C != EOF)
            ;
        if (result)
        {
            if (value < 0.0 || value > 4.0)
            {
                printf("Invalid GPA. (0.0 - 4.0)\n");
                continue;
            }
            return value;
        }
        printf("Invalid input. Please enter a float value.\n");
    }
}
// lab
int nodeHeight(node *node)

{
    return node ? node->height : 0;
}
node *insert(node *node, student s)
{
    if (!node)
        return createNode(s);

    if (s.id < node->data.id)
        node->left = insert(node->left, s);
    else if (s.id > node->data.id)
        node->right = insert(node->right, s);
    else
        return node;

    node->height = 1 + max(nodeHeight(node->left), nodeHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && s.id < node->left->data.id)
        return rightRotate(node);
    if (balance < -1 && s.id > node->right->data.id)
        return leftRotate(node);
    if (balance > 1 && s.id > node->left->data.id)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && s.id < node->right->data.id)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
int getBalance(node *node)
{
    return node ? nodeHeight(node->left) - nodeHeight(node->right) : 0;
}
node *rightRotate(node *y)
{
    node *x = y->left;
    node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    x->height = max(nodeHeight(x->left), nodeHeight(x->right)) + 1;
    return x;
}
node *leftRotate(node *x)
{
    node *y = x->right;
    node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(nodeHeight(x->left), nodeHeight(x->right)) + 1;
    y->height = max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    return y;
}