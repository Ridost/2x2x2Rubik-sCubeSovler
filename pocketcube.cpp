#include<iostream>
#include<string>
#include<fstream>
#include<math.h>

using namespace std;

//
void initial_pruning_table();
int base6(int);
void Rotate_U(char cube[6][4],bool direction);
void Rotate_R(char cube[6][4],bool direction);
void Rotate_B(char cube[6][4],bool direction);



char origin_cube[6][4]={
	{'0','0','0','0'},
	{'1','1','1','1'},
	{'2','2','2','2'},
	{'3','3','3','3'},
	{'4','4','4','4'},
	{'5','5','5','5'}
};

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
}
void initial_pruning_table_five()
{
	string Rtable[6] = {"R ","R'","U ","U'","B ","B'"};
	int i=-1;
	ofstream fout;
	fout.open("Prune_table_five.txt");

	while(1){
		i=base6(i);
		if(i==55555)break;      // operation length
		string Rnum = to_string((long long)i);
		Rnum.insert(Rnum.begin(),5-Rnum.length(),'0');  // 補0到operation length
		//非連續三個重複
		if((Rnum[0]==Rnum[1] && Rnum[0]==Rnum[2]) || (Rnum[1]==Rnum[2] && Rnum[1]==Rnum[3]) ||
			(Rnum[2]==Rnum[3] && Rnum[2]==Rnum[4]))    //判斷到 Rnum[operation length]
			continue;
		//非正逆
		bool counterwise = 0;
		for(int i2=0;i2<4;i2++){    //判斷到 operation length-1
			if(Rnum[i2]==counter(Rnum[i2+1]))	counterwise = 1;
		}
		if(counterwise==1)	continue;

		string ch;
		for(int j=0;j<5;j++){       //到 operation length
			ch=Rtable[(Rnum[j])-48];
			fout<<ch;
		}
		fout<<"  ";     //補空格到 12 char
		char t_cube[6][4]={
			{'0','0','0','0'},
			{'1','1','1','1'},
			{'2','2','2','2'},
			{'3','3','3','3'},
			{'4','4','4','4'},
			{'5','5','5','5'}
		};
		for(int j=4;j>=0;j--)
		{
			string input;
			Rnum[j]=counter(Rnum[j]);
			input=Rtable[Rnum[j]-48];
			if(input!="x"){

			if(input[0]=='U'){
				if(input[1]=='\'')Rotate_U(t_cube,1);
				else if(input[1]=='2'){Rotate_U(t_cube,0);Rotate_U(t_cube,0);}
				else Rotate_U(t_cube,0);
			}
			else if(input[0]=='R'){
				if(input[1]=='\'')Rotate_R(t_cube,1);
				else if(input[1]=='2'){Rotate_R(t_cube,0);Rotate_R(t_cube,0);}
				else Rotate_R(t_cube,0);
			}
			else if(input[0]=='B'){
				if(input[1]=='\'')Rotate_B(t_cube,1);
				else if(input[1]=='2'){Rotate_B(t_cube,0);Rotate_B(t_cube,0);}
				else Rotate_B(t_cube,0);
			}
			else{
				printf("Error\n");
				break;}
		}
		else break;
		}
		for(int j=0;j<6;j++)
		{
			fout.put(t_cube[j][0]);
			fout.put(t_cube[j][1]);
			fout.put(t_cube[j][2]);
			fout.put(t_cube[j][3]);
		}
		fout<<endl;
	}
	fout.close();
}
void initial_pruning_table_four()
{
	string Rtable[6] = {"R ","R'","U ","U'","B ","B'"};
	int i=-1;
	ofstream fout;
	fout.open("Prune_table_four.txt");

	while(1){
		i=base6(i);
		if(i==5555)break;      // operation length
		string Rnum = to_string((long long)i);
		Rnum.insert(Rnum.begin(),4-Rnum.length(),'0');  // 補0到operation length
		//非連續三個重複
		if((Rnum[0]==Rnum[1] && Rnum[0]==Rnum[2]) || (Rnum[1]==Rnum[2] && Rnum[1]==Rnum[3]))    //判斷到 Rnum[operation length]
			continue;
		//非正逆
		bool counterwise = 0;
		for(int i2=0;i2<3;i2++){    //判斷到 operation length-1
			if(Rnum[i2]==counter(Rnum[i2+1]))	counterwise = 1;
		}
		if(counterwise==1)	continue;

		string ch;
		for(int j=0;j<4;j++){       //到 operation length
			ch=Rtable[(Rnum[j])-48];
			fout<<ch;
		}
		fout<<"    ";     //補空格到 12 char
		char t_cube[6][4]={
			{'0','0','0','0'},
			{'1','1','1','1'},
			{'2','2','2','2'},
			{'3','3','3','3'},
			{'4','4','4','4'},
			{'5','5','5','5'}
		};
		for(int j=3;j>=0;j--)
		{
			string input;
			Rnum[j]=counter(Rnum[j]);
			input=Rtable[Rnum[j]-48];
			if(input!="x"){

			if(input[0]=='U'){
				if(input[1]=='\'')Rotate_U(t_cube,1);
				else if(input[1]=='2'){Rotate_U(t_cube,0);Rotate_U(t_cube,0);}
				else Rotate_U(t_cube,0);
			}
			else if(input[0]=='R'){
				if(input[1]=='\'')Rotate_R(t_cube,1);
				else if(input[1]=='2'){Rotate_R(t_cube,0);Rotate_R(t_cube,0);}
				else Rotate_R(t_cube,0);
			}
			else if(input[0]=='B'){
				if(input[1]=='\'')Rotate_B(t_cube,1);
				else if(input[1]=='2'){Rotate_B(t_cube,0);Rotate_B(t_cube,0);}
				else Rotate_B(t_cube,0);
			}
			else{
				printf("Error\n");
				break;}
		}
		else break;
		}
		for(int j=0;j<6;j++)
		{
			fout.put(t_cube[j][0]);
			fout.put(t_cube[j][1]);
			fout.put(t_cube[j][2]);
			fout.put(t_cube[j][3]);
		}
		fout<<endl;
	}
	fout.close();
}
void initial_pruning_table_one()
{
	string Rtable[6] = {"R ","R'","U ","U'","B ","B'"};
	int i=-1;
	ofstream fout;
	fout.open("Prune_table_one.txt");

	while(1){
		i=base6(i);
		if(i==10)break;      // operation length
		string Rnum = to_string((long long)i);
		//Rnum.insert(Rnum.begin(),1-Rnum.length(),'0');  // 補0到operation length
		//非連續三個重複
		/*
		if((Rnum[0]==Rnum[1] && Rnum[0]==Rnum[2]) || (Rnum[1]==Rnum[2] && Rnum[1]==Rnum[3]) ||
			(Rnum[2]==Rnum[3] && Rnum[2]==Rnum[4]))    //判斷到 Rnum[operation length]
			continue;
		//非正逆
		bool counterwise = 0;
		for(int i2=0;i2<4;i2++){    //判斷到 operation length-1
			if(Rnum[i2]==counter(Rnum[i2+1]))	counterwise = 1;
		}
		if(counterwise==1)	continue;
        */
		string ch;
		for(int j=0;j<1;j++){       //到 operation length
			ch=Rtable[(Rnum[j])-48];
			fout<<ch;
		}
		fout<<"          ";     //補空格到 12 char
		char t_cube[6][4]={
			{'0','0','0','0'},
			{'1','1','1','1'},
			{'2','2','2','2'},
			{'3','3','3','3'},
			{'4','4','4','4'},
			{'5','5','5','5'}
		};
		for(int j=0;j>=0;j--)
		{
			string input;
			Rnum[j]=counter(Rnum[j]);
			input=Rtable[Rnum[j]-48];
			if(input!="x"){

			if(input[0]=='U'){
				if(input[1]=='\'')Rotate_U(t_cube,1);
				else if(input[1]=='2'){Rotate_U(t_cube,0);Rotate_U(t_cube,0);}
				else Rotate_U(t_cube,0);
			}
			else if(input[0]=='R'){
				if(input[1]=='\'')Rotate_R(t_cube,1);
				else if(input[1]=='2'){Rotate_R(t_cube,0);Rotate_R(t_cube,0);}
				else Rotate_R(t_cube,0);
			}
			else if(input[0]=='B'){
				if(input[1]=='\'')Rotate_B(t_cube,1);
				else if(input[1]=='2'){Rotate_B(t_cube,0);Rotate_B(t_cube,0);}
				else Rotate_B(t_cube,0);
			}
			else{
				printf("Error\n");
				break;}
		}
		else break;
		}
		for(int j=0;j<6;j++)
		{
			fout.put(t_cube[j][0]);
			fout.put(t_cube[j][1]);
			fout.put(t_cube[j][2]);
			fout.put(t_cube[j][3]);
		}
		fout<<endl;
	}
	fout.close();
}
void initial_pruning_table()
{
	string Rtable[6] = {"R ","R'","U ","U'","B ","B'"};
	int i=-1;
	ofstream fout;
	fout.open("Prune_table.txt");

	while(1){
		i=base6(i);
		if(i==555555)break;
		string Rnum = to_string((long long)i);
		Rnum.insert(Rnum.begin(),6-Rnum.length(),'0');
		//非連續三個重複
		if((Rnum[0]==Rnum[1] && Rnum[0]==Rnum[2]) || (Rnum[1]==Rnum[2] && Rnum[1]==Rnum[3]) ||
			(Rnum[2]==Rnum[3] && Rnum[2]==Rnum[4]) ||(Rnum[3]==Rnum[4] && Rnum[3]==Rnum[5]))
			continue;
		//非正逆
		bool counterwise = 0;
		for(int i2=0;i2<5;i2++){
			if(Rnum[i2]==counter(Rnum[i2+1]))	counterwise = 1;
		}
		if(counterwise==1)	continue;

		string ch;
		for(int j=0;j<6;j++){
			ch=Rtable[(Rnum[j])-48];
			fout<<ch;
		}
		char t_cube[6][4]={
			{'0','0','0','0'},
			{'1','1','1','1'},
			{'2','2','2','2'},
			{'3','3','3','3'},
			{'4','4','4','4'},
			{'5','5','5','5'}
		};
		for(int j=5;j>=0;j--)
		{
			string input;
			Rnum[j]=counter(Rnum[j]);
			input=Rtable[Rnum[j]-48];
			if(input!="x"){

			if(input[0]=='U'){
				if(input[1]=='\'')Rotate_U(t_cube,1);
				else if(input[1]=='2'){Rotate_U(t_cube,0);Rotate_U(t_cube,0);}
				else Rotate_U(t_cube,0);
			}
			else if(input[0]=='R'){
				if(input[1]=='\'')Rotate_R(t_cube,1);
				else if(input[1]=='2'){Rotate_R(t_cube,0);Rotate_R(t_cube,0);}
				else Rotate_R(t_cube,0);
			}
			else if(input[0]=='B'){
				if(input[1]=='\'')Rotate_B(t_cube,1);
				else if(input[1]=='2'){Rotate_B(t_cube,0);Rotate_B(t_cube,0);}
				else Rotate_B(t_cube,0);
			}
			else{
				printf("Error\n");
				break;}
		}
		else break;
		}
		for(int j=0;j<6;j++)
		{
			fout.put(t_cube[j][0]);
			fout.put(t_cube[j][1]);
			fout.put(t_cube[j][2]);
			fout.put(t_cube[j][3]);
		}
		fout<<endl;
	}
	fout.close();
}
int base6(int in_num)
{
    //cout<<in_num<<" ";
	//in_num=000000
	int out_num=0,temp,i;
	temp=in_num+1;
	for(i=0;i<6;i++)
	{
		if(temp%10==6){
			temp = (temp/10)+1;
		}
		else{
			out_num+=(int)round(((temp%10)*(pow((double)10,(double)i))));
			//cout<<out_num<<": "<<(temp%10)<<"  "<<pow(10,(double)i)<<"  result :"<<(temp%10)*(pow((double)10,(double)i))<<endl;
			temp/=10;
		}
	}
    //cout<<(5%10)*(int)(pow((double)10,(double)2))<<endl;
	return out_num;
}

void Rotate_U(char cube[6][4],bool direction)
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

void Rotate_R(char cube[6][4],bool direction)
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

void Rotate_B(char cube[6][4],bool direction)
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

void printcube(char cube[6][4])
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

bool verification(char temp_cube[6][4])
{
	bool ver=false;
	if (temp_cube[0][0]==temp_cube[0][1] && temp_cube[0][0]==temp_cube[0][2] && temp_cube[0][0]==temp_cube[0][3])
		if (temp_cube[1][0]==temp_cube[1][1] && temp_cube[1][0]==temp_cube[1][2] && temp_cube[1][0]==temp_cube[1][3])
			if (temp_cube[2][0]==temp_cube[2][1] && temp_cube[2][0]==temp_cube[2][2] && temp_cube[2][0]==temp_cube[2][3])
				ver=true;
	return ver;
}

//int tc0[6][4],tc1[6][4],tc2[6][4],tc3[6][4],tc4[6][4],tc5[6][4];

void bfs_solver()
{
	int i;
	for(i=0;i<6;i++)
	{

	}



}



int main(){
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
	//cout<<verification(origin_cube);

	//cout<< base6(155555);

//用main遞迴創建pthread找最短解法
//pthread 每步找出所有可能轉法，並再開thread下去找最短解法


	//system("pause");

}
