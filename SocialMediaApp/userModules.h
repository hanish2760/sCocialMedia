#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include"mediaHeaders.h"

int fileSize(char* filename){
	FILE* fp = fopen(filename, "r");
	if (fp){
		fseek(fp, 0, SEEK_END);
		int s = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		fclose(fp);
		return s;
	}
	else{
		printf("\nCannot open the post\n Unable to upload post\n");
		return -1;
	}
}


void createDataBasefile(){
	FILE* f = fopen("SocialMediaDatabase.txt", "wb");
	fseek(f, (1024 * 1024 * 100) - 1, SEEK_SET);
	fwrite("", sizeof(char), 1, f);
	fclose(f);
}


void initialiseGlobals(){
	FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
	int a = 0;
	fwrite(&a, sizeof(int), 1, f);
	fwrite(&a, sizeof(int), 1, f);
	fseek(f, 0, SEEK_END);
	int offset = ftell(f);
	fseek(f, 2*sizeof(int), SEEK_SET);
	fwrite(&offset, sizeof(int), 1, f);
	fclose(f);
}
void preInitialisationTasks(){
	createDataBasefile();
	initialiseGlobals();
}

void getGlobals(int id){
	// try to authenticate user.
	FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
	fseek(f, 0, SEEK_SET);
	fread(&noOfusers,sizeof(int),1,f);
	fread(&noOfPosts, sizeof(int), 1, f);
	fread(&Offset, sizeof(int), 1, f);
	
	fseek(f, (3 * sizeof(int)) + (id*sizeof(user)), SEEK_SET);
	fread(&currentUser, sizeof(user), 1, f);
	fclose(f);
}


void setGlobals(){
	FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
	fwrite(&noOfusers, sizeof(int), 1, f);
	fwrite(&noOfPosts, sizeof(int), 1, f);
	fwrite(&Offset, sizeof(int), 1, f);
	fseek(f, (3 * sizeof(int)) + (currentUser.userid*sizeof(user)), SEEK_SET);
	fwrite(&currentUser, sizeof(user), 1, f);
	fclose(f);
	
}



// returns 1 on success.
// returns -1 on fail.
int saveuser(user u){
	FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
	fseek(f, (3 * sizeof(int))+(noOfusers*sizeof(user)), SEEK_SET);
	fwrite(&u, sizeof(user), 1, f);
	// check return values of fwrite!
	fclose(f);
	return 1;
}



// returns 1 on success.
//returns -1 on success.
user createUser(int *newuser,char* userName,char* pwd){
	user u;
	strcpy(u.userName, userName);
	strcpy(u.password,pwd);
	// index of users start with 0.
	u.userid = noOfusers;
	printf("your user id is %d",u.userid);
	
	for (int i = 0; i < 50; i++){
		u.userPostOffsets[i] = -1;//
	}

	// saving user.
	if (saveuser(u)<0){
		printf("\ncannot creeate user!\n");
		exit(0);
	}
	// user sucessfully created.
	noOfusers++;
	// updating no. of users


	

	return u;
}

post* checkInFreeList(){
	return NULL;// returns -1 if no freespace available in btween posts.
}
void writePostintoDatabase(FILE* fdest, FILE* fsrc,post p){
	char* buff = (char*)malloc(sizeof(char)*p.size);
	int n=fread(buff, sizeof(char),p.size , fsrc);
	buff[n] = '\0';
	fwrite(buff, sizeof(char),n, fdest);
	//free(buff);
}
/*
void createFreeList(){

}

void updateFreeList(post duummy){

}
*/
int storePost(post* p){
	post* delPost = checkInFreeList();
	int offset=0;// checks free space from the deleted posts nd returns offet.
	if (!delPost){
		offset = Offset;
		offset = offset - (sizeof(post) + p->size);
		Offset = offset;// updating OFFSET.
		/*
		if req. we need to post the updates into file at this point only.
		
		*/
		p->postStartOffset = offset + sizeof(post);
		p->postEndOffset = p->postStartOffset + p->size;
		FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
		fseek(f, offset, SEEK_SET);
		fwrite(p, sizeof(post), 1, f);
		FILE* fpost = fopen(p->filename, "rb");
		if (!fpost){
			printf("\ncannot oopen post\n");
			return -1;
		}
		writePostintoDatabase(f, fpost, *p);
		fclose(f);
		fclose(fpost);
		return 1;
		//fwrite(&p, sizeof(post), 1, f);
	}
	else{
		offset = (delPost->postStartOffset) - sizeof(post);
		p->postStartOffset = offset + sizeof(post);
		p->postEndOffset = p->postStartOffset + p->size;
		FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
		fseek(f, offset, SEEK_SET);
		fwrite(p, sizeof(post), 1, f);
		FILE* fpost = fopen(p->filename, "rb");
		writePostintoDatabase(f, fpost, *p);
		post dummy;
		strcpy(dummy.filename,"");
		dummy.likes = -1;
		dummy.postStartOffset = p->postEndOffset + sizeof(post);
		dummy.postEndOffset =delPost->postEndOffset ;
		dummy.size = delPost->size-p->size-sizeof(post);
		dummy.userid = -1;
		fwrite(&dummy, sizeof(post), 1, f);
		// update freelist
	//	updateFreeList(dummy);
		fclose(f);
		fclose(fpost);
		return 1;
		
	}

}
int createpost(int uid){
	printf("\nenter the post address\n");
	char filename[20];
	// file name with extension is max of 20.
	scanf("%s", filename);
	post p;
	strcpy(p.filename, filename);
	p.likes = 0;
	p.size = fileSize(filename);
	if (p.size < 0){
		return -1;// file opening fail. 
	}
	
	for (int i = 0; i < 20; i++){
		p.comments[i].del = '0';
	}
	p.userid = uid;
	if (storePost(&p) < 0){
		printf("\ncouldn't store post\n");
		return -1;
	}
	// update userdata with the post
	int i = 0;
	// users post offset has the startoffset of the metadata which is the post struct
	for ( i = 0; i < 50; i++){
		if (currentUser.userPostOffsets[i] < 0){
			currentUser.userPostOffsets[i] = p.postStartOffset - sizeof(post);
			break;
		}
	}
	return 1;
	
}