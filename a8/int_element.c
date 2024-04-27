  struct int_element {
    struct element_class *class;
    int value;
};

static void int_element_print(struct element *e) {
    struct int_element *ie = (struct int_element *)e;
    printf("%d", ie->value);
}

static int int_element_compare(struct element *e1, struct element *e2) {
    if (is_int_element(e2)) {
        int v1 = ((struct int_element *)e1)->value;
        int v2 = ((struct int_element *)e2)->value;
        if (v1 < v2) 
            return -1;
        else if (v1 == v2) 
            return 0;
        return 1;
    } else {
        return -1;
    }
}

static struct element_class int_element_class = { int_element_print, int_element_compare };

struct int_element *int_element_new(int value) {
    /* We have nothing to finalize here */
    struct int_element *e = rc_malloc(sizeof(struct int_element), NULL);
    e->class = &int_element_class;
    e->value = value;
    return e;
}

int int_element_get_value(struct int_element *e) {
    return e->value;
}

int is_int_element(struct element *e) {
    return e->class == &int_element_class;
}