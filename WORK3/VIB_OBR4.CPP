//-------------------------------------------------------------------------
//Программа вторичной обработки вибрации( Удаление постоянной составляющей,
//подгонка шкалы времени к отметчику, перераспределение файлов (теперь в
//одном файле будут 6 подрежимов одного канала), сглаживание скользящим
//средним,определение амплитуды вибрации и ее оценка)
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
const int Number=8192;
const int Num2=2048;
char ocenk[6];
float kof_st=1.08;//Коэффициент для подгонки шкалы времени к отметчику
float sum;
float sum_skl;
float koof;
float stepp;
int j;
int i;
int kof_zap=3;
int skl_sr=31;
float far *aRes;
float far *Otm;
float far *r_Sksr;
float *sr20max;
float *sr20min;
float *sr20maxskl;
float *sr20minskl;
float vr;
int fal[40];
	FILE *myfile;
	FILE *basefile;
	FILE *otmfile;
        char str[40];
	char rty[40];
	char otme[40];
	char fale[13];
	char vrvs[15];
	char vrnen[15];
           char tek_naz[6];
	   char unic_name [8];
	   char O_name [5];
	   int rar;

           char fdan[40];
	   FILE *datfile;
	   int tf;
           int ot=0;
	   int on=0;
	   int oc=0;
           int op=0;
           int kav=0;
	   char prom[50];
	   char t[2];
	   char sop[4];
typedef struct dat_reg
	  { char N_pp[6]; char Name_f[14]; char N_GA[5]; char Date_zam[12];
	    char Regim[6]; char Podreg[9]; char P[8]; char Q[9];
	    char Irot[8]; char Ist[9]; char Ust[10]; char RK[8];
	    char NA[9]; char Oborot[12];char tgel[9];char Napor[19];
	    char Rem[50];char chan[15][21];
	  } dtyp;
	dtyp spisok[60];
	      void obr_f_dan(void); //Обработка файла данных
              void nach_z_kon_f(void);//Начало заполнения конечного файла

  void main(void)  
{  //1
 clrscr();
 
  mkdir( "obr_rg" );
       aRes=(float*)farcalloc(Number,sizeof(float));
       if(aRes==NULL){printf("Out memory\n");return;}

	Otm=(float*)farcalloc(Number,sizeof(float));
	if(Otm==NULL){printf("Out memory\n");return;}

	

	clrscr();
	printf("\n\t\tВторичная обработка данных по вибрации\n");
	printf("\n");

	 obr_f_dan();  //Занесение данных по режиму из файла данных в каждую структуру
      
     //Циклическая обработка большой группы файлов (60) группками по 6 файлов
      	  tf=tf-1;
	  if(tf<=0) {printf("Файл данных пустой\n");return;}

	      rar=0;
	  do { //33
	    
	   //Формирование необходимых названий

	  switch(spisok[rar+1].Regim[1])       //Regim[1]-означает что взята вторая буква из названия режима
		{
	  case 'е': strcpy(O_name,"g_");  break;
	  case 'о': strcpy(O_name,"v_");  break;
	  case 'х': strcpy(O_name,"x_");  break;
	  case 'ы': strcpy(O_name,"b_");  break;
	  case 'с': strcpy(O_name,"s_");  break;
	     default: strcpy(O_name,"__");  break;
		}
	 strncpy(unic_name,spisok[rar+1].Name_f,5);

	 for(i=0;i<CN;i++)   //CN-число каналов
	  {   //4
	   if (kbhit())  { if (getch()==27) return;} //выход по ESC

           sprintf(rty,"c:\\DIAGR\\obr_rg\\%2s_%5s.r%02i",O_name,unic_name,i+1);
            printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL) printf("Не могу открыть файл%35s\n",rty);

	   

           sprintf(fale,"%2s_%5sr%02i",O_name,unic_name,i+1); //Формирование имени файла для передачи его в Excel
	   fprintf(basefile,"%- 12s \n",fale);
	   

	  for(j=1;j<ch_reg+1;j++)    //ch_reg-число подрежимов
	   {  //5
              strncpy (tek_naz,spisok[rar+j].Name_f,5);
	      int srav_naz=stricmp(unic_name,tek_naz);
              if(srav_naz!=0) {break;}
	    sprintf(str,"c:\\DIAGNOST\\oscill\\%5s_%02i.o%02i",unic_name,j,i+1);
	    myfile=fopen(str,"rt");
	     if(myfile==NULL) {printf("Не могу открыть файл%35s\n",str);}
	     else
	      {//10
                nach_z_kon_f();
	       

	    sprintf(otme,"c:\\DIAGNOST\\oscill\\%5s_%02i.o16",unic_name,j);
	     if(str!=otme)
	     {
	    otmfile=fopen(otme,"rt");
	    if(otmfile==NULL){printf("Не могу открыть файл%35s\n",otme);}
             }
	       float vspom;
	       float sum=0;
	       
	       fgets(vrvs,15,myfile);
//	       fscanf(myfile,"%s",&vrvs);
	       for(int k=0;k<Number;k++)    //Number-число шагов опроса АЦП
		{//6
		 fscanf(myfile,"%f",&vspom);
		 fscanf(myfile,"%f",&aRes[k]);
		 if(k==1) {stepp=vspom*kof_st;}
		 sum=aRes[k]+sum;
		} //6
		  koof=sum/Number;   //koof-постоянная составляющая сигнала
                  for(int r=0;r<Number;r++)  
		  aRes[r]=aRes[r]-koof;     //Вычитание постоянной составляющей
		if(str!=otme){ fscanf(otmfile,"%s",&vrnen);}  //Сканирование даты в файле отметчика(нигде не используется)
		if(str==otme){ fscanf(myfile,"%s",&vrnen);}
		for(int z=0;z<Number;z++)
		 {
	       if(str!=otme){ fscanf(otmfile,"%f %f",&vr,&Otm[z]);}
	       if(str==otme){ fscanf(myfile,"%f %f",&vr,&Otm[z]);}
		}
		 float maxil=0;
		 float minil=0;
		 for(r=0;r<Num2;r++)
		   {                      //Нахождение максимума и минимума (В диапазоне для построения диаграммы)
		   maxil=(aRes[r] > maxil) ? aRes[r]:maxil;
		   minil=(aRes[r] < minil) ? aRes[r]:minil;
                   }

		int flag=0;               //Обработка отметчика
		float sumotm=0;
                float otmkof=0;
		 for(int h=0;h<Number;h++)
		   sumotm=sumotm+fabs(Otm[h]);
		 otmkof=sumotm/Number;
		  for(h=0;h<Num2;h++)
		     if ((fabs(Otm[h])-otmkof)>kof_zap){flag=h;h=Num2;}  //Нахождение первого пика отметчика

		     int k_met=0;
                     int mark=0;
		  for(h=0;h<Number;h++)  //Подсчет кол-ва меток отметчика и времени их срабатывания
		   {   mark=mark-1;
		       if ((fabs(Otm[h])-otmkof)>kof_zap && mark<5)
		       {fal[k_met]=h;k_met=k_met+1;mark=20;}
		   }
			 
			 float srkv20sum=0;
		         float srkv20otk=0;
		         float sumpr=0;
                         float sr20sum=0;
			 float sr20=0;
                           sr20min=(float*)calloc(k_met-1,sizeof(float));
			   if(sr20min==NULL){printf("Out memory\n");return;}
			   sr20max=(float*)calloc(k_met-1,sizeof(float));
			   if(sr20max==NULL){printf("Out memory\n");return;}
           if (k_met>4)
		  {      
			
			 for(int t=0;t<k_met-1;t++)     //Подсчет максимумов и минимумов по 20 оборотам
		       { sr20max[t]=0; sr20min[t]=0;
		        for(int s=fal[t];s<fal[t+1];s++)
			{sr20min[t]=(aRes[s] < sr20min[t]) ? aRes[s]:sr20min[t];
			 sr20max[t]=(aRes[s] > sr20max[t]) ? aRes[s]:sr20max[t];}
		       }      
		      
		       
		     for(t=0;t<k_met-1;t++)     //Среднее значение вибрации по 20 оборотам
		      sr20sum=fabs(sr20min[t])+fabs(sr20max[t])+sr20sum;
                    
		       sr20=sr20sum/(k_met-1);

		      for(t=0;t<k_met-1;t++)   //Среднеквадратическое отклонение по 20 оборотам
		      { sumpr=fabs(sr20min[t])+fabs(sr20max[t]);
			srkv20sum=pow((sumpr-sr20),2)+srkv20sum;
                      }
		       srkv20otk=sqrt(srkv20sum/(k_met-2));
			 
		  }
			 free(sr20min);
			 free(sr20max);
		   for(h=0;h<Num2;h++)
                      { if ((Otm[h]-otmkof)>kof_zap) Otm[h]=maxil/1.42;
			if ((Otm[h]+otmkof)<(kof_zap*(-1))) Otm[h]=minil/1.42;
		      }
		   for(h=0;h<Num2;h++)
		       if ((fabs(Otm[h])-otmkof)<=kof_zap) Otm[h]=-1;

     //Оцениваем вибрацию
                 float maxi=0;
		float mini=0;
                float max_vibr=0;
		for( k=0;k<Number;k++)
		  {maxi=(aRes[k] > maxi) ? aRes[k]:maxi;} //Нахождение значения самого высокого пика
		for( k=0;k<Number;k++)
		  {mini=(aRes[k] < mini) ? aRes[k]:mini;} //Нахождение значения самой глубокой впадины
		  max_vibr=fabs(maxi)+fabs(mini); //Определение макс. значения вибрации

                  r_Sksr=(float*)farcalloc(Number,sizeof(float));
		  if(r_Sksr==NULL){printf("Out memory\n");return;}
                  int nach; int kon; int w;
     if(k_met<=4) {nach=16;kon=Number-16;}
      else   {nach=fal[1];kon=fal[k_met-2];}
		for( k=nach;k<kon;k++)   //Нахождение скользящего среднего по 31-ому
		{
		 sum_skl=0;
		 for(w=k;w<(skl_sr+k);w++)
		  {sum_skl=aRes[w-15]+sum_skl;}
		   r_Sksr[k]=sum_skl/skl_sr;
		}

                float maxim=0;
		float minim=0;
                float maxsr_vibr=0;
		for( k=nach;k<kon;k++)
		  {maxim=(r_Sksr[k] > maxim) ? r_Sksr[k]:maxim;} //Нахождение значения самого высокого пика
                for( k=nach;k<kon;k++)
		  {minim=(r_Sksr[k] < minim) ? r_Sksr[k]:minim;} //Нахождение значения самой глубокой впадины
		  maxsr_vibr=fabs(maxim)+fabs(minim); //Определение макс.среднего значения вибрации
		       float srkv20sumskl=0;
		       float srkv20otkskl=0;
		       float sr20sumskl=0;
		       float sr20skl=0;
		       float sumprskl=0;
			 sr20minskl=(float*)calloc(k_met-3,sizeof(float));
			 if(sr20minskl==NULL){printf("Out memory\n");return;}
			 sr20maxskl=(float*)calloc(k_met-3,sizeof(float));
			 if(sr20maxskl==NULL){printf("Out memory\n");return;}
	   if (k_met>4)
	       {
 //Обработка скользящего среднего по 31 значению (по 20 оборотам)
			 
			 for(int t=0;t<k_met-3;t++)     //Подсчет максимумов и минимумов по 20 оборотам
		       { sr20maxskl[t]=0; sr20minskl[t]=0;
			for(int s=fal[t+1];s<fal[t+2];s++)
			{sr20minskl[t]=(r_Sksr[s] < sr20minskl[t]) ? r_Sksr[s]:sr20minskl[t];
			 sr20maxskl[t]=(r_Sksr[s] > sr20maxskl[t]) ? r_Sksr[s]:sr20maxskl[t];}
		       }      

		       
		     for(t=0;t<k_met-3;t++)     //Среднее значение вибрации по 20 оборотам
		      sr20sumskl=fabs(sr20minskl[t])+fabs(sr20maxskl[t])+sr20sumskl;

		       sr20skl=sr20sumskl/(k_met-3);

		      for(t=0;t<k_met-3;t++)   //Среднеквадратическое отклонение по 20 оборотам
		      { sumprskl=fabs(sr20minskl[t])+fabs(sr20maxskl[t]);
			srkv20sumskl=pow((sumprskl-sr20skl),2)+srkv20sumskl;
                      }
		       srkv20otkskl=sqrt(srkv20sumskl/(k_met-4));
	       }
			free(sr20minskl);
			free(sr20maxskl);
	                farfree(r_Sksr);

		for(h=0;h<Num2;h++)        //Синхронизация с отметчиком
		 {
		  aRes[h]=aRes[h+flag];
		  if((h+flag)<Num2){Otm[h]=Otm[h+flag];}
		  else{Otm[h]=-1;}
		  }
		  strset(ocenk,0);
		  strncpy(sop,spisok[rar+j].chan[i],2);
		  oc=stricmp(sop,"СС");
		  op=stricmp(sop,"");

	       if(op!=0)
		 {
		   if(oc!=0) {
	            if(maxsr_vibr<180){strcpy(ocenk,"неуд");}//Оценка средней вибрации
                    else{strcpy(ocenk,"недоп");}
		    if(maxsr_vibr<140){strcpy(ocenk,"уд");}
                    if(maxsr_vibr<100){strcpy(ocenk,"хор");} 
		    if(maxsr_vibr<50){strcpy(ocenk,"отл");}
                              }
	           else { if(maxsr_vibr<80){strcpy(ocenk,"уд");}//Оценка средней вибрации
		          else{strcpy(ocenk,"неуд");}
		        }
		 }

                   fprintf(basefile," Ср.виб.  \t");
		      fprintf(basefile," Макс.виб.  \t");
		      fprintf(basefile," Ср.виб.20\t");
		      fprintf(basefile," Сркв.откл.20\t");
                      fprintf(basefile," Срскл.виб.20\t");
                      fprintf(basefile," Срквскл.откл.20\t");
		       fprintf(basefile," Оценка  \n");
			fprintf(basefile,"%- .1f мкм \t",maxsr_vibr);
			 fprintf(basefile,"%- .1f мкм \t",max_vibr);
			 fprintf(basefile,"%- .1f мкм \t",sr20);
			 fprintf(basefile,"%- .2f мкм \t",srkv20otk);
                         fprintf(basefile,"%- .1f мкм \t",sr20skl);
			 fprintf(basefile,"%- .2f мкм \t",srkv20otkskl);
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
	    rar=rar+(j-1);
	  }while(rar<tf); //33
 farfree(aRes);
 farfree(Otm);
}  //1

void obr_f_dan(void)
    {
    strcpy(fdan,"c:\\DIAGR\\soho.txt");
      datfile=fopen(fdan,"rt");  //Открытие файла данных
      if(datfile==NULL) printf("Не могу открыть файл%35s\n",fdan);
      char* df=NULL;
      for(tf=0;tf<60;tf++)
     {
      int c=0;
   do{
	 strcpy(prom,"");         //очистить prom
      do {
	  df=fgets(t,2,datfile); //    Скопировать один символ из файла в t
	    ot=stricmp(df,"\t"); //    Сравнить на наличие символа табуляции
	    on=stricmp(df,"\n"); //    Сравнить на наличие символа перевода строки
	    kav=stricmp(df,"\"");//    Сравнить на наличие символа кавычек
	     if (df==NULL) break;//    Если конец файла выйти из процедуры
	   if (ot!=0 && on!=0 && kav!=0) { strcat(prom,t);}  //Добавить символ в prom кроме \t, \n, \"

	 }  while (ot!=0 && on!=0); // Делать пока не табуляция или пока не перевод строки

	     switch(c)                // Заполнение структуры dat_reg массива spisok
	     {

	case 0: strcpy(spisok[tf].N_pp,prom);		 break;
	case 1: strcpy(spisok[tf].Name_f,prom);	 	break;
	case 2: strcpy(spisok[tf].N_GA,prom);           break;
	case 3: strcpy(spisok[tf].Date_zam,prom);	 break;
	case 4: strcpy(spisok[tf].Regim,prom); 	 	break;
	case 5: strcpy(spisok[tf].Podreg,prom);	 	break;
	case 6: strcpy(spisok[tf].P,prom);		 break;
	case 7: strcpy(spisok[tf].Q,prom);		 break;
	case 8: strcpy(spisok[tf].Irot,prom);	 	 break;
	case 9: strcpy(spisok[tf].Ist,prom);		 break;
	case 10: strcpy(spisok[tf].Ust,prom);	  	 break;
	case 11: strcpy(spisok[tf].RK,prom); 	 	 break;
	case 12: strcpy(spisok[tf].NA,prom);  	 	 break;
	case 13: strcpy(spisok[tf].Oborot,prom);  	 break;
	case 14: strcpy(spisok[tf].tgel,prom);  	 break;
	case 15: strcpy(spisok[tf].Napor,prom);  	 break;
	case 16: strcpy(spisok[tf].Rem,prom);  	 	 break;
        case 17: strcpy(spisok[tf].chan[0],prom);  	 break;
	case 18: strcpy(spisok[tf].chan[1],prom);  	 break;
	case 19: strcpy(spisok[tf].chan[2],prom);  	 break;
	case 20: strcpy(spisok[tf].chan[3],prom);  	 break;
	case 21: strcpy(spisok[tf].chan[4],prom);  	 break;
	case 22: strcpy(spisok[tf].chan[5],prom);  	 break;
	case 23: strcpy(spisok[tf].chan[6],prom);  	 break;
	case 24: strcpy(spisok[tf].chan[7],prom);  	 break;
	case 25: strcpy(spisok[tf].chan[8],prom);  	 break;
	case 26: strcpy(spisok[tf].chan[9],prom);  	 break;
	case 27: strcpy(spisok[tf].chan[10],prom);  	 break;
	case 28: strcpy(spisok[tf].chan[11],prom);  	 break;
	case 29: strcpy(spisok[tf].chan[12],prom);  	 break;
	case 30: strcpy(spisok[tf].chan[13],prom);  	 break;
	case 31: strcpy(spisok[tf].chan[14],prom);  	 break;
        default: break;
	     }
        if (df==NULL) break;
	  c=c+1;            // Счетчик номера поля структуры dat_reg массива spisok
      }  while (on!=0);
         if (df==NULL) break; 
     }	
    fclose(datfile);
    }       //КОНЕЦ ФУНКЦИИ  obr_f_dan

void nach_z_kon_f(void)
      {
             fprintf(basefile,"N п/п\t%s\t",spisok[rar+j].N_pp);
	     fprintf(basefile,"Файл\t%s\t",spisok[rar+j].Name_f);
	     fprintf(basefile,"ГА\t%2s\t",spisok[rar+j].N_GA);
	     fprintf(basefile,"Дата\t%s\t",spisok[rar+j].Date_zam);
          switch(O_name[0])
		{
	  case 'G':
	  case 'g':  fprintf(basefile,"Генераторный режим\t"); break;
	  case 'V':
	  case 'v':  fprintf(basefile,"Холостой ход с возбуждением\t"); break;
	  case 'X':
	  case 'x':  fprintf(basefile,"Холостой ход без возбуждения\t"); break;
	  case 'B':
	  case 'b':  fprintf(basefile,"Выбег\t"); break;
	  case 'S':
	  case 's':  fprintf(basefile,"Режим СК\t"); break;
	  default:  fprintf(basefile,"             \t");
		}
	      
	       fprintf(basefile,"%s\t%s\n",spisok[0].Podreg,spisok[rar+j].Podreg);

	       fprintf(basefile,"%s\t%s\t",spisok[0].P,spisok[rar+j].P);
	       fprintf(basefile,"%s\t%s\t",spisok[0].Q,spisok[rar+j].Q);
	       fprintf(basefile,"%s\t%s\t",spisok[0].Irot,spisok[rar+j].Irot);
	       fprintf(basefile,"%s\t%s\t",spisok[0].Ist,spisok[rar+j].Ist);
	       fprintf(basefile,"%s\t%s\t",spisok[0].Ust,spisok[rar+j].Ust);
	       fprintf(basefile,"%s\t%s\n",spisok[0].RK,spisok[rar+j].RK);

	       fprintf(basefile,"%s\t%s\t",spisok[0].NA,spisok[rar+j].NA);
	       fprintf(basefile,"%s\t%s\t",spisok[0].Oborot,spisok[rar+j].Oborot);
	       fprintf(basefile,"%s\t%s\t",spisok[0].tgel,spisok[rar+j].tgel);
	       fprintf(basefile,"%s\t%s\t",spisok[0].Napor,spisok[rar+j].Napor);
	       fprintf(basefile,"%s\t%s\t",spisok[0].chan[i],spisok[rar+j].chan[i]);
	       fprintf(basefile,"%s\t%s\n",spisok[0].Rem,spisok[rar+j].Rem);
      }        //КОНЕЦ ФУНКЦИИ nach_z_kon_f