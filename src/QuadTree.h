#include <iostream>
#include <memory> // for std::unique_ptr

struct XYZ
{
    float x;
    float y;
    float z;

    XYZ() : x(0), y(0), z(0) {}
    XYZ(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    XYZ(const XYZ& other): x(other.x), y(other.y), z(other.z) {}
    XYZ& operator=(const XYZ& other);
    XYZ operator+(const XYZ other);

    XYZ operator-(const XYZ other);
    XYZ& operator+=(const XYZ & other);
    XYZ& operator-=(const XYZ & other);


};

struct AABB{
    XYZ center;
    float halfSize;

    AABB() : center(XYZ(0, 0, 0)), halfSize(0.f) {}
    AABB(XYZ _center, float _halfSize) : center(_center), halfSize(_halfSize) {}
    AABB(const AABB& other);
    AABB& operator=(const AABB& other);

    bool containsPoint(XYZ point){
        float min_x = center.x - halfSize;
        float max_x = center.x + halfSize;

        float min_y = center.y - halfSize;
        float max_y = center.y + halfSize;

        float min_z = center.z - halfSize;
        float max_z = center.z + halfSize;


        return (point.x < max_x && point.x >= min_x &&\
                point.y < max_y && point.y >= min_y &&\
                point.z < max_z && point.z >= min_z);
    }
};

struct QuadTree{
    const static int MAX_HEIGHT = 2;
    std::unique_ptr<QuadTree> subTrees[4];

    AABB boundary;
    bool isLeaf;
    int height;

    std::unique_ptr<int> resource;

    QuadTree() : boundary(AABB()), isLeaf(true), height(0) {}
    QuadTree(AABB _boundary) : boundary(_boundary), isLeaf(true), height(0) {}
    QuadTree(AABB _boundary, int _height) : boundary(_boundary), isLeaf(true), height(_height) {}
    QuadTree(const QuadTree& other);
    QuadTree& operator =(const QuadTree& other);

    void subdivise();
    void insert(XYZ point);
    int maxHeight();
};

QuadTree::QuadTree(const QuadTree& other){
    boundary = other.boundary;
    isLeaf = other.isLeaf;
    //TODO: ressource
}
QuadTree& QuadTree::operator =(const QuadTree& other){
    if(this != &other){
        boundary = other.boundary;
        isLeaf = other.isLeaf;
    }
    return *this;
}


AABB::AABB(const AABB& other){
    center = other.center;
    halfSize = other.halfSize;
}

AABB& AABB::operator=(const AABB& other){
    if(this != &other){
        center = other.center;
        halfSize = other.halfSize;
    }
    return *this;
}


XYZ& XYZ::operator =(const XYZ& other){
    if(this != &other){
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

XYZ XYZ::operator-(const XYZ other){
    return XYZ(x-other.x, y-other.y, z-other.z);
}

XYZ& XYZ::operator +=(const XYZ & other){
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

XYZ& XYZ::operator -=(const XYZ & other){
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

XYZ XYZ::operator+(const XYZ other){
    return XYZ(x+other.x, y+other.y, z+other.z);
}


void QuadTree::subdivise(){
    if(height == MAX_HEIGHT){
        return;
    }

    isLeaf = false;

    XYZ center = boundary.center;
    float z = center.z;
    float next_halfsize = boundary.halfSize/2;
    XYZ nw = center + XYZ(-next_halfsize, next_halfsize, z);
    XYZ ne = center + XYZ(next_halfsize, next_halfsize, z);
    XYZ sw = center + XYZ(-next_halfsize, -next_halfsize, z);
    XYZ se = center + XYZ(next_halfsize, -next_halfsize, z);

    //std::cout << "Center:\t" << center.x << ", " << center.y << ", " << i_ << std::endl;

    subTrees[0] = std::unique_ptr<QuadTree>(new QuadTree(AABB(nw, next_halfsize), height+1));
    subTrees[1] = std::unique_ptr<QuadTree>(new QuadTree(AABB(ne, next_halfsize), height+1));
    subTrees[2] = std::unique_ptr<QuadTree>(new QuadTree(AABB(sw, next_halfsize), height+1));
    subTrees[3] = std::unique_ptr<QuadTree>(new QuadTree(AABB(se, next_halfsize), height+1));
}

void QuadTree::insert(XYZ point){
    // If point not in Quad -> out
    if(!boundary.containsPoint(point))
        return;

    // If we can't go further -> out
    if(height == MAX_HEIGHT && isLeaf)
        return;

    // If Quad a leaf, subdivise and call recursively
    if(isLeaf)
        this->subdivise();

    // Now we are sure that this Quad have childs
    for(int i=0; i<4; ++i){
        subTrees[i].get()->insert(point);
    }
}

int QuadTree::maxHeight(){
    // if leaf return value
    if(isLeaf)
        return height;

    // else
    int _height = -1;
    for(int i=0; i<4; ++i){
        int t_height = subTrees[i].get()->height;
        if(t_height > _height)
            _height = t_height;
    }

    return _height;
}
