#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <alloc.h>
#include <string.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <dir.h>

const int CN=2;
const int Number=8192;
void S_Furie(int, int);
float far *aRes;
float huge aGv[Number];
float huge aGm[Number];
float huge nGbv[Number>>1];
float huge nGbm[Number>>1];
float huge argum[Number>>1];
float Chast=400.0;                      	// частота опроса канала АЦП
int i, k;
int Kanal;
	FILE *basefile; // файл исходных данных
	FILE *myfile; //файл результата
        char str[40];
	char rty[40];
	char O_name[10];
	float stepp;
        char vrvs[16];

/***********************************************
*            Основная программа
************************************************/
void main(void)
{
 clrscr();
	printf("\n\t\tПреобразование Фурье\n");
          aRes=(float*)farcalloc(Number,sizeof(float));
          if(aRes==NULL){printf("Out memory\n");return;}
 strcpy(O_name,"dypo__01");
 sprintf(str,"c:\\DIAGNOST\\OSCILL\\%8s.o02",O_name);
 basefile=fopen(str,"rt");
  sprintf(rty,"c:\\DIAGNOST\\FURIE\\%8s.f02",O_name);
		 myfile=fopen(rty,"wt");
		 if(myfile==NULL)
		 {printf("Не могу открыть файл%35s\n",rty);}
		             
  if(basefile==NULL)
   {printf("Не могу открыть файл%35s\n",str);}
   else{
	       float vspom;
	       float sum=0;
	       float vr; 
	       fgets(vrvs,15,basefile);
//	       fscanf(basefile,"%s",&vrvs);
	       for(int k=0;k<Number;k++)    //Number-число шагов опроса АЦП
		{//6
		 fscanf(basefile,"%f",&vspom);
		 fscanf(basefile,"%f",&aRes[k]);
		 if(k==1) {stepp=vspom*1.08;}
		 sum=aRes[k]+sum;
		} //6
		 float koof=sum/Number;
		 for(int r=0;r<Number;r++)
		 aRes[r]=aRes[r]-koof;

		 S_Furie(Number,1);

                       for(int d=0;d<(Number>>1);d++)
		        { fprintf(myfile,"%9.4f \t",nGbm[d]);
			  fprintf(myfile,"%9.4f \t",nGbv[d]);
			  fprintf(myfile,"%9.4f \n",argum[d]);
                        }
		     
		 fclose(myfile);
       }

   farfree (aRes);
   fclose(basefile);
}
//--------------------------------------------------------
//   Быстрое преобразование Фурье
//--------------------------------------------------------
void S_Furie(int Num,int Chen)
{
	int nMatr;
	unsigned int i;
	unsigned int j;
	unsigned int k;
	unsigned int m;
	unsigned int nSt;
	long int ni;
	long int ip;
  float Koeff;
	float nV;
	float nM;
	float nGtv;
	float nGtm;
	float dT =(float)1/Chast;
	double SumKv;
        double arctan;
		Koeff=1.0;
	nMatr=1;
	for(i=0;(Num>>i)!=1;i++)
		nMatr++;
	for(i=1;i<nMatr;i++)
	{
		nSt=(Num>>i);
		ip=0;
		for(j=0;j<(1<<(i-1));j++)
		{
			nV=cos((2*M_PI/(1<<i))*j);
			nM=sin((2*M_PI/(1<<i))*j);
			for(k=0;k<nSt;k++)
			{
				ni=2*nSt*j+k;
				if (i==1)
				{
				aGv[ip]=Koeff*(aRes[ni]+aRes[ni+nSt]);
				aGv[ip+nSt]=Koeff*(aRes[ni]-aRes[ni+nSt]);
				aGm[ip]=0;
				aGm[ip+nSt]=0;
				}
				else
				{
				nGtv=aGv[ni]+nV*aGv[ni+nSt]+nM*aGm[ni+nSt];
				nGbv[ip]=aGv[ni]-nV*aGv[ni+nSt]-nM*aGm[ni+nSt];
				nGtm=aGm[ni]+nV*aGm[ni+nSt]-nM*aGv[ni+nSt];
				nGbm[ip]=aGm[ni]-nV*aGm[ni+nSt]+nM*aGv[ni+nSt];
				aGv[ip]=nGtv;
				aGm[ip]=nGtm;
				}
				ip++;
			}
		}
		if (i!=1)
		{
			for(m=0;m<(Num>>1);m++)
			{
				aGv[m+(Num>>1)]=nGbv[m];
				aGm[m+(Num>>1)]=nGbm[m];
			}
		}
	}
	i=0;
	nGbv[i]=aGv[i]/Num;
	nGbm[i]=0;
	for (i=1;i<(Num>>1);i++)
	{
		SumKv=sqrt((aGv[i]/Num)*(aGv[i]/Num)+(aGm[i]/Num)*(aGm[i]/Num));
		nGbv[i]=2.0*(float)SumKv;
		nGbm[i]=i/(dT*Num);
		arctan=atan(aGm[i]/aGv[i]);
                argum[i]=(float)arctan*(180/M_PI);
		if(aGv[i]<0){argum[i]=argum[i]+180;}
		if(argum[i]<0){argum[i]=argum[i]+360;}
	}
	return;
}