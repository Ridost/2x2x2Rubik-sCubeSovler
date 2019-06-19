#include<iostream>
#include<string>
#include<fstream>
#include<math.h>
#include<queue>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>


using namespace std;

//
void initial_pruning_table();
int base6(int);
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

string ShortestPath = "                                       ";
string Rtable[6] = {"R ","R'","U ","U'","B ","B'"};
void Rotate_U(char *cube[],bool direction);
void Rotate_R(char *cube[],bool direction);
void Rotate_B(char *cube[],bool direction);



char **origin_cube;


/*
	order = U->L->F->R->B->D
	0=Yellow;
	1=Blue;
	2=Red;
	3=Green;
	4=Orange;
	5=White;

              |---------|
              |  U0 U1  |
              |  U2 U3  |
     |--------| ------- |---------|---------|
     | L0 L1  |  F0 F1  |  R0 R1  |  B0 B1  |
     | L2 L3  |  F2 F3  |  R2 R3  |  B2 B3  |
     |--------|---------|---------|---------|
              |  D0 D1  |
              |  D2 D3  |
              |---------|

	Rotate_U : B->R->F->L->B (Layer 0 1)
	Rotate_U': B<-R<-F<-L<-B (Layer 0 1)

	Rotate_R : U->B(0&2)->D->F->B(0&2) (Layer 1 3)
    Rotate_R': U<-B(0&2)<-D<-F<-B(0&2) (Layer 1 3)
*/
/*
char counter(char num){
	switch (num){
		case '0':	num = '1'; break;
		case '1':	num = '0'; break;
		case '2':	num = '3'; break;
		case '3':	num = '2'; break;
		case '4':	num = '5'; break;
		case '5':	num = '4'; break;
	}
	return num;
}*/
string counter(string num){
       if(num=="R ") num = "R'";
        else if(num=="R'") num = "R ";
        else if(num=="U ") num = "U'";
        else if(num=="U'") num = "U ";
        else if(num=="B ") num = "B'";
        else if(num=="B'") num = "B ";

    return num;
}
void Rotate_U(char *cube[],bool direction)
{
	//dic=0->clockwise dic=1->counter-clockwise
	char temp_0=0,temp_1=0;
	int i,j;

	if(!direction){
		//swap | L0 L1  |  F0 F1  |  R0 R1  |  B0 B1  | ==>line
		temp_0=cube[1][0];
		temp_1=cube[1][1];
		for(i=1;i<=3;i++){
			cube[i][0]=cube[i+1][0];
			cube[i][1]=cube[i+1][1];
		}
		cube[4][0]=temp_0;
		cube[4][1]=temp_1;
		temp_0=cube[0][0];
		cube[0][0]=cube[0][2];
		cube[0][2]=cube[0][3];
		cube[0][3]=cube[0][1];
		cube[0][1]=temp_0;
	}
	else{

		temp_0=cube[4][0];
		temp_1=cube[4][1];
		for(i=3;i>=1;i--){
			cube[i+1][0]=cube[i][0];
			cube[i+1][1]=cube[i][1];
		}
		cube[1][0]=temp_0;
		cube[1][1]=temp_1;

		temp_0=cube[0][0];
		cube[0][0]=cube[0][1];
		cube[0][1]=cube[0][3];
		cube[0][3]=cube[0][2];
		cube[0][2]=temp_0;
	}
}

void Rotate_R(char *cube[],bool direction)
{
	//dic=0->clockwise dic=1->counter-clockwise
	char temp_0=0,temp_1=0;
	int i,j;

	if(!direction){
		temp_0=cube[0][1];
		temp_1=cube[0][3];

		cube[0][1]=cube[2][1];
		cube[0][3]=cube[2][3];

		cube[2][1]=cube[5][1];
		cube[2][3]=cube[5][3];

		cube[5][1]=cube[4][2];
		cube[5][3]=cube[4][0];

		cube[4][2]=temp_0;
		cube[4][0]=temp_1;

		temp_0=cube[3][0];
		cube[3][0]=cube[3][2];
		cube[3][2]=cube[3][3];
		cube[3][3]=cube[3][1];
		cube[3][1]=temp_0;
	}
	else{
		temp_0=cube[0][1];
		temp_1=cube[0][3];

		cube[0][1]=cube[4][2];
		cube[0][3]=cube[4][0];

		cube[4][2]=cube[5][1];
		cube[4][0]=cube[5][3];

		cube[5][1]=cube[2][1];
		cube[5][3]=cube[2][3];

		cube[2][1]=temp_0;
		cube[2][3]=temp_1;

		temp_0=cube[3][0];
		cube[3][0]=cube[3][1];
		cube[3][1]=cube[3][3];
		cube[3][3]=cube[3][2];
		cube[3][2]=temp_0;
	}
}

void Rotate_B(char *cube[],bool direction)
{
	//dic=0->clockwise dic=1->counter-clockwise
	char temp_0=0,temp_1=0;
	int i,j;

	if(!direction){
		temp_0=cube[0][0];
		temp_1=cube[0][1];

		cube[0][0]=cube[3][1];
		cube[0][1]=cube[3][3];

		cube[3][1]=cube[5][3];
		cube[3][3]=cube[5][2];

		cube[5][3]=cube[1][2];
		cube[5][2]=cube[1][0];

		cube[1][2]=temp_0;
		cube[1][0]=temp_1;

		temp_0=cube[4][0];
		cube[4][0]=cube[4][2];
		cube[4][2]=cube[4][3];
		cube[4][3]=cube[4][1];
		cube[4][1]=temp_0;
	}
	else{
		temp_0=cube[0][0];
		temp_1=cube[0][1];

		cube[0][0]=cube[1][2];
		cube[0][1]=cube[1][0];

		cube[1][2]=cube[5][3];
		cube[1][0]=cube[5][2];

		cube[5][3]=cube[3][1];
		cube[5][2]=cube[3][3];

		cube[3][1]=temp_0;
		cube[3][3]=temp_1;

		temp_0=cube[4][0];
		cube[4][0]=cube[4][1];
		cube[4][1]=cube[4][3];
		cube[4][3]=cube[4][2];
		cube[4][2]=temp_0;
	}
}

void printcube(char *cube[])
{
	int i,j;
	printf("     %c %c \n",cube[0][0],cube[0][1]);
	printf("     %c %c \n",cube[0][2],cube[0][3]);

	for(i=1;i<5;i++){
		for(j=0;j<=1;j++)
		{
			printf(" %c",cube[i][j]);
		}
	}
	printf("\n");
	for(i=1;i<5;i++){
		for(j=2;j<=3;j++)
		{
			printf(" %c",cube[i][j]);
		}
	}
	printf("\n");
	printf("     %c %c \n",cube[5][0],cube[5][1]);
	printf("     %c %c \n\n",cube[5][2],cube[5][3]);
}

bool verification(char *temp_cube[])
{
	bool ver=false;
	if (temp_cube[0][0]==temp_cube[0][1] && temp_cube[0][0]==temp_cube[0][2] && temp_cube[0][0]==temp_cube[0][3])
		if (temp_cube[1][0]==temp_cube[1][1] && temp_cube[1][0]==temp_cube[1][2] && temp_cube[1][0]==temp_cube[1][3])
			if (temp_cube[2][0]==temp_cube[2][1] && temp_cube[2][0]==temp_cube[2][2] && temp_cube[2][0]==temp_cube[2][3])
				ver=true;
	return ver;
}


void Solve(){
	//cout<<(long long)arg<<endl;

	char **t_cube;						//each thread own one cube
	t_cube = new char*[6];				//initnail thread cube
	for(int i=0;i<6;i++){
        t_cube[i] = new char[4];
    }
    for(int i=0;i<6;i++){
        for(int j=0;j<4;j++){
            t_cube[i][j] = origin_cube[i][j];
        }
    }

	if (verification(t_cube)) {ShortestPath="";}

	queue <string>path;	
    for(int i=0;i<6;i++)								//the solution optional queue
	    path.push(Rtable[i]);
	while (1)
	{
		string tans=path.front();						//current solution
		//cout<<tans<<endl;
		if(ShortestPath.length() <= tans.length() )		// length > current shortest path =>break
			break;		

		for(int i=0;i<tans.length();i+=2)
		{
			if(tans[i]=='R'){
				if(tans[i+1]=='\'')Rotate_R(t_cube,1);
				else Rotate_R(t_cube,0);
			}
			else if(tans[i]=='U'){
				if(tans[i+1]=='\'')Rotate_U(t_cube,1);
				else Rotate_U(t_cube,0);
			}
			else if(tans[i]=='B'){
				if(tans[i+1]=='\'') Rotate_B(t_cube,1);
				else Rotate_B(t_cube,0);
			}
		}
		
		if(verification(t_cube)) {	
			if(ShortestPath.length()>tans.length()){
				ShortestPath=tans;
				printcube(t_cube);
			}
            break;
		}
		else
		{
			for(int i=0;i<6;i++){
        		for(int j=0;j<4;j++){
            	t_cube[i][j] = origin_cube[i][j];
        		}
    		}
		}
		

		for(int i=0;i<6;i++)
		{	
			//cout<<"i="<<i<<endl;
			//cout<<"Sub:"<<tans.substr(tans.length()-2,2)<<endl;

			if (tans.substr(tans.length()-2,2)==counter(Rtable[i]))
				continue;
			if (tans.length()!=2){
			 	if( (tans.substr(tans.length()-4,2)==tans.substr(tans.length()-2,2)) && 
				 	((tans.substr(tans.length()-2,2))==Rtable[i]) )
				{
					continue;
				}
			}
			//cout<<"Before:"<<tans<<endl;
			tans+=Rtable[i];
			//cout<<"After:"<<tans<<endl;
			path.push(tans);
			tans=path.front();
		}	
		path.pop();
	}	
}

int main(){

    origin_cube = new char*[6];
    for(int i=0;i<6;i++){
        origin_cube[i] = new char[4];
    }
    for(int i=0;i<6;i++){
        for(int j=0;j<4;j++){
            origin_cube[i][j] = i+48;
        }
    }
	string inputstr;
	int count=0;
	//printcube(origin_cube);

	cout<<"Enter Serial Rotate(enter to run) : ";
	getline(cin,inputstr);
	while(1)
	{
		int x = inputstr.find(' ');
		if (x==-1)	count++;
		if(count==2) break;
		string input = inputstr.substr(0,x);
		inputstr = inputstr.substr(x+1);
		if(input!="x"){

			if(input[0]=='U'){
				if(input.length()==1) Rotate_U(origin_cube,0);
				else if(input[1]=='\'')Rotate_U(origin_cube,1);
				else if(input[1]=='2'){Rotate_U(origin_cube,0);Rotate_U(origin_cube,0);}

			}
			else if(input[0]=='R'){
				if (input.length()==1) Rotate_R(origin_cube,0);
				else if(input[1]=='\'')Rotate_R(origin_cube,1);
				else if(input[1]=='2'){Rotate_R(origin_cube,0);Rotate_R(origin_cube,0);}

			}
			else if(input[0]=='B'){
				if (input.length()==1) Rotate_B(origin_cube,0);
				else if(input[1]=='\'')Rotate_B(origin_cube,1);
				else if(input[1]=='2'){Rotate_B(origin_cube,0);Rotate_B(origin_cube,0);}

			}
			else{
				printf("Error\n");
				break;}
		}
		else break;

	}

	printcube(origin_cube);

    double start_utime,end_utime;
    struct timeval tv, tv2;
    gettimeofday(&tv,NULL);  
    start_utime = tv.tv_sec * 1000000 + tv.tv_usec;

    Solve();


    if(ShortestPath.length()>0){
		cout<<"Finish! Find "<<ShortestPath.length()/2<<" steps solution"<<endl;
    	cout<<"Solve step: "<<ShortestPath<<endl;
	}
	else{
		cout<<"You don't need this solver!!"<<endl;
	}
	gettimeofday(&tv2,NULL);  
	end_utime = tv2.tv_sec * 1000000 + tv2.tv_usec;  
	printf("Parallel Execution Time=  = %f(s)\n", (end_utime - start_utime)/1000000);

	//system("pause");
    return 0;
}
