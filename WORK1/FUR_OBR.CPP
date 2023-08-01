//-------------------------------------------------------------------------
//Программа вторичной обработки вибрации по Фурье(перераспределение файлов (теперь в
//одном файле будут 6 подрежимов одного канала))
//-------------------------------------------------------------------------
#include <stdio.h>
#include <conio.h>
#include <dir.h>
#include <string.h>
#include <alloc.h>
#include <stdlib.h>
const int ch_reg=6;
const int CN=16;
const int Number=4096;
const int Num2=2048;
float sum;
float koof;
float stepp;
float* aRes;
float* Chast;
FILE *myfile;
	FILE *basefile;
	char str[40];
	char rty[40];
	char unic_name [8];
	char O_name [5];
	char fales[15];
	char vrvs[15];
	int fl;
void main(int argc,char* argv[])
{  //1
 clrscr();

  mkdir( "fur_obr" );
       aRes=(float*)calloc(Number,sizeof(float));
        if(aRes==NULL){printf("Out memory\n");return;}
       Chast=(float*)calloc(Number,sizeof(float));
        if(Chast==NULL){printf("Out memory\n");return;}
   
	clrscr();
	printf("\n\t\tВторичная обработка данных по Фурье\n");
	printf("\n");
/*	 printf(" Выход по ESC или продолжать\n");
         int esc=getch();
	 if(esc==27) return; */

           printf(" Номер ГА (1-10)\n");
	 int ga=atoi(argv[1]);
	 printf("%d\n",ga);

          printf("\n Оценка вибрации по: 16)Опорным узлам 0)Спинке статора\n");
	 printf("Число от 0 до 16 означает до какого канала(включительно) \n");
	 printf("вибрация оценивается как для опорных узлов, на оставшихся каналах -  \n");
	 printf("как для спинки статора (по умолчанию 0)\n");
	 int fl=atoi(argv[2]);
	 if(fl<=0||fl>16){fl=0;}
         printf("%d\n",fl);

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
	   sprintf(rty,"c:\\DIAGR\\fur_obr\\%2s_%5s.v%02i",O_name,unic_name,i+1);
           printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL)  printf("Не могу открыть файл%35s\n",rty);

	      fprintf(basefile,"ГА %d\t",ga);
       /*       switch(fl)
                {
	  case 1:  fprintf(basefile,"Вибрация опорных узлов\t");
		   break;
	  case 2:  fprintf(basefile,"Вибрация спинки статора\t");
		   break;
	  default:  fprintf(basefile,"             \t");
		}  */

          if(i+1<=fl) fprintf(basefile,"Вибрация опорных узлов\t");
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
			  
	   sprintf(fales,"%2s_%5sv%02i",O_name,unic_name,i+1); //Формирование имени файла для передачи его в Excel
	   fprintf(basefile,"%- 12s \t",fales);
	   fprintf(basefile,"Канал N\t%d\n",i+1);

	  for(int j=0;j<ch_reg;j++)    //ch_reg-число подрежимов
	   {  //5
	    sprintf(str,"c:\\DIAGNOST\\furie\\%5s_%02i.f%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL)
	     {
	      printf("Не могу открыть файл%35s\n",str);
	     }
	      else
	      {
	       float vspom=0;
	       fgets(vrvs,15,myfile);
	       for(int k=0;k<Number;k++)
	       {
		fscanf(myfile,"%f",&vspom);
//		fscanf(myfile,"%f.5",&Chast[k]);
		Chast[k]=vspom;
		fscanf(myfile,"%f",&aRes[k]);
		 }
                 if(j>0) fprintf(basefile,"Подрежим N%-d\n",j+1);
		   else {
		         fprintf(basefile,"Подрежим N%-d\t",j+1);
			 fprintf(basefile,"%9s",vrvs);
			}
		  for(int d=0;d<(Num2+32);d++)
		   {
		    fprintf(basefile,"% .4f \t",Chast[d]);
		     fprintf(basefile,"% .4f \n",aRes[d]);
		   }
		}
                fclose(myfile);
	
           }  //5
          fclose(basefile);
	  }    //4
 free(aRes);
 free(Chast);
}  //1
