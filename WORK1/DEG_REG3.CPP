#include <stdio.h>
#include <conio.h>
#include <dos.h>
//#include <alloc.h>
#include <string.h>
//#include <sys\stat.h>
//#include <fcntl.h>
//#include <io.h>
//#include <math.h>
#include <time.h>
#include <stdlib.h>
const int CN=16;
int Number=0;
time_t sekund=0;
time_t vremia=0;
const unsigned char mask1=0x21;
void interrupt(*TIMER_INT)(...);  // точка входа прерывания таймера BIOS
void interrupt(*ADC_INT)(...);    // точка входа нового прерывания таймера
void interrupt multiple_int(...); // программа обрабоки прерывания таймера
int aRes;    //число прочитанное из порта
int Chast;                      	// частота опроса канала АЦП
int k;
char OldMask1;
unsigned int End_of_work;
int ustavka;
int *Nom_kol;
char chans[20];
int fl;
void kol_kan(char* strok, int *mas1); //Функция определения кол-ва каналов
/**********************************************************
//   Процедура инсталяции нового прерывания таймера
***********************************************************/
void near adc_install(void)
{
	unsigned int m_delay;
	outportb(0x223,0x88);            // программирование цифрового порта АЦП
	TIMER_INT=getvect(0x08);         // сохранение прерывания таймера BIOS
	disable();                       // запрет маскируемых прерываний
	OldMask1 = inportb(mask1);       // сохранение маски аппаратных прерываний
	outportb(mask1,0xFF);            // запрет аппаратных прерываний
	setvect(0x08,ADC_INT);           // установка нового прерывания
	outportb(0x43,0x36);                        //  #
	m_delay=(unsigned int)(1193820L/Chast);     //  # программирование таймера
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
	const int low_port=0x224;         // порты ввода/вывода АЦП (базовый
	const int hig_port=0x225;         // адрес 0х220 устанавливается перемычкой)
  for(k=0;k<fl;k++)   // ОПРОС КАНАЛОВ АЦП
	{
		outportb(hig_port,(unsigned char)(Nom_kol[k]));     // установка номера канала АЦП
		outportb(low_port,0xFF);                 // запуск АЦП(любое число)
		while((0x7f|inportb(0x222))!=0xff) {};   // ожидание конца преобразования 
		aRes=((0x0F&inportb(hig_port))*256+inportb(low_port)-2048)&0xFFFE;
		Number=Number+1;
		
		  if(Number==Chast)
	             {
	               Number=0;
		       sekund=sekund+1;//Подсчет секунд прошедших с начала опроса
		     }
		printf("\rКанал АЦП N %d",k);
		printf("\t%5d",aRes);
		if(abs(aRes)>=ustavka) break;
		
	}
	 long ouar=sekund/3600;
	 long min=(sekund-ouar*3600)/60;
         long sek=sekund-(ouar*3600+min*60);
	 printf("\t%03ld:%02ld:%02ld\r",ouar,min,sek);
	 if (kbhit())  { if (getch()==27) goto E_ESC;} //выход по ESC
	 if (kbhit())  { if (getch()){End_of_work=1; goto E_ESC;}}
	 if(abs(aRes)<ustavka) goto M_RET;          //условие продолжения опроса
 E_ESC:
	 outportb(mask1,0xFF);
	 if (abs(aRes)>=ustavka||End_of_work==1)  {End_of_work =1;}
	 else End_of_work =2;
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
/***********************************************
*            Основная программа
************************************************/
int main(int argc,char* argv[])
{
    clrscr();
    printf("\t\tПрограмма дежурного опроса АЦП\n");
    printf("    Выход в DOS по ESC\n");
    printf("  Немедленный запуск сбора данных по F1-F12\n");
     Nom_kol=(int*)calloc(16,sizeof(int));
     if(Nom_kol==NULL){printf("Out memory\n");return 0;}
     printf("\tКонтролируемые каналы\n");
     strcpy(chans,argv[1]);
     printf("%s\n",chans);
     fl=strlen(chans);
     if(fl>16||fl<1)
       {printf("Неправильное число каналов!!!\n");return 0;}
     printf("\tУставка по любому каналу\n");
     ustavka=abs(atoi(argv[2]));
     printf("%d\n",ustavka);
    if(ustavka>2047||ustavka<1)
       {printf("Неправильное абсолютное значение уставки!!!\n");return 0;}
    Chast=400;
    kol_kan(chans,Nom_kol);

    vremia=time(NULL); //Запомнить системное время

    End_of_work=0;
      printf("Текущее значение АЦП\n");
      ADC_INT =multiple_int;
      adc_install();
      for(;End_of_work==0;) {}
      free (Nom_kol);
      printf("\n");
      vremia=vremia+sekund;
      stime (&vremia);//Установить системное время с учетом работы программы
      if(End_of_work==1)
      {printf("Превышение уровня сигнала!!!");
      return 10;}
      if(End_of_work==2)
      { printf("Выход по ESC!!!");
	return 0;
      }

}
//-------------------------------------------------------------------------

     void kol_kan(char *strok, int *mas1)
     {
     char odsim[2];
     odsim[0]='\0';
     odsim[1]='\0';
     for(int tf=0;tf<fl;tf++)
     {
      memcpy(odsim,&strok[tf],1);      //Преобразование строки с номерами
      sscanf(odsim,"%x",&mas1[tf]); //каналов в десятичные числа и 
      strset(odsim,'\0');              //сохранение их в массиве mas1
     // printf("%d\n",mas1[tf]);
     }

}   //КОНЕЦ ФУНКЦИИ kol_kan
//---------------------------------------------------------------------