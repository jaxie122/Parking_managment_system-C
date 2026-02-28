#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct
{
    char car_number[50];
    char zone[20];
    int duration;
    float cost;
} ParkingTransaction;

int is_valid_car_number(const char *s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (!isdigit(s[i]) && !(s[i] >= 'A' && s[i] <= 'Z'))
            return 0;
    }
    return 1;
}

int read_positive_integer(const char *prompt)
{
    char input[100];
    int number;
    while (1)
    {
        printf("%s", prompt);
        scanf("%s", input);

        char *endptr;
        number = strtol(input, &endptr, 10);
        if (*endptr == '\0' && number > 0)
            return number;
        else
            printf("Invalid input! Please enter a positive integer.\n");
    }
}

void add_transaction()
{
    ParkingTransaction t;
    FILE *f = fopen("parking_data.txt", "a");
    if (!f)
    {
        printf("Error opening file!\n");
        return;
    }

    while (1)
    {
        printf("Enter car number (e.g., B123XYZ - UPPERCASE ONLY): ");
        scanf(" %[^\n]", t.car_number);

        if (is_valid_car_number(t.car_number))
            break;
        else
            printf("Invalid car number! Only uppercase letters and digits are allowed.\n");
    }

    int valid_zone = 0;
    while (!valid_zone)
    {
        printf("Enter zone (yellow/red/white): ");
        scanf("%s", t.zone);

        if (strcmp(t.zone, "yellow") == 0 || strcmp(t.zone, "red") == 0 || strcmp(t.zone, "white") == 0)
            valid_zone = 1;
        else
            printf("Invalid zone! Please enter 'yellow', 'red', or 'white'.\n");
    }

    t.duration = read_positive_integer("Enter duration in hours (positive integer): ");

    if (strcmp(t.zone, "yellow") == 0) t.cost = 1.5f * t.duration;
    else if (strcmp(t.zone, "red") == 0) t.cost = 2.0f * t.duration;
    else t.cost = 1.0f * t.duration;

    fprintf(f, "%s %s %d %.2f\n", t.car_number, t.zone, t.duration, t.cost);
    fclose(f);

    if (strcmp(t.zone, "red") == 0)
        printf("Payment successful! Cost: \033[1;31m%.2f RON\033[0m\n", t.cost);
    else if (strcmp(t.zone, "yellow") == 0)
        printf("Payment successful! Cost: \033[1;33m%.2f RON\033[0m\n", t.cost);
    else
        printf("Payment successful! Cost: \033[1;37m%.2f RON\033[0m\n", t.cost);
}

void view_history()
{
    ParkingTransaction t;
    FILE *f = fopen("parking_data.txt", "r");

    if (!f)
    {
        printf("No transaction history found.\n");
        return;
    }

    printf("\nTransaction History:\n");
    while (fscanf(f, "%49s %19s %d %f", t.car_number, t.zone, &t.duration, &t.cost) == 4)
    {
        printf("%s - %s Zone - %d hours - %.2f RON\n", t.car_number, t.zone, t.duration, t.cost);
    }
    fclose(f);
}

int main()
{
    int choice;
    do
    {
        printf("\n1. Pay for parking\n2. View history\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) add_transaction();
        else if (choice == 2) view_history();
        else if (choice == 3) printf("Goodbye!\n");
        else printf("Invalid choice.\n");
    } while (choice != 3);

    return 0;
}
