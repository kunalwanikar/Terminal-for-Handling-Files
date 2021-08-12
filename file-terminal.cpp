/*  Implemented the complete program in LINUX using C++14 compiler
	To execute the code on linux command line, enter the below commands sequentially
	1) g++ -o sol file-terminal.cpp
	2) ./sol
*/

#include<iostream>
#include<bits/stdc++.h>
#include <sstream>
#include<string>
using namespace std;

//structure of inode which contain all info about particular inode
struct inode
{
	char name[8];
	int size;
	char *blockpointers[8];
	int used;
};

//superblock clss which will contain all the fuction which can be applied
class superblock
{
	public:
		inode inode_list[16];	
		bool free_block_list[128];
		bool free_block_list2[128];
	//constructor which will intialize the free block list and inode	
	superblock()
	{
		//initialise the free_block_list
		free_block_list[0]=1;
		for(int i=1;i<128;i++)
		{
			free_block_list[i]=0;
		}
		free_block_list2[0]=1;
		for(int i=1;i<128;i++)
		{
			free_block_list2[i]=0;
		}
		//initialize the inode	
		for(int i=0;i<16;i++)
		{
			for(int j=0;j<8;j++)
			{
				inode_list[i].name[j]=' ';
			}
			
			inode_list[i].size=0;
			
			for(int j=0;j<8;j++)
			{
				inode_list[i].blockpointers[j]=0;
			}
			
			inode_list[i].used=0;
		}
	}	
	
	//create fuction will create a file which name will be less then 9
	void create(char name_passed[],int size_passed)
	{
		for(int i=0;i<16;i++)
		{
			int flag=0;
			//This doesnt allow to create file duplicate name
			for(int j=0;j<8;j++)
			{
				if(inode_list[i].name[j]==(name_passed[j]))
				{
					flag=0;
				}
				else
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
			{
				cout<<"File with duplicate name cannot be created "<<endl;
				return;
			}
		}
		for(int i=0;i<16;i++)
		{
			//Size of last block cant be greater then 7
			if(i==15&&size_passed==8)
			{
				cout<<"Size of last block cannot be greater then 7"<<endl;
				return;
			}
			//If all 16 files exist then 17th files cant be created
			if(inode_list[i].used==1)
			{
				if(i==15)
				{
					cout<<"All the files are already created "<<endl;
					return;
				}
			}
			else
			{
				//This will set the free block list with required size passed by user
				for(int k=0;k<size_passed;k++)
				{
					free_block_list[i*8+k+1]=1;
				}
				for(int j=0;j<8;j++)
				{
					inode_list[i].name[j]=name_passed[j];
				}
				//It will create a block of size 1024B and assign it to blockpointer
				for(int j=0;j<size_passed;j++)
				{
					inode_list[i].blockpointers[j]=new char[1024];
				}
				inode_list[i].size=size_passed;
				inode_list[i].used=1;	
				break;
			}
		}
	}
	
	//It delete the file with the name passed by user
	void delete_file(char name_passed[8])
	{
		for(int i=0;i<16;i++)
		{
			//This will check whether the file with that name exist or not
			int flag=0;
			for(int j=0;j<8;j++)
			{
				if(inode_list[i].name[j]==name_passed[j])
				{
					flag=0;
				}
				else
				{
					flag=1;
					break;
				}
			}
			//this will reset the free block list and the block pointer
			if(flag==0)
			{
				for(int j=0;j<8;j++)
				{
					inode_list[i].name[j]=' ';
				}
				//reset the free_block_list
				for(int k=0;k<inode_list[i].size;k++)
				{
					free_block_list[i*8+k+1]=0;	
				}
				for(int k=0;k<inode_list[i].size;k++)
				{
					free_block_list2[i*8+k+1]=0;	
				}
				inode_list[i].size=0;
				//reset the block pointer
				for(int j=0;j<8;j++)
				{
					inode_list[i].blockpointers[j]=NULL;
				}
				inode_list[i].used=0;
				return;
			}
		}
		cout<<"File with this name is not created--"<<endl;
	}
	
	//It is used to write the content passed by user in file
	void write(char name_passed[], int blockNum, string &buf)
	{
		for(int i=0;i<16;i++)
		{
			//It will check that the filename entered by user is valid or not
			int flag=0;
			for(int j=0;j<8;j++)
			{
				if(inode_list[i].name[j]==name_passed[j])
				{
					flag=0;
				}
				else
				{
					flag=1;
					break;
				}
			}
			//It will check whether valid block number is entered or not
			if(flag==0)
			{
				if(blockNum>=inode_list[i].size||blockNum<0)
				{
					cout<<"only "<<inode_list[i].size<<" block is created by user."<<"You can acces only 0 to "<<inode_list[i].size-1<<" blocks."<<endl;
					return;
				}
				//It will avoid overwriting of block
				if(free_block_list2[i*8+blockNum+1]==1)
				{
					cout<<"This block is already in use "<<endl;
					return;
				}
				free_block_list2[i*8+blockNum+1]=1;	
				char *pt=inode_list[i].blockpointers[blockNum];
				//It is used to store the data enter by user in the buffer
				for(int i=0;i<buf.length();i++)
				{
					(*(pt+i))=buf[i];
				}
				(*(pt+buf.length()))=NULL;
				return;
			}
		}
		cout<<"File with this name doesnot exist--"<<endl;
	}
	//It is used to read the data from the file stored in the particular block 
	bool read(char name_passed[], int blockNum, string &buf)
	{
		for(int i=0;i<16;i++)
		{
			//It will check that the filename entered by user is valid or not
			int flag=0;
			for(int j=0;j<8;j++)
			{
				if(inode_list[i].name[j]==name_passed[j])
				{
					flag=0;
				}
				else
				{
					flag=1;
					break;
				}
			}
			//It will read the data stored in the buffer
			if(flag==0)
			{
				//blockNum should be less then the number of block created by user
				if(blockNum>inode_list[i].size)
				{
					cout<<"only "<<inode_list[i].size<<" block is created by user."<<"You can acces only 0 to "<<inode_list[i].size-1<<" blocks."<<endl;
					return 0;
				}
				//used to check the file is empty or not
				if(free_block_list2[i*8+blockNum+1]==0)
				{
					cout<<"This file block is empty "<<endl;
					return 0;
				}
				int k=0;
				char *pt=inode_list[i].blockpointers[blockNum];
				buf.insert(0,pt);			
				return 1;
			}
		}
		cout<<"File with this name does not exist"<<endl;
		return 0;
	}
	//This fuction is used to show all the files created by user with the assigned size
	void list()
	{
		int flag=0;
		for(int i=0;i<16;i++)
		{
			if(inode_list[i].size!=0)
			{
				flag=1;
				cout<<"File Name:- ";
				for(int j=0;j<8;j++)
				{
					cout<<inode_list[i].name[j];
				}
				cout<<endl;
				cout<<"Size:- "<<inode_list[i].size<<endl;
			}
		}
		if(flag==0)
		cout<<"No file exist in the disk"<<endl;
	}
	//It is used to see the free block list
	void show()
	{
		for(int i=0;i<128;i++)
		cout<<free_block_list[i];
		cout<<endl;
	}
};
//Main Fuction
int main()
{
	superblock b;
	char buffer[1024];
	cout<<"_________________________________________________________"<<endl;
	cout<<"Type 0 to see the available choices:--"<<endl;
	cout<<"_________________________________________________________"<<endl;
	while(1)
	{	
		int choice;
		cout<<"Choice:- ";
		cin>>choice;
		string buf;
		string temp;
		int blockNum,res;
		switch(choice)
		{
			// use to see the available choices
			case 0:
					cout<<"Enter your choice :"<<endl;
					cout<<"1.Create a new file "<<endl;
					cout<<"2.Delete a file with a particular name "<<endl;
					cout<<"3.Read specified block from a file and store in a buffer"<<endl;
					cout<<"4.Write the data in the buffer to a specifed block in a file"<<endl;
					cout<<"5.List the name of all the file in the system and their size"<<endl;
					cout<<"6.To see the free block list"<<endl;
					cout<<"7.Exit"<<endl;
					break;
			// used to create the file 			
			case 1:
				char name[8];
				for(int i=0;i<8;i++)
				name[i]=' ';
				int size;
				cout<<"Enter the name of the file:";
				cin.ignore();
				getline(cin,temp);
				//file name should be less then equal to 8 character
				if(temp.size()>8)
				{
					cout<<"File name with greater then 8 char cannot be created--"<<endl;
					break;
				}
				for(int i=0;i<temp.length();i++)
				{
					name[i]=temp[i];
				}
				cout<<"Enter the size of file in blocks:";
				cin>>size;
				//block size should be less then 9
				if(size>8 || size<=0)
				{
					cout<<"Wrong size entered---"<<endl;
					break;
				}
				b.create(name,size);
				break;
			//used to delete the name of the file entered by user
			case 2:
				for(int i=0;i<8;i++)
				name[i]=' ';
				cout<<"Enter the name of the file to be deleted:";
				cin.ignore();
				getline(cin,temp);
				if(temp.size()>8)
				{
					cout<<"file name with greater then 8 char is entered--"<<endl;
					break;
				}
				for(int i=0;i<temp.length();i++)
					name[i]=temp[i];
				b.delete_file(name);
				break;
			//used to read the file	
			case 3:
				buf.clear();
				for(int i=0;i<8;i++)
				name[i]=' ';
				//enter the name of the file
				cout<<"Enter the name of the file: ";
				cin.ignore();
				getline(cin,temp);
				//file name greater then 8 is not allowed
				if(temp.size()>8)
				{
					cout<<"file name with greater then 8 char is entered--"<<endl;
					break;
				}
				for(int i=0;i<temp.length();i++)
					name[i]=temp[i];
				cout<<"Enter the block num(0-7) in which data is to be read: ";
				cin>>blockNum;
				//block number should be from 0-7
				if(blockNum>=8 || blockNum<0)
				{
					cout<<"Wrong blockNum entered---";
					break;
				}
				res=0;
				res=b.read(name, blockNum, buf);
				// display the content of buffer
				if(res==1)
				{
					cout<<"=======================Start file==========================="<<endl;
					for(int i=0;i<buf.length();i++)
					cout<<buf[i];
					cout<<endl;
					cout<<"========================End file============================="<<endl;					
				}
				break;
			//used to write the content to the file	
			case 4:
				buf.clear();
				for(int i=0;i<8;i++)
				name[i]=' ';
				cout<<"Enter the name of the file: ";
				cin.ignore();
				getline(cin,temp);
				//file name greater then 8 is not allowed
				if(temp.size()>8)
				{
					cout<<"file name with greater then 8 char is entered--"<<endl;
					break;
				}
				for(int i=0;i<temp.length();i++)
					name[i]=temp[i];
				//Input the blockNum	
				cout<<"Enter the block num(0-7) in which data is to be written:  ";
				cin>>blockNum;
				//blockNum should be from 0-7
				if(blockNum>=8 || blockNum<0)
				{
					cout<<"Wrong blockNum entered---"<<endl;
					break;
				}
				//Input the content of file
				cout<<"Enter the content of file: ";
				cin.ignore();
				getline(cin,buf);
				//content size sholud be less then 1024
				if(buf.size()>1024)
				{
					cout<<"Content is greater then block size";
					break;
				}
				b.write(name, blockNum, buf);
				break;
			//used to show the list of files that has been created		
			case 5:
				b.list();
				break;
			//use to show the free block list	
			case 6:
				cout<<"Free block list==";
				b.show();
				break;				
			//exit	
			case 7:
				exit(0);
			default:
				cout<<"Wrong choice entered"<<endl;	
				break;
		}
		cout<<"_________________________________________________________"<<endl;
		cout<<"_________________________________________________________"<<endl;
	}
	return 0;
}
