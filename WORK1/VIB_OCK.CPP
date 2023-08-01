//-------------------------------------------------------------------------
//Программа оценки по среднеарифметической вибрации( Удаление постоянной составляющей,
//определение уровня максимальной, среднеквадратичной вибрации, перераспределение файлов (теперь в
//одном файле будут 6 подрежимов 16 каналов))
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
const int Num2=4096;
const int skl_sr=31;
char ocenk[6];
float kof_st=1.0;
float sum;
float sum_skl;
float koof;
float* Sig;
float* r_Sksr;
float* Pik;
float* Razn;
signed char* Dop;
FILE *myfile;
	FILE *basefile;
	char str1[40];
	char str2[40];
	char fal[14];
       	char vrvs[15];
	char uzlji[15];
        int fl;
	char unic_name [8];
	char O_name [5];
	char* buffer;
        int pik1(float* mas1);
	 void pik2(float*mass2);
   void main(int argc,char* argv[])
{  //1
 clrscr();
  mkdir( "ocenka" );


        fl=0;
	clrscr();
	printf("\n\t\tОценка по вибрации\n");
	printf("\n");
 /*       printf(" Выход по ESC или продолжать\n");
         int esc=getch();
	 if(esc==27) return; */

	   printf(" Номер ГА (1-10)\n");
      /*	 GA_num[0]=3;
	 cgets(GA_num);
	 strcpy(GA_num,&GA_num[2]);   */
	 int ga=atoi(argv[1]);
         printf("%d\n",ga);

	 printf("\n Оценка вибрации по: 16)Опорным узлам 0)Спинке статора\n");
	 printf("Число от 0 до 16 означает до какого канала(включительно) \n");
	 printf("вибрация оценивается как для опорных узлов, на оставшихся каналах -  \n");
	 printf("как для спинки статора (по умолчанию 0)\n");
   /*	 fls[0]=3;
	 cgets(fls);
	 strcpy(fls,&fls[2]);  */
	 fl=atoi(argv[2]);
	 if(fl<=0||fl>16){fl=0;}
         printf("%d\n",fl);
        


    printf("Уникальное  наименование обрабатываемых файлов \n");
/*    unic_name[0]=6;
    buffer=cgets(unic_name);  */
    strcpy(unic_name,argv[3]);
    printf("%s\n",unic_name);

     printf("Наименование файла после обработки \n");
//     printf("Рекомендуемые имена: G[x]-генераторный режим,V[x]-возбуждение,\n");
//     printf("   X[x]-холостой ход,B[x]-выбег,S[x]-режим СК,где [x]-любой символ\n");
 /*     char* buf;
      O_name[0]=3;
      buf=cgets(O_name);  */
      strcpy(O_name,argv[4]);
      printf("%s\n",O_name);
      printf("\n");

       sprintf(str2,"c:\\DIAGR\\OCENKA\\%2s_%5s.ock",O_name,unic_name);
           printf("%35s\n",str2);
	   basefile=fopen(str2,"wt");
	    if(basefile==NULL){printf("Не могу открыть файл%35s\n",str2);}

             fprintf(basefile,"ГА %d\t",ga);
	   

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
				    
           sprintf(fal,"%2s_%5sock",O_name,unic_name); //Формирование имени файла для передачи его в Excel
	   fprintf(basefile,"%- 12s \t",fal);

     for(int i=0;i<CN;i++)   //CN-число каналов
	  {   //4
	  if (kbhit())  { if (getch()==27) break;} //выход по ESC
	  printf("        Обрабатывается канал N%-d\r",i+1);
        for(int j=0;j<ch_reg;j++)    //ch_reg-число подрежимов
	   {  //5
	    sprintf(str1,"c:\\DIAGNOST\\oscill\\%5s_%02i.o%02i",unic_name,j+1,i+1);
	    myfile=fopen(str1,"rt");
	   if(myfile==NULL){printf("Не могу открыть файл%35s\n",str1);}
	     else
	      {//10
		Sig=(float*)calloc(Number,sizeof(float));
		 if(Sig==NULL){printf("Out memory\n");return;}
	
               float vs;
	       float sum=0;
               float stepp=0;
	       fgets(vrvs,15,myfile);
	       for(int k=0;k<Number;k++)    //Number-число шагов опроса АЦП
		{//6
		 fscanf(myfile,"%f %f",&vs,&Sig[k]);
                 if(k==1) {stepp=vs*kof_st;}
		 sum=Sig[k]+sum;
		} //6
                  koof=sum/Number;        //koof-постоянная составляющая
		 for(int r=0;r<Number;r++) //Вычитание постоянной составляющей
		{Sig[r]=Sig[r]-koof;}
//Кость
                float maxi=0;
		float mini=0;
                float max_vibr=0;
		for( k=0;k<Num2;k++)
		  {maxi=(Sig[k] > maxi) ? Sig[k]:maxi;} //Нахождение значения самого высокого пика
		for( k=0;k<Num2;k++)
		  {mini=(Sig[k] < mini) ? Sig[k]:mini;} //Нахождение значения самой глубокой впадины
		  max_vibr=fabs(maxi)+fabs(mini); //Определение макс. значения вибрации

		r_Sksr=(float*)calloc(Num2,sizeof(float));
		 if(r_Sksr==NULL){printf("Out memory\n");return;}
	
		int w;
		for( k=0;k<Num2;k++)   //Нахождение скользящего среднего по 31-ому
		{
		 sum_skl=0;
		 for(w=k;w<(skl_sr+k);w++)
		  {sum_skl=Sig[w]+sum_skl;}
		   r_Sksr[k]=sum_skl/skl_sr;
		}
		float maxim=0;
		float minim=0;
                float maxsr_vibr=0;
		for( k=0;k<Num2;k++)
		  {maxim=(r_Sksr[k] > maxim) ? r_Sksr[k]:maxim;} //Нахождение значения самого высокого пика
                for( k=0;k<Num2;k++)
		  {minim=(r_Sksr[k] < minim) ? r_Sksr[k]:minim;} //Нахождение значения самой глубокой впадины
		  maxsr_vibr=fabs(maxim)+fabs(minim); //Определение макс. значения вибрации

		   for( k=0;k<Num2;k++)
		   { r_Sksr[k]=fabs(r_Sksr[k]);}

		     for( k=0;k<Number-15;k++)
		      {Sig[k]=Sig[k+15];}

	         for( k=0;k<Num2;k++)
		 {Sig[k]=fabs(Sig[k]);}

                Pik=(float*)calloc(Num2,sizeof(float));
		 if(Pik==NULL){printf("Out memory\n");return;}

		int sch=pik1(Sig);  //Функция подсчета кол-ва и значений пиков

                int sch1=0;
	        int sch2=0;
                 for(k=0;k<sch;k++)   //Перетасовка по условию (если высота пиков меньше 5мкм их не брать)
		 {
		  if(Pik[k]<5){Pik[k]=0;sch1=sch1+1;}
		  }
		float sum_kv=0;
                float srkv_vibr=0;
	        sch2=sch-sch1;
		if(sch2>1)
                {
		  for (int h=0;h<sch2;h++)  //Определение среднеквадратичного значения вибрации
		{ Pik[h]=Pik[h]*Pik[h];
		sum_kv=Pik[h]+sum_kv;Pik[h]=0;}
                 sum_kv=sqrt(sum_kv);
		 srkv_vibr=sum_kv/(sqrt(sch2/2));
		 }
		 else srkv_vibr=0;
                  free(Pik);

  // Определение средней вибрации на средней частоте
                  Razn=(float*)calloc(Num2,sizeof(float));
		  if(Razn==NULL){printf("Out memory\n");return;}
		    Dop=(signed char*)calloc(Num2,sizeof(signed char));
		    if(Dop==NULL){printf("Out memory\n");return;}
		      for(int d=0;d<Num2;d++)
		      Razn[d]=Sig[d]-r_Sksr[d];
		   
			for(d=0;d<Num2;d++)
		         {
			if(Razn[d]<0) Dop[d]=-1;
			else Dop[d]=1;
			Razn[d]=fabs(Razn[d]);
                         }

		   pik2(Razn);//Функция избавления от маленьких пиков

		/*	for(d=0;d<Num2/4;d++)
			{ fprintf(basefile,"%-.4f\t",Razn[d]);
			 fprintf(basefile,"%-d\n",Dop[d]);
			 }       */

			for(d=0;d<Num2;d++)
			{Razn[d]=Razn[d]*Dop[d];
			Sig[d]=Razn[d]+r_Sksr[d];}

                  /*    for(d=0;d<Num2/4;d++)
			{ fprintf(basefile,"%-.4f\t",Razn[d]);
			 fprintf(basefile,"%-.4f\n",Sig[d]);
			 }        */


                 free(Razn);
		 free(Dop);
		 free(r_Sksr);
		
               	Pik=(float*)calloc(Num2,sizeof(float));
		if(Pik==NULL){printf("Out memory\n");return;}

		  sch=pik1(Sig);  //Функция подсчета кол-ва и значений пиков

	       sch1=0;
	       sch2=0;
	       float sum_pik=0;
	       float sr_vibr=0;
               float sr_chast=0;
	       if(Pik[0]>1&&sch>1)  //Если есть пики делать , иначе переход к следующему подрежиму
	    {
	      
		 for(k=0;k<sch;k++)   //Перетасовка по условию (если высота пиков меньше 5 мкм их не брать)
		 {
		  if(Pik[k]<5){Pik[k]=0;sch1=sch1+1;}
	          }
	        sch2=sch-sch1;
		if(sch2>1)
		{
		 for (int s=0;s<sch;s++) //Определение среднего значения вибрации
		{ sum_pik=Pik[s]+sum_pik;}
		 sr_vibr=sum_pik/(sch2/2);
		 sr_chast=(sch2/2)/(Num2*stepp);
		  }
                 else  sr_vibr=0;
	      }
	       else sr_vibr=0;
                free(Pik);
                free(Sig);
		if(i+1<=fl)
                {
	          if(maxsr_vibr<180){strcpy(ocenk,"неуд");}//Оценка средней вибрации
                  else{strcpy(ocenk,"недоп");}
		  if(maxsr_vibr<140){strcpy(ocenk,"уд");}
                  if(maxsr_vibr<100){strcpy(ocenk,"хор");} 
		  if(maxsr_vibr<50){strcpy(ocenk,"отл");}
                  strcpy(uzlji,"Опорные узлы");
		}
		else
                  {
                  if(maxsr_vibr<80){strcpy(ocenk,"уд");}//Оценка средней вибрации
		  else{strcpy(ocenk,"неуд");}
		  strcpy(uzlji,"Спинка статора");
		  }

		if(i==0&&j==0) fprintf(basefile,"%9s",vrvs);
		if(j<1)
		{
		fprintf(basefile,"---------------------------------------------\n");
		 fprintf(basefile,"Канал N%-d\t",i+1);
		 fprintf(basefile,"%15s\n",uzlji);
		 }
		  fprintf(basefile,"Подрежим N%-d\n",j+1);
		   fprintf(basefile,"Ср.виб.\t");
                    fprintf(basefile,"Сркв.виб.\t");
		     fprintf(basefile,"Макс.виб.\t");
		      fprintf(basefile,"Оценка\t");
		       fprintf(basefile,"Ср.виб.\t");
			fprintf(basefile,"на ср.ч\n");
			 fprintf(basefile,"%6.2f\t",maxsr_vibr);
			  fprintf(basefile," %6.2f \t",srkv_vibr);
			   fprintf(basefile,"%6.2f  \t",max_vibr);
			    fprintf(basefile,"%- 6s \t",ocenk);
			     fprintf(basefile,"%6.2f\t",sr_vibr);
			      fprintf(basefile,"%6.2f\n",sr_chast);
	       } //10
	    fclose(myfile);
	   }  //5
	  }    //4
	 fclose(basefile);


}  //1 
              //    КОНЕЦ

 int pik1(float* mas1)
 {
 
 float vn_maxim, sr_zn=0;
 int a,b;
 int vn_sch=0,cc=0;
  for(int k=0;k<Num2;k++)  // начало цикла с k
{                          // Нахождение значения и кол-ва пиков
 for(int u=1;u<Num2;u++)   //Проверка возрастает функция или убывает
  { //пр
  sr_zn=mas1[k];
  if(sr_zn!=mas1[k+u]){sr_zn=mas1[k];cc=u;u=Num2;}
  else {if(u==Num2-1) return vn_sch;}
  }
   if(sr_zn<mas1[k+cc])    //Если условие соблюдается , то функция возрастает
    {vn_maxim=0;
 for( a=k;a<Num2;a++)
     {
  if(mas1[a]<=mas1[a+1]){vn_maxim=mas1[a];}
   else{ Pik[vn_sch]=vn_maxim;
	 vn_sch=vn_sch+1;
	 k=a-1;
	 a=Num2;}
     }
      }
 else               //Иначе функция убывает
  {
 for (b=k;b<Num2;b++)
  if(mas1[b]<mas1[b+1]) { k=b-1;b=Num2;}
  }
 } //конец цикла с k
 return vn_sch;
  }
//---------------------------------------------------------------------
void pik2(float* mass2) 
{
 float vr_maxim, sr_znah=0;
 int l,m,p,flagok=1;       //Если flagok=1 то пик больше положеного
 int cs=0,n_min=0;
  for(int r=0;r<Num2;r++)  // начало цикла с k
  {                     // Нахождение значения и кол-ва пиков
 for(int u=1;u<Num2;u++) //Проверка возрастает функция или убывает
  { //пр
  sr_znah=mass2[r];
  if(sr_znah!=mass2[r+u]){sr_znah=mass2[r];cs=u;u=Num2;}
  else {if(u==Num2-1) return;}
  }
   if(sr_znah<mass2[r+cs])    //Если условие соблюдается , то функция возрастает
    {vr_maxim=0;
 for( l=r;l<Num2;l++)
     {
  if(mass2[l]<=mass2[l+1]){vr_maxim=mass2[l];}
   else{ if(vr_maxim<2.5){ flagok=1;
			   for(p=n_min;p<l;p++)
			   mass2[p]=0;}
	 else {flagok=0;}
	    r=l-1;
	    l=Num2;}
     }
    }
     else               //Иначе функция убывает
  {
 for (m=r;m<Num2;m++)
 {
  if(mass2[m]<mass2[m+1]) {n_min=m;r=m-1;m=Num2;}
  if((mass2[m]>mass2[m+1])&&(flagok==1)) mass2[m]=0;
 }
  }
 } //конец цикла с k

}