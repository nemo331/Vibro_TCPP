#include <stdio.h>           
#include <conio.h>
#include <stdlib.h>

  FILE *myfile;
  FILE *basefile;
	char str1[40];
	char str2[40];
  int fl;
  int Number=2048;
  int huge aRes[16][2048];

  void main(int argc,char* argv[])
  {
   clrscr();
	printf("Перетасовка данных по каналам\n");

    printf("\nНаименование обрабатываемого файла\n");
    printf("%s\n",argv[1]);
 
	 printf("Кол-во обрабатываемых каналов\n");
	 fl=atoi(argv[2]);
	 if(fl<=1||fl>16){fl=1;}
          printf("%d\n",fl);

       sprintf(str1,"c:\\tc\\WORK2\\DAR\\DATA\\%s.dat",argv[1]);
	    myfile=fopen(str1,"rb");
	   if(myfile==NULL){printf("Не могу открыть файл%35s\n",str1);}

       for(int k=0;k<fl;k++)
       {
	for(int i=0;i<Number;i++)
	{
	fseek(myfile,2*k+2*fl*i,SEEK_SET);
	fread(&aRes[k][i],4,1,myfile);
	if(feof(myfile)){i=Number;k=fl;}
        }
       }


       for(int j=0;j<fl;j++)
       {
	sprintf(str2,"c:\\tc\\work2\\DAR\\DATA\\%s%i.dat",argv[1],j+1);
           printf("%35s\n",str2);
	   basefile=fopen(str2,"wb");
	    if(basefile==NULL){printf("Не могу открыть файл%35s\n",str2);}

	  for(k=0;k<Number;k++)
	 fwrite(&aRes[j][k],2,1,basefile);

	 fclose(basefile);
       }
           fclose(myfile);

  }