/*
 *  PolyModel.cpp
 *
 */

#include "PolyModel.h"


#include "Vec3.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
using namespace std;

void PolyModel::update(Matrix m,int n)
{
    m_verts[n].x() = m.val[0][0];
    m_verts[n].y() = m.val[1][0];
    m_verts[n].z() = m.val[2][0];
    
}
void PolyModel::computeFaceNormals()
{
}

void PolyModel::computeVertexNormals()
{
    
}

void PolyModel::translate(const Vec3f& o)
{
    Matrix mat(4,1);//set translate vector in matrix
    Matrix change(4,4);
    Matrix result(4,1);
    
    //std::cout<<radius;
    for(int i = 0; i < m_verts.size(); i++)
    {
        mat.val[0][0] = m_verts[i].x();
        mat.val[1][0] = m_verts[i].y();
        mat.val[2][0] = m_verts[i].z();
        mat.val[3][0] = 1;
        
        change.setIdentity(4, 4);
        change.val[0][3] = o.x();
        change.val[1][3] = o.y();
        change.val[2][3] = o.z();
        
        result = change.mlt(mat);
        update(result,i);
        
    }
    


}

void PolyModel::rotate(float z)
{
    Matrix mat(4,1);
    Matrix change(4,4);
    Matrix result(4,1);
    Vec3f center = getCenter();
    for(int i = 0; i < m_verts.size(); i++)
    {
        mat.val[0][0] = m_verts[i].x();
        mat.val[1][0] = m_verts[i].y();
        mat.val[2][0] = m_verts[i].z();
        mat.val[3][0] = 1;
        
        
        change.setIdentity(4, 4);
        change.val[0][3] = -center.x();
        change.val[1][3] = -center.y();
        change.val[2][3] = -center.z();
        result = change;
        result = result.mlt(mat);
       
        change.setIdentity(4, 4);
        change.val[0][0] = cos(z);
        change.val[0][1] = -sin(z);
        change.val[1][0] = sin(z);
        change.val[1][1] = cos(z);
        result = change.mlt(result);
        
        
        change.setIdentity(4, 4);
        change.val[0][3] = center.x();
        change.val[1][3] = center.y();
        change.val[2][3] = center.z();
        result = change.mlt(result);
        update(result, i);
    }
}

void PolyModel::draw()
{
    
        for(int i = 0;i < m_polys.capacity(); i++)//DRAW TWO TRIANGLES
        {
        //glViewport(0, 0, 600, 600);//IF I USE THIS THE GRAPH WILL NOT CHAGE TO RECTANGLE
        //glColor3f(0, 1, 1);//MAYBE CHANGE COLOR TO BLUE
            glBegin(GL_TRIANGLES);
            glVertex3fv(m_verts[m_polys[i].at(1)].getPtr());
            glVertex3fv(m_verts[m_polys[i].at(2)].getPtr());
            glVertex3fv(m_verts[m_polys[i].at(3)].getPtr());
            glEnd();
            glFlush();
        }
    
   
        for(int i = 0;i < m_polys.capacity(); i++)//DRAW TWO TRIANGLES
        {
            //glViewport(0, 0, 600, 600);//IF I USE THIS THE GRAPH WILL NOT CHAGE TO RECTANGLE
            //glColor3f(0, 1, 1);//MAYBE CHANGE COLOR TO BLUE
            glBegin(GL_TRIANGLES);
            glVertex3fv(m_verts[m_polys[i].at(1)].getPtr());
            glVertex3fv(m_verts[m_polys[i].at(2)].getPtr());
            glVertex3fv(m_verts[m_polys[i].at(3)].getPtr());
            glEnd();
            glFlush();
        }
}

bool PolyModel::loadModel(std::istream& istr)
{
    
    int vertex_count;
    int face_count;
    string data;
    
    if (!istr.good())
        return false;
    
    char line[1024];
    istr.getline(line, 1024);

    std::istringstream sstr(line);
    sstr >> data >> vertex_count >> face_count;
    std::string attrib_type;
    bool has_texcoords = false;
    while (!sstr.eof() && sstr.good()) {
        sstr >> attrib_type;
        if (attrib_type == "tex-coords1")
            has_texcoords = true;
    }
    //m_verts.clear();
    //m_polys.clear();
    m_verts.resize(vertex_count);
    m_polys.resize(face_count);
    for(int i = 0; i < m_verts.capacity(); i++)//STORE VERTEX
    {
        string temp1,temp2,temp3;
        istr.getline(line, 1024);
        std::istringstream sstr(line);
        sstr >> temp1 >> temp2 >> temp3;
        m_verts[i].set(atof(temp1.c_str()), atof(temp2.c_str()), atof(temp3.c_str()));
        
        
    }
    
    
    for(int i = 0; i < m_polys.capacity(); i++)//STORE PLOY NUMBERS
    {
        string temp1,temp;
        istr.getline(line, 1024);
        std::istringstream sstr(line);
        sstr>>temp1;
        m_polys[i].insert(m_polys[i].begin(), atoi(temp1.c_str()));

        for(int j = 0; j < atoi(temp1.c_str()); j++)
        {
            sstr>>temp;
            atoi(temp.c_str());
            m_polys[i].insert(m_polys[i].begin()+j+1,atoi(temp.c_str()));
        }
        
        
    }
   /* for(int i =0;i<2;i++)
     {
     for(int j =0;j<4;j++)
     cout<<m_polys[i][j]<<" ";
     cout<<"\n";
     }*/
    m_center = 0.0f;
    m_max_bounding = -numeric_limits<float>::max();
    m_min_bounding = numeric_limits<float>::max();
    
    
    computeFaceNormals();
    computeVertexNormals();
    
    return true;
}
