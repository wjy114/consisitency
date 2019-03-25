#include <stdio.h>
#include <string.h>
#define SS {87}

	struct ainfo{
		int a;
        	int b;
        	char *c;
		char *d;
	};
struct ainfo **mm;
struct ainfo *m;

struct ainfo* bb(){
	int S=7;
	mm=(char *)malloc(sizeof(struct ainfo *));
      	if(mm=0){
             return 0;
      	}
        m=(struct ainfo*)malloc(sizeof(struct ainfo));
        if(m=0){
               return 0;
        }

//struct ainfo *aa;
const struct ainfo inf={
		.a=7,
		.b=6,
		.d=&inf.b
	};
m->a= inf.a;
m->b= inf.b;
m->d= inf.d;
return m;
}


int main(){
	
//	mm=(char *)malloc(sizeof(struct ainfo *));
//	if(mm!=0){
	
//		mm=bb(*mm);
//	}
//	m=(struct ainfo*)malloc(sizeof(struct ainfo));
//		if(m!=0){
		m=bb();
	
	printf("m");
	return 0;
}

