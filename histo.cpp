#include "histo.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void histo(FILE *f, int period, int which) {
	int result=0,length=0,count=0;
	int *histo=new int[27];
	int *index=new int[27];
	unsigned char* buff=new unsigned char;
	int i;
	for(i=0;i<27;i++){
		histo[i]=0;
		index[i]=i;
	}
	while(true) {
		result=fread(buff,1,1,f);
		if(result==0)
			break;
		if(*buff=='\n') {
			break;
		}
		length++;
		if(*buff<'a' || *buff>'z') {
			continue;
		}
		if((*buff<0x20 || *buff>0x7e) && *buff!='\n') {
			fputs("Error Reading File\n",stderr);
			exit(1);
		}
		if((length%period)==which || ((length%period)==0 && (period==which))) {
			histo[*buff-96]++;
			count++;
		}
	}
	cout<<"L="<<count<<endl;
	//bubble sort array
	int j;
	int flag = 1;
	int t;             
	for(i = 2; (i <= 27) && flag; i++) {
	          flag = 0;
	          for (j=1; j < 26; j++)
	         {
	               if (histo[j+1] > histo[j])      
	              { 
	                    t = histo[j];             
	                    histo[j] = histo[j+1];
	                    histo[j+1] = t;
						t=index[j];
						index[j]=index[j+1];
						index[j+1]=t;
	                    flag = 1;               
	               }
	          }
	     }
	//bubble sort end
	char temp;
	double percentage;
	for(i=1;i<27;i++) {
		temp='a'+index[i]-1;
		percentage=histo[i];
		percentage/=count;
		percentage*=100;
		printf("%c: %d (%.2f%%)\n", temp, histo[i], percentage);
	}
	
	free(buff);
	fclose(f);
}

