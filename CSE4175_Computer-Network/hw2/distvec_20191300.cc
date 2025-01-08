#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#define INF 999
#pragma warning(disable: 4996)

using namespace std;

int node_num = 0;
int distance_matrix[101][101]; // maximum node_num = 100
typedef struct node
{
    int from[101];
    int dist[101];
} node;
node routing_table[101];
FILE *fp_top, *fp_msg, *fp_change, *fp_out;
/* 
    fp_top:      
    node_num
    node_num 수만큼 node1 node2 cost

    fp_msg:
    src dest messageline

    fp_change:
    node1 node2 cost

    fp_out:
    routing table node_num 수만큼 반복 
    print message
    --- next change --     
*/

int minimum(int a, int b)
{
    if (a<b) return a;
    else return b;
}

void init_matrix()
{
    int t_node1, t_node2, t_distance;
    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            if (i == j) distance_matrix[i][j] = 0; // i to i distance = 0
            else distance_matrix[i][j] = INF;
        }
    }

    // debug
    printf("after initialise\n");
    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            printf("%d ", distance_matrix[i][j]);
        }
        printf("\n");
    }

    while(1)
    {
        fscanf(fp_top, "%d%d%d", &t_node1, &t_node2, &t_distance);
        if (feof(fp_top))break;
        distance_matrix[t_node1][t_node2] = t_distance;
        distance_matrix[t_node2][t_node1] = t_distance;
    }

    // debug
    printf("distance matrix after topology input\n");
    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            printf("%d ", distance_matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            if (i == j)
            {
                routing_table[i].from[j] = j;
                routing_table[i].dist[j] = 0;
            }
            
            else 
            {
                routing_table[i].from[j] = -1;
                routing_table[i].dist[j] = INF;
                routing_table[j].dist[i] = INF;
            }
        }
    }

    // debug
    printf("routing table in initialise\n");
     for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            printf("%d %d %d\n", j, routing_table[i].from[j], routing_table[i].dist[j]);
        }
        printf("\n");
    }
}

void changed_matrix()
{
    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            if (i == j)
            {
                routing_table[i].from[j] = j;
                routing_table[i].dist[j] = 0;
            }
            
            else 
            {
                routing_table[i].from[j] = -1;
                routing_table[i].dist[j] = INF;
                routing_table[j].dist[i] = INF;
            }
        }
    }

    // debug
    printf("changed distance matrix!\n");
    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            printf("%d ", distance_matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return;
}

void bellman_ford()
{
    int flag;
    do 
    {
        flag = 0;
        for (int i = 0; i < node_num; i++)
        {
            for (int j = 0; j < node_num; j++)
            {
                for (int k = 0; k < node_num; k++)
                {
                    // i ~ j > i ~ k + k ~ j
                    if (routing_table[i].dist[j] >= (distance_matrix[i][k]+ routing_table[k].dist[j])) 
                    {
                        if (routing_table[i].dist[j] == (distance_matrix[i][k]+ routing_table[k].dist[j]))
                        {   
                            // tie breaking rule
                            flag--;
                            if (min(k, routing_table[i].from[j]) == i) routing_table[i].from[j] = k;
                            else routing_table[i].from[j] = min(k, routing_table[i].from[j]);
                        }
                        // 다음 노드 바꿔줌.
                        else routing_table[i].from[j] = k;
                        // minimum으로 갱신. 
                        routing_table[i].dist[j] = distance_matrix[i][k] + routing_table[k].dist[j];
                        flag++;
                    }
                   
                   
                    // debug
                    // if (flag >= 1)
                    // {
                    //     printf("routing table after during bellman, flag = %d\n", flag);
                    //     for (int i = 0; i < node_num; i++)
                    //     {
                    //         for (int j = 0; j < node_num; j++)
                    //         {
                    //             printf("%d %d %d\n", j, routing_table[i].from[j], routing_table[i].dist[j]);
                    //         }
                    //         printf("\n");
                    //     }
                    // }
                    
                }
            }
        }
    } while (flag);

    
    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            if (routing_table[i].dist[j] == INF)continue;
            fprintf(fp_out, "%d %d %d \n", j, routing_table[i].from[j], routing_table[i].dist[j]);
            // debug
            printf("%d %d %d\n", j, routing_table[i].from[j], routing_table[i].dist[j]);
        }
        fprintf(fp_out, "\n");
        printf("\n");

    }

    return;
}

void message()
{
    char msg[801];
    int m_node1, m_node2;
    rewind(fp_msg);
    while (1)
    {
        fscanf(fp_msg, "%d %d %[^\n]", &m_node1, &m_node2, msg);
        if (feof(fp_msg)) break;
        // debug
        // printf("%d->%d, msg: %s\n", m_node1, m_node2, msg);
        
        if (routing_table[m_node1].dist[m_node2] != INF) // 정보가 있다면
        {
            fprintf(fp_out, "from %d to %d cost %d hops %d ", m_node1, m_node2, routing_table[m_node1].dist[m_node2], m_node1);
            // debug
            printf("from %d to %d cost %d hops %d ", m_node1, m_node2, routing_table[m_node1].dist[m_node2], m_node1);
            int next_node = routing_table[m_node1].from[m_node2];
            while (next_node != m_node2)
            {
                fprintf(fp_out, "%d ", next_node);
                printf("%d ", next_node);
                next_node = routing_table[next_node].from[m_node2];
            }
            fprintf(fp_out, "message %s", msg);
            printf("message %s", msg);
            fprintf(fp_out, "\n");
            printf("\n");
        }
        else 
        {
            fprintf(fp_out, "from %d to %d cost infinite hops unreachable message %s", m_node1, m_node2, msg);
            fprintf(fp_out, "\n");
        }
    }
    return;
}

int main(int argc, char *argv[])
{

    // 1. check arguments number
    if (argc != 4)
    {
        fprintf(stderr, "usage: distvec topologyfile messagesfile changesfile");
        exit(1);
    }
    
    // 2. open and check files
    fp_top = fopen(argv[1], "r");
    if (!fp_top) 
    {
        fprintf(stderr, "Error: open input file\n");
        exit(1);
    }

    fp_msg = fopen(argv[2], "r");
    if (!fp_msg) 
    {
        fprintf(stderr, "Error: open input file\n"); 
    }

    fp_change = fopen(argv[3], "r");
    if (!fp_change)
    {
        fprintf(stderr, "Error: open input file\n");
        exit(1);
    }

    fp_out = fopen("output_dv.txt", "w");

    fscanf(fp_top, "%d", &node_num);


    // 3. first routingTable
    init_matrix();

    // 4. exchange table and update (bellman-ford algorithm)
    bellman_ford();
    
    // 5. message handle
    message();
    fprintf(fp_out, "\n");

    // step 0 end.
    // should repeat these steps line by line of changes.txt
   
    int c_node1, c_node2, c_distance;
    while (1)
    {
        fscanf(fp_change, "%d%d%d", &c_node1, &c_node2, &c_distance);
        if (feof(fp_change))break;
        if (c_distance == -999) c_distance = INF;
        distance_matrix[c_node1][c_node2] = c_distance;
        distance_matrix[c_node2][c_node1] = c_distance;
        changed_matrix(); // reinitialising routing table. 
        bellman_ford();
        message();
        fprintf(fp_out, "\n");
    }
    fclose(fp_top);fclose(fp_msg);fclose(fp_change);fclose(fp_out);
}    