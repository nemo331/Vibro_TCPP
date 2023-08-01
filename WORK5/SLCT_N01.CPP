/***********************************************************************/
/*Программа подбора имен для файлов замеров */
/***********************************************************************/
	   
#include <stdio.h>           
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>
#include <sys\stat.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <dos.h>
#include <io.h>
#include <dir.h>

struct stat statbuf;
FILE *myfile;   //файл с названиями уже имеющихся файлов
FILE *basefile; //файл с результатами подбора имени
char f_ust[45]; //Имя файла

//int dlin_naz;
char nazv_f[8];
char nazv_f2[8];
//int kol_vv_char; //кол-во введенных символов
char *alfabet= "abcdefghijklmnopqrstuvwxyzz";
char bukva[2];
int raznica;
int sluch;
char str1[50];
char msg[8];  
int nom_stolb;
int flag=0;
char *q;
char *p;
char *o;	 
int i;
int ef=0;
char otvet[4];
    

  void main(int argc, char* argv[])
  {
	   strcpy(str1,argv[1]);
	   
   START:  clrscr(); 
	   printf("                   Подбор имени\n");
	   printf("\n");
	   printf(" Выход по ESC или продолжать\n");
	   int esc=getch();
	   if(esc==27) return;
  NACHALO: printf("\rВведите кол-во символов в названии (не больше 6)\n");
	   char kol_naz[3];
	   kol_naz[0]=2;
	   //p = NULL;
	   p=cgets(kol_naz);
	   strcpy(kol_naz,p);

	   int dlin_naz=atoi(kol_naz);
	   if  (dlin_naz>6)
	   {dlin_naz=6;}

	   if  (dlin_naz<3)
	   {dlin_naz=3;}

	    printf("\nКол-во символов: %d\n",dlin_naz);
	    printf("\rВведите первые буквы названия файла (не больше 6 символов)\n");
	    nazv_f[0]=dlin_naz + 1;
	    //q=NULL;
	    q=cgets(nazv_f);
	    strcpy(nazv_f,q);
	    printf("\nВведенное название: %s\n",nazv_f);
	    memset(nazv_f2,'',7);
	    strcpy(nazv_f2,nazv_f);
	    int kol_vv_char = strlen(nazv_f);
	    //printf("\nКол символов: %d\n",dlin_naz);
	    //printf("\nnaz_f2: %s\n",nazv_f2);
	    if (dlin_naz>kol_vv_char)
	    {
	     raznica = dlin_naz - kol_vv_char;	    
	       randomize();
	       for (int i=kol_vv_char; i<dlin_naz; i++)
	       {
	       sluch = random (26);
	       //printf("\nsluch: %d\t",sluch);
	       //bukva[0] = NULL;
	       bukva[0] = alfabet[sluch];
	       //printf("Выбр буква: %s\n", bukva);
	       nazv_f2[i]=bukva[0];
	       }
	       
	       printf("Первоначальное название файла: %s\n", nazv_f);
	       printf("Окончательное название файла: %s\n", nazv_f2);
	    }
	   
	   //sprintf(str1,"C:\\TC\\Work5\\proba.txt");
	   //printf("%s\n",str1);
	   myfile=fopen(str1,"at+"); 
	   if(myfile==NULL)
	   {printf("Не могу открыть файл%35s\n",str1);
	   return;
	   }
	   
	   ef=0;
           flag=0;
	   fseek(myfile, 0, SEEK_SET);
	   do
	   {
	   ef=fscanf(myfile, "%s", &msg);
	   //printf("%s\n", msg);
	   if(strcmp(msg,nazv_f2)==0)
	    {
	     
	     flag=1;
	     break;
	    }
	   } while (ef!=EOF);
	   if(flag==1)
	   {
	   printf("Такое название файла уже есть. Попробуйте еще раз.\n");
	   fclose(myfile);
	   goto NACHALO;
	   }
	   else
	   {
	   printf("Записать название файла в базу(%s)? Y/N(по умолчанию N)\n",str1);
	   //memset(otvet,'',5);
	   otvet[0]=2;
	   o=cgets(otvet);
	   strcpy(otvet,o);
	   if (strcmp(strlwr(otvet),"y")==0)
	   {
	   fseek(myfile, 0, SEEK_END);
	   fprintf(myfile,"%s\n",nazv_f2); 
	   }
           }
	    fclose(myfile);
	   printf("\nСоздать еще одно название файла? Y/N(по умолчанию N)\n");
	   //memset(otvet,'',5);
	   otvet[0]=2;
	   o=cgets(otvet);
	   strcpy(otvet,o);
	   if (strcmp(strlwr(otvet),"y")==0)
	   {
	   goto START;
	   }
	   
  }