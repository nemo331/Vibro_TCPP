//-------------------------------------------------------------------------
//�ணࠬ�� ���筮� ��ࠡ�⪨ ����樨( �������� ����ﭭ�� ��⠢���饩,
//�������� 誠�� �६��� � �⬥�稪�, �����।������ 䠩��� (⥯��� �
//����� 䠩�� ���� 6 ���०���� ������ ������), ᣫ�������� ᪮���騬
//�।���,��।������ �������� ����樨 � �� �業��)
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
float kof_st=1.08;//�����樥�� ��� �������� 誠�� �६��� � �⬥�稪�
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
	      void obr_f_dan(void); //��ࠡ�⪠ 䠩�� ������
              void nach_z_kon_f(void);//��砫� ���������� ����筮�� 䠩��

  void main(void)  
{  //1
 clrscr();
 
  mkdir( "obr_rg" );
       aRes=(float*)farcalloc(Number,sizeof(float));
       if(aRes==NULL){printf("Out memory\n");return;}

	Otm=(float*)farcalloc(Number,sizeof(float));
	if(Otm==NULL){printf("Out memory\n");return;}

	

	clrscr();
	printf("\n\t\t���筠� ��ࠡ�⪠ ������ �� ����樨\n");
	printf("\n");

	 obr_f_dan();  //����ᥭ�� ������ �� ०��� �� 䠩�� ������ � ������ ��������
      
     //������᪠� ��ࠡ�⪠ ����让 ��㯯� 䠩��� (60) ��㯯���� �� 6 䠩���
      	  tf=tf-1;
	  if(tf<=0) {printf("���� ������ ���⮩\n");return;}

	      rar=0;
	  do { //33
	    
	   //��ନ஢���� ����室���� ��������

	  switch(spisok[rar+1].Regim[1])       //Regim[1]-����砥� �� ���� ���� �㪢� �� �������� ०���
		{
	  case '�': strcpy(O_name,"g_");  break;
	  case '�': strcpy(O_name,"v_");  break;
	  case '�': strcpy(O_name,"x_");  break;
	  case '�': strcpy(O_name,"b_");  break;
	  case '�': strcpy(O_name,"s_");  break;
	     default: strcpy(O_name,"__");  break;
		}
	 strncpy(unic_name,spisok[rar+1].Name_f,5);

	 for(i=0;i<CN;i++)   //CN-�᫮ �������
	  {   //4
	   if (kbhit())  { if (getch()==27) return;} //��室 �� ESC

           sprintf(rty,"c:\\DIAGR\\obr_rg\\%2s_%5s.r%02i",O_name,unic_name,i+1);
            printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL) printf("�� ���� ������ 䠩�%35s\n",rty);

	   

           sprintf(fale,"%2s_%5sr%02i",O_name,unic_name,i+1); //��ନ஢���� ����� 䠩�� ��� ��।�� ��� � Excel
	   fprintf(basefile,"%- 12s \n",fale);
	   

	  for(j=1;j<ch_reg+1;j++)    //ch_reg-�᫮ ���०����
	   {  //5
              strncpy (tek_naz,spisok[rar+j].Name_f,5);
	      int srav_naz=stricmp(unic_name,tek_naz);
              if(srav_naz!=0) {break;}
	    sprintf(str,"c:\\DIAGNOST\\oscill\\%5s_%02i.o%02i",unic_name,j,i+1);
	    myfile=fopen(str,"rt");
	     if(myfile==NULL) {printf("�� ���� ������ 䠩�%35s\n",str);}
	     else
	      {//10
                nach_z_kon_f();
	       

	    sprintf(otme,"c:\\DIAGNOST\\oscill\\%5s_%02i.o16",unic_name,j);
	     if(str!=otme)
	     {
	    otmfile=fopen(otme,"rt");
	    if(otmfile==NULL){printf("�� ���� ������ 䠩�%35s\n",otme);}
             }
	       float vspom;
	       float sum=0;
	       
	       fgets(vrvs,15,myfile);
//	       fscanf(myfile,"%s",&vrvs);
	       for(int k=0;k<Number;k++)    //Number-�᫮ 蠣�� ���� ���
		{//6
		 fscanf(myfile,"%f",&vspom);
		 fscanf(myfile,"%f",&aRes[k]);
		 if(k==1) {stepp=vspom*kof_st;}
		 sum=aRes[k]+sum;
		} //6
		  koof=sum/Number;   //koof-����ﭭ�� ��⠢����� ᨣ����
                  for(int r=0;r<Number;r++)  
		  aRes[r]=aRes[r]-koof;     //���⠭�� ����ﭭ�� ��⠢���饩
		if(str!=otme){ fscanf(otmfile,"%s",&vrnen);}  //�����஢���� ���� � 䠩�� �⬥�稪�(����� �� �ᯮ������)
		if(str==otme){ fscanf(myfile,"%s",&vrnen);}
		for(int z=0;z<Number;z++)
		 {
	       if(str!=otme){ fscanf(otmfile,"%f %f",&vr,&Otm[z]);}
	       if(str==otme){ fscanf(myfile,"%f %f",&vr,&Otm[z]);}
		}
		 float maxil=0;
		 float minil=0;
		 for(r=0;r<Num2;r++)
		   {                      //��宦����� ���ᨬ㬠 � �����㬠 (� ��������� ��� ����஥��� ����ࠬ��)
		   maxil=(aRes[r] > maxil) ? aRes[r]:maxil;
		   minil=(aRes[r] < minil) ? aRes[r]:minil;
                   }

		int flag=0;               //��ࠡ�⪠ �⬥�稪�
		float sumotm=0;
                float otmkof=0;
		 for(int h=0;h<Number;h++)
		   sumotm=sumotm+fabs(Otm[h]);
		 otmkof=sumotm/Number;
		  for(h=0;h<Num2;h++)
		     if ((fabs(Otm[h])-otmkof)>kof_zap){flag=h;h=Num2;}  //��宦����� ��ࢮ�� ���� �⬥�稪�

		     int k_met=0;
                     int mark=0;
		  for(h=0;h<Number;h++)  //������ ���-�� ��⮪ �⬥�稪� � �६��� �� �ࠡ��뢠���
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
			
			 for(int t=0;t<k_met-1;t++)     //������ ���ᨬ㬮� � �����㬮� �� 20 ����⠬
		       { sr20max[t]=0; sr20min[t]=0;
		        for(int s=fal[t];s<fal[t+1];s++)
			{sr20min[t]=(aRes[s] < sr20min[t]) ? aRes[s]:sr20min[t];
			 sr20max[t]=(aRes[s] > sr20max[t]) ? aRes[s]:sr20max[t];}
		       }      
		      
		       
		     for(t=0;t<k_met-1;t++)     //�।��� ���祭�� ����樨 �� 20 ����⠬
		      sr20sum=fabs(sr20min[t])+fabs(sr20max[t])+sr20sum;
                    
		       sr20=sr20sum/(k_met-1);

		      for(t=0;t<k_met-1;t++)   //�।���������᪮� �⪫������ �� 20 ����⠬
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

     //�業����� ������
                 float maxi=0;
		float mini=0;
                float max_vibr=0;
		for( k=0;k<Number;k++)
		  {maxi=(aRes[k] > maxi) ? aRes[k]:maxi;} //��宦����� ���祭�� ᠬ��� ��᮪��� ����
		for( k=0;k<Number;k++)
		  {mini=(aRes[k] < mini) ? aRes[k]:mini;} //��宦����� ���祭�� ᠬ�� ��㡮��� �������
		  max_vibr=fabs(maxi)+fabs(mini); //��।������ ����. ���祭�� ����樨

                  r_Sksr=(float*)farcalloc(Number,sizeof(float));
		  if(r_Sksr==NULL){printf("Out memory\n");return;}
                  int nach; int kon; int w;
     if(k_met<=4) {nach=16;kon=Number-16;}
      else   {nach=fal[1];kon=fal[k_met-2];}
		for( k=nach;k<kon;k++)   //��宦����� ᪮���饣� �।���� �� 31-���
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
		  {maxim=(r_Sksr[k] > maxim) ? r_Sksr[k]:maxim;} //��宦����� ���祭�� ᠬ��� ��᮪��� ����
                for( k=nach;k<kon;k++)
		  {minim=(r_Sksr[k] < minim) ? r_Sksr[k]:minim;} //��宦����� ���祭�� ᠬ�� ��㡮��� �������
		  maxsr_vibr=fabs(maxim)+fabs(minim); //��।������ ����.�।���� ���祭�� ����樨
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
 //��ࠡ�⪠ ᪮���饣� �।���� �� 31 ���祭�� (�� 20 ����⠬)
			 
			 for(int t=0;t<k_met-3;t++)     //������ ���ᨬ㬮� � �����㬮� �� 20 ����⠬
		       { sr20maxskl[t]=0; sr20minskl[t]=0;
			for(int s=fal[t+1];s<fal[t+2];s++)
			{sr20minskl[t]=(r_Sksr[s] < sr20minskl[t]) ? r_Sksr[s]:sr20minskl[t];
			 sr20maxskl[t]=(r_Sksr[s] > sr20maxskl[t]) ? r_Sksr[s]:sr20maxskl[t];}
		       }      

		       
		     for(t=0;t<k_met-3;t++)     //�।��� ���祭�� ����樨 �� 20 ����⠬
		      sr20sumskl=fabs(sr20minskl[t])+fabs(sr20maxskl[t])+sr20sumskl;

		       sr20skl=sr20sumskl/(k_met-3);

		      for(t=0;t<k_met-3;t++)   //�।���������᪮� �⪫������ �� 20 ����⠬
		      { sumprskl=fabs(sr20minskl[t])+fabs(sr20maxskl[t]);
			srkv20sumskl=pow((sumprskl-sr20skl),2)+srkv20sumskl;
                      }
		       srkv20otkskl=sqrt(srkv20sumskl/(k_met-4));
	       }
			free(sr20minskl);
			free(sr20maxskl);
	                farfree(r_Sksr);

		for(h=0;h<Num2;h++)        //����஭����� � �⬥�稪��
		 {
		  aRes[h]=aRes[h+flag];
		  if((h+flag)<Num2){Otm[h]=Otm[h+flag];}
		  else{Otm[h]=-1;}
		  }
		  strset(ocenk,0);
		  strncpy(sop,spisok[rar+j].chan[i],2);
		  oc=stricmp(sop,"��");
		  op=stricmp(sop,"");

	       if(op!=0)
		 {
		   if(oc!=0) {
	            if(maxsr_vibr<180){strcpy(ocenk,"���");}//�業�� �।��� ����樨
                    else{strcpy(ocenk,"�����");}
		    if(maxsr_vibr<140){strcpy(ocenk,"�");}
                    if(maxsr_vibr<100){strcpy(ocenk,"��");} 
		    if(maxsr_vibr<50){strcpy(ocenk,"��");}
                              }
	           else { if(maxsr_vibr<80){strcpy(ocenk,"�");}//�業�� �।��� ����樨
		          else{strcpy(ocenk,"���");}
		        }
		 }

                   fprintf(basefile," ��.���.  \t");
		      fprintf(basefile," ����.���.  \t");
		      fprintf(basefile," ��.���.20\t");
		      fprintf(basefile," �ઢ.�⪫.20\t");
                      fprintf(basefile," ��᪫.���.20\t");
                      fprintf(basefile," �ઢ᪫.�⪫.20\t");
		       fprintf(basefile," �業��  \n");
			fprintf(basefile,"%- .1f ��� \t",maxsr_vibr);
			 fprintf(basefile,"%- .1f ��� \t",max_vibr);
			 fprintf(basefile,"%- .1f ��� \t",sr20);
			 fprintf(basefile,"%- .2f ��� \t",srkv20otk);
                         fprintf(basefile,"%- .1f ��� \t",sr20skl);
			 fprintf(basefile,"%- .2f ��� \t",srkv20otkskl);
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
      datfile=fopen(fdan,"rt");  //����⨥ 䠩�� ������
      if(datfile==NULL) printf("�� ���� ������ 䠩�%35s\n",fdan);
      char* df=NULL;
      for(tf=0;tf<60;tf++)
     {
      int c=0;
   do{
	 strcpy(prom,"");         //������ prom
      do {
	  df=fgets(t,2,datfile); //    �����஢��� ���� ᨬ��� �� 䠩�� � t
	    ot=stricmp(df,"\t"); //    �ࠢ���� �� ����稥 ᨬ���� ⠡��樨
	    on=stricmp(df,"\n"); //    �ࠢ���� �� ����稥 ᨬ���� ��ॢ��� ��ப�
	    kav=stricmp(df,"\"");//    �ࠢ���� �� ����稥 ᨬ���� ����祪
	     if (df==NULL) break;//    �᫨ ����� 䠩�� ��� �� ��楤���
	   if (ot!=0 && on!=0 && kav!=0) { strcat(prom,t);}  //�������� ᨬ��� � prom �஬� \t, \n, \"

	 }  while (ot!=0 && on!=0); // ������ ���� �� ⠡���� ��� ���� �� ��ॢ�� ��ப�

	     switch(c)                // ���������� �������� dat_reg ���ᨢ� spisok
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
	  c=c+1;            // ���稪 ����� ���� �������� dat_reg ���ᨢ� spisok
      }  while (on!=0);
         if (df==NULL) break; 
     }	
    fclose(datfile);
    }       //����� �������  obr_f_dan

void nach_z_kon_f(void)
      {
             fprintf(basefile,"N �/�\t%s\t",spisok[rar+j].N_pp);
	     fprintf(basefile,"����\t%s\t",spisok[rar+j].Name_f);
	     fprintf(basefile,"��\t%2s\t",spisok[rar+j].N_GA);
	     fprintf(basefile,"���\t%s\t",spisok[rar+j].Date_zam);
          switch(O_name[0])
		{
	  case 'G':
	  case 'g':  fprintf(basefile,"�������� ०��\t"); break;
	  case 'V':
	  case 'v':  fprintf(basefile,"�����⮩ 室 � ����㦤�����\t"); break;
	  case 'X':
	  case 'x':  fprintf(basefile,"�����⮩ 室 ��� ����㦤����\t"); break;
	  case 'B':
	  case 'b':  fprintf(basefile,"�롥�\t"); break;
	  case 'S':
	  case 's':  fprintf(basefile,"����� ��\t"); break;
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
      }        //����� ������� nach_z_kon_f