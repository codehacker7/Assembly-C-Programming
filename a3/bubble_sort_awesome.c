#include <stdlib.h>
#include <stdio.h>

void custom_sort(int* array, int length) {
    int temp;

    for (int i = length - 1; i > 0; i--) {
        int* current_element = array + i;
        for (int j = i - 1; j >= 0; j--) {
            int* previous_element = array + j;
            if (*current_element < *previous_element) {
                temp = *current_element;
                *current_element = *previous_element;
                *previous_element = temp;
            }
        }
    }
}

int main(int argument_count, char** arguments) {
    char* end_pointer;
    int array_length;
    array_length = argument_count - 1;
    int* dynamic_array = malloc(array_length * sizeof(int));

    for (int i = 0; i < array_length; i++) {
        int* current_element = dynamic_array + i;
        *current_element = strtol(*(arguments + (i + 1)), &end_pointer, 10);
        if (*end_pointer) {
            fprintf(stderr, "Argument %d is not a number\n", i);
            return -1;
        }
    }

    custom_sort(dynamic_array, array_length);
    for (int i = 0; i < array_length; i++) {
        int* current_element = dynamic_array + i;
        printf("%d\n", *current_element);
    }

    free(dynamic_array); // don't forget to free the allocated memory
    return 0;
}