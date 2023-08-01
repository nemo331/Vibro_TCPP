/***********************************************************************/
/*Программа сбора данных по вибрации с помощью платы LA70 (оперативный */
/* и дежурный опросы                                                   */
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

int *Nom_kol; //Массив с номерами каналов сбора
int *Nom_deg; //Массив с номерами каналов деж опроса
int nom_kan_otm[1];

FILE *myfile;   //Файл с параметрами опроса
FILE *basefile; //Файл замера

char ch_kan[40]; //Каналы опроса в виде строки
char ch_deg[40]; //Каналы дежурного опроса в виде строки
int far **aRes; //Массив с данными по вибрации в кодах АЦП
char f_ust[45]; //Имя файла с параметрами замера
char name_f1[15]; //Имя первого файла замера(вводится из ком строки или bat-файла)
char name_f2[7]; //первые 6 символов полного названия файла
char rassh[5]; //расширение файла замера
int nom_fila;    //номер введенного названия файла
char nom_f_c[3];
	char str1[50];
	char str2[50];
	char kod[8];
	char vremia[18];
	
  typedef struct 
       { char regim_opr; //режим опроса (0-оперативный 1-дежурный)
	 int kol_filov; //кол-во файлов сбора
	 int adres;  //адрес платы АЦП (220)
	 float Chastota; //Частота опроса АЦП
	 int fl; //Кол-во каналов для сбора
	 int Number; //Кол-во отсчетов на канал
	 char zap_otm; //Запуск от отметчика (1-да, 0-нет)
	 char num_otm[2]; //Номер канала отметчика
	 int num_nab;  //Номер набора коэффициентов
	 float min_porog;//Пороговые значения для запуска опроса
	 float max_porog;
	 int fl_deg; //Кол-во каналов для деж опроса
       } partip;   //структура данных с параметрами опроса

  typedef struct 
       { int kol_filov2; //кол-во файлов сбора
	 float Chastota2; //Частота опроса АЦП
	 char ch_kan2[18]; //Каналы опроса в виде строки
	 int Number2; //Кол-во отсчетов на канал
	 int num_otm2; //Номер канала отметчика
	 int num_nab2;
       } vuhtip;   //структура данных с параметрами для записи в выходной файл

    float aKof1[]={0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,
		   0.25,0.25,0.25,0.25,0.25,0.25,0.25,};
    float aKof2[]={0.02657878, 0.254065041, 0.026966292, 0.257953568,
                   0.026773762, 0.259515571, 0.027548209, 0.265674814,
		   0.02608242, 0.247116969, 0.027083145, 0.260642919,
		   0.026821157, 0.255362615, 0.02728513, 0.261551874};//БКВ-1 (Выходы на шлейфовый осциллограф)
   float aKof3[]={0.273224044,0.267379679,0.273224044,0.304878049,
		  0.263157895,0.267379679,0.255102041,0.267379679,
		  0.273224044,0.267379679,0.273224044,0.304878049,
		  0.263157895,0.267379679,0.255102041,0.267379679}; //БКВ-2 (Выходы по пост. напряжению на ЭВМ)		 
   float aKof4[]={0.025106703,0.239808153,0.025106703,0.234192037,
		  0.025297243,0.235294118,0.02728513,0.265251989,
		  0.024431957,0.228832952,0.024956326,0.232018561,
		  0.024697456,0.224719101,0.0264527839,0.230946882};//БКВ-2 (Выходы на шлейфовый осциллограф) 
   float aKof[16]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
		   1.0,1.0,1.0,1.0};

  partip par;
  vuhtip vuh;
  int obr_f_dan(void);   // Функция обработки файла установок
  void kol_kan(char* strok, int *mas1); //Функция определения кол-ва каналов и кол-ва их повторений

  void main(int argc,char* argv[])
  { clrscr();
    mkdir ("biv"); //каталог куда будет занесен собранный файл
    printf("\n\t\tОпрос каналов АЦП \n");
    strncpy(name_f1,argv[1],8);
    if (strlen(argv[1])!=12)
     {printf("Неправильная длина названия файла замера!\n"); return;}
    if(strrchr(argv[1],'.')==NULL)
     {printf("Не указано расширение в названии файла замера!\n"); return;}
    strncpy(name_f2,argv[1],6);
    strrev(strncpy(rassh,strrev(argv[1]),4));
    strrev(argv[1]);
    strrev(strncpy(nom_f_c,strrev(name_f1),2));
    strrev(name_f1);
    nom_fila=atoi(nom_f_c);

    strcpy(f_ust,"C:\\TC\\WORK2\\param.txt");
    int osh=obr_f_dan();
    if (osh>0)
    {printf("\nВ файле паспортов допущены %d ошибки(ок) \n",osh);}
    printf("\t\tПараметры опроса \n");
    printf("\n\t\tРежим опроса: ");
    switch  (par.regim_opr)
	 { case 0: printf("Оперативный"); break;
	   case 1: printf("Дежурный"); break; }
    printf("\n\t\tКол-во файлов сбора: %d",par.kol_filov);
    printf("\n\t\tАдрес платы АЦП LA70: %d",par.adres);
    printf("\n\t\tЧастота опроса АЦП: %.3f",par.Chastota);
    printf("\n\t\tКаналы АЦП для сбора: %s",ch_kan);
    printf("\n\t\tКол-во отсчетов на канал: %d",par.Number);
    printf("\n\t\tЗапуск от отметчика? ");
    switch (par.zap_otm)
	 { case 0: printf("Нет"); break;
	   case 1: printf("Да");  break; }
    printf("\n\t\tНомер канала отметчика: %s",par.num_otm);
    printf("\n\t\tНомер набора коэффициентов: %d",par.num_nab);
    if (par.regim_opr==1)
       { printf("\n\t\tПороговые значения для запуска опроса:");
	 printf("\n\t\t\tМинимальное: %.2f",par.min_porog);
	 printf("\n\t\t\tМаксимальное: %.2f",par.max_porog);
         printf("\n\t\tКаналы АЦП для дежурного опроса: %s",ch_deg);
       }

      
    Nom_kol=(int*)calloc(par.fl,sizeof(int));
    if(Nom_kol==NULL){printf("Out memory\n");return;}
    kol_kan(ch_kan,Nom_kol); 
    Nom_deg=(int*)calloc(par.fl_deg,sizeof(int));
    if(Nom_deg==NULL){printf("Out memory\n");return;}
    kol_kan(ch_deg,Nom_deg);
    kol_kan(par.num_otm,nom_kan_otm);

    sprintf(kod,"vibracia");
    sprintf(str2,"biv\\%s",argv[1]);
    basefile=fopen(str2,"wb+");

    fwrite(&kod,8,1,basefile);
    fwrite(&name_f1,8,1,basefile);
    fwrite(&par.regim_opr,sizeof(par.regim_opr),1,basefile);
    fwrite(&par.kol_filov,sizeof(par.kol_filov),1,basefile);
    fwrite(&par.adres,sizeof(par.adres),1,basefile);
    fwrite(&par.Chastota,sizeof(par.Chastota),1,basefile);
    fwrite(&ch_kan,16,1,basefile);
    fwrite(&par.Number,sizeof(par.Number),1,basefile);
    fwrite(&par.zap_otm,sizeof(par.zap_otm),1,basefile);
    fwrite(&nom_kan_otm[0],2,1,basefile);
    fwrite(&par.num_nab,sizeof(par.num_nab),1,basefile);
    fwrite(&par.min_porog,sizeof(par.min_porog),1,basefile);
    fwrite(&par.max_porog,sizeof(par.max_porog),1,basefile);
    fwrite(&ch_deg,16,1,basefile);
    fclose(basefile);
    free(Nom_kol); 
    free(Nom_deg);	 
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
	case 1: par.regim_opr=atoi(prom);		 break;
	case 2: par.kol_filov=atoi(prom);	 	 break;
	case 3: par.adres=atoi(prom);                    break;
	case 4: par.Chastota=atof(prom);	         break;
	case 5: par.fl=strlen(prom);
		//fl=par.fl;
		if ((par.fl>16)||(par.fl<1))
		{osh=osh+1;}
		strcpy(ch_kan,prom);   	 	 
		break;
	case 6: par.Number=atoi(prom); 	 	 break;
	case 7:  if (strchr(prom,'Y')!=NULL) 
	         {par.zap_otm=1;}
		 else par.zap_otm=0; 	 	 break;
	case 8:  strncpy(par.num_otm,prom,1); 	 
		 break;
	case 9: par.num_nab=atoi(prom); 	 break;
	case 10: par.min_porog=atof(prom); 	 break;
	case 11: par.max_porog=atof(prom); 	 break;
	case 12: par.fl_deg=strlen(prom); 
		 //fl_deg=par.fl_deg;       
		 if ((par.fl_deg>16)||(par.fl_deg<1))
		 {osh=osh+1;}
                 strcpy(ch_deg,prom);
	         break;
	default: break;
	     }
	   
        if (df==NULL) break;
	
      }  while (on!=0);
        if (df==NULL) break; 
     }	
     fclose(myfile);
      return osh;
       } //КОНЕЦ ФУНКЦИИ obr_f_dan
/*************************************************************/
/* Определение кол-ва каналов и занесение их номеров в массив*/
/*************************************************************/
  void kol_kan(char *strok, int *mas1)
     {
     char odsim[2];
     odsim[0]='\0';
     odsim[1]='\0';
     int ch_ccc=strlen(strok);
     for(int tf=0;tf<ch_ccc;tf++)
     {
      memcpy(odsim,&strok[tf],1);      //Преобразование строки с номерами
      sscanf(odsim,"%x",&mas1[tf]); //каналов в десятичные числа и 
      strset(odsim,'\0');              //сохранение их в массиве mas1
     }
/*        int vrNom=0;
         for(tf=0;tf<fl;tf++)             //Заполнение второго столбца массива
         {                                //отвечающего за количество повторений  
          vrNom=mas1[0][tf];               //данного канала
           int schNom=0;
           for(int ff=tf;ff<fl;ff++)
	    {if (vrNom==mas1[0][ff]&&mas1[1][ff]==0)
		 {schNom=schNom+1;mas1[1][ff]=-1;}
            }
          mas1[1][tf]=schNom;
	 }                       */
      //	 for(tf=0;tf<fl;tf++)
     //	 printf("%d\n",mas1[tf]);
     }   //КОНЕЦ ФУНКЦИИ kol_kan