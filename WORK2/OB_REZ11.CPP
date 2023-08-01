/***********************************************************************/
/*Программа обработки rez-файлов */
/***********************************************************************/
/*в версии 1.1 данные из rez-файлов считываются через интервал в 4-знакоместа*/
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

int Vel_ust[16]; //Величины уставок по каждому каналу
int kol_obr_f=0; //Кол-во обработанных файлов
int kol_zap_f=0; //Кол-во записанных файлов
int kol_prev_f=0;//Кол-во файлов имеющих превышения значения над уставкой

FILE *myfile;   //Файл с уставками
FILE *basefile; //log-файл
 

char f_ust[45]; //Имя файла с параметрами замера
char name_f1[150]; //Маска пути для нахождения первого файла замера
	char str1[60];
	char tttt[30];
	char str3[60];
	char str4[13];
	char* filu=NULL;
	char naz_filov[128];
int osh2=0;
int srv=1;
int flg_srv=0;
	
  int obr_f_dan(void);   // Функция обработки файла установок
  int obr_f_rez(char *str);   //Функция чтения rez-файла
  

  int main(void)
  { 
    char oldname[80], newname[80];
    mkdir ("biv_end");
    printf("\n\tОбработка файлов результов OB_REZ (Версия 1.1)\n");
    strcpy(tttt,"rez\\");
    strcpy(f_ust,"ust.txt");
    int osh=obr_f_dan();
    if (osh>0)
    {printf("\nВ файле паспортов допущены %d ошибки(ок) \n",osh);}
    
    struct ffblk ffblk;
    int done;
    char *sp;
    strcpy(name_f1,"REZ\\*.rez");
    sp=name_f1;
    done=findfirst(sp,&ffblk,0);
    sprintf(str3,"obr_rez.log");
    while (!done)
    {
       strncpy(str4,ffblk.ff_name,8);
       osh2=obr_f_rez(str4);
       if(osh2>0)
       {
	basefile=fopen(str3,"at+");
	if(basefile==NULL)
	    {printf("Не могу открыть файл%35s\n",str3);}
	srv=1;
	flg_srv=0;
	do
		{
		filu=fgets(naz_filov,13,basefile);
		srv=strcmp(filu,strlwr(ffblk.ff_name));
		if (srv==0) {flg_srv++;}
		filu=fgets(naz_filov,13,basefile);
		}  while (filu!=NULL);
	if (flg_srv==0)
		{ fprintf(basefile,"%s\n",strlwr(ffblk.ff_name));
		  size_t kol_sim = strlen(ffblk.ff_name);
		  kol_sim=kol_sim-4;
		  char ur_name[80];
		  memset(ur_name,'',80);
		  memmove(ur_name,ffblk.ff_name,kol_sim);
		  sprintf(oldname,"BIV\\%s.bin",ur_name);
		  int exi=access(oldname,0);
		  //printf("%d\n",exi);
		  if (exi==0)
		  {sprintf(newname,"BIV_END\\%s.bin",ur_name);}
		  else
		   {
		    sprintf(oldname,"BIV\\%s.biv",ur_name);
                    sprintf(newname,"BIV_END\\%s.biv",ur_name);
		   }

                  if (rename(oldname, newname) == 0)
		     printf("Файл переименован  с %s в %s\n", oldname, newname);
                  else
		     perror("rename"); 
                        
		  
		  kol_zap_f++;
		}
	fclose(basefile);	 
       kol_prev_f++;	
       }
       kol_obr_f++;
       done=findnext(&ffblk);
    }
    printf("Обработано файлов - %d, из них %d превышает уставку и %d записано в log-файл\n",kol_obr_f,kol_prev_f,kol_zap_f);
   if (kol_zap_f>0) {return 1;}
   else {return 0;}
  }
/**************************/
/* Чтение файла параметров*/
/**************************/
  int obr_f_dan(void)
       {

	int osh=0;
	int c;
	int ot=0;        //Флаг символа табуляции
	int on=0;        //Флаг символа конца строки
	int kav=0;       //Флаг символа кавычек
	int fldv=0;      //Флаг символа двоеточия
	char pred[100];
	char prom[100];
	char t[2];
       sprintf(str1,"%s",f_ust);       //  C:\\TC\\WORK2\\
       myfile=fopen(str1,"rt");  //Открытие установочного файла
      if(myfile==NULL)
	    {printf("Не могу открыть файл%35s\n",str1);return 0;}
      char* df=NULL;

      c=0;
      for(int tf=0;tf<100;tf++)
     {
   do{
	 strcpy(pred,"");
         fldv=0;    //флаг по двоеточию сброшен
      do {
	  df=fgets(t,2,myfile);
	    ot=stricmp(df,"\t"); //    Сравнить на наличие символа табуляции
	    on=stricmp(df,"\n"); //    Сравнить на наличие символа перевода строки
	    kav=stricmp(df,"\:");
	    if(kav==0) {fldv=1;}
             if (df==NULL) break; 
	   if (fldv!=0 && on!=0 && kav!=0) { strcat(pred,t);}
	   if (fldv!=0) strcpy(prom,pred);
	   if(kav==0) {c=c+1;}
	 }  while (ot!=0 && on!=0); // Делать пока не табуляция или пока не перевод строки
	  
	     switch(c)
	     { 
	case 1: Vel_ust[0]=atoi(prom);		 break;
	case 2: Vel_ust[1]=atoi(prom);	 	 break;
	case 3: Vel_ust[2]=atoi(prom);            break;
	case 4: Vel_ust[3]=atoi(prom);	         break;
	case 5: Vel_ust[4]=atoi(prom);		 break;
	case 6: Vel_ust[5]=atoi(prom); 	 	 break;
	case 7: Vel_ust[6]=atoi(prom);		 break;
	case 8: Vel_ust[7]=atoi(prom);	 	 break;
	case 9: Vel_ust[8]=atoi(prom);            break;
	case 10: Vel_ust[9]=atoi(prom);	         break;
	case 11: Vel_ust[10]=atoi(prom);		 break;
	case 12: Vel_ust[11]=atoi(prom); 	 	 break;
	case 13: Vel_ust[12]=atoi(prom);		 break;
	case 14: Vel_ust[13]=atoi(prom);	 	 break;
	case 15: Vel_ust[14]=atoi(prom);           break;
	case 16: Vel_ust[15]=atoi(prom);	         break;
	
	default: break;
	     }
	   
        if (df==NULL) break;
	
      }  while (on!=0);
        if (df==NULL) break; 
     }	
     fclose(myfile);
      return osh;
       } //КОНЕЦ ФУНКЦИИ obr_f_dan

/********************/
/* Чтение rez-файла */
/********************/
  int obr_f_rez(char* str)
       { 
	 FILE *rezfile; //rez-файл
	 int Nom_kol[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Массив флагов по тем каналам которые есть
	 int aRes[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Массив с данными по вибрации целые числа
	 int osh3=0;
	 int on=0;        //Флаг символа конца строки
	 int flg_kan=0;   //Флаг канала
	 int flg_K=0;     //флаг буквы к
	 int flg_space=0;  //флаг пробела
	 char* pred=" ";
	 char prom[100];
	 char stroka[128];
	 char ssss[80];
	 
	 char t[2];
	 int n_pp=0;
	 int kol_kan=0;
	 strcpy(ssss,tttt);
	 strcat(str,".rez");
	 strcat(ssss,str);
	 rezfile=fopen(ssss,"rt+");  //Открытие файла результатов
	 if(rezfile==NULL)
	    {printf("Не могу открыть файл%35s\n",ssss);return 0;}
	 char* df=NULL;
	 
	  df=fgets(stroka,11,rezfile);

	  do {  
		df=fgets(t,2,rezfile);
		on=stricmp(df,"\n"); //    Сравнить на наличие символа перевода строки
		flg_K=stricmp(df,"К");
		flg_space=stricmp(df," "); 
		if (flg_K==0) flg_kan=1;
		if (flg_space==0) flg_kan=0;
		
		if (flg_kan==1 && flg_K!=0) 
		  { 
		    strcat(pred,t);
		    n_pp++;
		    if (n_pp>1)
		    {Nom_kol[kol_kan]=atoi(pred)+1;
		     n_pp = 0;
		     strcpy(pred," ");
		     kol_kan++;
		    }
		  }

	     }while (on!=0);
	  df=fgets(stroka,128,rezfile);
	  df=fgets(stroka,128,rezfile);
	  df=fgets(stroka,11,rezfile);
	  for(int ttt=0;ttt<kol_kan;ttt++)
	  { 
	  df=fgets(stroka,5,rezfile);
	  //df=fgets(t,2,rezfile);
	  aRes[Nom_kol[ttt]-1]=atoi(stroka);
	  if (Vel_ust[Nom_kol[ttt]-1]>0)
	     {
	     if(aRes[Nom_kol[ttt]-1]>Vel_ust[Nom_kol[ttt]-1])
	     { osh3=1;}
	     }
	  }
	 df=fgets(stroka,128,rezfile);
	 fclose(rezfile);
	 return osh3;
       }     //КОНЕЦ ФУНКЦИИ obr_f_rez