
int x[8] = {1,2,3,-1,-2,0,184,340057058};
int y[8];

int f(int a){
    
    int l1 = 0;
    while(a != 0){
        if((a & 0x80000000) != 0){
            l1++;
        }
        a = a << 1;        
    }
    return l1;
}

int main(){
    
        for(int i=7;i>=0;i--){
        y[i] = f(x[i]);
        }
        for(int i=0;i<8;i++){
                printf("%i\n", x[i]);
        }
        for(int i=0; i<8;i++){
                printf("%i\n", y[i]);
         }

}