#include<iostream>
#include<string>
#include<fstream>
#include<math.h>
#include<pthread.h>

using namespace std;

//
void initial_pruning_table();
int base6(int);
pthread_mutex_t mutex;
size_t size = 0x10000;
pthread_attr_t tattr;
string ShortestPath = "                     ";
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


void *Solve(void * arg){
    int steps = (int)arg;
    int temp = steps;
    //cout<<steps<<endl;
    string log="";
    for(int i=0;i<8;i++){
        switch(temp%10){
            case 1: log+="R ";break;
            case 2: log+="R'";break;
            case 3: log+="U ";break;
            case 4: log+="U'";break;
            case 5: log+="B ";break;
            case 6: log+="B'";break;
        }
        temp/=10;
        if(temp==0)    break;
    }
    //cout<<log<<endl;
    if(log.length()>=ShortestPath.length()) pthread_exit(0);              //Get out
    char **cube = new char*[6];
    for(int i=0;i<6;i++){
        cube[i] = new char[4];
    }
    for(int i=0;i<6;i++){
        for(int j=0;j<4;j++){
            cube[i][j] = origin_cube[i][j];
        }
    }

    for(int i=0;i<log.length();i+=2){                           // rotate cube
        string input = log.substr(i,2);
        if(input!="x"){

			if(input[0]=='U'){
				if(input[1]=='\'')Rotate_U(cube,1);
				else if(input[1]=='2'){Rotate_U(cube,0);Rotate_U(cube,0);}
                else Rotate_U(cube,0);
			}
			else if(input[0]=='R'){
				if(input[1]=='\'')Rotate_R(cube,1);
				else if(input[1]=='2'){Rotate_R(cube,0);Rotate_R(cube,0);}
                else Rotate_R(cube,0);
			}
			else if(input[0]=='B'){
				if(input[1]=='\'')Rotate_B(cube,1);
				else if(input[1]=='2'){Rotate_B(cube,0);Rotate_B(cube,0);}
                else Rotate_B(cube,0);
			}
			else{
				printf("Error\n");
				break;}
        }
    }
    if( verification(cube) ){                               // Solve Complete
        //cout<<log.length()<<" "<<ShortestPath.length()<<endl;
        pthread_mutex_lock(&mutex);
        if(log.length()<ShortestPath.length()){
                ShortestPath = log;  // Update ShortestPath
        }
        pthread_mutex_unlock(&mutex);
        for(int i=0;i<6;i++)
            delete[] cube[i];
        delete[] cube;                                        // free memory resource
        pthread_exit(0);
    }
    else{
        string status="";
        for(int i=0;i<6;i++){
            for(int j=0;j<4;j++){
                status += cube[i][j];
            }
        }
        for(int i=0;i<6;i++)
            delete[] cube[i];
        delete[] cube;
        if(log.length()==16){
            //cout<<log.length()<<endl;                               // depth 8
            for(int i=0;i<=6;i++){
                ifstream fin;
                string filename = "Prune_Table"+to_string(i)+".txt";
                //cout<<filename<<endl;
                fin.open(filename);
                string input;
                while(getline(fin,input)){
                    if(input.substr(12,24)==status){                            // find
                        log += input.substr(0,input.find("  "));
                        pthread_mutex_lock(&mutex);
                        if(log.length()<ShortestPath.length())   ShortestPath = log;  // Update ShortestPath
                        pthread_mutex_unlock(&mutex);
                        fin.close();
                        pthread_exit(0);
                    }
                }
                fin.close();
            }
            pthread_exit(0);                                                 // not found
        }
        else{                       // depth 1~7

            pthread_t tid[6];
            bool busy[6] ={false,false,false,false,false,false};
            /*for(int i=0;i<6;i++){
                cout<<busy[i];
            }*/
            int arg=0,temp=steps;
            int count =0;
            do{count++;}while(temp/=10);
            for(int i=0;i<6;i++){
                arg = steps+(i+1)*pow(count,10);
                //cout<<arg<<endl;
                int len = log.length();
                if(len>=4){
                    if(Rtable[i]==log.substr(len-2,2) && Rtable[i]==log.substr(len-4,2)) continue;    //3 similar
                    else if (Rtable[i]==counter(log.substr(len-2,2)))   continue;                   // counterwise
                }
                //cout<<i<<" "<<arg<<endl;
                pthread_create(&tid[i],&tattr,Solve,(void*)arg);
                busy[i]=true;
            }
            for(int i=0;i<=6;i++){
                if(busy[i]==true) {
                        //cout<<i<<" join"<<endl;
                        pthread_join(tid[i],NULL);
                        //cout<<i<<" release"<<endl;
                }
            }
            pthread_exit(0);
        }
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

	cout<<"Enter Serial Rotate(enter to run)： ";
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
    pthread_t tid;
    int arg = 0;
    pthread_mutex_init(&mutex,NULL);

    if (pthread_attr_init(&tattr) != 0) {
    fprintf(stderr, "pthread_attr_init() failed\n");
    }
    if (pthread_attr_setstacksize(&tattr,size) != 0) {
    fprintf(stderr, "pthread_attr_stacksize() failed\n");
    }
    pthread_create(&tid,&tattr,Solve,(void*)arg);
    pthread_join(tid,NULL);
    cout<<ShortestPath<<endl;
	//cout<<verification(origin_cube);

	//cout<< base6(155555);

//用main遞迴創建pthread找最短解法

//pthread 每步找出所有可能轉法，並再開thread下去找最短解法


	//system("pause");
    return 0;
}
