int arr[10] = {0,0,0,0,0,0,0,0,0,0};

void function2(int c, int d){
    arr[d] = arr[d] + c;
}

void function1(int a, int b){
    function2(3, 4);
    function2(a, b);
}

int main(){
    
    function1(1,2);
    for(int i = 0; i < 10; i++)
        printf("%i\n", arr[i]);
        
}