int array[4] = {0, 0, 0, 0};

int q2(int param_0, int param_1, int param_2) {
     if (param_0 < 10 || param_0 > 18)
        return 0;
    switch (param_0 - 10) {
        case 0:
            param_2 += param_1;
            break;
        case 1:
            param_2 = 0;
            break;
        case 2:
            param_2 = param_1 - param_2;
            break;
        case 3:
            param_2 = 0;
            break;
        case 4:
            if (param_1 > param_2)
                param_2 = 1;
            else
                param_2 = 0;
            break;
        case 5:
            param_2 = 0;
            break;
        case 6:
            if (param_2 > param_1)
                param_2 = 1;
            else
                param_2 = 0;
            break;
        case 7:
            param_2 = 0;
            break;
        case 8:
            if (param_2 == param_1)
                param_2 = 1;
            else
                param_2 = 0;
            break;
    }
    
    return param_2;
}

int main() {
    array[3] = q2(array[0], array[1], array[2]);
    return 0;
}