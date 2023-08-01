//-------------------------------------------------------------------------
//Программа вторичной обработки вибрации( Удаление постоянной составляющей,
//подгонка шкалы времени к отметчику, перераспределение файлов (теперь в
//одном файле будут 6 подрежимов одного канала))
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
const int Number=6144;
const int Num2=2048;
char ocenk[6];
float kof_st=1.0;//Коэффициент для подгонки шкалы времени к отметчику
float sum;
float sum_skl;
float koof;
float stepp;
int kof_zap=3;
int skl_sr=31;
float* aRes;
float* Otm;
float* r_Sksr;
        FILE *myfile;
	FILE *basefile;
        FILE *otmfile;
	char str[40];
	char rty[40];
	char otme[40];
	char fale[13];
	char vrvs[15];
	char vrnen[15];

	char unic_name [8];
	char O_name [5];

void main(int argc,char* argv[])
{  //1
 clrscr();
 
  mkdir( "obr_rg" );
       aRes=(float*)calloc(Number,sizeof(float));
       if(aRes==NULL){printf("Out memory\n");return;}

	Otm=(float*)calloc(Num2,sizeof(float));
	if(Otm==NULL){printf("Out memory\n");return;}

        r_Sksr=(float*)calloc(Num2,sizeof(float));
         if(r_Sksr==NULL){printf("Out memory\n");return;}

	clrscr();
	printf("\n\t\tВторичная обработка данных по вибрации\n");
   /*	 printf(" Выход по ESC или продолжать\n");
         int esc=getch();
	 if(esc==27) return;  */

	  printf(" Номер ГА (1-10)\n");
	 int ga=atoi(argv[1]);
         printf("%d\n",ga);

 //	  printf("\nОценка вибрации по: 1)Опорным узлам 2)Спинке статора\n");

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
           sprintf(rty,"c:\\DIAGR\\obr_rg\\%2s_%5s.r%02i",O_name,unic_name,i+1);
	 
           printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL) printf("Не могу открыть файл%35s\n",rty);

	    fprintf(basefile,"ГА %d\t",ga);
      /*        switch(fl)
                {
	  case 1:  fprintf(basefile,"Вибрация опорных узлов\t");
		   break;
	  case 2:  fprintf(basefile,"Вибрация спинки статора\t");
		   break;
	  default:  fprintf(basefile,"             \t");
		}              */

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


	   sprintf(fale,"%2s_%5sr%02i",O_name,unic_name,i+1); //Формирование имени файла для передачи его в Excel
	   fprintf(basefile,"%- 12s \t",fale);
	   fprintf(basefile,"Канал N\t%d\n",i+1);

	  for(int j=0;j<ch_reg;j++)    //ch_reg-число подрежимов
	   {  //5
	    sprintf(str,"c:\\DIAGNOST\\oscill\\%5s_%02i.o%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL)
	     {
	      printf("Не могу открыть файл%35s\n",str);
	     }
	    else
	      {//10
	    sprintf(otme,"c:\\DIAGNOST\\oscill\\%5s_%02i.o16",unic_name,j+1);
	     if(str!=otme)
	     {
	    otmfile=fopen(otme,"rt");
	    if(otmfile==NULL){printf("Не могу открыть файл%35s\n",otme);}
             }
	       float vspom;
	       float sum=0;
	       float vr;
	       fgets(vrvs,15,myfile);
//	       fscanf(myfile,"%s",&vrvs);
	       for(int k=0;k<Number;k++)    //Number-число шагов опроса АЦП
		{//6
		 fscanf(myfile,"%f",&vspom);
		 fscanf(myfile,"%f",&aRes[k]);
		 if(k==1) {stepp=vspom*kof_st;}
		 sum=aRes[k]+sum;
		} //6
		if(str!=otme){ fscanf(otmfile,"%s",&vrnen);}
	       if(str==otme){ fscanf(myfile,"%s",&vrnen);}
		for(int z=0;z<Num2;z++)
		 {
	       if(str!=otme){ fscanf(otmfile,"%f %f",&vr,&Otm[z]);}
	       if(str==otme){ fscanf(myfile,"%f %f",&vr,&Otm[z]);}
		}
		 koof=sum/Number;
		 float maxil=0;
		 float minil=0;
		 for(int r=0;r<Number;r++)
		 aRes[r]=aRes[r]-koof;
		 for(r=0;r<Num2;r++)
                   {
		   maxil=(aRes[r] > maxil) ? aRes[r]:maxil;
		   minil=(aRes[r] < minil) ? aRes[r]:minil;
                   }

		int flag=0;               //Обработка отметчика
		float sumotm=0;
                float otmkof=0;
		 for(int h=0;h<Num2;h++)
		   sumotm=sumotm+fabs(Otm[h]);
		 otmkof=sumotm/Num2;
		  for(h=0;h<Num2;h++)
		     if ((fabs(Otm[h])-otmkof)>kof_zap){flag=h;h=Num2;}
		   for(h=0;h<Num2;h++)
                      {
		      if ((Otm[h]-otmkof)>kof_zap) Otm[h]=maxil/1.42;
		      if ((Otm[h]+otmkof)<(kof_zap*(-1))) Otm[h]=minil/1.42;
		      }
		   for(h=0;h<Num2;h++)
		       if ((fabs(Otm[h])-otmkof)<=kof_zap) Otm[h]=-1;

     //Оцениваем вибрацию
                 float maxi=0;
		float mini=0;
                float max_vibr=0;
		for( k=0;k<Num2;k++)
		  {maxi=(aRes[k] > maxi) ? aRes[k]:maxi;} //Нахождение значения самого высокого пика
		for( k=0;k<Num2;k++)
		  {mini=(aRes[k] < mini) ? aRes[k]:mini;} //Нахождение значения самой глубокой впадины
		  max_vibr=fabs(maxi)+fabs(mini); //Определение макс. значения вибрации

                int w;
		for( k=0;k<Num2;k++)   //Нахождение скользящего среднего по 31-ому
		{
		 sum_skl=0;
		 for(w=k;w<(skl_sr+k);w++)
		  {sum_skl=aRes[w]+sum_skl;}
		   r_Sksr[k]=sum_skl/skl_sr;
		}

                float maxim=0;
		float minim=0;
                float maxsr_vibr=0;
		for( k=0;k<Num2;k++)
		  {maxim=(r_Sksr[k] > maxim) ? r_Sksr[k]:maxim;} //Нахождение значения самого высокого пика
                for( k=0;k<Num2;k++)
		  {minim=(r_Sksr[k] < minim) ? r_Sksr[k]:minim;} //Нахождение значения самой глубокой впадины
		  maxsr_vibr=fabs(maxim)+fabs(minim); //Определение макс.среднего значения вибрации

		for(h=0;h<Num2;h++)        //Синхронизация с отметчиком
		 {
		  aRes[h]=aRes[h+flag];
		  if((h+flag)<Num2){Otm[h]=Otm[h+flag];}
		  else{Otm[h]=-1;}
		  }

                 if(i+1<=fl)
		 {
	          if(maxsr_vibr<180){strcpy(ocenk,"неуд");}//Оценка средней вибрации
                  else{strcpy(ocenk,"недоп");}
		  if(maxsr_vibr<140){strcpy(ocenk,"уд");}
                  if(maxsr_vibr<100){strcpy(ocenk,"хор");} 
		  if(maxsr_vibr<50){strcpy(ocenk,"отл");}
                  }
	     else { if(maxsr_vibr<80){strcpy(ocenk,"уд");}//Оценка средней вибрации
		  else{strcpy(ocenk,"неуд");}
		  }

		 if(j>0) fprintf(basefile,"Подрежим N%-d\n",j+1);
		   else {
		         fprintf(basefile,"Подрежим N%-d\t",j+1);
			 fprintf(basefile,"%9s",vrvs);
			}

                   fprintf(basefile," Ср.виб.  \t");
		      fprintf(basefile," Макс.виб.  \t");
		       fprintf(basefile," Оценка  \n");
			fprintf(basefile,"%- .1f мкм \t",maxsr_vibr);
			 fprintf(basefile,"%- .1f мкм \t",max_vibr);
			  fprintf(basefile,"%- 6s \n",ocenk);
		  for(int d=0;d<1157;d++)
		   {
		    fprintf(basefile,"%- 4.4f \t",stepp*d);
		     fprintf(basefile,"%- .4f \t",aRes[d]);
		      fprintf(basefile,"%- .4f \n",Otm[d]);
		   }
                   
               } //10 
	        fclose(myfile);
		fclose(otmfile);
           }  //5
          fclose(basefile);
       	  }    //4
     
 free(aRes);
 free(r_Sksr);
 free(Otm);
}  //1