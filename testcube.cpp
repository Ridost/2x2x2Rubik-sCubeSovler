#ifndef _STDAFX_H_
#define _STDAFX_H_
#include "stdafx.h"
#endif

#include "cube.h"

#define TABLE_FILE_NAME "p_table.txt"
#define SOLVE_FILE_NAME "s_table.txt"

#define SOLVE_TABLE_LAYERS 7

bool SOLVE = false;

map<string, string> solve_table[SOLVE_TABLE_LAYERS + 1];
map<string, string> prune_table;

void solve(cube &rubiks)
{
    string input;
    vector<string> instructions;

    // inputs
    rubiks.print();
    cout << endl
         << "input instruction stream: ";
    while (cin >> input)
    {
        if (input == "0")
            break;

        instructions.push_back(input);
        rubiks.rotate(input);
        // cout << cube_status(rubiks) << endl;
    }

    rubiks.print();
    // check if no instructions
    if (instructions.size() == 0)
    {
        cout << "No instruction needs to be executed. Terminated." << endl;
        return;
    }

    string basic_solution = "";
    for (int i = instructions.size() - 1; i >= 0; i--) // reverse the instructions
    {
        basic_solution.append(reverse(instructions[i]));
    }

    cout << "Basic Solution: " << basic_solution << endl;

    string status = cube_status(rubiks);
    bool found = false;
    int table_num;
    for (table_num = 0; table_num <= SOLVE_TABLE_LAYERS; table_num++)
    {
        if (solve_table[table_num].find(status) != solve_table[table_num].end())
        {
            found = true;
            break;
        }
    }

    if (found)
    {
        string str = solve_table[table_num][status];
        vector<string> tmp;
        for (int k = 0; k < str.length(); k += 2)
        {
            string r;
            r.assign(str, k, 2);
            tmp.push_back(r);
        }

        str = "";
        for (int i = tmp.size() - 1; i >= 0; i--)
        {
            rubiks.rotate(reverse(tmp[i]));
            str += reverse(tmp[i]);
        }
        if (rubiks.is_complete())
            cout << "Ideal Solution: " << str << endl;
        else
            cout << "Error: No Solution." << endl;
    }
    else
    {
        
    }

    return;
}

void make_table(cube &rubiks)
{
    solve_table[0][cube_status(rubiks)] = "";
    queue<string> q;
    q.push("");

    ofstream file, s_table;
    file.open(TABLE_FILE_NAME);
    s_table.open(SOLVE_FILE_NAME);
    if (!file || !s_table)
    {
        cout << "Cannot open file" << endl;
        return;
    }

    for (int i = 0; i <= SOLVE_TABLE_LAYERS; i++)
    {
        int run = q.size();
        // cout << run << endl;
        for (int j = 0; j < run; j++)
        {
            string tmp = q.front(); // take an item out
            q.pop();
            cube newcube;
            vector<string> ins;

            for (int k = 0; k < tmp.length(); k += 2)
            {
                string r;
                r.assign(tmp, k, 2);
                ins.push_back(r);
            }

            int size = ins.size();

            for (int k = 0; k < ins.size(); k++) // rotate a new cube
            {
                newcube.rotate(ins[k]);
            }

            string status = cube_status(newcube);

            if (solve_table[i].find(status) != solve_table[i].end())
            {
                if (i != 0)
                {
                    string sta = solve_table[i][status];
                    file << tmp << " - " << sta << endl;
                    prune_table[tmp] = sta;
                    continue;
                }
            }
            else if (i >= 2 && solve_table[i - 2].find(status) != solve_table[i - 2].end())
            {
                if (i != 0)
                {
                    string sta = solve_table[i - 2][status];
                    file << tmp << " - " << sta << endl;
                    prune_table[tmp] = sta;
                    continue;
                }
            }
            else
            {
                solve_table[i][status] = tmp;
            }

            if (i == SOLVE_TABLE_LAYERS)
                continue;

            string str = "";
            if (ins.size() != 0)
                str = ins.back();

            for (int k = 0; k < 6; k++)
            {
                if (str == reverse(newcube.transform[k]))
                    continue;
                if (size >= 2 && newcube.transform[k] == str && str == ins[size - 2])
                    continue;

                if (k == 1 && str == "R'")
                    continue;
                if (k == 3 && str == "U'")
                    continue;
                if (k == 5 && str == "B'")
                    continue;

                string newstr = tmp + newcube.transform[k];
                for (auto it = prune_table.begin(); it != prune_table.end(); it++)
                {
                    if (newstr.find(it->first) != string::npos)
                    {
                        //cout << "found" << endl;
                        continue;
                    }
                }
                q.push(newstr);
            }
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (auto it = solve_table[i].begin(); it != solve_table[i].end(); it++)
        {
            s_table << it->first << " - " << it->second << endl;
        }
    }

    cout << "Table is now complete." << endl;
    return;
}

int main(int argc, char *argv[])
{
    cube rubiks;

    make_table(rubiks);
    solve(rubiks);

    system("pause");
    return 0;
}
