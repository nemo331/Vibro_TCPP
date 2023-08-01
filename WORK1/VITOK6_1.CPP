//-------------------------------------------------------------------------
//Программа нахождения массива среднего по 10 замерам для 96 полюсов ротора
//-------------------------------------------------------------------------
#include <stdio.h>           
#include <conio.h>
#include <dir.h>
#include <string.h>
#include <alloc.h>
#include <stdlib.h>
#include <math.h>
int ch_zam=10;
const int Number=8192;
const int Polus=150;
const int vrem=200;
const int r_Polus=96;
char kk;
float sum;
float sum_skl;
float koef;
float koef2;
float* Res;
float* r_Pol;
float* r_Pol_dl;
float* Pol;
float* v;
float* vr;
float** quit;
FILE *myfile;
FILE *youfile;
	FILE *basefile;
	FILE *konfile;
	float ga[10];
	char str1[40];
	char str2[40];
	char str3[40];
	char str4[40];
        char datevs[15];
	char unic_name [8];
	char O_name [5];
	char kof[8];
	char n_v[4];
	char GA_num[5];
	char* buffer;
	char* bu;
        char* num_b;

void main(void)
{  //1
 clrscr();
  mkdir( "c:\\DIAGR\\OBR_VIT" );
         Res=(float*)calloc(vrem,sizeof(float));
	if(Res==NULL){printf("Out memory\n");return;}
         Pol=(float*)calloc(vrem,sizeof(float));
	if(Pol==NULL){printf("Out memory\n");return;}
	 r_Pol=(float*)calloc(r_Polus,sizeof(float));
	if(r_Pol==NULL){printf("Out memory\n");return;}
        r_Pol_dl=(float*)calloc(r_Polus,sizeof(float));
	if(r_Pol_dl==NULL){printf("Out memory\n");return;}
         v=(float*)calloc(vrem,sizeof(float));
	if(v==NULL){printf("Out memory\n");return;}
         vr=(float*)calloc(vrem,sizeof(float));
	if(vr==NULL){printf("Out memory\n");return;}
	quit=(float**)calloc(r_Polus,sizeof(float*));
         if(quit==NULL){printf("Out memory\n");return;}
	for(int q=0;q<r_Polus;q++)
	 {
	  quit[q]=(float*)calloc(ch_zam,sizeof(float));
	  if(quit[q]==NULL){printf("Out memory\n");return;}
	}
         do
       {  //3
	clrscr();
	printf("                   Виток\n");
	printf("\n");
        printf(" Выход по ESC или продолжать\n");
         int esc=getch();
	 if(esc==27) return;

	 
         int n_ga=1;
	 do
	 {  printf("\rВведите номер ГА (1-10)\n");
	    GA_num[0]=3;
	    num_b=cgets(GA_num);
	    strcpy(GA_num,num_b);

	    n_ga=atoi(GA_num);
	 }
	 while((n_ga<1)||(n_ga>10));
	 

	 printf("\n Введите уникальное  наименование обрабатываемых файлов [xxxxx]\n");
    unic_name[0]=6;
    buffer=cgets(unic_name);
    strcpy(unic_name,buffer);

	  
	      n_ga=n_ga-1;
	   sprintf(str4,"c:\\DIAGR\\config.con");
	  konfile=fopen(str4,"r+t");
	  if(konfile==NULL){printf("Не могу открыть файл%35s\n",str4);}
	  for(int dv=0;dv<10;dv++)
	    fscanf(konfile,"%f ",&ga[dv]);

             printf("\n Введите значение отклонения по замеру клиновым щупом [xxxxx] \n");
	  printf("\t по умолчанию последнее использованное=\t");
	   koef2=ga[n_ga];
	   printf("% .4f\n",koef2);
	   kof[0]=6;
	   bu=cgets(kof);
	   koef2=atof(bu);

	  if(koef2==0) koef2=ga[n_ga];
	  else ga[n_ga]=koef2;
	  
	  fseek(konfile,0L,SEEK_SET);
	  for(dv=0;dv<10;dv++)

	    fprintf(konfile,"% .4f \n",ga[dv]);
          fclose(konfile);

	  printf("\nВведите номер витка [x] по умолчанию он равен 1\n");

     n_v[0]=2;
     bu=cgets(n_v);
     int nv=atoi(bu);
     if(nv==0||nv>3) nv=1;

     printf("\n Введите наименование файла после обработки  [xx]\n");
	do
	  {char* buf;
	   O_name[0]=3;
	   buf=cgets(O_name);
	   strcpy(O_name,buf);}
	while(O_name[0]==0);
	     printf("\n");
	     int sch_ch_zam=10; 
	     for(int j=0;j<ch_zam;j++)    //ch_zam-число замеров
	   {  //5
	       sprintf(str1,"c:\\DIAGNOST\\OGIBANIE\\%5s_%02i.g01",unic_name,j+1);
	       myfile=fopen(str1,"rt");
	       if(myfile==NULL)
		  {printf("Не могу открыть файл%35s\n",str1);
		   sch_ch_zam--;
		  }

	       sprintf(str3,"c:\\DIAGNOST\\OGIBANIE\\%5s_%02i.g02",unic_name,j+1);
	       youfile=fopen(str3,"rt");
	       if(youfile==NULL){printf("Не могу открыть файл%35s\n",str3);}
	       else
	       {
		 float vv;
		 float sl;
                 float time=0;
		 fgets(datevs,12,myfile);
		 for(int k=0;k<vrem;k++)    //Number-число шагов опроса АЦП
		    fscanf(myfile,"%f %f %f",&v[k],&vv,&Pol[k]);  //Виток
		 fgets(datevs,12,youfile);
		 for(int e=0;e<vrem;e++)    //Number-число шагов опроса АЦП
		 fscanf(youfile,"%f %f %f",&vr[e],&Res[e],&sl); //Отметчик

		 float max_v=0;
		 int flag=0;
		 for( k=0;k<vrem;k++)
		    {
		    max_v=(Res[k] > max_v) ? Res[k]: max_v ;
		    }
		 for( k=0;k<vrem;k++)
		    {
		    if(Res[k]==max_v){flag=k;}
		    Res[k]=0;
		    }
		 time=vr[flag];
		 int ch=0;
		 int  metka;
		 for(int yu=0;yu<Polus;yu++)
		    {if(v[yu]!=0){ch=ch+1;}
		    if(v[yu]<=time&&v[yu]!=0){metka=yu+1;} //Можно убрать еденицу
		    v[yu]=0; 
		    }
		 int ost_p=0;
		 if(ch-metka<r_Polus)
		    {
		    ost_p=r_Polus-(ch-metka);//Остаток полюсов
		    int metka2=metka-ost_p;
		    for(int x=0;x<(ch-metka);x++)
		    quit[x][j]=Pol[x+metka];     //Заполнение двумерного массива
		    for(int y=0;y<ost_p;y++)
		    quit[y+x][j]=Pol[y+metka2];
		    }
		 else
		    {
		    for(int a=0;a<r_Polus;a++)
		    quit[a][j]=Pol[a+metka];
		    }
	       }
	      fclose(myfile);
	      fclose(youfile);
	   }
     if(sch_ch_zam!=0) 
	   {   
	   ch_zam=sch_ch_zam;    
	   
	   sprintf(str2,"c:\\DIAGR\\OBR_VIT\\%2s_%5s.vit",O_name,unic_name);
	   printf("%35s\n",str2);
	   basefile=fopen(str2,"wt");
	   if(basefile==NULL){printf("Не могу открыть файл%35s\n",str2);}

	      float summ;
	      float sr_znch;
	      for(int ia=0;ia<r_Polus;ia++)
	      {
              summ=0;sr_znch=0;
	      for(int d=0;d<ch_zam;d++)
	      { summ=summ+quit[ia][d];}
	       sr_znch=summ/ch_zam;
	      r_Pol[(r_Polus-1)-ia]=sr_znch; //Заполнение массива r_Pol значениями зазора ротор-статор
					    //от 1-го полюса до 96-го
    
	      }
	          Pol[0]=r_Pol[94];    // Заполнение массива Pol[100] значениями из массива r_Pol[96]
	          Pol[1]=r_Pol[95];   // причем 2 первых из конца r_Pol ,а 2 последних из начала
                for(ia=2;ia<r_Polus+2;ia++)
                 Pol[ia]=r_Pol[ia-2];
		  Pol[98]=r_Pol[0];
		  Pol[99]=r_Pol[1];

              	int uu;
		for(int zz=0;zz<r_Polus;zz++)   //Нахождение скользящего среднего по 5-ти
		{
		 sum_skl=0;
		 for(uu=zz;uu<(5+zz);uu++)
		  {sum_skl=Pol[uu]+sum_skl;}
		   r_Pol[zz]=sum_skl/5;
		}
	      float summa=0;
              float srednee=0;
	       for(int d=0;d<r_Polus;d++)
	       summa=r_Pol[d]+summa;
		srednee=summa/r_Polus;
		for(d=0;d<r_Polus;d++)
		r_Pol[d]=(r_Pol[d]-srednee);
		float maxid=0;
		float minid=0;
		for(int k=0;k<r_Polus;k++)
		  {maxid=(r_Pol[k] > maxid) ? r_Pol[k]:maxid;
		  minid=(r_Pol[k] < minid) ? r_Pol[k]:minid; }
		  koef=koef2/(fabs(maxid)+fabs(minid));

		for(d=0;d<r_Polus;d++)
		r_Pol_dl[d]=r_Pol[d]*koef;

		 fprintf(basefile,"Виток%d\t ",nv);
                  fprintf(basefile,"Коэф.\t");
		   fprintf(basefile,"%.4f \t",koef);
		   fprintf(basefile,"%s",datevs);
		   fprintf(basefile,"ГА N%d\t ",n_ga+1);
		   fprintf(basefile,"Uвитка\t %.2f В\t",srednee);
                   fprintf(basefile,"Мех.зазор\t %.2f мм\n",koef2);
		 //fprintf(basefile,"Полюс\t ");
		 fprintf(basefile,"Отклонение от среднего:\t ");
		 fprintf(basefile,"Зазора\t Напряжения\n");
		 switch(nv)
		 {
	case 1: for(d=0;d<r_Polus;d++)
		fprintf(basefile,"% 2d\t % .4f\t % .4f \n",d+1,r_Pol_dl[d],r_Pol[d]);
		break;
	case 2: for(d=0;d<32;d++)
		fprintf(basefile,"% 2d\t % .4f\t % .4f \n",d+1,r_Pol_dl[64+d],r_Pol[64+d]);
		for(d=32;d<r_Polus;d++)
		fprintf(basefile,"% 2d\t % .4f\t % .4f \n",d+1,r_Pol_dl[d-32],r_Pol[d-32]);
		break;
        case 3: for(d=0;d<64;d++)
		fprintf(basefile,"% 2d\t % .4f\t % .4f \n",d+1,r_Pol_dl[32+d],r_Pol[32+d]);
		for(d=64;d<r_Polus;d++)
		fprintf(basefile,"% 2d\t % .4f\t % .4f \n",d+1,r_Pol_dl[d-64],r_Pol[d-64]);
		break;
		}

	      fclose(basefile);
	   }
	   printf("        Продолжать ? [y/n]       \n");
	   kk='';
	kk=getch();
  }  //3
 while ((kk!='n')&&(kk!='N'));
 free(Res);
 free(Pol);
 free(r_Pol);
 free(r_Pol_dl);
 free(vr);
 free(v);
 for(q=0;q<r_Polus;q++)
 free (quit[q]);
 free (quit);
}  //1
