#ifndef _STDAFX_H_
#define _STDAFX_H_
#include "stdafx.h"
#endif

#include "cube.h"

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

	rotate_one_U : B->R->F->L->B (Layer 0 1)
	rotate_one_U': B<-R<-F<-L<-B (Layer 0 1)

	rotate_one_R : U->B(0&2)->D->F->B(0&2) (Layer 1 3)
    rotate_one_R': U<-B(0&2)<-D<-F<-B(0&2) (Layer 1 3)
*/

int main(int argc, char *argv[])
{
    cube rubiks;
    string input;
    rubiks.print();
    cout << endl << "input instruction stream: ";
    while (true)
    {
        cin >> input;

        rubiks.rotate(input);
    }




    system("pause");
    return 0;
}