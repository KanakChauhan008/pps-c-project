/*
 * Student Record System
 * Subject : PPS (Programming for Problem Solving)
 * Language: C
 * Features: Add, Display, Search, Update, Delete student records
 *           (data is saved permanently in a file)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"
#define TEMP_FILE "temp.dat"

struct Student {
    int roll;
    char name[50];
    char branch[30];
    float marks;
};

/* ---------- Helper functions ---------- */

void readString(char *str, int size) {
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = '\0';
}

int readInt() {
    char buf[20];
    fgets(buf, sizeof(buf), stdin);
    return atoi(buf);
}

float readFloat() {
    char buf[20];
    fgets(buf, sizeof(buf), stdin);
    return (float)atof(buf);
}

int rollExists(int roll) {
    struct Student s;
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) return 0;
    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == roll) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void printStudent(struct Student s) {
    printf("%-8d %-20s %-15s %.2f\n", s.roll, s.name, s.branch, s.marks);
}

void printHeader() {
    printf("\n%-8s %-20s %-15s %s\n", "Roll", "Name", "Branch", "Marks");
    printf("--------------------------------------------------------\n");
}

/* ---------- Main features ---------- */

void addStudent() {
    struct Student s;
    FILE *fp;

    printf("\n--- Add Student ---\n");
    printf("Enter roll number: ");
    s.roll = readInt();

    if (rollExists(s.roll)) {
        printf("A student with this roll number already exists!\n");
        return;
    }

    printf("Enter name: ");
    readString(s.name, sizeof(s.name));
    printf("Enter branch: ");
    readString(s.branch, sizeof(s.branch));
    printf("Enter marks (out of 100): ");
    s.marks = readFloat();

    fp = fopen(FILE_NAME, "ab");
    if (fp == NULL) {
        printf("Error: could not open file.\n");
        return;
    }
    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
    printf("Student added successfully!\n");
}

void displayStudents() {
    struct Student s;
    int count = 0;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("\nNo records found.\n");
        return;
    }

    printf("\n--- All Students ---");
    printHeader();
    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printStudent(s);
        count++;
    }
    fclose(fp);

    if (count == 0)
        printf("No records found.\n");
    else
        printf("\nTotal students: %d\n", count);
}

void searchStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("\nNo records found.\n");
        return;
    }

    printf("\nEnter roll number to search: ");
    roll = readInt();

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == roll) {
            printHeader();
            printStudent(s);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
        printf("Student not found.\n");
}

void updateStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL) {
        printf("\nNo records found.\n");
        return;
    }

    printf("\nEnter roll number to update: ");
    roll = readInt();

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("Enter new name: ");
            readString(s.name, sizeof(s.name));
            printf("Enter new branch: ");
            readString(s.branch, sizeof(s.branch));
            printf("Enter new marks: ");
            s.marks = readFloat();

            fseek(fp, -(long)sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found)
        printf("Record updated successfully!\n");
    else
        printf("Student not found.\n");
}

void deleteStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp;

    if (fp == NULL) {
        printf("\nNo records found.\n");
        return;
    }

    temp = fopen(TEMP_FILE, "wb");
    if (temp == NULL) {
        printf("Error: could not create temporary file.\n");
        fclose(fp);
        return;
    }

    printf("\nEnter roll number to delete: ");
    roll = readInt();

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == roll)
            found = 1;
        else
            fwrite(&s, sizeof(s), 1, temp);
    }
    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename(TEMP_FILE, FILE_NAME);

    if (found)
        printf("Record deleted successfully!\n");
    else
        printf("Student not found.\n");
}

/* ---------- Main menu ---------- */

int main() {
    int choice;

    do {
        printf("\n==============================\n");
        printf("   STUDENT RECORD SYSTEM\n");
        printf("==============================\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        choice = readInt();

        switch (choice) {
            case 1: addStudent();      break;
            case 2: displayStudents(); break;
            case 3: searchStudent();   break;
            case 4: updateStudent();   break;
            case 5: deleteStudent();   break;
            case 6: printf("\nThank you! Goodbye.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    return 0;
}
