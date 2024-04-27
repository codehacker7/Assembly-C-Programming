#include <stdio.h>
#include <stdlib.h>

#include "element.h"
#include "int_element.h"
#include "refcount.h"
#include "str_element.h"

/* If the string is numeric, return an int_element. Otherwise return a str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null terminator. */
  int value = strtol(str, &endp, 10);
  if(str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}


int element_compare(const void *a, const void *b) {
    const struct element *elemA = *(const struct element **)a;
    const struct element *elemB = *(const struct element **)b;
    return elemA->class->compare(elemA, elemB);
}

int main(int argc, char **argv) {
    struct element **elements = malloc((argc - 1) * sizeof(struct element *));
    if (elements == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        elements[i - 1] = parse_string(argv[i]);
    }

    qsort(elements, argc - 1, sizeof(struct element *), element_compare);

    printf("Sorted: ");
    for (int i = 0; i < argc - 1; i++) {
        elements[i]->class->print(elements[i]);
        printf(" ");
        rc_free_ref(elements[i]);
    }
    printf("\n");

    free(elements);

    return 0;
}