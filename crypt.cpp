#include "crypt.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void crypt(FILE *f, FILE *kf) {
	int result=0,period=0,length=0;
		//printf("abcdefghijklmnopqrstuvwxyz\n\n");
	unsigned char *buff=new unsigned char;
	int i=0,j=0;
	while(true) {	//read period
		result=fread(buff,1,1,kf);
		if(result==0)
			break;
		if(*buff=='\n') {
			period++;
		}
		if((*buff<0x20 || *buff>0x7e) && *buff!='\n') {
			fputs("Error Reading File\n",stderr);
			exit(1);
		}
	}
	rewind(kf);
	unsigned char **keyarr=new unsigned char *[period];
	for(i=0;i<period;i++)
		keyarr[i] = new unsigned char[26];
	i=0;
	while(true) {	//save to array
		result=fread(buff,1,1,kf);
		if(result==0)
			break;
		if(*buff=='\n') {
			i++;
			j=0;
			continue;
		}
		keyarr[i][j++]=*buff;
	}
		/* Print Keyfile
		for(i=0;i<period;i++) {
		for(j=0;j<26;j++)
		cout<<keyarr[i][j];
		cout<<endl;
		}
		*/
	length=0;
		/*
		while(true) {	//read length
		result=fread(buff,1,1,f);
		if(result==0)
		break;
		if(*buff=='\n') {
		//cout<<endl;
		break;
		}
		//cout<<*buff;
		length++;
		}
		*/
	//unsigned char* filearr=new unsigned char[length];
	//unsigned char* cipher=new unsigned char[length];
		//rewind(f);
	i=0;
	int count=0;
	while(true) {
		result=fread(buff,1,1,f);
		if(result==0)
			break;
		//filearr[i]=*buff;
		length++;
		if(*buff=='\n') {
			cout<<'\n';
			break;
		}
		if(*buff<'a' || *buff>'z') {
			count++;
			count=count%period;
			cout<<*buff;
			continue;
		}
		if((*buff<0x20 || *buff>0x7e) && *buff!='\n') {
			fputs("Error Reading File\n",stderr);
			exit(1);
		}
		//cipher[i]=keyarr[count][*buff-97];
		cout<<keyarr[count][*buff-97];
		i++;
		count++;
		count=count%period;
	}
	length=i;
	fclose(f);
	fclose(kf);
	free(buff);
	//free(filearr);
	//free(cipher);
	for(i=0;i<period;i++)
		delete [] keyarr[i];
	delete [] keyarr;
}


