
 
/*
Sonal Unadkat  
Assignment 2	 	 	 	 
301110334
sunadkat@sfu.ca
CMPT 300 D100
Instructor: Brian G Booth
TA: Scott Kristjanson
 
*/
 
#include "decrypt.h"
#include "memwatch.h"
 
// HELPER FUNCTIONS
//__________________________________________________________________
 
void clear(char* string) {
   string[0] ='\0';
}
 
int cypher_to_text(char s) {
 
    int asc; 
    if(s >= 97 && s <= 122) asc = s - 96;
    else if(s == ' ') asc = 0;
    else if(s == '#') asc = 27;
    else if(s == '.') asc = 28;
    else if(s == ',') asc = 29;
    else if(s == '\'') asc = 30;
    else if(s == '!') asc = 31;
    else if(s == '?') asc = 32;
    else if (s == '(') asc = 33;
    else if (s == ')') asc = 34;
    else if (s == '-') asc = 35;
    else if (s == ':') asc = 36;
    else if (s == '$') asc = 37;
    else if (s == '/') asc = 38;
    else if(s == '&') asc = 39;
    else if(s == '\\') asc = 40;
  
    return asc;
 
}
 
unsigned char text_to_cypher(int s) {
      char decrypt;
      if(s >= 1 && s<= 26) decrypt = s + 96;
      else if(s == 0) decrypt = ' ';
      else if(s == 27) decrypt = '#';
      else if(s == 28) decrypt = '.';
      else if(s == 29) decrypt = ',';
      else if(s == 30) decrypt = '\'';
      else if(s == 31) decrypt = '!';
      else if(s == 32) decrypt = '?';
      else if(s == 33) decrypt = '(';
      else if(s == 34) decrypt = ')';
      else if(s == 35) decrypt = '-';
      else if(s == 36) decrypt = ':';
      else if(s == 37) decrypt = '$';
      else if(s == 38) decrypt = '/';
      else if(s == 39) decrypt = '&';
      else if(s == 40) decrypt = '\\';
       
      return decrypt;
}
 
// step 2- polynomial factoring - Horner Method -> learnt in cmpt 225
 
unsigned long long int horner_method(char* string) {
    int base = 41;
    int size = 0;
    unsigned long long int cyphernumber = cypher_to_text(string[size]);
    
    size += 1;
    while(size < 6) {
       cyphernumber = (cyphernumber * base) + cypher_to_text(string[size]);
       size +=1;
    }
    return cyphernumber;
}
 
// step 3. - modular exponation
// source: http://ww3.algorithmdesign.net/sample/ch10-crypto.pdf     
 
unsigned long long int modular_exponentation(unsigned long long int v, unsigned long long int exponent, unsigned long long int mod) {
  unsigned long long int check = v % mod;
  if(exponent == 0 ) return 1;
  else if(exponent % 2 == 0) return modular_exponentation(check*check % mod, exponent / 2, mod);
  else if(exponent == 1) return check;
  else
   
      return (check* modular_exponentation(check, exponent -1, mod)) % mod;
  }
   
  // step 4 - inverse decryption.
   
  void inverse_decrypt(unsigned long long int number,char* check) {
   int base = 41;
   char word[6];
   
   int character_number = number % 41;
   char c = text_to_cypher(character_number);
   
      word[5]= c;
     int joke = 4;
       while(joke >=0 ) {
         number = (number - character_number) /41;
    
         character_number = number % 41;
         c = text_to_cypher(character_number);
         word[joke] = c;
         joke -= 1;
       }
       joke = 0;
       while(joke < 6) {
         check[joke] = word[joke];
         joke += 1;
       }
}  
 
// MAIN FUNCTIONS DEFINED IN DECRYPT.H 
//__________________________________________________________________
 
int decrypt(char* encrypted_string, FILE* file) {
 
  // Int Variables & String Variables (Char* - Use Dynamic Memory)
  int size = 140;
  unsigned long long int exponent = 1921821779;
  unsigned long long int mod = 4294434817;
  char * modified_string = (char*) malloc(sizeof(char) * 168);
  char* newstring = malloc(sizeof(char) * 7);
  clear(newstring);
  clear(modified_string);
   
  // step 1: removed 8th character   
   
   int i = strlen(encrypted_string);
   int j = 0;
   int k = 0;
   
  while(k < i) {
       if ((k + 1) % 8 == 0) { k += 1; }
   else {
     modified_string[j] = encrypted_string[k];
     k += 1;
     j += 1;
   }
  }
   modified_string[j +1] = '\0';
   int dummy = strlen(modified_string);
   int loops = (dummy / 6);
   int start = 0;
   int end = 6;
   
   encrypted_string[0] = '\0';
  // Iterate every 6 letter words:
     while(loops > 0) {
     int test = 0;
     clear(newstring);
     
     while(start < end) {
       newstring[test] = modified_string[start];
       test += 1;
       start += 1;
     }
         unsigned long long int encrypted_number = horner_method(newstring); // step 2. Polynomial evaluation - Horner Method
         encrypted_number = modular_exponentation(encrypted_number,exponent,mod); // step 3. Modular Exponentation
         clear(newstring); // reuse of string for decrypted word
         inverse_decrypt(encrypted_number,newstring); // step 4. Decryption.
         newstring[6] = '\0';
         strcat(encrypted_string,newstring); // Concatenate each decrypted word
         
         
     if((end + 6) >= dummy) { // Editing Loop Conditions
       end = dummy;
     }
     else {end += 6;}
     test = 0;
     loops -= 1;
   
       } // while
   
   encrypted_string[167] = '\0';
   strcat(encrypted_string,"\n");
   fputs(encrypted_string,file);
   free(newstring);
   free(modified_string);  
   
   return 0;
    }

int Decryption(char string[]) {
    char* encrypted_filename= malloc(sizeof(char)* 1026);
    char* decrypted_filename = malloc(sizeof(char) * 1026);
    encrypted_filename[0] = '\0';
    decrypted_filename[0] = '\0';
    int size = strlen(string);
   
      int i = 0;
      int size2 = 0;
      int size3 = 0;
      // Splits Each String into Encrypted file and Decrypted File Name. 
    while(string[i] != ' ') {  // Encrypted file
      encrypted_filename[i] = string[i];
      ++i;
      ++size2;
    }
    encrypted_filename[size2] = '\0';
    ++i;
    int k = 0;
    while(string[i] != 10 && string[i] != 13 && i < size) { // Decrypted File
      decrypted_filename[k] = string[i];
      ++size3;
      ++i;
      ++k;
    }
    decrypted_filename[size3] = '\0';
  
        FILE *encrypted_file = fopen(encrypted_filename,"r");
        FILE *decrypted_file = fopen(decrypted_filename,"w");
      if(encrypted_file == NULL || decrypted_file == NULL) {
            //   PrintTime();
            //   printf("Child Process ID: #%d cannot complete, due to File Retreieval Error \n",getpid());
               free(encrypted_filename);
               free(decrypted_filename);
               return -2;
       }  
      char *encryptstring = malloc(sizeof(char)* 168);
      int stringsize = strlen(encryptstring);
      int check = 0;
      //3. Iterate through each tweet
      while(fgets(encryptstring,168,encrypted_file) != NULL) { // while end of file is not reached
        encryptstring[167] = '\0';
        check = decrypt(encryptstring, decrypted_file); // Calls Decryption Function defined in decrypt.h
        if(check != 0) {
               fclose(decrypted_file);
               fclose(encrypted_file);
               free(encrypted_filename);
               free(decrypted_filename);
               free(encryptstring);
               return -1;
            }
        } // while
        fclose(decrypted_file);
        fclose(encrypted_file);
        free(encryptstring); // Garbage Collection
        free(encrypted_filename);
        free(decrypted_filename);
        return 0;
  } 
    
 

