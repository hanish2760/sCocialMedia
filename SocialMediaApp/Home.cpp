#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"userModules.h"

/*
 started with creating file of size 100 mb ie our database. with last character as " ".
 
 FILE* f = fopen("SocialMediaDatabase.txt", "wb");
 fseek(f, (1024 * 1024 * 100) - 1, SEEK_SET);
 fwrite(" ", sizeof(char), 1, f);
 fclose(f);


 then initialise no.of users to 0 in the first 4 bytes.
 then initialised no. of posts to 0 aswell.
 then initialise the offset os the post.....

 FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
 int a = 0;
 fwrite(&a, sizeof(int), 1, f);
 fwrite(&a, sizeof(int), 1, f);
 fseek(f,0,SEEK_END);
 int offset=f
 fwrite(
 fclose(f);


 to over wrrite file pen in rb+ mode.
 */
/* 


*/
void startPageUI(){
	for (int i = 0; i < 25; i++)printf("~");
	printf("  SOCIAL MEDIA APP  \n");
	for (int i = 0; i < 25; i++)printf("~");
	printf("\n");
	for (int i = 0; i < 5; i++)printf("\t");
	printf("ENTER UID : ");
	int id = 0;
	fflush(stdin);

	scanf("%d", &id);
	getGlobals(id);

}
void displaypost(post p){

	char* buff = (char*)malloc((sizeof(char)*(p.size)+1));
	FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
	fseek(f, p.postStartOffset, SEEK_SET);
	fread(buff, sizeof(buff), 1, f);
	printf("\n");
	for (int i = 0; i < 25; i++)printf("_");
	printf("\n\n");
	printf("%s", buff);
	printf("\n\n");
	for (int i = 0; i < 25; i++)printf("_");

	fclose(f);

}

void displayoptions(){
	int c = 0;
	while (c != 6){
		printf("1.\n2.\n3.\n4.\n5.");
	}
}
void  showPosts(){
	
	post p;
	int i = 0;
	FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
	int ofset;
	while (true){
		fread(&p, sizeof(post), 1, f);
		if (strcmp(p.filename, "") || p.userid < 0){
			ofset = p.postEndOffset;
			fseek(f, ofset,SEEK_SET);
			continue;
		}
		displaypost(p);
		displayoptions();
		// check for the condittion when there are no posts.
		ofset = p.postEndOffset;
		fseek(f, ofset, SEEK_SET);
		fread(&p, sizeof(p), 1, f);
	}

	
}
void postfunc(post* p){
	int indx = 0;
	
	char filename[20];
	FILE* fpp, *fpp2,*fhk;
	char* buff;
	//may be display post
	int ch = 0;
	while (ch != 6){
		system("cls");
		printf("POST of %d", p->userid);
		printf("\nFILE NAME %s\n", p->filename);
		printf("\nno of likes : %d \n\n", p->likes);
		printf("\nComments :\n");
		for (int i = 0; i < 20; i++){
			if (p->comments[i].del == '0'){
				continue;  
			}
			else{
				printf("Index [ %d ] userid: %d comment-> %s\n", i + 1, p->comments[i].userid, p->comments[i].comment);
			}
		}
		printf("\n\n\n1)like  2)add comment  3) back ");
		if (p->userid == currentUser.userid){
			printf("4) Download  5)Delete comment  6) Delete-post\n");
		}
		scanf("%d",&ch);
		switch (ch){
		case 1:
			p->likes++;
		    fpp = fopen("SocialMediaDatabase.txt", "rb+");
			fseek(fpp, p->postStartOffset - sizeof(post), SEEK_SET);
			fwrite(p, sizeof(post), 1, fpp);
			fclose(fpp);
			// increment like and update  in db
			break;
		case 2:
			char comm[50];
			printf("Enter ur comment\n-> \n");
			fflush(stdin);
			scanf("%[^\n]s", comm);
			fflush(stdin);
			for (int i = 0; i < 20; i++){
				if (p->comments[i].del == '0'){
					strcpy(p->comments[i].comment, comm);
					p->comments[i].userid = currentUser.userid;
					p->comments[i].del = 't';
					fpp = fopen("SocialMediaDatabase.txt", "rb+");
					fseek(fpp, p->postStartOffset - sizeof(post), SEEK_SET);
					fwrite(p, sizeof(post), 1, fpp);
					fclose(fpp);
					break;
				}
			}
			printf("\ncannot add\n");
			// add comment to the p and store it in the database. ie, update in the database.
			break;

		case 3:
			return;
			break;
		case 4:
			if (p->userid == currentUser.userid){
				// store to local disk.
				printf("Enter file name\n");
				scanf("%s", filename);
				fhk = fopen(filename,"w");
				if (!fhk){
					printf("\ncannot open file\n");
					break;
				}
				buff = (char*)malloc(sizeof(char)*(p->size));
				fpp2 = fopen("SocialMediaDatabase.txt", "rb+");
				fseek(fpp2, p->postStartOffset, SEEK_SET);
				int n=fread(buff, sizeof(char),p->size, fpp2);
				buff[n] = '\0';
				
				fwrite(buff,sizeof(char),n, fhk);
//				free(buff);
				fclose(fhk);
				fclose(fpp2);
			}
			break;
		case 5:
			if (p->userid == currentUser.userid){
	//  delete comment and update  in the user and as well as the database.
				while (true){
					printf("\nEnter index to delete comment\n");
					indx = 0;
					fflush(stdin);
					scanf("%d", &indx);
					if (p->comments[indx-1].comment[0] == '0')continue;
					p->comments[indx-1].del = '0';
					fpp2 = fopen("SocialMediaDatabase.txt", "rb+");
					fseek(fpp2, p->postStartOffset - sizeof(post), SEEK_SET);
					fwrite(p, sizeof(post), 1, fpp2);
					fclose(fpp2);
					break;
				}
			}
			break;
		case 6:
			if (p->userid == currentUser.userid){
				//  delete post andd update  in the user and as well as the database.
				p->userid = -1;
				strcpy(p->filename ,"");
				fpp2 = fopen("SocialMediaDatabase.txt", "rb+");
				fseek(fpp2, p->postStartOffset - sizeof(post), SEEK_SET);
				fwrite(p, sizeof(post), 1, fpp2);

			}
			break;
		default  :
			break;
		}
	}
}
void viewPosts(int id){
	FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
	if (!f){
		printf("\ncannot open DataBase file\n");
		return;
	}
	fseek(f, Offset, SEEK_SET);
	int i = 0;
	post* p=(post*)malloc(sizeof(post)*noOfPosts);
	int j = 1;
	int tempcount = 0;
	printf("\nUSER POSTS\n");
	while (tempcount < noOfPosts){
		post temppost;// = (post*)malloc(sizeof(post));
//		fread(&p[i], sizeof(post), 1, f);
		fread(&temppost, sizeof(post), 1, f);

		if (strcmp(temppost.filename, "")==0  || temppost.userid < 0){ 
			fseek(f, temppost.postEndOffset, SEEK_SET);
			//getting into infinite loop here.
			continue;
		}
		if (id == temppost.userid){

			p[i] = temppost;
			printf("\n%d) %s", j++, temppost.filename);
			i++;

		}
		fseek(f, temppost.postEndOffset, SEEK_SET);
		//display post??
		tempcount++;
	}
	if (j == 1){
		printf("\nNo postss Yet !\n"); 
		getchar();
		fflush(stdin);
		return;
	}
	int ch = 0;
	int pno = 0;
	while(ch != 2){
		system("cls");
		int tempcount2 = 0;
		int postsno = 1;
		while (tempcount2 < i){

			if (id == p[tempcount2].userid){
				printf("\n%d) %s", postsno++, p[tempcount2].filename);
			}
			tempcount2++;
		}
		printf("\n\n\n1)Enter  post  2)Back\n");
		fflush(stdin);
		scanf("%d", &ch);
		switch (ch){
		case 1:
			printf("\n\n\nEnter post number\n");
			scanf("%d", &pno);
			//do validation here!
			if (pno<0 || pno>postsno - 1){
				printf("enter valid post numer");
				break;
			}
			postfunc(&p[pno - 1]);
			break;
		case 2:
			fclose(f);
			free(p);
			return;
			break;

		}
	}
	fclose(f);

}
void listOfUsers(){
	FILE* f = fopen("SocialMediaDatabase.txt", "rb+");
	if (!f){
		printf("\ncannot open file\n");
		return;
	}
	fseek(f, 3 * sizeof(int), SEEK_SET);
	user u;
	/*system("cls");
	for (int i = 0; i < noOfusers; i++){
		fread(&u, sizeof(user), 1, f);
		printf("\n\n%d)%s",i+1 ,u.userName);
	}*/
	int ch = 0;
	int id = 0;
	printf("\n");
	for (int i = 0; i < 25; i++)printf("_");
	while (ch != 2){
		system("cls");
		fseek(f, 3 * sizeof(int), SEEK_SET);
for (int i = 0; i < noOfusers; i++){
			fread(&u, sizeof(user), 1, f);
			printf("\n\n%d)%s", i + 1, u.userName);
		}
		printf("\n");
		printf("\n\n\n1) view posts\n2)Back \n");
		fflush(stdin);
		scanf("%d", &ch);
		switch (ch)
		{
		case 1:
			printf("\n\nEnter user id : ");
			scanf("%d", &id);
			viewPosts(id-1);
			break;
		case 2:
			fclose(f);
			return;
			break;
		default:
			break;
		}
	}
	fclose(f);
}

void afterlogin(int id){
	
	int ch = 0;
	while (ch != 3){

		printf("\n\n\nENTER choice:1)Add Post 2)List of users 3)exit\n");
		fflush(stdin);
		scanf("%d", &ch);
		switch (ch){
		case 1:
			createpost(id);
			noOfPosts++;
			break;
		case 2:			
			listOfUsers();
			break;
		case 3:
			setGlobals();
			exit(1);
			break;
		default:
			break;
		}


	}
}
void login(){
	int id = 0;
	printf("\nenter user id\n");
	scanf("%d", &id);
	getGlobals(id);
	if (id >= noOfusers){
		printf("~~~~~~~~invalid  uid~~~~~~");
		return;
	}
	printf("\nEnter password :\n");
	fflush(stdin);
	
	char password[20];
	scanf("%s", password);
	if (strcmp(password, currentUser.password) == 0){
		afterlogin(id);
	}

}
void signup(){
	char name[20];
	printf("Enter user name\n");
	scanf("%s", name);
	printf("Enter password\n");
	char pwd[20];
	fflush(stdin);
	scanf("%s", pwd);
	//user *u = (user*)malloc(sizeof(user));
	int id = 0;
	getGlobals(id);
	user u=createUser(&id, name,pwd);
	// here in the above function i am using the noofusers global variale before assigning it in the below function.
	//getGlobals(id);
	currentUser.userid = u.userid;
	strcpy(currentUser.password , pwd);

	strcpy(currentUser.userName, name);

	strcmp(currentUser.userName ,u.userName);
	for (int i = 0; i < 50;i++)
	currentUser.userPostOffsets[i] = u.userPostOffsets[i];
	//i was already updating the number of users in the create user function.
	
	//noOfusers++;

	afterlogin(id);
}
int main(){
	/* call preInitialisationTasks() at the first time running of the application . do something for that like reading one int from last*/
	//preInitialisationTasks();
//	startPageUI();
	//showPosts();
	int ch = 0;
	while (ch != 3){
		printf("1)Login 2) signup 3) Exit\n");
		fflush(stdin);
		scanf("%d", &ch);
		switch (ch){
		case 1:
			login();
			break;
		case 2:
			signup();
			break;
		default:
			break;
		}


	}
	system("pause");

}