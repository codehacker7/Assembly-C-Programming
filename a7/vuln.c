struct str str1 = {30, "Welcome! Please enter a name:\n"};
struct str str2 = {11, "Good luck, "};
struct str str3 = {43, "The secret phrase is \"squeamish ossifrage\"\n"};


void print(struct str *s) {
  write(1, s->data, s->length);
}

void proof() {
  print(&str3);
  exit(0); 
}

int main() {
  char buf[128];
  print(&str1);

  ssize_t size =  read(0, buf, 256);
  
  struct str userName = {size, buf};

  print(&str2);
  print(&userName);


}