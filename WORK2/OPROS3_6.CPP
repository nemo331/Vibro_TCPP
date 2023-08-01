/***********************************************************************/
/*Программа сбора данных по вибрации с помощью платы LA70 (оперативный */
/* и дежурный опросы                                                   */
/***********************************************************************/
/*В OPROS2 добавляю возможность опроса по дежурным каналам вместо общих*/
/*В OPROS3_1 добавлены следующие возможности:
    1. Немедленный запуск при деж опросе при нажатии на любую клавишу
    2. Обработка ошибок при чтении файла параметров
    3. Не выводить амплитуду сигнала канала отметчика
    4. Выводить значение амплитуды превышающее уставку и по макс значению 
*/
/*В OPROS3_3 добавлена возможность создания файла с результатами обработки*/
/*В OPROS3_4 добавлена возможность создания log-файла*/
/*В OPROS3_5: 	изменен формат log-файла
		изменено условие переустановки времени
		добавлена возможность грузить наборы коэф из файла*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>
#include <sys\stat.h>
#include <sys\timeb.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <dos.h>
#include <io.h>
#include <dir.h>

char name_time[7];
char Versia[10];
float Num=0;
time_t sekund=0;
time_t sekund2=0;
time_t vremia=0;
const unsigned char mask1=0x21;
int low_port; // порты ввода/вывода АЦП 
int hig_port;         
int bas_adr;  //базовый адрес (наприм.0х220) устанавливается перемычкой
void interrupt(*TIMER_INT)(...);  // точка входа прерывания таймера BIOS
void interrupt(*ADC_INT)(...);    // точка входа нового прерывания таймера
void interrupt multiple_int(...); // программа обрабоки прерывания таймера
float aRes;
int Res;    
float Chast;    // частота опроса каналов АЦП
unsigned int m_delay;
int k=0;
int kkk;
char OldMask1;
unsigned int End_of_work;
int ustavka;
//int *Nom_kol;
char chans[20];
//int fl;
int kol_otsch;
int i=0;
int j=0;
int hr=0;
int hi=0;
float sredd=0;	
//void kol_kan(char* strok, int *mas1); //Функция определения кол-ва каналов
long ouar;
long ouar2;
long minute;
long minute2;
long sek;
long sek2;
int msek;
char flag;
int kan_prev;
int kan_max;
float maxim_zn;
int nach_n_f;
int mas[3][32];
int **mas_end;
int op_d_kan;  //кол-во опросн и деж каналов (общее с совп кан-ми)
int por_otm=0;
float kor_time=0;
float ostat=0;
time_t sekund3=0;
int msec2=0;
long timer=0;
unsigned char tip_por=1;
unsigned char tip_por2=1;
char log_f=0;
float **mas_srkv; //Массив с суммой данных замера для кажд канала и макс значений вибр
long slw=0;
long kol_cikl=0;
char paus_f[15];
int exist_f=-1;
int flag_os=0;
/**************************************************************************/
int *Nom_kol; //Массив с номерами каналов сбора
int *Nom_deg; //Массив с номерами каналов деж опроса
int **Rez_kol;
int nom_kan_otm[]={-1};
float **sklz_sr;//массив для нахождения сколз ср. по 31
int kol_sklzsr=31;

FILE *myfile;   //Файл с параметрами опроса
FILE *basefile; //Файл замера
FILE *tempfile; //временный файл
FILE *rezfile;  //файл с результатами обработки
FILE *logfile;  //log-файл
FILE *koffile;  //файл с коэффициентами

char ch_kan[40]; //Каналы опроса в виде строки
char ch_deg[40]; //Каналы дежурного опроса в виде строки
int far **aResult; //Массив с данными по вибрации в кодах АЦП
char f_ust[45]; //Имя файла с параметрами замера
char name_f1[15]; //Имя первого файла замера(вводится из ком строки или bat-файла)
char name_f2[7]; //первые 6 символов полного названия файла
char rassh[5]; //расширение файла замера
int nom_fila;    //номер введенного названия файла
char nom_f_c[3];
	char str1[50];
	char str2[50];
	char str3[10];
	char str4[50];
	char kod[8];
	char dob_k_filu[52];//добавка к началу файла замера (чтоб было 128 байт)
	
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


    float aKof1[]={10.0,100.0,10.0,100.0,10.0,100.0,10.0,100.0,
		   10.0,100.0,10.0,100.0,10.0,100.0,10.0,100.0};
    float aKof2[]={10.631512,101.626016,10.786517,103.181427,
		   10.709505,103.806228,11.019284,106.269926,
		   10.432968,98.846788,10.833258,104.257168,
		   10.728463,102.145046,10.914052,104.620750};//БКВ-1 (Выходы на шлейфовый осциллограф)
   float aKof3[]={109.289618,106.951872,109.289618,121.951220,
		  105.263158,106.951872,102.040816,106.951872,
		  109.289618,106.951872,109.289618,121.951220,
		  105.263158,106.951872,102.040816,106.951872}; //БКВ-2 (Выходы по пост. напряжению на ЭВМ)		 
   float aKof4[]={10.042681,95.923261,10.042681,93.676815,
		  10.118897,94.117647,10.914052,106.100796,
		  9.772783,91.533181,9.982531,92.807425,
		  9.878982,89.887640,9.811136,92.378753};//БКВ-2 (Выходы на шлейфовый осциллограф) 
   float aKof5[]={109.289618,106.951872,109.289618,121.951220,
		  105.263158,106.951872,102.040816,106.951872,
		  109.289618,106.951872,109.289618,121.951220,
		  105.263158,106.951872,102.040816,106.951872};//БКВ-1(Выход на ЭВМ)
   float aKof7[]={100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,
		  100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0 };//коэф-ты 100 для всех каналов
   float aKof8[]={101.626016,103.181427,103.806228,106.269926,
		  98.846788,104.257168,102.145046,104.620750,
		  95.923261,93.676815,94.117647,106.100796,
		  91.533181,92.807425,89.887640,92.378753};//БКВ-1,БКВ-2 (шлейфовый осциллограф)
   float aKof9[]={95.923261,93.676815,94.117647,106.100796,
		  91.533181,92.807425,89.887640,92.378753,
		  101.626016,103.181427,103.806228,106.269926,
		  98.846788,104.257168,102.145046,104.620750};
   float aKof10[]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
		   1.0,1.0,1.0,1.0};
   float aKof11[]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
		   1.0,1.0,1.0,1.0};

   float aKof[16]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
		   1.0,1.0,1.0,1.0};

  partip par;
  int obr_f_dan(void);   // Функция обработки файла установок
  void kol_kan(char* strok, int *mas1); //Функция определения кол-ва каналов и кол-ва их повторений
  int opr_deg_kan(int* mmm1, int* mmm2); //Добавление деж номеров каналов в массив с номерами каналов опроса АЦП
  int obr_osh(void); //Функция обработки ошибок
  void obr_f_kof(int); //Функция чтения файла с коэффициентами
  void n_f_t(void);
  long cikl;
 /**********************************************************
//   Процедура инсталяции нового прерывания таймера
***********************************************************/
void near adc_install(void)
{
	
	outportb(bas_adr+3,0x88);            // программирование цифрового порта АЦП
	TIMER_INT=getvect(0x08);         // сохранение прерывания таймера BIOS
	disable();                       // запрет маскируемых прерываний
	OldMask1 = inportb(mask1);       // сохранение маски аппаратных прерываний
	outportb(mask1,0xFF);            // запрет аппаратных прерываний
	setvect(0x08,ADC_INT);           // установка нового прерывания
	outportb(0x43,0x36);                        //  #
	m_delay=(unsigned int)((float)1193820.0/Chast);     //  #1193820L программирование таймера
	outportb(0x40,(unsigned char)m_delay);      //  #   (новое значение)
	outportb(0x40,(unsigned char)(m_delay>>8)); //  #  
	outportb(mask1,0xF8);             // установка маски аппаратных прерываний
	enable();                         // разрешение прерываний
	return;              
}
/********************************************************
//  Программа обработки нового прерывания таймера
*********************************************************/
void interrupt multiple_int(...)
{                 
   
   // ОПРОС КАНАЛОВ АЦП
   if (cikl==(kol_cikl-1))
   {
    for(k=0;k<op_d_kan;k++)
     {  
		outportb(hig_port,(mas_end[3][k]));     // установка номера канала АЦП
		outportb(low_port,0xFF);                 // запуск АЦП(любое число)
		while((0x7f|inportb(bas_adr+2))!=0xff) { };   // ожидание конца преобразования 
		Res=((0x0F&inportb(hig_port))*256+inportb(low_port)-2048)&0xFFFE;
		if (par.zap_otm!=1)
		{
		   
		if (mas_end[1][k]==1)
                {
			if (par.regim_opr==0)
			{
			    if (mas_end[3][k]!=nom_kan_otm[0])
			    {	
				if(abs((Res*aKof[mas_end[3][k]]))>abs(maxim_zn))
				{ aRes=Res*aKof[mas_end[3][k]];
				maxim_zn=aRes;
				kan_prev=mas_end[3][k];
				kan_max=mas_end[3][k];}
			    }
			}
		 aResult[k][j]=Res;
		 switch (tip_por2)
		   { 
		case 0: break;
		case 1: {if((int)(Res*aKof[mas_end[3][k]])>Rez_kol[0][k])
				Rez_kol[0][k]=(int)(Res*aKof[mas_end[3][k]]+0.5);
			 if((int)(Res*aKof[mas_end[3][k]])<Rez_kol[1][k])
				Rez_kol[1][k]=(int)(Res*aKof[mas_end[3][k]]+0.5);
			 Rez_kol[2][k]=abs(Rez_kol[0][k])+abs(Rez_kol[1][k]);	   
			 break;}
		case 2:  {if (hr==kol_sklzsr) {hr=0;}
			  sklz_sr[k][hr]=(Res*aKof[mas_end[3][k]]);
			  
			  if (j>kol_sklzsr-2)
			  {
			  sredd=0;
			  for (hi=0;hi<kol_sklzsr;hi++)
			     {sredd=sredd+sklz_sr[k][hi];
			     //printf(" %.2f",sredd);
			     }
			  sredd=sredd/kol_sklzsr;
		       
			  if(Rez_kol[0][k]<(int)sredd) {Rez_kol[0][k]=(int)(sredd+0.5);}
			  if(Rez_kol[1][k]>(int)sredd) {Rez_kol[1][k]=(int)(sredd+0.5);}
			  Rez_kol[2][k]=abs(Rez_kol[0][k])+abs(Rez_kol[1][k]);	   
			  }
		       break;	
			 }
		case 3: { sklz_sr[k][0]=sklz_sr[k][0]+((Res*aKof[mas_end[3][k]])*(Res*aKof[mas_end[3][k]]));
				    if((j+1)==par.Number)
				    { sklz_sr[k][0]=(float)(sqrt((double)(sklz_sr[k][0]/par.Number))); 
				      Rez_kol[0][k]=(int)(sklz_sr[k][0]+0.5);
				      Rez_kol[2][k]=(int)(sklz_sr[k][0]*2+0.5);
				    }
			  break;
			}
		case 4: {if((int)(Res*aKof[mas_end[3][k]])/1.4142>Rez_kol[0][k])
				Rez_kol[0][k]=(int)(Res*aKof[mas_end[3][k]]/1.4142+0.5);
			 if((int)(Res*aKof[mas_end[3][k]])/1.4142<Rez_kol[1][k])
				Rez_kol[1][k]=(int)(Res*aKof[mas_end[3][k]]/1.4142+0.5);
			 Rez_kol[2][k]=abs(Rez_kol[0][k])+abs(Rez_kol[1][k]);	   
			 break;}
		   }  
		 //Rez_kol[0][k]=(int)abs((Res*aKof[mas_end[3][k]])+0.5);
		 }
		   if (mas_end[2][k]==1)
		   {
		      if (par.regim_opr==1)
		      { 
			if (mas_end[3][k]!=nom_kan_otm[0])
		        {
			switch (tip_por)
			  {
			  case 1:{ if((Res*aKof[mas_end[3][k]])>=par.max_porog||(Res*aKof[mas_end[3][k]])<=par.min_porog)
				    {flag=1;
				     aRes=(Res*aKof[mas_end[3][k]]);
				     kan_prev=mas_end[3][k];
				    }
				    
				    break;}
			  case 2: { mas_srkv[0][k]=mas_srkv[0][k]+((Res*aKof[mas_end[3][k]])*(Res*aKof[mas_end[3][k]]));
				    if((j+1)==par.Number)
				    { mas_srkv[0][k]=(float)(sqrt((double)(mas_srkv[0][k]/par.Number))); 
				      if((int)mas_srkv[0][k]>=par.max_porog)
				       {flag=1;
					aRes=mas_srkv[0][k];
					kan_prev=mas_end[3][k];
				       }
				    }
				  break;
				  }
			  case 3:{ if((Res*aKof[mas_end[3][k]])/1.4142>=par.max_porog||(Res*aKof[mas_end[3][k]])/1.4142<=par.min_porog)
				    {flag=1;
				     aRes=(Res*aKof[mas_end[3][k]])/1.4142;
				     kan_prev=mas_end[3][k];
				    }
				    
				    break;}
			 
			  
			  case 4: { if(abs(Res*aKof[mas_end[3][k]])>mas_srkv[1][k])
				    { mas_srkv[1][k]=abs(Res*aKof[mas_end[3][k]]);}

				    mas_srkv[0][k]=mas_srkv[0][k]+((Res*aKof[mas_end[3][k]])*(Res*aKof[mas_end[3][k]]));
				    if((j+1)==par.Number)
				    { mas_srkv[0][k]=(float)(sqrt((double)(mas_srkv[0][k]/par.Number))); 
				      //printf("\nmas_srkv[0][%d]=%.3f\n",k,mas_srkv[0][k]);
				      //printf("\nmas_srkv[1][%d]=%.3f\n",k,mas_srkv[1][k]);	

				      if(((mas_srkv[1][k]/mas_srkv[0][k])<4)&&((int)mas_srkv[0][k]>=par.max_porog))
				       {flag=1;
					aRes=mas_srkv[0][k];
					kan_prev=mas_end[3][k];
				       }
				    }
				  break;
				  }
			  default: { if((Res*aKof[mas_end[3][k]])>=par.max_porog||(Res*aKof[mas_end[3][k]])<=par.min_porog)
				    {flag=1;
				     aRes=(Res*aKof[mas_end[3][k]]);
				     kan_prev=mas_end[3][k];
				    }
				  break;}
			  }
			if((abs(Res*aKof[mas_end[3][k]]))>abs(maxim_zn))
			      { maxim_zn=Res*aKof[mas_end[3][k]];
				kan_max=mas_end[3][k];}
			}        
		      }
		   }
		}
		else
		{
		  if (Res>por_otm) 
		  {  //printf("Res=%d",Res);
		     End_of_work = 1; 
		     par.zap_otm=2;
		     goto E_ESC;
		  } 
		}
     }
     cikl=-1;	
     j++;
     hr++;
     }
	  if (par.zap_otm!=1) 
	  {slw++;}
	  cikl++;
	  Num=Num+(1/Chast)+kor_time;
	  if(Num>=1)
		     {  exist_f=access(paus_f,0);
			Num=Num-1;
			sekund++;//Подсчет секунд прошедших с начала опроса
			sekund2++;
			//sekund3++;
			ouar=sekund/3600;
			ouar2=sekund2/3600;
			minute=(sekund-ouar*3600)/60;
			minute2=(sekund2-ouar2*3600)/60;
			sek=sekund-(ouar*3600+minute*60);
			sek2=sekund2-(ouar2*3600+minute2*60);
			//printf("\n%1.7f\n",Num);
			printf("\r");
			if (par.zap_otm==1)
			   {printf("\tЖду сигнал отметчика");}
			else 
			{if ((par.regim_opr==1&&flag==1)||par.regim_opr==0)
			     { printf("  Сбор данных!!! ");
			       printf("Пик/Кан=%4.0f/%2d",maxim_zn,kan_max);
			       printf("   Файл:%s%02i%s",name_f2,(nom_fila+nach_n_f),rassh);
			     }            
			 else {printf(" Дежурный опрос! ");
			       printf("Пик/Кан=%4.0f/%2d",maxim_zn,kan_max);
			       printf("   Файл:%s%02i%s",name_f2,(nom_fila+nach_n_f),rassh);}
			}

			msek=Num*1000.0;
			
			printf("   %02ld:%02ld:%02ld,%03d",ouar,minute,sek,msek);
			printf("   %03ld:%02ld:%02ld\r",ouar2,minute2,sek2);		     }
	 
	 if( kbhit() ) 
	 { char ch = getch(); 
	   switch( ch )
	   {
	     case 27 :  goto E_ESC; break; 
	     default :   flag=1;   break;
	   }
	 }
	  if(exist_f==0) {End_of_work=1; goto E_ESC;}
	  if(slw!=((long)par.Number*kol_cikl)) goto M_RET;  //условие продолжения опроса
	 
 E_ESC:
	 outportb(mask1,0xFF);
	 if (slw==(long)par.Number*kol_cikl||End_of_work==1) 
	 {
	 msek=Num*1000.0;
	 //printf("\n%1.7f\n",Num);
	 Num=Num-(float)msek/(float)1000.0;
	 //printf("\n%1.7f\n",Num);
	 End_of_work =1;
	 }
	 else 
	 {msek=Num*1000.0; 
	 Num=Num-(float)msek/(float)1000.0;
	 End_of_work =2;}
	 disable();                 // запрет маскируемых прерываний
	 setvect(0x08,TIMER_INT);   // восстановление прерывания таймера BIOS
	 outportb(0x43,0x36);       //  #
	 outportb(0x40,0xFF);       //  # программирование таймера
	 outportb(0x40,0xFF);       //  # (восстановление значения)
	 outportb(mask1,OldMask1);  // восстановление маски аппаратных прерываний
	 enable();                  // разрешение прерываний
M_RET:
	 outportb(0x20,0x20);
return;
}
/***************************************************************************/
/*    Основная программа                                                   */
/***************************************************************************/ 
  int main(int argc, char* argv[])
  { sprintf(paus_f,"pause.txt");
    struct timeb ttm;
    struct timeb ttv;
    //clrscr();
    mkdir ("biv"); //каталог куда будет занесен собранный файл
    mkdir ("rez"); //каталог куда будет занесен файл с результатами обработки
    mkdir ("log"); //каталог куда будет занесен log-файл
    strcpy(Versia,"3.6");
    printf("\n\t\tОпрос каналов АЦП - Opros (Версия %s)\n",Versia);
    strncpy(name_f1,argv[1],8);
    if (strlen(argv[1])!=12)
     {printf("Неправильная длина названия файла замера!\n"); return 0;}
    if(strrchr(argv[1],'.')==NULL)
     {printf("Не указано расширение в названии файла замера!\n"); return 0;}
    strncpy(name_f2,argv[1],6);
    strrev(strncpy(rassh,strrev(argv[1]),4));
    strrev(argv[1]);
    strrev(strncpy(nom_f_c,strrev(name_f1),2));
    strrev(name_f1);
    nom_fila=atoi(nom_f_c);
    if(strncmp(name_f2,"*****",5)==0)
    {
      n_f_t();
      
      strncpy(name_f2,name_time,5);
      strncpy(name_f1,name_time,5);
      //printf("%s\n",name_f1);   
    }
    if(argv[2]==NULL) {strcpy(f_ust,"param.prm");}
    else {strcpy(f_ust,argv[2]);}
    
    int osh=obr_f_dan();
    int osh2=obr_osh();
    osh=osh+osh2;
    if (osh>0)
    {printf("\nВ файле параметров допущены %d ошибки(ок) \n",osh);return 0;}

     if (par.regim_opr==0)
       {
       for(int cht=0;cht<par.fl_deg;cht++)
       ch_deg[cht]=NULL;
       strcpy(ch_deg,ch_kan);
       par.fl_deg=par.fl;
       }
    printf("\t\t\tПараметры опроса:");
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
    if (strncmp(par.num_otm,"n",1)==0)
       { printf("\n\t\tНомер канала отметчика не задан!!!");
	 par.zap_otm=0;}
    else 
       {printf("\n\t\tНомер канала отметчика: %s",par.num_otm);}
    printf("\n\t\tНомер набора коэффициентов: %d\n",par.num_nab);
    if (par.regim_opr==1)
       { printf("\t\tПороговые значения для запуска опроса ");
	 switch (tip_por)
	    {
	     case 1:   printf("(Пиковое):"); break;
	     case 2:   printf("(Среднеквадратичное): "); break;
	     case 3:   printf("Псевдосреднеквадратичное:"); break;
	     case 4:   printf("(Отстройка от ударов):"); break;
	     }
	 printf("\n\t\t\tМинимальное: %.2f",par.min_porog);
	 printf("\n\t\t\tМаксимальное: %.2f",par.max_porog);
	 printf("\n\t\tКаналы АЦП для дежурного опроса: %s\n",ch_deg);
       }

    Nom_kol=(int*)calloc(par.fl,sizeof(int));
    if(Nom_kol==NULL){printf("Out memory\n");return 0;}
    kol_kan(ch_kan,Nom_kol); 

    Nom_deg=(int*)calloc(par.fl,sizeof(int));
    if(Nom_deg==NULL){printf("Out memory\n");return 0;}
    kol_kan(ch_deg,Nom_deg);

    

    if (strncmp(par.num_otm,"n",1)!=0)
       {kol_kan(par.num_otm,nom_kan_otm);}

       int kt;
       switch (par.num_nab)
	{
	case 1: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof1[kt]*0.0025;
		break;
	case 2: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof2[kt]*0.0025;
		break;
	case 3: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof3[kt]*0.0025;
		break;
	case 4: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof4[kt]*0.0025;
		break;
	case 5: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof5[kt]*0.0025;
		break;
	case 6: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof[kt];
		break;
	case 7: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof7[kt]*0.0025;
		break;
	case 8: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof8[kt]*0.0025;
		break;
	case 9: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof9[kt]*0.0025;
		break;
	case 10: for(kt=0;kt<16;kt++)
		aKof[kt]=aKof10[kt]*0.0025;
		break;
	default: obr_f_kof(par.num_nab);
		 for(kt=0;kt<16;kt++)
		 aKof[kt]=aKof11[kt]*0.0025;
		break;
	}
	

    char prom_adr[4];
    sprintf (prom_adr,"%d",par.adres);
    sscanf(prom_adr,"%x",&bas_adr);
    low_port=bas_adr+4;  
    hig_port=bas_adr+5;
    if (par.Chastota<20.0)
    {
    kol_cikl=(long)(floor((double)(418/par.Chastota+0.5)));
    m_delay=(unsigned int)((float)1193820.0/418.0);
    Chast=(float)1193820/m_delay;
    printf("Действительная частота опроса АЦП=%4.4f\n",(Chast/(float)kol_cikl));
    }
    else
    {
    m_delay=(unsigned int)((float)1193820.0/par.Chastota);
    Chast=(float)1193820/m_delay;
    kol_cikl=1;
    printf("Действительная частота опроса АЦП=%4.4f\n",Chast);
    }
    sekund=(long)floor(((1.0/(Chast/(float)kol_cikl))+kor_time)*(float)par.Number+0.5);
       ouar=sekund/3600;
       minute=(sekund-ouar*3600)/60;
       sek=sekund-(ouar*3600+minute*60);
    printf("\t\tПримерное время опроса: %02ld:%02ld:%02ld\n",ouar,minute,sek);
    sekund=0;
    
    op_d_kan=opr_deg_kan(Nom_kol, Nom_deg);

      mas_end=(int**)calloc(4,sizeof(int*));
	 if(mas_end==NULL){printf("Out memory\n");return 0;}
	for(int q=0;q<4;q++)
	 {
	  mas_end[q]=(int*)calloc(op_d_kan,sizeof(int));
	  if(mas_end[q]==NULL){printf("Out memory\n");return 0;}
	 }   
	
	 for (int vs=0;vs<3;vs++)  
	  {
	   for (int osn=0;osn<op_d_kan;osn++)
	   { mas_end[vs][osn]=mas[vs][osn];
	    // printf("mas_end[%d][%d]=%d\n",vs,osn,mas_end[vs][osn]);
	   }
	   //printf("\n");
	  }

	sprintf(str3,"1.tmp");
	tempfile=fopen(str3,"rb+");
	if(tempfile!=NULL)
	  { fread(&timer,2,1,tempfile);
	    fread(&Num,4,1,tempfile);
	    fclose(tempfile);
	    remove(str3);
	  }
	int ppp;
	long hour1;
	long minut1;
	long sec1;

	
	
	nach_n_f=0;
	flag=0;
	struct time vrm;
	struct date dte;
	struct time vrm2;
	struct date dte2;
	sekund2=0;
	
     do                          
     {	
      aResult=(int**)farcalloc(par.fl,sizeof(int*));
      if(aResult==NULL){printf("Out memory1\n");return 0;}
	for(int qq=0;qq<par.fl;qq++)
		{    
		aResult[qq]=(int*)farcalloc(par.Number,sizeof(int));
		if(aResult[qq]==NULL){printf("Out memory2\n");return 0;}
		}
      mas_srkv=(float**)calloc(2,sizeof(float*));
	  if(mas_srkv==NULL){printf("Out memory\n");return 0;}
	  for(q=0;q<2;q++)
	   {
	    mas_srkv[q]=(float*)calloc(op_d_kan,sizeof(float));
	    if(mas_srkv[q]==NULL){printf("Out memory\n");return 0;}
	   } 
      Rez_kol=(int**)calloc(3,sizeof(int*));
      if(Rez_kol==NULL){printf("Out memory\n");return 0;}	     	   
	    for(q=0;q<3;q++)
	   {Rez_kol[q]=(int*)calloc(par.fl,sizeof(int));
	    if(Rez_kol[q]==NULL){printf("Out memory\n");return 0;}}

      sklz_sr=(float**)calloc(par.fl,sizeof(float*));
      if(sklz_sr==NULL){printf("Out memory\n");return 0;}	     	   
	    for(q=0;q<par.fl;q++)
	   {sklz_sr[q]=(float*)calloc(kol_sklzsr,sizeof(float));
	    if(sklz_sr[q]==NULL){printf("Out memory\n");return 0;}}

     NACH:
	   slw=0;
	   cikl=0;
	   End_of_work=0;
	   j=0;
	   hr=0;
	   hi=0;
	   sredd=0;	
	   maxim_zn=0;
	   sekund3=0;
	   gettime(&vrm);
	   getdate(&dte);
		int my_year=dte.da_year;
		char my_mon=dte.da_mon;
		char my_day=dte.da_day;
	   ftime (&ttm);
	   int letvr;
	   if (ttm.dstflag==0) {letvr=0;}
	   else {letvr=1;}
	   double prty=3600L*24L;
	   double prty3=(ttm.time+timer)-((long)ttm.timezone-60L*(long)letvr)*60L;
	   long sekund_f=(long)prty3;
	   double result=fmod(prty3,prty);
	   sekund=(time_t)result;
	   sekund3=sekund;
	   Num=(float)ttm.millitm/1000.0+Num;
	   //printf("Время %ld\n", sekund);			 
	   
      if (par.zap_otm==1&&nach_n_f==0)
	 {  for(int gh=0;gh<op_d_kan;gh++)
	  mas_end[3][gh]=nom_kan_otm[0];
	 }
       else
	 { 
          for(int gh=0;gh<op_d_kan;gh++)
	   mas_end[3][gh]=mas_end[0][gh];
           if (par.zap_otm!=0)
	   {par.zap_otm=3;}
	 }
      
      ADC_INT =multiple_int;
      adc_install(); 
	  
      for(;End_of_work==0;) 
      {   
      }


      //timer=0;
      hour1=sekund/3600L;
      minut1=(sekund-hour1*3600L)/60L;
      sec1=(sekund-hour1*3600L-minut1*60L);
      

      vrm.ti_hour=(unsigned char)hour1;
      vrm.ti_min=(unsigned char)minut1;
      vrm.ti_sec=(unsigned char)sec1;
      vrm.ti_hund=(unsigned char)(msek/10+1);
      if (flag_os==0)
      {
      if (hour1>=24)
      { int sutki=hour1/24;
	hour1=hour1-sutki*24;
	dte.da_day=dte.da_day+(char)sutki;
	setdate(&dte);
	//system("date");
      }
      settime(&vrm);
      //printf("DOS\n");
      ftime (&ttm); //проверка если время программы не установилось то изменить его в следующем цикле программы
      if (ttm.dstflag==0) {letvr=0;}
      else {letvr=1;}
      prty=3600L*24L;
      prty3=ttm.time-((long)ttm.timezone-60L*(long)letvr)*60L;
      result=fmod(prty3,prty);

      if (((time_t)result+1)<sekund)
	   {printf("\nsekund=%ld",sekund);
	    printf("\nresult=%ld\n",(long)result);
	    timer=sekund-(long)result;  
	    Num=Num+(float)msek/(float)1000.0;
	     if (((nach_n_f+1)>=par.kol_filov)&&(par.zap_otm!=2))
	       { tempfile=fopen(str3,"wb+");
		 fwrite(&timer,2,1,tempfile);
		 fwrite(&Num,4,1,tempfile);
		 fclose(tempfile);
	       }
	   }
      else {timer=0;}
      }
      if(End_of_work==2)
      {break;}
      if(exist_f==0)
      {break;}

      if (par.zap_otm==2) goto NACH;

     if ((par.regim_opr==1&&flag==1)||(par.regim_opr==0)) 
    { 
    sprintf(str2,"biv\\%s%02i%s",name_f2,(nom_fila+nach_n_f),rassh);
      basefile=fopen(str2,"wb+");

    if(strcmp(rassh,".bin")!=0)
    {
    sprintf(kod,"vibracia");
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
    fwrite(&sekund_f,4,1,basefile);
    fwrite(&dob_k_filu,sizeof(dob_k_filu),1,basefile);
    }
      //	Num=Num+kor_time;
	
      for(j=0;j<par.Number;j++)
	{
	for (k=0;k<par.fl;k++)
	fwrite(&aResult[k][j],2,1,basefile);
	}

       fclose(basefile);
       time_t ouar3=sekund3/3600;
       time_t minute3=(sekund3-ouar3*3600)/60;
       time_t sek3=sekund3-(ouar3*3600+minute3*60);

       if (tip_por2!=0)
       {
       sprintf(str4,"rez\\%s%02i%s",name_f2,(nom_fila+nach_n_f),".rez");
       rezfile=fopen(str4,"wt+");
       fprintf(rezfile,"%s%02i  ",name_f2,(nom_fila+nach_n_f));
       for (k=0;k<par.fl;k++)
	 {if (k<par.fl-1)
	  {fprintf(rezfile," К%02i",mas_end[0][k]);}
	  else
	  {fprintf(rezfile," К%02i\n",mas_end[0][k]);}
	 }

	 for (j=0;j<3;j++)
	 {
	   switch (j)
	   {
	   case 0:  {fprintf(rezfile,"%02ld:%02ld:%02ld  ",ouar3,minute3,sek3); break;}
	   case 1:  {fprintf(rezfile,"%02d.%02d.%02d",my_day,my_mon,my_year); break;}
	   case 2:  {switch (tip_por2)
			 {
			   case 1:  {fprintf(rezfile,"пиковое   "); break;} 
			   case 2:  {fprintf(rezfile,"склз.сред "); break;} 
			   case 3:  {fprintf(rezfile,"срквадрат "); break;} 
			   case 4:  {fprintf(rezfile,"псевдосркв"); break;} 
			   default: {fprintf(rezfile,"          "); break;}
			  }
		       break;
		       }
	   default: {fprintf(rezfile,"          "); break;}
	   }
	   for (k=0;k<par.fl;k++)
	   {if (k<par.fl-1)
	     {fprintf(rezfile,"%4d",Rez_kol[j][k]);}
	    else
	     {fprintf(rezfile,"%4d\n",Rez_kol[j][k]);}
	   }
	 }
       fclose(rezfile);
       }
       if (log_f==1)
       {
       sprintf(str1,"log\\%s%02i%s",name_f2,(nom_fila+nach_n_f),".log");
       logfile=fopen(str1,"wt+");
       //fprintf(logfile,"%s\n",name_f1);
       fprintf(logfile,"%d\n",par.regim_opr);
       fprintf(logfile,"%d\n",par.kol_filov);
       fprintf(logfile,"%d\n",par.adres);
       fprintf(logfile,"%.2f\n",par.Chastota);
       fprintf(logfile,"%s\n",ch_kan);
       fprintf(logfile,"%d\n",par.Number);
       fprintf(logfile,"%d\n",par.zap_otm);
       fprintf(logfile,"%d\n",nom_kan_otm[0]);
       fprintf(logfile,"%d\n",por_otm);
       fprintf(logfile,"%d\n",par.num_nab);
       fprintf(logfile,"%d\n",par.min_porog);
       fprintf(logfile,"%d\n",par.max_porog);
       fprintf(logfile,"%s\n",ch_deg);
       fprintf(logfile,"%d\n",tip_por);
       fprintf(logfile,"%d\n",tip_por2);
       fprintf(logfile,"%d\n",log_f);
       fprintf(logfile,"%f\n",kor_time);
       fprintf(logfile,"\n");
       fprintf(logfile,"\n");
	      //time_t ouar3=sekund3/3600;
	      //time_t minute3=(sekund3-ouar3*3600)/60;
	      //time_t sek3=sekund3-(ouar3*3600+minute3*60);
       fprintf(logfile,"%02ld:%02ld:%02ld\n",ouar3,minute3,sek3);
       fprintf(logfile,"%02d.%02d.%02d",my_day,my_mon,my_year);
       fclose(logfile);
       }
       nach_n_f++;
    
    }
    

    if (flag==0&&par.zap_otm!=0) {par.zap_otm=1;}
      if(par.Chastota<600)
      {
      printf("\rПрев/Кан=");
      printf("%4.0f/%2d ",aRes,kan_prev);
      printf("Пик/Кан=%4.0f/%2d",maxim_zn,kan_max);
      if ((par.regim_opr==1&&flag==1)||(par.regim_opr==0)) 
      {printf("  Файл:%s%02i%s",name_f2,(nom_fila+nach_n_f-1),rassh);}
       printf("   %03ld:%02ld:%02ld,%03d\r",ouar,minute,sek,msek);           
      }

    for(q=0;q<par.fl;q++)
       free (sklz_sr[q]);
       free (sklz_sr);

    for(q=0;q<3;q++)
       free (Rez_kol[q]);
       free (Rez_kol);

    for(qq=0;qq<par.fl;qq++)
       farfree (aResult[qq]);
       farfree (aResult);

    for(q=0;q<2;q++)
       free (mas_srkv[q]);
       free (mas_srkv);
	
    } while (nach_n_f<par.kol_filov);
    free(Nom_kol); 
    free(Nom_deg);
     
      for(q=0;q<3;q++)
       free (mas_end[q]);
      free (mas_end);

      
       
    if(par.regim_opr==1&&End_of_work==1)
    {printf("\nПревышение уровня сигнала!!!");
      return 5;}

      if(End_of_work==1)
    {printf("\nКонец опроса!!!");
      return 0;}

    if(End_of_work==2)
      { printf("\nВыход по ESC!!!");
	return 10;
      }	 
    printf("\n");
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
		if ((par.fl<=16)&&(par.fl>=1))
		{strcpy(ch_kan,prom);}
		if (par.fl>16)
		{strncpy(ch_kan,prom,16);}
		break;
	case 6: par.Number=atoi(prom); 	 	 
		//printf("\npar.Number=%d\n",par.Number); 
		break;
	case 7:  if (strchr(strupr(prom),'Y')!=NULL) 
	         {par.zap_otm=1;}
		 else par.zap_otm=0; 	 	 break;
	case 8:  strncpy(par.num_otm,strlwr(prom),1); 	 
		 break;
	case 9:  por_otm=atoi(prom);             break;
	case 10: par.num_nab=atoi(prom); 	 break;
	case 11: par.min_porog=atof(prom); 	 break;
	case 12: par.max_porog=atof(prom); 	 break;
	case 13: par.fl_deg=strlen(prom); 
		 //fl_deg=par.fl_deg;       
		 //if ((par.fl_deg>16)||(par.fl_deg<1))
		 //{osh=osh+1;}
		 strcpy(ch_deg,prom);
		 break;
	case 14: tip_por=atoi(prom);              break;
	case 15: tip_por2=atoi(prom);             break;
	case 16: if (strchr(strupr(prom),'Y')!=NULL) 
		 {log_f=1;}
		 else log_f=0; 	 	          break;
	case 17: kor_time=atof(prom);             break;
	case 18: flag_os=atoi(prom);              break;
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
	// for(tf=0;tf<ch_ccc;tf++)
       // printf("%d\n",mas1[tf]);
     }   //КОНЕЦ ФУНКЦИИ kol_kan

/***********************************************************************/
/*Добавление деж номеров каналов в массив с номерами каналов опроса АЦП*/ 
/***********************************************************************/
int opr_deg_kan(int* mmm1, int* mmm2)
{
  int prm=0;
  int kol_m1=par.fl;
  int kol_m2=par.fl_deg;
    for (int osn=0;osn<kol_m1;osn++)
    mas[0][osn]=mmm1[osn];
    
    for (int vs=0;vs<kol_m2;vs++)
     {
       int rt=0;
       for (osn=0;osn<kol_m1;osn++)
       { if (mmm2[vs]==mmm1[osn]) {rt++;} }
       if (rt==0)  { mas[0][kol_m1+prm]=mmm2[vs];prm++;}
     }

     for (osn=0;osn<kol_m1;osn++)
     mas[1][osn]=1;

     for (vs=0;vs<kol_m2;vs++)
     {
       for (osn=0;osn<kol_m1+prm;osn++)
       { if (mmm2[vs]==mas[0][osn]) 
	  {mas[2][osn]=1;} 
       }
       
     } 

     return (kol_m1+prm);
}

/*******************/
/*Обработка ошибок*/
/******************/
int obr_osh(void)
{ int osh=0;
  if (par.regim_opr!=0&&par.regim_opr!=1)
  {par.regim_opr=0;}
  if (par.kol_filov<1||par.kol_filov>100)
  {par.kol_filov=1;}
  if(par.adres<1)
  {par.adres=200;}
  if(par.fl<1)
  {
   strcpy(ch_kan,"0123456789abcdef");
   par.fl=16;
   //printf("\nТак как каналы не были заданы, установлены следующие:0123456789abcdef\n");
  }		
  if(par.Chastota*par.fl>14000||par.Chastota*par.fl<=0)
  {
   par.Chastota=400;
   //printf("\nЧастота изменена на 400Гц так как не задана или больше допустимой\n");
  }
  if(par.Number<1)
  {par.Number=8192;
   //printf("\nКол-во отсчетов изменено на 8192 так как не задано или больше допустимого\n");
  }
  char ch_kannn[18];
  strcpy(ch_kannn,"0123456789abcdefn");
  char *ptr;
  ptr=strpbrk(ch_kannn,par.num_otm);
  if(ptr==NULL)
  {strncpy(par.num_otm,"f",1);
  //printf("\nКанал отметчика был задан неправильно поэтому изменен на %s \n",par.num_otm);
  }
  if(por_otm==0) 
      por_otm=1024;
  if (par.num_nab==0)
      par.num_nab=6;
  if(par.min_porog==0)
      par.min_porog=-100;
  if(par.max_porog==0)
      par.max_porog=100;
  
  if ((par.fl_deg>16)||(par.fl_deg<1))
    {par.fl_deg=par.fl;
     strcpy(ch_deg,ch_kan);}

  if((tip_por<1)||(tip_por>4))
     tip_por=1;

  if (tip_por2>4)
      tip_por2=0;

  if (abs(flag_os)>1)
       { flag_os=1;
	 //printf("\nflag_os=%d\n",flag_os);
       }

  return osh;
}

/*******************************/
/*Чтение коэффициентов из файла*/
/*******************************/
void obr_f_kof(int naz_f1)
{     char stroka1[100];
      sprintf(stroka1,"%d.kof",naz_f1);       //  C:\\TC\\WORK2\\
      koffile=fopen(stroka1,"rt");  //Открытие файла с коэффициентами
      if(koffile==NULL)
	    {printf("Не могу открыть файл%35s\n",stroka1);return ;}
      for(int ttf=0;ttf<16;ttf++)
      {
	fscanf(koffile,"%f ",&aKof11[ttf]);
	//printf("%f\n",aKof11[ttf]);
      }
      fclose(koffile);
      return;
}



 void n_f_t(void)
  {
    time_t miut;
 
    int mass[5];
    double promel;
    char *alfabet= "0123456789abcdefghijklmnopqrstuvwxyz";
    time_t sec=time(NULL);
    miut = sec/(time_t)60;
    
    for(int krk=4;krk>-1;krk--)
       {
       promel=fmod((double)miut,(double)36);
       miut=miut/(time_t)36;
       mass[krk]=(int)promel;
       name_time[krk]=alfabet[mass[krk]];
       }
       //printf("Остаток  %s\n",name_time);
       
  }