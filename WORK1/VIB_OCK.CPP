//-------------------------------------------------------------------------
//�ணࠬ�� �業�� �� �।����䬥��᪮� ����樨( �������� ����ﭭ�� ��⠢���饩,
//��।������ �஢�� ���ᨬ��쭮�, �।��������筮� ����樨, �����।������ 䠩��� (⥯��� �
//����� 䠩�� ���� 6 ���०���� 16 �������))
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
	printf("\n\t\t�業�� �� ����樨\n");
	printf("\n");
 /*       printf(" ��室 �� ESC ��� �த������\n");
         int esc=getch();
	 if(esc==27) return; */

	   printf(" ����� �� (1-10)\n");
      /*	 GA_num[0]=3;
	 cgets(GA_num);
	 strcpy(GA_num,&GA_num[2]);   */
	 int ga=atoi(argv[1]);
         printf("%d\n",ga);

	 printf("\n �業�� ����樨 ��: 16)����� 㧫�� 0)������ ����\n");
	 printf("��᫮ �� 0 �� 16 ����砥� �� ������ ������(�����⥫쭮) \n");
	 printf("������ �業������� ��� ��� ������ 㧫��, �� ��⠢���� ������� -  \n");
	 printf("��� ��� ᯨ��� ���� (�� 㬮�砭�� 0)\n");
   /*	 fls[0]=3;
	 cgets(fls);
	 strcpy(fls,&fls[2]);  */
	 fl=atoi(argv[2]);
	 if(fl<=0||fl>16){fl=0;}
         printf("%d\n",fl);
        


    printf("������쭮�  ������������ ��ࠡ��뢠���� 䠩��� \n");
/*    unic_name[0]=6;
    buffer=cgets(unic_name);  */
    strcpy(unic_name,argv[3]);
    printf("%s\n",unic_name);

     printf("������������ 䠩�� ��᫥ ��ࠡ�⪨ \n");
//     printf("��������㥬� �����: G[x]-�������� ०��,V[x]-����㦤����,\n");
//     printf("   X[x]-宫��⮩ 室,B[x]-�롥�,S[x]-०�� ��,��� [x]-�� ᨬ���\n");
 /*     char* buf;
      O_name[0]=3;
      buf=cgets(O_name);  */
      strcpy(O_name,argv[4]);
      printf("%s\n",O_name);
      printf("\n");

       sprintf(str2,"c:\\DIAGR\\OCENKA\\%2s_%5s.ock",O_name,unic_name);
           printf("%35s\n",str2);
	   basefile=fopen(str2,"wt");
	    if(basefile==NULL){printf("�� ���� ������ 䠩�%35s\n",str2);}

             fprintf(basefile,"�� %d\t",ga);
	   

          switch(O_name[0])
		{
	  case 'G':
	  case 'g':  fprintf(basefile,"�������� ०��\n");
		   break;
	  case 'V':
	  case 'v':  fprintf(basefile,"�����⮩ 室 � ����㦤�����\n");
		   break;
	  case 'X':
	  case 'x':  fprintf(basefile,"�����⮩ 室 ��� ����㦤����\n");
		   break;
	  case 'B':
	  case 'b':  fprintf(basefile,"�롥�\n");
		    break;
	  case 'S':
	  case 's':  fprintf(basefile,"����� ��\n");
		   break;
	  default:  fprintf(basefile,"             \n");
		}
				    
           sprintf(fal,"%2s_%5sock",O_name,unic_name); //��ନ஢���� ����� 䠩�� ��� ��।�� ��� � Excel
	   fprintf(basefile,"%- 12s \t",fal);

     for(int i=0;i<CN;i++)   //CN-�᫮ �������
	  {   //4
	  if (kbhit())  { if (getch()==27) break;} //��室 �� ESC
	  printf("        ��ࠡ��뢠���� ����� N%-d\r",i+1);
        for(int j=0;j<ch_reg;j++)    //ch_reg-�᫮ ���०����
	   {  //5
	    sprintf(str1,"c:\\DIAGNOST\\oscill\\%5s_%02i.o%02i",unic_name,j+1,i+1);
	    myfile=fopen(str1,"rt");
	   if(myfile==NULL){printf("�� ���� ������ 䠩�%35s\n",str1);}
	     else
	      {//10
		Sig=(float*)calloc(Number,sizeof(float));
		 if(Sig==NULL){printf("Out memory\n");return;}
	
               float vs;
	       float sum=0;
               float stepp=0;
	       fgets(vrvs,15,myfile);
	       for(int k=0;k<Number;k++)    //Number-�᫮ 蠣�� ���� ���
		{//6
		 fscanf(myfile,"%f %f",&vs,&Sig[k]);
                 if(k==1) {stepp=vs*kof_st;}
		 sum=Sig[k]+sum;
		} //6
                  koof=sum/Number;        //koof-����ﭭ�� ��⠢�����
		 for(int r=0;r<Number;r++) //���⠭�� ����ﭭ�� ��⠢���饩
		{Sig[r]=Sig[r]-koof;}
//�����
                float maxi=0;
		float mini=0;
                float max_vibr=0;
		for( k=0;k<Num2;k++)
		  {maxi=(Sig[k] > maxi) ? Sig[k]:maxi;} //��宦����� ���祭�� ᠬ��� ��᮪��� ����
		for( k=0;k<Num2;k++)
		  {mini=(Sig[k] < mini) ? Sig[k]:mini;} //��宦����� ���祭�� ᠬ�� ��㡮��� �������
		  max_vibr=fabs(maxi)+fabs(mini); //��।������ ����. ���祭�� ����樨

		r_Sksr=(float*)calloc(Num2,sizeof(float));
		 if(r_Sksr==NULL){printf("Out memory\n");return;}
	
		int w;
		for( k=0;k<Num2;k++)   //��宦����� ᪮���饣� �।���� �� 31-���
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
		  {maxim=(r_Sksr[k] > maxim) ? r_Sksr[k]:maxim;} //��宦����� ���祭�� ᠬ��� ��᮪��� ����
                for( k=0;k<Num2;k++)
		  {minim=(r_Sksr[k] < minim) ? r_Sksr[k]:minim;} //��宦����� ���祭�� ᠬ�� ��㡮��� �������
		  maxsr_vibr=fabs(maxim)+fabs(minim); //��।������ ����. ���祭�� ����樨

		   for( k=0;k<Num2;k++)
		   { r_Sksr[k]=fabs(r_Sksr[k]);}

		     for( k=0;k<Number-15;k++)
		      {Sig[k]=Sig[k+15];}

	         for( k=0;k<Num2;k++)
		 {Sig[k]=fabs(Sig[k]);}

                Pik=(float*)calloc(Num2,sizeof(float));
		 if(Pik==NULL){printf("Out memory\n");return;}

		int sch=pik1(Sig);  //�㭪�� ������ ���-�� � ���祭�� �����

                int sch1=0;
	        int sch2=0;
                 for(k=0;k<sch;k++)   //����ᮢ�� �� �᫮��� (�᫨ ���� ����� ����� 5��� �� �� ����)
		 {
		  if(Pik[k]<5){Pik[k]=0;sch1=sch1+1;}
		  }
		float sum_kv=0;
                float srkv_vibr=0;
	        sch2=sch-sch1;
		if(sch2>1)
                {
		  for (int h=0;h<sch2;h++)  //��।������ �।��������筮�� ���祭�� ����樨
		{ Pik[h]=Pik[h]*Pik[h];
		sum_kv=Pik[h]+sum_kv;Pik[h]=0;}
                 sum_kv=sqrt(sum_kv);
		 srkv_vibr=sum_kv/(sqrt(sch2/2));
		 }
		 else srkv_vibr=0;
                  free(Pik);

  // ��।������ �।��� ����樨 �� �।��� ����
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

		   pik2(Razn);//�㭪�� ���������� �� �����쪨� �����

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

		  sch=pik1(Sig);  //�㭪�� ������ ���-�� � ���祭�� �����

	       sch1=0;
	       sch2=0;
	       float sum_pik=0;
	       float sr_vibr=0;
               float sr_chast=0;
	       if(Pik[0]>1&&sch>1)  //�᫨ ���� ���� ������ , ���� ���室 � ᫥���饬� ���०���
	    {
	      
		 for(k=0;k<sch;k++)   //����ᮢ�� �� �᫮��� (�᫨ ���� ����� ����� 5 ��� �� �� ����)
		 {
		  if(Pik[k]<5){Pik[k]=0;sch1=sch1+1;}
	          }
	        sch2=sch-sch1;
		if(sch2>1)
		{
		 for (int s=0;s<sch;s++) //��।������ �।���� ���祭�� ����樨
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
	          if(maxsr_vibr<180){strcpy(ocenk,"���");}//�業�� �।��� ����樨
                  else{strcpy(ocenk,"�����");}
		  if(maxsr_vibr<140){strcpy(ocenk,"�");}
                  if(maxsr_vibr<100){strcpy(ocenk,"��");} 
		  if(maxsr_vibr<50){strcpy(ocenk,"��");}
                  strcpy(uzlji,"����� 㧫�");
		}
		else
                  {
                  if(maxsr_vibr<80){strcpy(ocenk,"�");}//�業�� �।��� ����樨
		  else{strcpy(ocenk,"���");}
		  strcpy(uzlji,"������ ����");
		  }

		if(i==0&&j==0) fprintf(basefile,"%9s",vrvs);
		if(j<1)
		{
		fprintf(basefile,"---------------------------------------------\n");
		 fprintf(basefile,"����� N%-d\t",i+1);
		 fprintf(basefile,"%15s\n",uzlji);
		 }
		  fprintf(basefile,"���०�� N%-d\n",j+1);
		   fprintf(basefile,"��.���.\t");
                    fprintf(basefile,"�ઢ.���.\t");
		     fprintf(basefile,"����.���.\t");
		      fprintf(basefile,"�業��\t");
		       fprintf(basefile,"��.���.\t");
			fprintf(basefile,"�� ��.�\n");
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
              //    �����

 int pik1(float* mas1)
 {
 
 float vn_maxim, sr_zn=0;
 int a,b;
 int vn_sch=0,cc=0;
  for(int k=0;k<Num2;k++)  // ��砫� 横�� � k
{                          // ��宦����� ���祭�� � ���-�� �����
 for(int u=1;u<Num2;u++)   //�஢�ઠ �����⠥� �㭪�� ��� �뢠��
  { //��
  sr_zn=mas1[k];
  if(sr_zn!=mas1[k+u]){sr_zn=mas1[k];cc=u;u=Num2;}
  else {if(u==Num2-1) return vn_sch;}
  }
   if(sr_zn<mas1[k+cc])    //�᫨ �᫮��� ᮡ����� , � �㭪�� �����⠥�
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
 else               //���� �㭪�� �뢠��
  {
 for (b=k;b<Num2;b++)
  if(mas1[b]<mas1[b+1]) { k=b-1;b=Num2;}
  }
 } //����� 横�� � k
 return vn_sch;
  }
//---------------------------------------------------------------------
void pik2(float* mass2) 
{
 float vr_maxim, sr_znah=0;
 int l,m,p,flagok=1;       //�᫨ flagok=1 � ��� ����� ����������
 int cs=0,n_min=0;
  for(int r=0;r<Num2;r++)  // ��砫� 横�� � k
  {                     // ��宦����� ���祭�� � ���-�� �����
 for(int u=1;u<Num2;u++) //�஢�ઠ �����⠥� �㭪�� ��� �뢠��
  { //��
  sr_znah=mass2[r];
  if(sr_znah!=mass2[r+u]){sr_znah=mass2[r];cs=u;u=Num2;}
  else {if(u==Num2-1) return;}
  }
   if(sr_znah<mass2[r+cs])    //�᫨ �᫮��� ᮡ����� , � �㭪�� �����⠥�
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
     else               //���� �㭪�� �뢠��
  {
 for (m=r;m<Num2;m++)
 {
  if(mass2[m]<mass2[m+1]) {n_min=m;r=m-1;m=Num2;}
  if((mass2[m]>mass2[m+1])&&(flagok==1)) mass2[m]=0;
 }
  }
 } //����� 横�� � k

}