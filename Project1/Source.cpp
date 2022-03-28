#include <conio.h>
#include <iostream>
#include <fstream>
using namespace std;
const int N = 6;

int read_matrix(int matr[N][N]) {
    char line[2];
    ifstream in("l3-2.txt");
    printf("Розмiрнiсть матрицi: \n");
    if (in.is_open())
    {
        in.getline(line, 2);
        std::cout << line << std::endl;
        printf("Задана матриця: \n");
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                in >> matr[i][j];

        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
                cout << matr[i][j] << "\t";
            cout << "\n";
        }

        in.close();
        printf("\n");
        return **matr;
    }
    else
    {
        cout << "Файл не знайдено.";
        return 0;
    }
}

int fin_way[N + 1];
bool visited[N];
int fin_res = INT_MAX;

void copyToFinal(int curr_path[])
{
    for (int i = 0; i < N; i++) {
        fin_way[i] = curr_path[i];
        fin_way[N] = curr_path[0];
    }
}

int firstMin(int adj[N][N], int i)
{
    int min = INT_MAX;
    for (int k = 0; k < N; k++)
        if (adj[i][k] < min && i != k)
            min = adj[i][k];
    return min;
}

int secondMin(int adj[N][N], int i)
{
    int first = INT_MAX, second = INT_MAX;
    for (int j = 0; j < N; j++)
    {
        if (i == j)
            continue;

        if (adj[i][j] <= first)
        {
            second = first;
            first = adj[i][j];
        }
        else if (adj[i][j] <= second &&
            adj[i][j] != first)
            second = adj[i][j];
    }
    return second;
}

void KomiRec(int adj[N][N], int cur_bound, int cur_weight,
    int level, int cur_way[])
{
    if (level == N)
    {
        if (adj[cur_way[level - 1]][cur_way[0]] != 0)
        {
            int cur_res = cur_weight +
                adj[cur_way[level - 1]][cur_way[0]];
            if (cur_res < fin_res)
            {
                copyToFinal(cur_way);
                fin_res = cur_res;
            }
        }
        return;
    }

    for (int i = 0; i < N; i++)
    {
        if (adj[cur_way[level - 1]][i] != 0 &&
            visited[i] == false)
        {
            int temp = cur_bound;
            cur_weight += adj[cur_way[level - 1]][i];

            if (level == 1)
                cur_bound -= ((firstMin(adj, cur_way[level - 1]) +
                    firstMin(adj, i)) / 2);
            else
                cur_bound -= ((secondMin(adj, cur_way[level - 1]) +
                    firstMin(adj, i)) / 2);

            if (cur_bound + cur_weight < fin_res)
            {
                cur_way[level] = i;
                visited[i] = true;

                KomiRec(adj, cur_bound, cur_weight, level + 1,
                    cur_way);
            }

            cur_weight -= adj[cur_way[level - 1]][i];
            cur_bound = temp;

            memset(visited, false, sizeof(visited));
            for (int j = 0; j <= level - 1; j++)
                visited[cur_way[j]] = true;
        }
    }
}

void Komi(int adj[N][N])
{
    int curr_path[N + 1];

    int curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    memset(visited, 0, sizeof(curr_path));

    for (int i = 0; i < N; i++)
        curr_bound += (firstMin(adj, i) +
            secondMin(adj, i));
    curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 :
        curr_bound / 2;
    visited[0] = true;
    curr_path[0] = 0;
    KomiRec(adj, curr_bound, 0, 1, curr_path);
}

int main()
{
    setlocale(LC_ALL, "Ukrainian");
    int adj[N][N];
    read_matrix(adj);
    Komi(adj);
    printf("\nГамiльтонiв цикл:\n");
    for (int i = 0; i <= N; i++) {
            printf("%d   ", fin_way[i]);
    }
    printf("\nЗагальна вага шляху: %d\n\n", fin_res);
    system("pause");
}