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
    //int i = 0;
    for (int i = 0; i < m_polys.size() ; i++) {
        Vec3<float> v1 = m_verts[m_polys[i].at(1)].getPtr();
        Vec3<float> v2 = m_verts[m_polys[i].at(2)].getPtr();
        Vec3<float> v3 = m_verts[m_polys[i].at(3)].getPtr();
        Vec3<float> temp1 = v2 - v1;
        Vec3<float> temp2 = v3 - v1;
        temp1 = temp1.cross(temp2);
        temp1.normalize();
        face_normal[i] = temp1;
       
        vertex_normal[m_polys[i].at(1)] = vertex_normal[m_polys[i].at(1)] + temp1;
        vertex_normal[m_polys[i].at(2)] = vertex_normal[m_polys[i].at(2)] + temp1;
        vertex_normal[m_polys[i].at(3)] = vertex_normal[m_polys[i].at(3)] + temp1;
        
        
    }
    //cout<<m_polys.size()<<"\n";
   // cout<<i<<"\n";
}

void PolyModel::computeVertexNormals()
{
    for(int i = 0; i < m_verts.size(); i++)
    {
        vertex_normal[i].normalize();
    }
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
    
    glDisable(GL_CULL_FACE);

        for(int i = 0;i < m_polys.capacity(); i++)//DRAW TWO TRIANGLES
        {
        //glViewport(0, 0, 600, 600);//IF I USE THIS THE GRAPH WILL NOT CHAGE TO RECTANGLE
            //glColor3f(0, 1, 1);//MAYBE CHANGE COLOR TO BLUE
            glBegin(GL_TRIANGLES);
            //glNormal3f(face_normal[i].x(), face_normal[i].y(), face_normal[i].z());
            glTexCoord2f(m_tex[m_polys[i].at(1)].y(), m_tex[m_polys[i].at(1)].z());
            glNormal3f(vertex_normal[m_polys[i].at(1)].x(),vertex_normal[m_polys[i].at(1)].y(),vertex_normal[m_polys[i].at(1)].z());
            glVertex3fv(m_verts[m_polys[i].at(1)].getPtr());
            
            glNormal3f(vertex_normal[m_polys[i].at(2)].x(),vertex_normal[m_polys[i].at(2)].y(),vertex_normal[m_polys[i].at(2)].z());
            glTexCoord2f(m_tex[m_polys[i].at(2)].y(), m_tex[m_polys[i].at(2)].z());
            
            glVertex3fv(m_verts[m_polys[i].at(2)].getPtr());
            glNormal3f(vertex_normal[m_polys[i].at(3)].x(),vertex_normal[m_polys[i].at(3)].y(),vertex_normal[m_polys[i].at(3)].z());
            glTexCoord2f(m_tex[m_polys[i].at(3)].y(), m_tex[m_polys[i].at(3)].z());
            glVertex3fv(m_verts[m_polys[i].at(3)].getPtr());
            glEnd();
            //glFlush();
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
    m_tex.resize(vertex_count);
    //cout<<m_verts.size()<<"\n";

    for(int i = 0; i < m_verts.capacity(); i++)//STORE VERTEX
    {
        string temp1,temp2,temp3;
        istr.getline(line, 1024);
        std::istringstream sstr(line);
        sstr >> temp1 >> temp2 >> temp3;
        m_verts[i].set(atof(temp1.c_str()), atof(temp2.c_str()), atof(temp3.c_str()));
        
        
    }
    //cout<<m_verts.size()<<"\n";

    

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
//    cout<<has_texcoords;
    if(has_texcoords)
    {
        for(int i = 0; i < m_tex.capacity(); i++)
        {
            string temp1,temp2,temp3;
            istr.getline(line, 1024);
            std::istringstream sstr(line);
            sstr>>temp1>>temp2>>temp3;
            m_tex[i].set(atof(temp1.c_str()), atof(temp2.c_str()), atof(temp3.c_str()));
            
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
