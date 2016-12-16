#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

#define IP1 10
#define IP2 50

/*
 * 开放定址-线性探测法
 * 扩展实际hash空间为 hash聚集区的 N倍，这样可大幅度降低冲突数
 * 适用于大小固定的哈希
 */
#define ARRAY_MUL_NUM 4

#define HASH_ARRAY_SIZE (IP1*IP2)

typedef struct _hash_array
{
	int key;
	char *ip;
	int npos;
}HashArray;


int hash(char *data)
{
	assert(data!=NULL);
	char *p = data;
	unsigned int hashVal = 0;
	while(*p)
	{
		hashVal ^= (hashVal<<5)+  *p++*3 + (hashVal>>3);
	}


	hashVal %= HASH_ARRAY_SIZE*ARRAY_MUL_NUM;
	//printf("hash(%s) %d\n",data,hashVal);
	return hashVal;
}

HashArray* init_array_hash(int n)
{
	HashArray *p = NULL;
	p = (HashArray*)calloc(n*sizeof(HashArray)+1,1);

	return p;
}

void insert_array_hash(HashArray *hashtbl, char *data)
{
	assert(hashtbl!=NULL && data!=NULL);	
	int hashAddr = 0;
	int addr = 0;
	int pos = 0;
	char *pdata = NULL;
	HashArray *p;

	
	hashAddr = hash(data);
	addr = hashAddr;
	p = hashtbl + hashAddr;
	while(p->ip != NULL)
	{
		//开放定址-线性探测法
		hashAddr = (++hashAddr)%(HASH_ARRAY_SIZE*ARRAY_MUL_NUM) ;	
		if(addr == hashAddr)
		{
			printf("hashtbl is full addr:%d\n",addr);
			break;
		}
		p = hashtbl + hashAddr;
		pos++;
	}
	//cout<<"insert "<<data<<" addr:"<<hashAddr<<endl;
	pdata = (char*)calloc(strlen(data)+1,1);
	memcpy(pdata,data, strlen(data));
	p->ip = pdata;
	p->key  = addr;
	p->npos = pos;

}

int search_array_hash(HashArray *hashtbl,char *data)
{
	assert(hashtbl!=NULL && data!=NULL);	
	int hashAddr = 0;
	int addr = 0;
	int n = 0;
	HashArray *p;

	hashAddr = hash(data);
	addr = hashAddr;
	p = hashtbl + hashAddr;
	while(p->ip!=NULL && (strcmp(p->ip,data)!=0))
	{
		//开放定址-线性探测法
		hashAddr = (++hashAddr)%(HASH_ARRAY_SIZE*ARRAY_MUL_NUM) ;
		if(hashAddr == addr)
			return -1;
		cout<<"search "<<data<<" ip:"<<p->ip<<endl;
		p = hashtbl + hashAddr;
		n++;
	}
	cout<<"search ip:"<<p->ip<<" key:"<<p->key<<" use:"<<n<<"times\n";
		
	return hashAddr;
}

void free_array_hash(HashArray *hashtbl,int n)
{
	if(hashtbl == NULL)
	return;
	HashArray *h = hashtbl;
	for(int i=0;i<n;i++)
	{
		if(h->ip != NULL)
		{
			//cout<<"free "<<h->ip<<endl;
			free(h->ip);
		}
		h++;
	}
	free(hashtbl);
}

void echo_array_hash(HashArray *hashtbl, int n)
{
	HashArray *h = hashtbl;
	int t = 0;
	for(int i=0;i<n;i++)
	{
		if(h->ip != NULL)
		printf("echo index %d  key:%d ip:%s npos:%d\n",i,h->key, h->ip, h->npos);
		t = t>h->npos?t:h->npos;
		h++;
	}
	cout<<"most npos:"<<t<<endl;
}

int main(int argc, char *argv[])
{
	HashArray *p_hash = NULL;
	
	p_hash = (HashArray*)init_array_hash(HASH_ARRAY_SIZE*ARRAY_MUL_NUM);
	if(p_hash == NULL)
		printf("init_array_hash() failed !!!\n");
	
	int i=0;
	char buf[16];
	vector<int> v;
	vector<int>::iterator itv;
	int h = 0;
	for(i=0;i<IP1;i++)
	{
		for(int j=0;j<IP2;j++)
		{
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"192.128.%d.%d",i,j);
		h = hash(buf);
		v.push_back(h);
		insert_array_hash(p_hash, buf);
		}
	}
	echo_array_hash(p_hash, HASH_ARRAY_SIZE*ARRAY_MUL_NUM );

	#if 0
	sort(v.begin(),v.end());
	for(i=0;i<v.size();i++)
		cout<<v[i]<<endl;
	itv = unique(v.begin(),v.end());
	cout<<"\nleft:"<<itv-v.begin()<<endl;
	#endif
	char *pbuf = argv[1];
	search_array_hash(p_hash,(char*)pbuf);

	free_array_hash(p_hash, HASH_ARRAY_SIZE*ARRAY_MUL_NUM);

	return 0;
}
