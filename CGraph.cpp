#include "CGraph.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <climits>

bool pVertexComp(CVertex*, CVertex*);
bool pVertexRevComp(CVertex*, CVertex*);
bool pVertexCompRate(CVertex*, CVertex*);

// 默认构造函数,随机生成有向图
CGraph::CGraph(int nVertex, int nEdge)
{
    // 初始化顶点数与边数
    numVertex = nVertex;
    numEdge = nEdge;
    
    // 按照当前时间生成随机数
    srand((unsigned int)time(0));

    // 生成nVertex个顶点,顶点ID为i(i=0,1,2,...,nVertex-1)
    for (int i = 0; i < nVertex; i++)
    {
        CVertex * vertex = new CVertex(i);
        mapVID_Vertex.insert(pair<const int, CVertex *>(i, vertex));
    }
    
    int headID; // 边的头部顶点ID
    int tailID; // 边的尾部顶点ID
    int weight; // 边的权重
    int capacity;   // 边的带宽限制

    // 生成nEdge条有向边
    for (int i = 0; i < nEdge; i++)
    {
        // 尾部顶点ID不能为nVertex-1
        while ((tailID = rand() % nVertex) == nVertex - 1)
        {
            continue;
        }
        // 头部顶点ID大于尾部顶点ID(防止双向边),并且头部顶点ID不能与尾部顶点ID相同
        while((headID = tailID + rand() % (nVertex - tailID)) == tailID)
        {
            continue;
        }
        
        weight = rand() % 10 + 1;   // 随机的权重范围为 1 ~ 10
        capacity = rand() % 20 + 1; // 随机的带宽限制为 1 ~ 20
        
        CEdge * edge = new CEdge(tailID, headID, weight, capacity);
        
        // 查重,防止生成重复边
        bool isExit = false;
        list<CEdge*>::iterator l1;
        if (!IncidentList.empty()) {
            for (l1 = IncidentList.begin(); l1 != IncidentList.end(); l1++) {
                if ((*l1)->getTail() == tailID && (*l1)->getHead() == headID) {
                    isExit = true;
                    break;
                }
            }
        }
        if (!isExit) {
            IncidentList.push_back(edge);
            mapVID_Vertex[tailID]->VListEdge.push_back(edge);
        }
        else
            i--;
    }
    
    // 记录与顶点关联的出度边
    for(int key = 0; key < nVertex; key++)
    {
        list<CEdge*> edgeList = mapVID_Vertex[key]->VListEdge;
        mapVID_listEdge.insert(pair<const int, list<CEdge*> >(key, edgeList));
    }

    int breakVertex;    // 无入度顶点
    // 循环补充边使生成连通图
    while ((breakVertex = travel()) != -1) {
        headID = breakVertex;
        tailID = rand() % headID;
        weight = rand() % 10 + 1;
        capacity = rand() % 20 + 1;
        CEdge * edge = new CEdge(tailID, headID, weight, capacity);
        IncidentList.push_back(edge);   // 更新边列表
        mapVID_Vertex[tailID]->VListEdge.push_back(edge);   // 更新顶点出度列表
        mapVID_listEdge[tailID].push_back(edge);    // 更新出度map记录
        numEdge++;  // 更新边数目
    }
}

// 构造函数,生成随机通过率图
CGraph::CGraph(string gType, int nVertex, int nEdge)
{
    // 初始化顶点数与边数
    numVertex = nVertex;
    numEdge = nEdge;
    
    // 按照当前时间生成随机数
    srand((unsigned int)time(0));
    
    // 生成nVertex个顶点,顶点ID为i(i=0,1,2,...,nVertex-1)
    for (int i = 0; i < nVertex; i++)
    {
        CVertex * vertex = new CVertex(i);
        mapVID_Vertex.insert(pair<const int, CVertex *>(i, vertex));
    }
    
    int headID; // 边的头部顶点ID
    int tailID; // 边的尾部顶点ID
    float rate; // 边的通过率
    int capacity;   // 边的带宽限制
    
    // 生成nEdge条有向边
    for (int i = 0; i < nEdge; i++)
    {
        // 尾部顶点ID不能为nVertex-1
        while ((tailID = rand() % nVertex) == nVertex - 1)
        {
            continue;
        }
        // 头部顶点ID大于尾部顶点ID(防止双向边),并且头部顶点ID不能与尾部顶点ID相同
        while((headID = tailID + rand() % (nVertex - tailID)) == tailID)
        {
            continue;
        }
        // 随机的通过率范围为 (0.1,1]
        rate = (rand() % 10 + 1) / 10.0;
        capacity = rand() % 20 + 1;
        
        CEdge * edge = new CEdge(tailID, headID, rate, capacity);
        
        // 查重,防止生成重复边
        bool isExit = false;
        list<CEdge*>::iterator l1;
        if (!IncidentList.empty()) {
            for (l1 = IncidentList.begin(); l1 != IncidentList.end(); l1++) {
                if ((*l1)->getTail() == tailID && (*l1)->getHead() == headID) {
                    isExit = true;
                    break;
                }
            }
        }
        if (!isExit) {
            IncidentList.push_back(edge);
            mapVID_Vertex[tailID]->VListEdge.push_back(edge);
        }
        else
            i--;
    }
    
    // 记录与顶点关联的出度边
    for(int key = 0; key < nVertex; key++)
    {
        list<CEdge*> edgeList = mapVID_Vertex[key]->VListEdge;
        mapVID_listEdge.insert(pair<const int, list<CEdge*> >(key, edgeList));
    }
    
    int breakVertex;    // 无入度顶点
    // 循环增加边生成连通图
    while ((breakVertex = travel()) != -1) {
        headID = breakVertex;
        tailID = rand() % headID;
        rate = (rand() % 10 + 1) / 10.0;
        capacity = rand() % 20 + 1;
        CEdge * edge = new CEdge(tailID, headID, rate, capacity);
        IncidentList.push_back(edge);   // 更新边列表
        mapVID_Vertex[tailID]->VListEdge.push_back(edge);   // 更新顶点出度列表
        mapVID_listEdge[tailID].push_back(edge);    // 更新出度map记录
        numEdge++;  // 更新边数目
    }
}


// 析构函数,释放new创建的顶点与边的空间
CGraph::~CGraph()
{
    // 释放顶点空间
    map<int, CVertex*>::iterator i1,iend1;
    iend1 = mapVID_Vertex.end();
    for (i1 = mapVID_Vertex.begin(); i1 != iend1; i1++) {
        delete (*i1).second;
    }
    // 释放边空间
    list<CEdge*>::iterator i2,iend2;
    iend2 = IncidentList.end();
    for (i2 = IncidentList.begin(); i2 != iend2; i2++) {
        delete (*i2);
    }
}

// 更新顶点函数
void CGraph::Update(int v)
{
	list<CEdge*> lEdge = mapVID_listEdge[v];  // 获取顶点v的所有出度边
	list<CEdge*>::iterator i,iend;
	iend = lEdge.end();
	for(i = lEdge.begin(); i != iend; i++)  // 更新相邻每一个顶点
	{
		int w = (*i)->getWeight();  // 获取边权重
		CVertex* h = mapVID_Vertex[(*i)->getHead()];    // 获取边头部顶点
		CVertex* t = mapVID_Vertex[v];  // 获取顶点v
		// 判断权重是否更小,更新距离,将前驱指针指向v
		if(t->d + w < h->d)
		{
			h->d = t->d + w;
			h->p = mapVID_Vertex[v];    // 设置边头部顶点的前驱指针
		}
	}
}

// 反向更新顶点函数
void CGraph::UpdateRev(int v)
{
    list<CEdge*> lEdge = mapVID_listRevEdge[v]; // 获取顶点v的所有入度边
    list<CEdge*>::iterator i, iend;
    iend = lEdge.end();
    for (i = lEdge.begin(); i != iend; i++) {   // 更新相邻每一个顶点
        int w = (*i)->getWeight();  // 获取边权重
        CVertex* t = mapVID_Vertex[(*i)->getTail()];    // 获取边尾部顶点
        CVertex* h = mapVID_Vertex[v];  // 获取顶点v
        // 判断权重差是否更小,仅更新距离
        if (h->d_temp - w > t->d_temp) {
            t->d_temp = h->d_temp - w;
            t->p_rev = h;
        }
    }
}

// 最大通过率版本,更新顶点函数
void CGraph::Update(int v, string type)
{
    list<CEdge*> lEdge = mapVID_listEdge[v];  // 获取顶点v的所有相邻边
    list<CEdge*>::iterator i,iend;
    iend = lEdge.end();
    for(i = lEdge.begin(); i != iend; i++)  // 更新相邻的每一个顶点
    {
        float r = (*i)->getRate();  // 获取边通过率
        CVertex* h = mapVID_Vertex[(*i)->getHead()];    // 获取头部顶点
        CVertex* t = mapVID_Vertex[v];  // 获取顶点v
        // 判断通过率是否更大,更新通过率,将前驱指针指向v
        if(t->dRate * r > h->dRate)
        {
            h->dRate = t->dRate * r;
            h->p = mapVID_Vertex[v];
        }
    }
}

// 带宽限制函数,删除小于bandLimit的边,更新mapVID_listEdge
void CGraph::bandWidthLimit(int limit)
{
    int tempTail, tempHead; // 临时记录要删除的边的头尾顶点
    list<CEdge*>::iterator lc, lct;
    for (lc = IncidentList.begin(); lc != IncidentList.end(); lc++) {
        // 遍历边的集合,删除所有带宽小于limit的边,并在mapVID_listEdge中更新
        if ((*lc)->getCapacity() < limit) {
            tempTail = (*lc)->getTail();
            tempHead = (*lc)->getHead();
            for (lct = mapVID_listEdge[tempTail].begin(); lct != mapVID_listEdge[tempTail].end(); lct++) {
                if ((*lct)->getHead() == tempHead) {
                    mapVID_listEdge[tempTail].remove(*lct);
                    break;
                }
            }
        }
    }
}

// 权重Dijkstra算法函数
void CGraph::DijkstraAlg(int s)
{
	// 寻找算法的入口顶点并初始化临时标记列表
	map<int, CVertex*>::iterator i,iend;
	iend = mapVID_Vertex.end();
	for(i = mapVID_Vertex.begin(); i != iend; i++)
	{
		if(i->second->ID == s)
			i->second->d = 0;
		listTempMark.push_back(i->second);
	}

	Update(s);	// 更新除入口顶点外的其他顶点

	while(!listTempMark.empty())
	{
		listTempMark.sort(pVertexComp);	 // 对剩余顶点按d进行排序
		int j = (*listTempMark.begin())->ID;  // 获取最短顶点的ID
		listTempMark.pop_front();  // 将最短顶点移出临时列表
		Update(j);  // 进行下一轮更新
	}
    
    // 生成CPath对象,并输出路径信息
    CVertex* sVertex = mapVID_Vertex[0];
    CVertex* eVertex = mapVID_Vertex[numVertex-1];
    CPath path(sVertex, eVertex);
    path.showPathInConsole();
    std::cout << std::endl;
}

// 单源单宿最短路权重Dijkstra算法
void CGraph::DijkstraAlg(int s, int e)
{
    // 寻找算法的入口顶点并初始化临时标记列表
    map<int, CVertex*>::iterator i,iend;
    iend = mapVID_Vertex.end();
    for(i = mapVID_Vertex.begin(); i != iend; i++)
    {
        if(i->second->ID == s)
            i->second->d = 0;
        listTempMark.push_back(i->second);
    }
    
    Update(s);	// 更新除入口顶点外的其他顶点
    
    while(!listTempMark.empty())
    {
        listTempMark.sort(pVertexComp);	 // 对剩余顶点按d进行排序
        int j = (*listTempMark.begin())->ID;  // 获取最短顶点的ID
        listTempMark.pop_front();  // 将最短顶点移出临时列表
        // 找到宿点,停止遍历
        if (j == e) {
            std::cout << "找到宿点 " << j << " , 遍历路径如下:" << std::endl;
            break;
        }
        Update(j);  // 进行下一轮更新
    }
    
    // 生成CPath对象,并输出路径信息
    CVertex* sVertex = mapVID_Vertex[s];
    CVertex* eVertex = mapVID_Vertex[e];
    CPath path(sVertex, eVertex);
    path.showPathInConsole();
    std::cout << std::endl;
}

// 最大通过率Dijkstra算法函数
void CGraph::DijkstraAlg(int s, string type)
{
    // 寻找算法的入口顶点并初始化临时标记列表
    map<int, CVertex*>::iterator i,iend;
    iend = mapVID_Vertex.end();
    for(i = mapVID_Vertex.begin(); i != iend; i++)
    {
        if(i->second->ID == s)
            i->second->dRate = 1;
        listTempMark.push_back(i->second);
    }
    
    Update(s, "rate");	// 更新除入口顶点外的其他顶点
    
    while(!listTempMark.empty())
    {
        listTempMark.sort(pVertexCompRate);	 // 对剩余顶点按dRate进行排序
        int j = (*listTempMark.begin())->ID;  // 获取最大通过率顶点的ID
        listTempMark.pop_front();  // 将最大通过率顶点移出临时列表
        Update(j, "rate");  // 进行下一轮更新
    }
}

// 以链表方式显示图的信息
void CGraph::showGraphInConsole()
{
    using std::cout;
    using std::endl;
    list<CEdge*>::iterator lc;
    for (int key = 0; key < getNumVertex(); key++) {
        cout << key;
        if (!mapVID_listEdge[key].empty()) {
            for (lc = mapVID_listEdge[key].begin(); lc != mapVID_listEdge[key].end(); lc++) {
                cout << "-->" << (*lc)->getHead();
            }
        }
        cout << endl;
    }
}

// 遍历图,查看是否连通图,思路:入口点可到达其他全部顶点
int CGraph::travel()
{
    list<CEdge*>::iterator lc;
    map<int, list<CEdge*> >::iterator mi, ml;
    // 检查每一个顶点有无入度边
    for (int key = 1; key < numVertex; key++) {
        bool hasInDegree = false;
        // 检查其它顶点邻接边上有无该顶点
        for (ml = mapVID_listEdge.begin(); ml != mapVID_listEdge.end(); ml++) {
            if (!(*ml).second.empty()) {
                for (lc = (*ml).second.begin(); lc != (*ml).second.end(); lc++) {
                    if ((*lc)->getHead() == key) {
                        hasInDegree = true;
                        break;
                    }
                }
            }
            // 本轮循环找到该顶点则退出循环
            if (hasInDegree)
            {
                break;
            }
        }
        // 顶点没有入度边,返回其ID值
        if (!hasInDegree)
        {
            return key;
        }
    }
    // 每一个顶点都有入度边,返回-1表示图连通
    return -1;
}

// 显示经过Dijstra算法的图从入口点到其它点的全部路径
void CGraph::showDijPathInConsole()
{
    CVertex* sVertex = mapVID_Vertex[0];
    CVertex* eVertex;
    for (int key = 1; key < numVertex; key++) {
        eVertex = mapVID_Vertex[key];
        CPath path(sVertex, eVertex);
        path.showPathInConsole();
    }
}

// listTempMark.sort(pVertexComp) 将指针列表按d值从小到大排列
bool pVertexComp(CVertex* x, CVertex* y)
{
    if(x->d < y->d)
        return true;
    else
        return false;
}

// listTempMark.sort(pVertexComp) 将指针列表按d值从小到大排列
bool pVertexRevComp(CVertex* x, CVertex* y)
{
    if(x->d_temp < y->d_temp)
        return true;
    else
        return false;
}

// listTempMark.sort(pVertexCompRate) 将指针列表按dRate值从大到小排列
bool pVertexCompRate(CVertex* x, CVertex* y)
{
    if(x->dRate > y->dRate)
        return true;
    else
        return false;
}

// 初始化mapVID_listRevEdge()函数
void CGraph::initMapVID_listRevEdge()
{
    int tempHead;
    list<CEdge*> tempCEdge;
    list<CEdge*>::iterator lc;
    for (int id = 0; id < numVertex; id++)
        mapVID_listRevEdge.insert(pair<const int, list<CEdge*> >(id, tempCEdge));
    for (lc = IncidentList.begin(); lc != IncidentList.end(); lc++) {
        tempHead = (*lc)->getHead();
        mapVID_listRevEdge[tempHead].push_back(*lc);
    }
}

// 双向Dijkstra算法
void CGraph::BidDijkstraAlg(int s, int e)
{
    using std::cout;
    using std::endl;
    
    // 初始化顶点入度边mapVID_listRevEdge
    initMapVID_listRevEdge();
    
    clock_t startTime = clock();    // 开始计时器
    
    // 寻找算法的入口顶点与出口顶点并初始化临时标记列表
    list<CVertex*> listTempRevMark;
    map<int, CVertex*>::iterator i,iend;
    iend = mapVID_Vertex.end();
    for(i = mapVID_Vertex.begin(); i != iend; i++)
    {
        if (i->second->ID == s)
        {
            i->second->d = 0;
            i->second->flag = 1;
        }
        else if (i->second->ID == e)
        {
            i->second->d_temp = INT_MAX / 2;
            i->second->flag = 2;
        }
        listTempMark.push_back(i->second);
        listTempRevMark.push_back(i->second);
    }
    
    Update(s);  // 更新入口点邻接点
    UpdateRev(e);   // 更新出口点邻接点
    
    bool flag = true;   // 在正反向Dijkstra算法间切换
    list<CEdge*>::iterator lc;
    
    while (!listTempMark.empty())
    {
        if (flag)
        {
            listTempMark.sort(pVertexComp);
            int j = (*listTempMark.begin())->ID;
            listTempMark.pop_front();   // 固定顶点j
            // 如果访问到被反向遍历固定的顶点,结束条件
            if (mapVID_Vertex[j]->flag == 2)
            {
                break;
            }
            Update(j);
            mapVID_Vertex[j]->flag = 1;
        }
        else
        {
            listTempRevMark.sort(pVertexRevComp);   // 此处应按d_temp从小到大排列
            int j = (*(--listTempRevMark.end()))->ID;
            listTempRevMark.pop_back(); // 固定顶点j
            // 如果访问到被正向遍历固定的顶点,结束条件
            if (mapVID_Vertex[j]->flag == 1)
            {
                break;
            }
            UpdateRev(j);
            mapVID_Vertex[j]->flag = 2;
        }
        flag = !flag;
    }
    
    // 枚举路径获取必经的边,寻找该边的尾部ansTail与头部ansHead
    int temp = INT_MAX;
    int ansTail = -1, ansHead = -1;
    for (lc = IncidentList.begin(); lc != IncidentList.end(); lc++)
    {
        CVertex* tempHead = mapVID_Vertex[(*lc)->getHead()];
        CVertex* tempTail = mapVID_Vertex[(*lc)->getTail()];
        int d_rev = (INT_MAX/10 == tempHead->get_dtemp()?INT_MAX / 10 : INT_MAX/10 - tempHead->get_dtemp());
        if (temp > (tempTail->get_d() + (*lc)->getWeight() + d_rev)) {
            temp = tempTail->get_d() + (*lc)->getWeight() + d_rev;
            ansTail = tempTail->ID;
            ansHead = tempHead->ID;
        }
    }
    
    
    clock_t endTime = clock();      // 结束计时器
    
    // 输出路径信息
    list<CVertex*> path;
    list<CVertex*>::iterator l;
    CVertex* ansTailVertex = mapVID_Vertex[ansTail];
    CVertex* ansHeadVertex = mapVID_Vertex[ansHead];
    CVertex* tempVertexPointer = ansTailVertex;
    do{
        path.push_back(tempVertexPointer);
        tempVertexPointer = tempVertexPointer->p;
    }while (tempVertexPointer != nullptr);
    path.reverse();
    tempVertexPointer = ansHeadVertex;
    do{
        path.push_back(tempVertexPointer);
        tempVertexPointer = tempVertexPointer->p_rev;
    }while (tempVertexPointer != nullptr);
    cout << "Vertex(" << (*path.begin())->ID << ")";
    for (l = ++path.begin(); l != path.end(); l++) {
        cout << "-->Vertex(" << (*l)->ID << ")";
    }
    cout << endl;
    std::cout << "双向Dijkstra算法耗时: " << (endTime - startTime)*1000 / CLOCKS_PER_SEC << " ms." << std::endl;
    std::cout << std::endl;
}

void CGraph::initVertexMap()
{
    listTempMark.clear();
    CVertex* tempVertex;
    for (int key = 0; key < numVertex; key++) {
        tempVertex = mapVID_Vertex[key];
        tempVertex->d = INT_MAX / 10;
        tempVertex->d_temp = tempVertex->d;
        tempVertex->flag = 0;
        tempVertex->dRate = 0.0;
        tempVertex->p = nullptr;
        tempVertex->p_rev = nullptr;
    }
}