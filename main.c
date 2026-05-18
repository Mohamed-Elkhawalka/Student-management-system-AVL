#include <stdio.h>
#include <string.h>
#include "student.h"

int main(void)
{
    extern node *root;
    root = Loadfromfile(root);
    int Useing = 1;

    while (Useing)
    {
        printf("____________________________________________\n"
               "\tStudent Management System\t\n"
               "____________________________________________\n"
               "1. Add Student\n"
               "2. Delete Student\n"
               "3. Update Student\n"
               "4. Search Student By ID\n"
               "5. Search Student By Name\n"
               "6. Display All Students\n"
               "7. Delete all students\n"
               "8. Display Tree Statistics\n"
               "9. Display GPA Statistics\n"
               "10. find Predecessor and Successor\n"
               "0. Exit\n");

        switch (inputint("Enter your choice: "))
        {
        case 1:
            AddStudent();
            break;

        case 2:
            root = deleteStudent(root, inputint("Enter the ID of the student you want to delete: "));
            break;

        case 3:
            UpdateStudent();
            break;

        case 4:
        {
            int id = inputint("Enter the student ID: ");
            node *temp = searchByID(root, id);
            if (temp)
                printf("Student found!\nName: %s\nID: %d\nGPA: %.2f\n", temp->data.name, temp->data.id, temp->data.gpa);
            else
                printf("Student with ID %d is not found\n", id);
            break;
        }

        case 5:
        {
            char name[50];
            printf("Student name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;
            node *temp = searchByName(root, name);
            if (temp)
                printf("Student found!\nName: %s\nID: %d\nGPA: %.2f\n", temp->data.name, temp->data.id, temp->data.gpa);
            else
                printf("Student with the name %s is not found\n", name);
            break;
        }

        case 6:
            printf("_________________________\n\tAll Students:\t\n__________________________\n");
            DisplayAllStudents(root);
            break;

        case 7:
            deleteAllStudents(root);
            root = NULL;
            Savetofile();
            break;

        case 8:
            displayTreeStatistics();
            break;
        case 9:
            displayGPAStatistics();
            break;
        case 10:
            findPredecessorSuccessor(root, inputint("Enter the student ID: "));
            break;
        case 0:
            Useing = 0;
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
