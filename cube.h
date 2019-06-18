#ifndef _STDAFX_H_
#define _STDAFX_H_
#include "stdafx.h"
#endif

template <class T>
void spinswap(T &a, T &b, T &c, T &d)
{
    T tmp = a;
    a = b;
    b = c;
    c = d;
    d = tmp;
}

struct corner
{
    string sides = "YBRGOW";
};

struct cube
{
    corner block[8];

    string transform[6] = {"R", "R'", "U", "U'", "B", "B'"};

    int turns[3][4] = {
        {1, 3, 7, 5}, // R
        {0, 1, 5, 4}, // U
        {4, 5, 7, 6}  // B
    };

    int faces[3][4] = {
        {0, 4, 5, 2}, // R
        {4, 3, 2, 1}, // U
        {0, 1, 5, 3}  // B
    };

    void print_one()
    {
        cout << block[5].sides << endl;
    }

    void print()
    {
        cout << "  " << block[4].sides[0] << block[5].sides[0] << endl
             << "  " << block[0].sides[0] << block[1].sides[0] << endl
             << block[4].sides[1] << block[0].sides[1] << block[0].sides[2] << block[1].sides[2]
             << block[1].sides[3] << block[5].sides[3] << block[5].sides[4] << block[4].sides[4] << endl
             << block[6].sides[1] << block[2].sides[1] << block[2].sides[2] << block[3].sides[2]
             << block[3].sides[3] << block[7].sides[3] << block[7].sides[4] << block[6].sides[4] << endl
             << "  " << block[2].sides[5] << block[3].sides[5] << endl
             << "  " << block[6].sides[5] << block[7].sides[5] << endl;
    }

    void rotate_one(int f)
    {
        int F = f / 2;

        if (f % 2)
        {
            for (int i = 0; i < 4; i++)
            {
                spinswap(
                    block[turns[F][i]].sides[faces[F][0]],
                    block[turns[F][i]].sides[faces[F][1]],
                    block[turns[F][i]].sides[faces[F][2]],
                    block[turns[F][i]].sides[faces[F][3]]);
            }
            spinswap(
                block[turns[F][3]].sides,
                block[turns[F][2]].sides,
                block[turns[F][1]].sides,
                block[turns[F][0]].sides);
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                spinswap(
                    block[turns[F][i]].sides[faces[F][3]],
                    block[turns[F][i]].sides[faces[F][2]],
                    block[turns[F][i]].sides[faces[F][1]],
                    block[turns[F][i]].sides[faces[F][0]]);
            }
            spinswap(
                block[turns[F][0]].sides,
                block[turns[F][1]].sides,
                block[turns[F][2]].sides,
                block[turns[F][3]].sides);
        }
    }

    void rotate(string f)
    {
        switch (f[0])
        {
        case 'R':
            if (f[1] == '\'')
            {
                rotate_one(1);
            }
            else
            {
                rotate_one(0);
            }
            break;
        case 'U':
            if (f[1] == '\'')
            {
                rotate_one(3);
            }
            else
            {
                rotate_one(2);
            }
            break;
        case 'B':
            if (f[1] == '\'')
            {
                rotate_one(5);
            }
            else
            {
                rotate_one(4);
            }
            break;
        }
    }

    bool is_complete()
    {
        for (int i = 1; i < 8; i++)
        {
            if (block[0].sides != block[i].sides)
                return false;
        }
        return true;
    }
};