#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>



//size = 104
typedef struct
{
	int userid;        //id to uniquely identify users who commented on my post
	char del;            // del = 0 then comment deleted ,   del > 0 then it is comment size
	char comment[100];
}comment;

typedef struct
{
	//short postid;                //this id uniquely identifies the post
	int userid;  //this iunique;ly identifies a user
	int likes;    //no of likes for this post
	char filename[20];            //if filename == ""  then this post is deleted , else it consisits of filename
	int postStartOffset;         //this offset refers to the position of 
	int postEndOffset;
	int size;
	comment comments[20];
} post;


typedef struct
{
	char userName[20];
	int userid;
//	short noOfPosts;
	char password[20];
	int userPostOffsets[50]; 

//postoffset is -1 if there is no post.
// here we can have any no. of users but every user can post only 50 posts.
// users post offset has the startoffset of the metadata which is the post struct

}user;

user currentUser;
int noOfPosts, noOfusers, Offset;