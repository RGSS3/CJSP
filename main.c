#include <stdio.h>

typedef enum JType {
   JNUM, JSTRING, JARRAY, JOBJECT, JTRUE, JFALSE, JNULL
} JType;
typedef struct JValue JValue;
struct JValue {
   JType type;
   union {
      double jnum;
      char const * jstring;
      JValue ** jarray;
      JValue ** jobject;
   } v;
   int length;
};

#define lengthof(...) (sizeof((__VA_ARGS__)) / sizeof((__VA_ARGS__)[0]))
#define N(a)  ((JValue []){{.type = JNUM, .v = {.jnum    = a}}})
#define S(a)  ((JValue []){{.type = JSTRING, .v = {.jstring = a}}})
#define A(...)  ((JValue []){{.type = JARRAY,  .v = {.jarray = (JValue*[]){__VA_ARGS__}}, .length = lengthof((JValue*[]){__VA_ARGS__})}})
#define O(...)  ((JValue []){{.type = JOBJECT,  .v = {.jobject = (JValue*[]){__VA_ARGS__}}, .length = lengthof((JValue*[]){__VA_ARGS__}) / 2}})
#define T()   ((JValue []){{.type = JTRUE}})
#define F()   ((JValue []){{.type = JFALSE}})
#define E()   ((JValue []){{.type = JNULL}})


void print(JValue *j) {
   switch (j->type) {
      case JNUM: printf("%g", j->v.jnum); break;
      case JSTRING: printf("\"%s\"", j->v.jstring); break;
      case JARRAY: printf("[");
         for(int i = 0; i < j->length; ++i) {
            if (i) printf(",");
	        print(j->v.jarray[i]);
         }
         printf("]"); break;
      case JOBJECT: printf("{");
         for(int i = 0; i < j->length; ++i) {
            if (i) printf(",");
	        print(j->v.jarray[i * 2]);
            printf(":");
	        print(j->v.jarray[i * 2 + 1]);
         }
         printf("}"); break;
      case JTRUE: printf("true"); break;
      case JFALSE: printf("false"); break;
      case JNULL: printf("null"); break;
      default: break;
   }
}

int main() {
   print(O(S("a"), N(3), S("b"), A(N(1), T(), F(), E(), N(3), S("Hello"), O(S("a"), N(3), S("b"), N(5)))));
}


