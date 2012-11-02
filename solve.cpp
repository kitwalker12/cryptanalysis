#include "solve.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void solve(FILE *f, int max_t) {
	int result=0,length=0,count=0,i=0;
	unsigned char* buff=new unsigned char;
	
	int countarr[26]={0};
	
	while(true) {
		result=fread(buff,1,1,f);
		if(result==0)
			break;
		if(*buff=='\n') {
			break;
		}
		if(*buff>='a' && *buff<='z') {
			count++;
			countarr[*buff-97]++;
		}
		length++;
		if((*buff<0x20 || *buff>0x7e) && *buff!='\n') {
			fputs("Error Reading File\n",stderr);
			exit(1);
		}
	}
	rewind(f);
	unsigned char* filearr = new unsigned char[length];
	string file;
	while(true) {
		result=fread(buff,1,1,f);
		if(result==0)
			break;
		if(*buff=='\n') {
			break;
		}
		filearr[i++]=*buff;
		file+=*buff;
	}
	
	int len=4;
	int j,k;
	string temp1;
	bool flag=true;
	bool found=false;
	printf("Kasiski Method\n==============\n");
	while(flag) {
		for(i=0;i<length;i++) {
			for(k=i;k<(i+len);k++) {
				if(filearr[k]<'a' || filearr[k]>'z') {
					i=k;
					temp1="";
					break;
				}
				temp1+=filearr[k];
			}
			if((int)temp1.length()==len) {
				j=file.find(temp1,k);
				while(j!=-1) {
					printf("len=%d, i=%d, j=%d, j-i=%d, %s\n",len,i,j,(j-i),temp1.c_str());
					j=file.find(temp1,(j+len));
					found=true;
				}
				temp1="";
			}
		}
		if(found) {
			len++;
			found=false;
		} else {
			printf("len=%d, no more matches\n\n",len);
			flag=false;
		}
	} //kasiski end
	
	printf("Average Index of Coincidence\n============================\nL=%d\n",count);
	char temp;
	for(i=0;i<26;i++) {
		temp=i+97;
		printf("f('%c')=%d\n",temp,countarr[i]);
	}
	double IC=0;
	double temp2;
	for (i=0;i<26;i++){
		temp2=(countarr[i]*(countarr[i]-1));
		IC+=temp2;
	}
	IC/=(count*(count-1));
	printf("IC=%.8g\n",IC);
	
	double eIC=0.0,kp=0.0658,kr=1.0;
	kr/=26;
	double L=(double)count;
	for(double t=1;t<=max_t;t++) {
		eIC=(((L-t)*kp)/(t*(L-1)))+(((t-1)*L*kr)/(t*(L-1)));
		printf("t=%d, E(IC)=%.8g\n",(int)t,eIC);
	}
	//End calculate IC
	
	printf("\nAuto-correlation Method\n=======================\n");
	int dupcount=0;
	for(i=1;i<=max_t;i++) {
		for(j=0;j<length-i;j++) {
			if(filearr[j]>='a' && filearr[j]<='z' && filearr[j+i]>='a' && filearr[j+i]<='z' && filearr[j]==filearr[j+i]) {
				dupcount++;
			}
		}
		printf("t=%d, count=%d\n",i,dupcount);
		dupcount=0;
	}
	printf("\n");
	
	
	free(buff);
	free(filearr);
	fclose(f);
	
}

