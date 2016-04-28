#include <stdio.h>
#include <iostream>
#include "CGraph.h"

// 获取遍历的path路径
CPath::CPath(CVertex* s, CVertex* e)
{
    do
    {
        path.push_back(e);
    }
    while ((e = e->p) != nullptr && e->ID != s->ID);
    if (e != nullptr) {
        path.push_back(e);
    }
    path.reverse();
}

// 显示path路径
void CPath::showPathInConsole()
{
    using std::cout;
    using std::endl;
    if ((*path.begin())->ID == 0) {
        list<CVertex*>::iterator l;
        cout << "Vertex(" << (*path.begin())->ID << ")";
        for (l = ++path.begin(); l != path.end(); l++) {
            cout << "-->Vertex(" << (*l)->ID << ")";
        }
        cout << endl;
    }
}