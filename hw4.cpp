#include "crypt.h"
#include "histo.h"
#include "solve.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

unsigned char* getkey() {	//get local time in milliseconds
	unsigned char* key;
	key=new unsigned char[11];
	struct timeval now;
	long mtime,seconds,useconds;
	gettimeofday(&now,NULL);
	seconds=now.tv_sec;
	useconds=now.tv_usec;
	mtime=seconds+useconds;
	sprintf((char*)key,"%ld", mtime);
	return key;
}

unsigned char* RC4() {	//RC4 with 26 states to create key
	int i=0,j=0;
	unsigned char *s,temp;
	s=new unsigned char[26];
	unsigned char *key=getkey();
	for(i=0;i<26;i++) {
		s[i]=i;
	}
	for(i=0;i<26;i++) {
		j=(j+s[i]+key[i % 11]) % 26 ;
		temp = s[j];
		s[j] = s[i];
		s[i] = temp;
	}
	for (i=0;i<26;i++) {
		s[i]+=97;
	}
	free(key);
	return s;
}

int main (int argc, char *argv[]) {
	if(argc==3) {
		if(strcmp(argv[1],"keygen")==0) {	//if arg is keygen
			string option,param,temp;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-t=")==0) {
				param=temp.substr(3,temp.length());	//extract period
				if(param.compare("")==0) {
					fputs("Malformed command\n",stderr);
					exit(1);
				}
				int t;
				t=atoi(param.c_str());
				unsigned char* key;
				for(int i=0;i<t;i++) {
					key=RC4();
					usleep(1000);
					printf("%s\n",key);
				}
				free(key);
				return 0;
			}
			fputs("Malformed command\n",stderr);
			exit(1);
		}
		if(strcmp(argv[1],"invkey")==0) {	//if arg is invkey
			FILE *f;
			f=fopen(argv[2],"rb");
			if(f==NULL) {fputs("Error in opening file\n",stderr);exit(1);}
			int result=0,period=0,i=0,j=0;
			unsigned char *buff=new unsigned char;
			while(true) {	//read period
				result=fread(buff,1,1,f);
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
			rewind(f);
			unsigned char **keyarr=new unsigned char *[period];
			unsigned char **inv=new unsigned char *[period];
			for(i=0;i<period;i++) {
				keyarr[i] = new unsigned char[26];
				inv[i] = new unsigned char[26];
			}
			i=0;
			while(true) {	//save to array
				result=fread(buff,1,1,f);
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
				cout<<'\n';
			}
			*/
			for(i=0;i<period;i++) {
				for(j=0;j<26;j++) {
					inv[i][(keyarr[i][j]-97)]=j+97;
				}
			}
			for(i=0;i<period;i++) {
				for(j=0;j<26;j++)
					cout<<inv[i][j];
				cout<<'\n';
			}
			
			fclose(f);
			free(buff);
			for(i=0;i<period;i++) {
				delete [] keyarr[i];
				delete [] inv[i];
			}
			delete [] keyarr;
			delete [] inv;
			return 0;
		}
	}
	if(argc==3 || argc==4) {
		if(strcmp(argv[1],"crypt")==0) {	//if arg is crypt
			string option,keyfile,temp;
			FILE *f,*kf;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-k=")==0) {
				keyfile=temp.substr(3,temp.length());
				if(keyfile.compare("")==0) {
					fputs("Malformed command\n",stderr);
					exit(1);
				}
				if(argc==4) {
					f=fopen(argv[3],"rb");
					if(f==NULL) {fputs("Error in opening file\n",stderr);exit(1);}
				} else {
					f=stdin;
				}
				kf=fopen(keyfile.c_str(),"rb");
				if(kf==NULL) {fputs("Error in opening file\n",stderr);exit(1);}
				crypt(f,kf);
				return 0;
			}
		}
	}
	if(argc==4 || argc==5) {
		if(strcmp(argv[1],"histo")==0) {
			string temp,option1,option2,period,which;
			FILE *f;
			temp=argv[2];
			option1=temp.substr(0,3);
			if(option1.compare("-t=")==0) {
				period=temp.substr(3,temp.length());
				if(period.compare("")==0) {
					fputs("Malformed command\n",stderr);
					exit(1);
				}
				temp=argv[3];
				option2=temp.substr(0,3);
				if(option2.compare("-i=")==0) {
					which=temp.substr(3,temp.length());
					if(which.compare("")==0) {
						fputs("Malformed command\n",stderr);
						exit(1);
					}
					if(argc==5) {
						f=fopen(argv[4],"rb");
						if(f==NULL) {fputs("Error in opening file\n",stderr);exit(1);}
					} else {
						f=stdin;
					}
					histo(f,atoi(period.c_str()),atoi(which.c_str()));
					return 0;
				}
			}
		}
		if(strcmp(argv[1],"solve")==0) {
			string temp, option, max_t;
			FILE *f;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-l=")==0) {
				max_t=temp.substr(3,temp.length());
				if(max_t.compare("")==0) {
					fputs("Malformed command\n",stderr);
					exit(1);
				}
				f=fopen(argv[3],"rb");
				if(f==NULL) {fputs("Error in opening file\n",stderr);exit(1);}
				solve(f,atoi(max_t.c_str()));
				return 0;
			}
		}
	}
	fputs("Malformed command\n",stderr);
	exit(1);
	return 1;
}

