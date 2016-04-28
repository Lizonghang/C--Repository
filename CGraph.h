#ifndef CGRAPH_H_
#define CGRAPH_H_
#include <map>
#include <list>
#include <vector>
#include <climits>
#include <string>
using std::list;
using std::map;
using std::pair;
using std::vector;
using std::string;

// 边类
class CEdge
{
private:
	int tail;	// 尾部顶点ID
	int head;	// 头部顶点ID
	int weight;	// 边距离
    float rate; // 边通过率
    int capacity;   // 带宽约束
public:
    CEdge(int t, int h, int w, int c = 0){ tail = t; head = h; weight = w; capacity = c; rate = 1.0; };	// 权重边构造函数
    CEdge(int t, int h, float r, int c = 0){ tail = t; head = h; rate = r; capacity = c; weight = 0; };  // 通过率边构造函数
    ~CEdge(){};	// 析构函数
    
	int getWeight() const { return weight; };
    int getHead() const { return head; };
    int getTail() const { return tail; };
    int getCapacity() const { return capacity; };
    float getRate() const { return rate; };
    
    void setTail(int t) { tail = t; };
    void setHead(int h) { head = h; };
    void setWeight(int w) { weight = w; };
    void setRate(float r) { rate = r; };
    void setCapacity(int c) { capacity = c; };
};

// 顶点类
class CVertex
{
public:
    int d;		// 距离起始点的距离
    int d_temp; // 距离终点的距离,用于反向Dijkstra
    int ID;		// 顶点的ID
    int flag;   // 用于双向Dijkstra算法标记是否已被遍历
    float dRate;    // 通过率"路径"
    CVertex* p;		// 指向目前顶点的前驱顶点
    CVertex* p_rev;     // 反向Dijkstra指向目前顶点前驱顶点
    list<CEdge*> VListEdge;     // 该顶点的出度边列表
    
    CVertex(int id){ d = INT_MAX / 10; d_temp = d; p = nullptr; p_rev = nullptr ; ID = id; dRate = 0.0; flag = 0; }
    ~CVertex(){};	// 析构函数
    
    int get_d() const { return d; };
    int get_dtemp() const { return d_temp; };
};

// 图类
class CGraph
{
private:
	int numVertex;			// 顶点数目
	int numEdge;			// 边数目
	list<CEdge*> IncidentList;	// 边的列表
	map<int, CVertex*> mapVID_Vertex;  // 所有的顶点
	list<CVertex*> listTempMark;	// 暂时标记的顶点集合
	map<int, list<CEdge*> > mapVID_listEdge;  // 记录与顶点关联的出度边
    map<int, list<CEdge*> > mapVID_listRevEdge; // 记录与顶点关联的入度边
    
	void Update(int VID);		// 更新顶点距离
    void UpdateRev(int VID);    // 反向更新顶点距离
    void Update(int VID, string type);  // 最大通过率版本更新顶点距离

    int travel();    // 检查是否为连通图

    void initMapVID_listRevEdge();  // 初始化mapVID_listRevEdge
public:
    friend class CPath;

	CGraph(int nVertex = 6, int nEdge = 9); // 默认构造函数,随机生成图
    CGraph(string type, int nVertex = 6, int nEdge = 9); // 构造函数,生成随机通过率图
	// CGraph(char* inputFile);	// 给定文件
	// CGraph(list<CEdge*> listEdge);	// 给定边的列表
    CGraph(CGraph &){};		// 复制构造函数
	~CGraph();			// 析构函数
    
    int getNumVertex(){ return numVertex; };
    int getNumEdge(){ return numEdge; };
    
    void initVertexMap();   // 初始化顶点集,用于比较算法效率
    
    void bandWidthLimit(int bandLimit); // 带宽限制函数
    
    void DijkstraAlg(int VID);  // 常规Dijkstra算法
    void DijkstraAlg(int sVID, int eVID);   // 单源单宿最短路Dijkstra算法
    void DijkstraAlg(int VID, string type); // 最大通过率Dijkstra算法
    void BidDijkstraAlg(int sVID, int eVID);    // 双向Dijkstra算法

    void showGraphInConsole();   // 在命令行中显示结果
    void showDijPathInConsole();    // 将经过最短路径Dijstra算法的图路径显示到命令行
};

// 路径类
class CPath
{
private:
    list<CVertex*> path;    // 记录路径
public:
    CPath(CVertex* s, CVertex* e);  // 获取遍历路径
    ~CPath(){};                     // 默认析构函数
    void showPathInConsole();    // 显示遍历路径
};

#endif
