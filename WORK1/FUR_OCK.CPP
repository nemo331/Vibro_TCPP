//-------------------------------------------------------------------------
//Программа оценки вибрации по Фурье(по основным гармоникам и частотам ,
//перераспределение файлов (теперь в одном файле будут 6 подрежимов одного канала))
//-------------------------------------------------------------------------
#include <stdio.h>
#include <conio.h>
#include <dir.h>
#include <string.h>
#include <alloc.h>
#include <stdlib.h>
#include <math.h>
const int ch_reg=6;
const int CN=16;
const int Number=4096;
const int Num2=2048;
float sum;
float koof;
float stepp;
float* aRes;
float* Chast;
char** occh;
FILE *myfile;
	FILE *basefile;
	char str[40];
	char rty[40];
	char unic_name [8];
	char O_name [5];
	char falek[15];
        char vrvd[15];
	int los;
	float garm[]={1.04,2.08,3.12,4.16,8.32,12.48,16.64,20.8,24.96,33.28,49.92,99.84};
	float sch[12];
	int gr[]={1,2,3,4,8,12,16,20,24,32,50,100};
	int diap[]={16,16,16,16,30,40,60,60,60,60,60,120};

void main(int argc,char* argv[])
{  //1
 clrscr();
  mkdir( "fur_ock" );
       aRes=(float*)calloc(Number,sizeof(float));
        if(aRes==NULL){printf("Out memory\n");return;}
       Chast=(float*)calloc(Number,sizeof(float));
	if(Chast==NULL){printf("Out memory\n");return;}
       occh=(char**)calloc(12,sizeof(char*));
	 if(occh==NULL){printf("Out memory\n");return;}
	   for(int q=0;q<12;q++)
	    {
	     occh[q]=(char*)calloc(7,sizeof(char));
	     if(occh[q]==NULL){printf("Out memory\n");return;}
	    }

	clrscr();
	printf("\n\t\tОценка данных по Фурье\n");
	printf("\n");
 /*	 printf(" Выход по ESC или продолжать\n");
         int esc=getch();
	 if(esc==27) return;   */

          printf(" Номер ГА (1-10)\n");
	 int ga=atoi(argv[1]);
         printf("%d\n",ga);


          printf("\n Оценка вибрации по: 16)Опорным узлам 0)Спинке статора\n");
	 printf("Число от 0 до 16 означает до какого канала(включительно) \n");
	 printf("вибрация оценивается как для опорных узлов, на оставшихся каналах -  \n");
	 printf("как для спинки статора (по умолчанию 0)\n");
   /*	 fls[0]=3;
	 cgets(fls);
	 strcpy(fls,&fls[2]);  */
	 los=atoi(argv[2]);
	 if(los<=0||los>16){los=0;}
	 printf("%d\n",los);

    printf("Уникальное  наименование обрабатываемых файлов \n");
    strcpy(unic_name,argv[3]);
    printf("%s\n",unic_name);

      printf("Наименование файла после обработки \n");
//   printf("Рекомендуемые имена: G[x]-генераторный режим,V[x]-возбуждение,\n");
//   printf("   X[x]-холостой ход,B[x]-выбег,S[x]-режим СК,где [x]-любой символ\n");
      strcpy(O_name,argv[4]);
      printf("%s\n",O_name);

	 printf("\n");
	     
	 for(int i=0;i<CN;i++)   //CN-число каналов
	  {   //4
	  if (kbhit())  { if (getch()==27) break;} //выход по ESC
	   sprintf(rty,"c:\\DIAGR\\fur_ock\\%2s_%5s.c%02i",O_name,unic_name,i+1);
             printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL) printf("Не могу открыть файл%35s\n",rty);
             fprintf(basefile,"ГА %d\t",ga);
   /*            switch(los)
                {
	  case 1:  fprintf(basefile,"Вибрация опорных узлов\t");
		   break;
	  case 2:  fprintf(basefile,"Вибрация спинки статора\t");
		   break;
	  default:  fprintf(basefile,"             \t");
		} */

	  if(i+1<=los) fprintf(basefile,"Вибрация опорных узлов\t");
	  else  fprintf(basefile,"Вибрация спинки статора\t");

           switch(O_name[0])
		{
	  case 'G':
	  case 'g':  fprintf(basefile,"Генераторный режим\n");
		   break;
	  case 'V':
	  case 'v':  fprintf(basefile,"Холостой ход с возбуждением\n");
		   break;
	  case 'X':
	  case 'x':  fprintf(basefile,"Холостой ход без возбуждения\n");
		   break;
	  case 'B':
	  case 'b':  fprintf(basefile,"Выбег\n");
		    break;
	  case 'S':
	  case 's':  fprintf(basefile,"Режим СК\n");
		   break;
	  default:  fprintf(basefile,"             \n");
		}

	     sprintf(falek,"%2s_%5sc%02i",O_name,unic_name,i+1); //Формирование имени файла для передачи его в Excel
	   fprintf(basefile,"%- 12s \t",falek);
	   fprintf(basefile,"Канал N\t%d\n",i+1);

	  for(int j=0;j<ch_reg;j++)    //ch_reg-число подрежимов
	   {  //5
	    sprintf(str,"c:\\DIAGNOST\\furie\\%5s_%02i.f%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL) printf("Не могу открыть файл%35s\n",str);
	      else
	      {
	       float vspom=0;
	       fgets(vrvd,15,myfile);
	       for(int k=0;k<Number;k++)
	         {
		fscanf(myfile,"%f",&vspom);
		Chast[k]=vspom;
		fscanf(myfile,"%f",&aRes[k]);
		 }
                 float maxi;
	       for(k=0;k<12;k++)
	       { maxi=0;
	       for(int gf=0;gf<Number;gf++)
		{  if(Chast[gf]<=garm[k]&&Chast[gf+1]>garm[k])
		      {  for(int zz=gf-(diap[k]/2);zz<gf+(diap[k]/2);zz++)
			 maxi=(aRes[zz] > maxi) ? aRes[zz]:maxi;
			 sch[k]=maxi;} 
		 }
		}

                   int d;
               if(i+1<=los)
		    {
	           for(d=0;d<10;d++)
		   {
		   if(sch[d]<(-45.65*log(garm[d])+195)) sprintf(occh[d],"неуд.");//Оценка вибрации на данной частоте
		  else
		   {
		   if ((-45.65*log(garm[d])+195)<0)  sprintf(occh[d],"отл.");
                   else sprintf(occh[d],"недоп.");
		   }
	  if(sch[d]<(-34.00*log(garm[d])+150)) sprintf(occh[d],"уд.");
	   if(sch[d]<(-23.129*log(garm[d])+100)) sprintf(occh[d],"хор.");
	     if(sch[d]<(-16.403*log(garm[d])+53)) sprintf(occh[d],"отл.");
		   }
                   }
		else
	       {  for(int d=0;d<3;d++)
		   {
		    if(sch[d]>80) sprintf(occh[d],"неуд.");
		    else  sprintf(occh[d],"уд.");
		   }
		   for(d=3;d<10;d++)
		   sprintf(occh[d],"     ");

                   }
		   for(d=10;d<12;d++)
		   {
		   if(sch[d]<30) sprintf(occh[d],"уд.");
		   else sprintf(occh[d],"неуд.");
		   }

                    if(j>0) fprintf(basefile,"Подрежим N%-d\n",j+1);
		   else {
		         fprintf(basefile,"Подрежим N%-d\t",j+1);
			 fprintf(basefile,"%9s",vrvd);
			}

		  fprintf(basefile,"N гарм.\t");
		  fprintf(basefile,"Вибрация\t");
                  fprintf(basefile,"Оценка\n");
		  for( d=0;d<12;d++)
		     {
		     fprintf(basefile,"%d \t",gr[d]);
		     fprintf(basefile,"% .4f \t",sch[d]);
                     fprintf(basefile,"% 6s\n",occh[d]);
		     }
	      }
		fclose(myfile);

	   }  //5
          fclose(basefile);
       	  }    //4

 free(aRes);
 free(Chast);
  for(q=0;q<12;q++)
 free (occh[q]);
 free (occh);
}  //1



