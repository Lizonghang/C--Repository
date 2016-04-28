#include "CGraph.h"
#include <iostream>
#include <ctime>
#define MAXITEM 7
#define VERTEX_NUM  1500
#define EDGE_NUM    1500

int main(int argc, char * argv[])
{
    using std::cout;
    using std::endl;
    using std::cin;
    
    int chooseItem;
    
    cout << "输入序列号执行对应项目:\n"
         << "1. 生成随机有向连通图\n"
         << "2. 单源单宿最短路算法\n"
         << "3. 最大通过率路径算法\n"
         << "4. 带宽约束最短路算法\n"
         << "5. 双向Dijkstra算法\n"
         << "6. 经典Dijkstra算法\n"
         << "7. 比较经典Dijkstra与双向Dijkstra\n\n";
        
    do{ cout << "选择: "; cin >> chooseItem; } while (chooseItem < 1 || chooseItem > MAXITEM);
        
    if (chooseItem == 1)
    {
        cout << "1. 生成随机有向连通图\n\n";
        CGraph digraph;
        cout << "顶点数量: " << digraph.getNumVertex() << endl;
        cout << "边的数量: " << digraph.getNumEdge() << endl;
        cout << endl;
        cout << "随机生成的有向图链表表达式如下:\n";
        digraph.showGraphInConsole();
        cout << endl;
    }
    else if (chooseItem == 2)
    {
        cout << "2. 单源单宿最短路算法\n\n";
        CGraph digraph;
        cout << "顶点数量: " << digraph.getNumVertex() << endl;
        cout << "边的数量: " << digraph.getNumEdge() << endl;
        cout << endl;
        // cout << "随机生成的有向图链表表达式如下:\n";
        // digraph.showGraphInConsole();
        // cout << endl;
        int maxVertexID = digraph.getNumVertex() - 1;
        clock_t startTime = clock();    // 起始计时器
        digraph.DijkstraAlg(0, maxVertexID);  // 单源单宿算法
        clock_t endTime = clock();  // 结束计时器
        // cout << "入口点到可达点的最短路路径如下:\n";
        // digraph.showDijPathInConsole();
        // cout << endl;
        cout << "单源单宿Dijkstra算法耗时: " << endTime - startTime << " clocks." << endl;
    }
    else if (chooseItem == 3)
    {
        cout << "3. 最大通过率路径算法\n\n";
        CGraph digraph("rate");
        cout << "顶点数量: " << digraph.getNumVertex() << endl;
        cout << "边的数量: " << digraph.getNumEdge() << endl;
        cout << endl;
        cout << "随机生成的有向图链表表达式如下:\n";
        digraph.showGraphInConsole();
        cout << endl;
        digraph.DijkstraAlg(0, "rate"); // 最大通过率算法
        cout << "入口点到可达点的最短路路径如下:\n";
        digraph.showDijPathInConsole();
        cout << endl;
    }
    else if (chooseItem == 4)
    {
        cout << "4. 带宽约束最短路算法\n\n";
        CGraph digraph;
        cout << "顶点数量: " << digraph.getNumVertex() << endl;
        cout << "边的数量: " << digraph.getNumEdge() << endl;
        cout << endl;
        cout << "随机生成的有向图链表表达式如下:\n";
        digraph.showGraphInConsole();
        cout << endl;
        digraph.bandWidthLimit(5);
        digraph.DijkstraAlg(0);
        cout << "入口点到可达点的最短路路径如下:\n";
        digraph.showDijPathInConsole();
        cout << endl;
    }
    else if (chooseItem == 5)
    {
        cout << "5. 双向Dijkstra算法\n\n";
        CGraph digraph;
        cout << "顶点数量: " << digraph.getNumVertex() << endl;
        cout << "边的数量: " << digraph.getNumEdge() << endl;
        cout << endl;
        cout << "随机生成的有向图链表表达式如下:\n";
        digraph.showGraphInConsole();
        cout << endl;
        int maxVertexID = digraph.getNumVertex() - 1;
        digraph.BidDijkstraAlg(0, maxVertexID);
        cout << endl;
    }
    else if (chooseItem == 6)
    {
        cout << "6. 简单Dijkstra算法\n\n";
        CGraph digraph;
        cout << "顶点数量: " << digraph.getNumVertex() << endl;
        cout << "边的数量: " << digraph.getNumEdge() << endl;
        cout << endl;
        cout << "随机生成的有向图链表表达式如下:\n";
        digraph.showGraphInConsole();
        cout << endl;
        clock_t startTime = clock();    // 开始计时器
        digraph.DijkstraAlg(0);
        clock_t endTime = clock();      // 结束计时器
        // cout << "入口点到可达点的最短路路径如下:\n";
        // digraph.showDijPathInConsole();
        cout << endl;
        cout << "经典Dijkstra算法耗时: " << endTime - startTime << " clocks." << endl;
    }
    else if (chooseItem == 7)
    {
        cout << "7. 比较经典Dijkstra与双向Dijkstra\n\n" << endl;
        CGraph digraph(VERTEX_NUM, EDGE_NUM);
        int maxVertexID = digraph.getNumVertex() - 1;
        clock_t startTime, endTime;
        
        cout << "单源单宿:\n";
        startTime = clock();
        digraph.DijkstraAlg(0, maxVertexID);
        endTime = clock();
        cout << "单源单宿Dijkstra算法耗时: " << (endTime - startTime)*1000 / CLOCKS_PER_SEC << " ms." << endl;
        digraph.initVertexMap();
        cout << "--------------------------------------------------------------------------" << endl;
        
        cout << "经典算法:\n";
        startTime = clock();
        digraph.DijkstraAlg(0);
        endTime = clock();
        cout << "经典Dijkstra算法耗时: " << (endTime - startTime)*1000 / CLOCKS_PER_SEC << " ms." << endl;
        digraph.initVertexMap();
        cout << "--------------------------------------------------------------------------" << endl;
        
        cout << "双向算法:\n";
        digraph.BidDijkstraAlg(0, maxVertexID);
        digraph.initVertexMap();
    }
    return 0;
}