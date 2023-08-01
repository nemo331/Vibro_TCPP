#include <stdio.h>           
#include <conio.h>
#include <stdlib.h>
#include <process.h>
int chan=16;
int fl;
char oldname[80], newname[80];
char arg0[80],arg1[20],arg2[20],arg3[20],arg4[20];

 void main(int argc,char* argv[])
 {
     clrscr();
    printf("Кол-во обрабатываемых каналов\n");
	 fl=atoi(argv[2]);
	 if(fl<=1||fl>16){fl=1;}
          printf("%d\n",fl);


  for(int j=0;j<fl;j++)
  {
   sprintf(oldname,"C:\\TC\\WORK2\\DAR\\DATA\\%s%d.dat",argv[1],j+1);
   sprintf(newname,"C:\\TC\\WORK2\\DAR\\%s.dat","ps");
  // remove(newname);
   if(rename(oldname,newname)!=0)
      {
      printf("Ошибка при переименовании\n");    
   //   return;    
      }
      sprintf(arg0,"c:\\TC\\WORK2\\DAR\\file_fft.exe");
      sprintf(arg1,"/c1");
      sprintf(arg2,"/v1");
      sprintf(arg3,"/n2048");
      sprintf(arg4,"/d12");

      spawnl(P_WAIT,"c:\\TC\\WORK2\\DAR\\file_fft.exe",arg0,arg1,arg2,arg3,arg4,NULL);
      printf("\n");
  }
 }