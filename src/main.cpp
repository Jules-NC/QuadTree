#include <iostream>
#include <chrono>
#include <cmath>
#include <memory> // for std::unique_ptr
#include<omp.h>
#include <assert.h>     /* assert */

#include "QuadTree.h"

using namespace std::chrono;

inline bool aprx(float x, float y){
    return std::abs(x-y) < 0.001;
}

void benchmark(){
    auto start = high_resolution_clock::now();
    for(int i=0; i<1000; ++i)
    {
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    std::cout << "TIME: " << duration.count()/1000 << std::endl;

}
void testContainsPoint(){
    AABB aabb(XYZ(0, 0, 0), 1);
    assert(aabb.containsPoint(XYZ(0, 0, 0)));
    assert(!aabb.containsPoint(XYZ(1.1, 0, 0)));
    assert(!aabb.containsPoint(XYZ(-1.1, 0, 0)));
    assert(!aabb.containsPoint(XYZ(0, 1.1, 0)));
    assert(!aabb.containsPoint(XYZ(0, -1.1, 0)));
    std::cerr << "testContainsPoint() DONE\n";
}
void testInsert(){
    QuadTree qt(AABB(XYZ(0, 0, 0), 50));
    qt.insert(XYZ(-25.5, 25.5, 0));

    assert(QuadTree::MAX_HEIGHT >= 2);
    assert(qt.isLeaf == false);
    assert(qt.subTrees[0].get()->isLeaf == false);
    assert(aprx(qt.subTrees[0].get()->boundary.center.x, -25));
    assert(aprx(qt.subTrees[0].get()->boundary.center.y, 25));
    assert(aprx(qt.subTrees[0].get()->boundary.halfSize, 25));
    assert(aprx(qt.subTrees[1].get()->boundary.center.x, 25));
    assert(aprx(qt.subTrees[0].get()->subTrees[0].get()->boundary.center.x, -37.5));

    std::cerr << "testInsert() DONE\n";
}
bool testXYZ(){
    bool res;
    XYZ p;
    XYZ p1 = XYZ(1, 1, 0);
    XYZ p2 = p1;
    // null
    assert(aprx(p.x, 0) && aprx(p.y, 0));
    assert(aprx(p1.x, 1) && aprx(p1.y, 1));
    assert(aprx(p2.x, 1) && aprx(p2.y, 1));

    p2 = p2+p1;

    assert(aprx(p1.x, 1) && aprx(p1.y, 1));
    assert(aprx(p2.x, 2) && aprx(p2.y, 2));

    p2 = p2 - p1;

    assert(aprx(p1.x, 1) && aprx(p1.y, 1));
    assert( aprx(p2.x, 1) && aprx(p2.y, 1));

    p2 += p1;

    assert(aprx(p1.x, 1) && aprx(p1.y, 1));
    assert(aprx(p2.x, 2) && aprx(p2.y, 2));

    p2 -= p1;

    assert(aprx(p1.x, 1) && aprx(p1.y, 1));
    assert(aprx(p2.x, 1) && aprx(p2.y, 1));

    p2+p1;
    p2-p1;

    assert(aprx(p1.x, 1) && aprx(p1.y, 1));
    assert(aprx(p2.x, 1) && aprx(p2.y, 1));

    std::cerr << "testXYZ() DONE\n";
}
void testAABB(){
    bool res = true;;
    AABB a;
    AABB a1(XYZ(10, 11, 0), 12);
    AABB a2(a1);
    AABB a3(XYZ(1, 1, 0),1);
    AABB a4(XYZ(2, 2, 0),2);

    assert(aprx(a.center.x, 0) && aprx(a.center.y, 0) && aprx(a.halfSize, 0));
    assert(aprx(a2.center.x, 10) && aprx(a2.center.y, 11) && aprx(a2.halfSize, 12));

    a2.center.x = 13;
    assert(aprx(a2.center.x, 13) && aprx(a1.center.x, 10));

    a3 = a4;
    a4.halfSize = 5;
    assert(aprx(a3.halfSize, 2) && aprx(a4.halfSize, 5));
    std::cerr << "testAABB() DONE\n";

}
bool testQuadTree(){
    QuadTree q;
    QuadTree q1(AABB(XYZ(25, 25, 0), 25));
    QuadTree q2(q1);
    QuadTree q3(AABB(XYZ(0, 0, 0), 10));

    assert(aprx(q.boundary.center.x, 0) && aprx(q.boundary.center.y, 0));
   // res = res && aprx(q1.boundary.center.x, 25) && (q1.isLeaf && q2.isLeaf);

    q3.subdivise();
    assert(("Height is badly distributed", q3.height==0 && q3.subTrees[0].get()->height==1));
    assert(("Leaf is not set properly", q3.isLeaf==false && q3.subTrees[0].get()->isLeaf==true));

    std::cerr << "testQuadTree() DONE\n";
}

void test(){
    testXYZ();
    testAABB();
    testContainsPoint();
    testInsert();
    testQuadTree();
    std::cerr << "[ALL TESTS PASSED]\n";
}

int main(){

    test();

    QuadTree q(AABB(XYZ(0, 0, 0), 50));
    q.subdivise();
}
