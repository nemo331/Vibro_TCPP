#include <stdio.h>
#include <conio.h>
#include <string.h>
//---------- 業�஢�� �뢮����� ��ப�

// x1 � x2 ����� � �ࠢ�� ���न��� ����
// str �뢮����� ��ப�
int px(int x1,int x2,char str[40] )
{  char *sp;
	sp=str;
	return int ((x2-x1+1-strlen(str))/2+1);
}
//-------- ������ � ⥪�⮢��� ०��� DOS

void oldreg(void)
{                               textbackground(0);
				textcolor(15);
				window(1,1,80,25);
				clrscr();
}
//------- ���⪠ ��࠭� � ���������� ���

void clear(void)
{
//  textcolor(7);
  window (1,1,80,24);              //���⪠
    textbackground(7);
//  textbackground(1);
  clrscr();
//  int i;
// for (i=0; i<1920; i++) {putchar(178);};

}
//-------���� ����
// x1 � x2 ����� � �ࠢ�� ���न��� ����
// y1 � oy2 ������ ���न��� � ���-�� ��ப
void okno(int x1, int y1, int x2, int oy2)
{   _setcursortype(_NOCURSOR);
    window(x1+1,y1+1,x2+1,y1+1+oy2);        //⥭�
    textbackground(0);
    clrscr();
    int i;
    window(x1,y1,x2,y1+oy2);             //����
    textbackground(3);
    textcolor(15);
    clrscr();

    gotoxy (1,1);                         //ࠬ��
    char j;
    putchar (201);
    for (i=x1 ; i<x2-1 ; i++ )  putchar (205);
    putchar (187);

    for (i=2 ; i<oy2+1 ; i++)
		{ gotoxy (1,i);
		  putchar(186);
		  gotoxy(x2-x1+1,i);
                  putchar(186);
                }; 
 
   gotoxy (1,oy2+1);
    putchar (200);
    for (i=x1 ; i<x2-1 ; i++ )  putchar (205);
    putchar (188);
   window(x1+1,y1,x2-1,oy2+y1);
   gotoxy(2,2);

}
//------ �뢮� ᮮ�饭��
// x1 � y1 ���न���� ��砫� ��ப�
void soob(int x1,int y1, char str[40])
{  int x2;
   char *sp;
   sp=str;
   x2=x1+strlen(sp)+3;
   okno(x1,y1,x2,2);
   gotoxy(2,2);
   puts(sp);
}

//-------�㭪�� ����
// x1,y1,x2 �����, �ࠢ�� � ������ ���न���� ����
// str ��������� ����
// str1..str5 �㭪�� ����
// �����頥� ����� �㭪�

int win(int x1,int y1, int x2,char str[40],char str1[40],char str2[40],char str3[40],char str4[40],char str5[40])
{   typedef char pm[40]   ;
  pm m[6];
  int n=1;
  char *sp;
  int oy2=1;
  int i;
  sp=str;
  stpcpy(m[0],sp) ;
  sp=str1;
  stpcpy(m[1],sp) ;
  sp=str2;
  stpcpy(m[2],sp) ;
  sp=str3;
  stpcpy(m[3],sp) ;
  sp=str4;
  stpcpy(m[4],sp) ;
  sp=str5;
  stpcpy(m[5],sp) ;
   char *sr="";
   int ptr;

  do
	     {
	       ptr=strcmp(m[oy2],sr);
	       oy2++;
	     }
 while ((ptr!=0)&&(oy2<7));
    okno(x1,y1,x2,oy2);     //okno


  textcolor(14);
  gotoxy(px(x1,x2,m[0]),1);
  cputs(m[0]);
  for (i=1;i<(oy2-1);i++) {      textbackground(i==1?0:3);
				 textcolor(i==1?15:0);
				 gotoxy(px(x1,x2,m[i]),i+2);
				 cputs(m[i]);
			  } ;
  int c;
  i=0 ;
   while (i==0) {
		switch ( getch() )
		   { case 13 :
				return n ;
		     case  0 : switch ( getch() )
				   {
				     case 72 :{  textbackground(3);
						 textcolor(0);
						 gotoxy(1,n+2);
						 clreol();
						 gotoxy( px(x1,x2,m[n]),n+2);
						 cputs(m[n]);
						n=n==1?oy2-2:n-1 ;
						textbackground(0);
						textcolor(15);
						gotoxy( 1,n+2);
						clreol();
						gotoxy( px(x1,x2,m[n]),n+2);
						cputs(m[n]);
					      }; break;
				     case 80 :{  textbackground(3);
						 textcolor(0);
						 gotoxy(1,n+2);
						 clreol();
						 gotoxy( px(x1,x2,m[n]),n+2);
						 cputs(m[n]);
						n=n==oy2-2?1:n+1 ;
						textbackground(0);
						textcolor(15);
						gotoxy( 1,n+2);
						clreol();
						gotoxy( px(x1,x2,m[n]),n+2);
						cputs(m[n]);
					      }; break;


				    }; break;
		    };
		  };
}
//---��������� � ������ ��� ��࠭�
void zag(char str[200])
{  textbackground(1);
  window(1,25,80,25);
  clreol();
  char *sp;
  sp=str;
  textcolor(15);
  gotoxy ( px(1,80,sp),1);
  cputs(sp);
  window(1,1,80,24);
}