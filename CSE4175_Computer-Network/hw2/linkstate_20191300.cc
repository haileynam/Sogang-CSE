#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <queue>

#define INF 999
#pragma warning(disable: 4996)

using namespace std;

int node_num = 0;
int link_num = 0;
//vector <pair<int, int>> distance_matrix[101];
int** distance_matrix; // maximum node_num = 100
int link[4951][3]; // 100 nodes maximum link = 4950
typedef struct node
{
    int from[101];
    int dist[101];
} node;
node routing_table[101];

typedef struct spt_node
{
    int distance;
    int parent;
} spt_node;

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

    while (1)
    {
        fscanf(fp_top, "%d%d%d", &t_node1, &t_node2, &t_distance);
        if (feof(fp_top)) break;
        distance_matrix[t_node1][t_node2] = t_distance;
        distance_matrix[t_node2][t_node1] = t_distance;
        link[link_num][0] = t_node1;
        link[link_num][1] = t_node2;
        link[link_num][2] = t_distance;
        link_num++;
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
                if (distance_matrix[i][j] < INF)
                {
                    routing_table[i].from[j] = j;
                    routing_table[i].dist[j] = distance_matrix[i][j];
                }
                else 
                {
                    routing_table[i].from[j] = 0;
                    routing_table[j].dist[i] = 0;
                    routing_table[i].dist[j] = 0;
                }
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
                if (distance_matrix[i][j] < INF)
                {
                    routing_table[i].from[j] = j;
                    routing_table[i].dist[j] = distance_matrix[i][j];
                }
                else 
                {
                    routing_table[i].from[j] = 0;
                    routing_table[i].dist[j] = 0;
                    routing_table[j].dist[i] = 0;
                }
                
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

void update_routing_table (int node, vector<vector<int>> lsp)
{
    int parent = -1, dist = 0, next_node;
    for (int i = 0; i < node_num; i++)
    {
        if (node == i) continue;
        if (lsp[i][0] == INF)
        {
            routing_table[node].dist[i] = INF;
            continue;
        }

        dist = lsp[i][0];
        next_node = i;
        while (1)
        {
            parent = lsp[next_node][1];
            if (parent == node) break;
            next_node = parent;
        }
        routing_table[node].from[i] = next_node;
        routing_table[node].dist[i] = dist;
    }
}

void dijkstra()
{
    vector<int> SPT;
    int src, next_node, min_dist, min_node;
    vector<vector<int>> dij;
    /*
    dij[node_num][3] : initialised with INF
    dij[node_num][0] : distance
    dij[node_num][1] : parent
    dij[node_num][2] : visited
    */

    for (int i = 0; i < node_num; i++)
    {
        src = i;
        SPT.push_back(src);
        dij.assign(node_num, vector<int>(3, INF));
        // <inf inf inf>
        // <inf inf inf> ...
        next_node = i;
        // i to i : distance = 0, parent = i. 
        dij[i][0] = 0;
        dij[i][1] = 0;
        dij[i][2] = 1;
        
        printf("dij: \n");
        for(int a =0; a< node_num; a++)
        {
            printf("%d %d %d\n", dij[a][0], dij[a][1], dij[a][2]);
        }
        // for (int b = 0; b < node_num; b++)
        // {
        //     for (int c = 0; c < node_num; c++)
        //     {
        //         cout<< c << routing_table[b].from[c] << routing_table[b].dist[c] << endl;
        //     }
        //     cout << '\n';
        // }

        for (int j = 0; j < node_num; j++)
        {
            min_dist = INF;
            min_node = INF;
            for (int k =0; k < node_num; k++)
            {
                printf("how about dij?\n");
                for (int a = 0; a < node_num; a++)
                {
                    printf("%d %d %d\n", dij[a][0],dij[a][1],dij[a][2]);
                }
                printf("i: %d, j: %d, k:%d, next node: %d, min node: %d\n",i, j, k, next_node, min_node);
                if (dij[k][2] != 1 && distance_matrix[next_node][k] != INF)
                {
                    if (distance_matrix[next_node][k] < INF && dij[k][2] != 1)
                    {
                        if (dij[next_node][0] + routing_table[next_node].dist[k] <= dij[k][0])
                        {
                            if (dij[next_node][0] + routing_table[next_node].dist[k] == dij[k][0])
                            {
                                if (next_node < k)
                                {
                                    dij[k][1] = next_node;
                                }
                            }
                            else
                            {
                                dij[k][0] = dij[next_node][0] + routing_table[next_node].dist[k];
                                dij[k][1] = next_node;
                            }
                        }
                    }
                }

                if (dij[k][2] != 1 && dij[k][0] <= min_dist)
                {
                    if (dij[k][0] == min_dist)
                    {
                        if (k < min_node)
                        {
                            //min_dist = dij[k][0];
                            min_node = k;
                        }
                    }
                    else 
                    {
                        min_dist = dij[k][0];
                        min_node = k;
                    }
                }
            }

            if (j == node_num -1 || min_node == INF) break;
            SPT.push_back(min_node);
            dij[min_node][2] = 1;
            next_node = min_node;
        }
        printf("before update rt\n");
        for (int a = 0; a < node_num; a++)
        {
            printf("%d %d %d\n", dij[a][0],dij[a][1],dij[a][2]);
        }
        update_routing_table(i, dij);
    }
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
        
        if (routing_table[m_node1].dist[m_node2] < INF) // 정보가 있다면
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

    fp_out = fopen("output_ls.txt", "w");

    fscanf(fp_top, "%d", &node_num);
    distance_matrix = (int**)malloc(sizeof(int*)*node_num);
    for (int i= 0; i < node_num; i++)
    {
        distance_matrix[i] = (int*)malloc(sizeof(int)*node_num);
    }

    // 3. first routingTable
    init_matrix();

    // 4. exchange table and update (dijkstra algorithm)
    dijkstra();
    
    // 5. message handle
    message();

    // step 0 end.
    // should repeat these steps line by line of changes.txt
    fprintf(fp_out, "\n");
    int c_node1, c_node2, c_distance;
    while (1)
    {
        fscanf(fp_change, "%d%d%d", &c_node1, &c_node2, &c_distance);
        if (feof(fp_change))break;
        if (c_distance == -999) c_distance = INF;
        distance_matrix[c_node1][c_node2] = c_distance;
        distance_matrix[c_node2][c_node1] = c_distance;
        changed_matrix(); // reinitialising routing table. 
        dijkstra();
        message();
        fprintf(fp_out, "\n");
        
    }
    fclose(fp_top);fclose(fp_msg);fclose(fp_change);fclose(fp_out);
}    