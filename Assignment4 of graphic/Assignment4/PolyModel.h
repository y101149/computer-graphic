/*
 *  PolyModel.h
 *
 */

#pragma once
#include "Vec3.h"
#include <vector>
#include <iostream>
#include "Matrix.h"
class PolyModel
{
public:
    
    int number = 0;//the number of exterior edge + 1
    int ex[10000][2];//the poly of exterior edge
    Vec3f collisionEdge;

    PolyModel(){}
    PolyModel(std::istream& istr);
    
    void draw();
    
    void computeFaceNormals();
    void computeVertexNormals();
        
    bool loadModel(std::istream& istr);
    
    bool detectCollision2(Vec3f center, float radius)//use projection and vector to detect collision
    {
        Vec3f p = center;
        float mindistance = std::numeric_limits<float>::max();
        Vec3f edge;
        
        for(int i = 0; i < number; i++)
        {
            int verticle1 = ex[i][0];
            int verticle2 = ex[i][1];
            float distance;
            Vec3f temp1 = m_verts[verticle1].getPtr();//two endpoints
            Vec3f temp2 = m_verts[verticle2].getPtr();
            Vec3f line = temp2 - temp1;
            float lenth = std::sqrt(line.x()*line.x()+line.y()*line.y()+line.z()*line.z());
            Vec3f danwei;
            Vec3f good = p - temp1;
            danwei.set(line.x()/lenth, line.y()/lenth, line.z()/lenth);
            float lineprojection = good.dot(danwei);
            Vec3f intersectpoint = temp1 + danwei*lineprojection;
            if(((intersectpoint.x()-temp1.x())/line.x()>=0 && (intersectpoint.x()-temp1.x())/line.x()<=1)
               ||((intersectpoint.y()-temp1.y())/line.y()>=0 && (intersectpoint.y()-temp1.y())/line.y()<=1))//if intersectpoint is between temp1 and temp2
            {
                float deltax = intersectpoint.x() - center.x();
                float deltay = intersectpoint.y() - center.y();
                float deltaz = intersectpoint.z() - center.z();
                distance = std::sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);//just the distance between center and intersect point
            }
            else
            {
                float deltax = temp1.x() - center.x();
                float deltay = temp1.y() - center.y();
                float deltaz = temp1.z() - center.z();
                distance = std::sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
                deltax = temp2.x() - center.x();
                deltay = temp2.y() - center.y();
                deltaz = temp2.z() - center.z();
                distance = std::min(distance, std::sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz));
                //mindistance should be the minimum between the center and the two endpoints
            }
            if(mindistance >= distance)
            {
                mindistance = distance;
                edge = danwei;
                
            }
            //mindistance = std::min(mindistance, distance);
            
        }
        if(mindistance <= radius)
        {
            collisionEdge = edge;
            //std::cout<<collisionEdge.x()<<"  "<<collisionEdge.y()<<"\n";
            return true;
        }
        else
            return false;
    }
    bool detectCollision(Vec3f center, float radius)//use line equstion to get the intersect point and detect collision
    {
        
        float mindistance = std::numeric_limits<float>::max();
        for(int i = 0; i < number; i++)
        {
            int verticle1 = ex[i][0];
            int verticle2 = ex[i][1];
            float distance,m,n,m1,n1,a,b,c;
            Vec3f temp1 = m_verts[verticle1].getPtr();
            Vec3f temp2 = m_verts[verticle2].getPtr();
            //if the line is not k = 0 and not k = infinite
            if((temp1.x() - temp2.x())!=0 && (temp1.y() - temp2.y())!=0)
            {
                m = (temp1.y() - temp2.y())/(temp1.x() - temp2.x());
                n = temp1.y() - m * temp1.x();//y = mx + n
                a = m, b = -1, c = n;
                m1 = -1/m;
                n1 = center.y() - m1 * center.x();//y = m1x + n1
                float x;
                x = (n1 - n)/(m - m1);//intersection point
                //center project on the line
                if((x>=temp1.x() && x<=temp2.x()) || (x<=temp1.x()&&x>=temp2.x()))
                {
                    distance = std::abs(a*center.x()+b*center.y()+c)/sqrt(a*a+b*b);
                }
                //center projection not on the line
                else{
                    distance = std::min(std::sqrt((center.x()-temp1.x())*(center.x()-temp1.x())+(center.y()-temp1.y())*(center.y()-temp1.y())), std::sqrt((center.x()-temp2.x())*(center.x()-temp2.x())+(center.y()-temp2.y())*(center.y()-temp2.y())));
                }

                
            }
            //line is k = 0
            else if((temp1.x() - temp2.x())==0)
            {
                if((center.y()>=temp1.y() && center.y()<=temp2.y())|| (center.y()>=temp2.y() && center.y()<=temp1.y()))
                {
                    distance = std::abs(center.x() - temp1.x());
                }
                else
                {
                    distance = std::min(std::sqrt((center.x()-temp1.x())*(center.x()-temp1.x())+(center.y()-temp1.y())*(center.y()-temp1.y())), std::sqrt((center.x()-temp2.x())*(center.x()-temp2.x())+(center.y()-temp2.y())*(center.y()-temp2.y())));
                    
                }
            }
            //line is k = infinite
            else if((temp1.y() - temp2.y()) == 0)
            {
                if((center.x()>=temp1.x()&&center.x()<=temp2.x())||(center.x()>=temp2.x()&& center.x()<=temp1.x()))
                {
                    distance = std::abs(center.y() - temp1.y());
                }
                else
                {
                    distance = std::min(std::sqrt((center.x()-temp1.x())*(center.x()-temp1.x())+(center.y()-temp1.y())*(center.y()-temp1.y())), std::sqrt((center.x()-temp2.x())*(center.x()-temp2.x())+(center.y()-temp2.y())*(center.y()-temp2.y())));
                }
            }
            mindistance = std::min(mindistance,distance);
            
        }
        
        if(mindistance <= radius)
            return true;
        else
            return false;
        
    }
    
    Vec3f getCenter() /*const*/
    {
        float x = 0.0;
        float y = 0.0;
        float z = 0.0;
        for(int i = 0; i < m_verts.size(); i++)
        {
            x = x + m_verts[i].x();
            y = y + m_verts[i].y();
            z = z + m_verts[i].z();
      
        }
        
        m_center.set(x / m_verts.size(), y / m_verts.size(), z / m_verts.size());
        
//        
//        Vec3f temp1 = getMinBounding();
//        Vec3f temp2 = getMaxBounding();
//        float x = (temp1.x()+temp2.x())/2.0;
//        float y = (temp1.y()+temp2.y())/2.0;
//        float z = (temp1.z()+temp2.z())/2.0;
//        m_center.set(x, y, z);
        return m_center;
    }
    Vec3f getMinBounding() /*const*/
    {
        m_min_bounding.x() = std::numeric_limits<float>::max();
        m_min_bounding.y() = std::numeric_limits<float>::max();
        m_min_bounding.z() = std::numeric_limits<float>::max();
        for(int i = 0; i < m_verts.size();i++)
        {
            m_min_bounding.x() = std::min(m_min_bounding.x(), m_verts[i].x());
            m_min_bounding.y() = std::min(m_min_bounding.y(), m_verts[i].y());
            m_min_bounding.z() = std::min(m_min_bounding.z(), m_verts[i].z());
        }
        return m_min_bounding;
    }
    Vec3f getMaxBounding() /*const*/
    {
        m_max_bounding.x() = -999999.999;//std::numeric_limits<float>::min();
        m_max_bounding.y() = -999999.999;//std::numeric_limits<float>::min();
        m_max_bounding.z() = -999999.999;/*std::numeric_limits<float>::min();*/
        for(int i = 0; i < m_verts.size(); i++)
        {
            m_max_bounding.x() = std::max(m_max_bounding.x(), m_verts[i].x());
            m_max_bounding.y() = std::max(m_max_bounding.y(), m_verts[i].y());
            m_max_bounding.z() = std::max(m_max_bounding.z(), m_verts[i].z());
        }
        return m_max_bounding;
    }
    void translate(const Vec3f& o);
    void rotate(float z);
    
    std::vector<Vec3f>& getVertices() { return m_verts; }
    std::vector<std::vector<int> >& getPolygons() { return m_polys; }
    
    void update(Matrix m,int n);
    float getRadius()//radius as the farest distance between the verticles and the center
    {
        float radius = 0;
        Vec3f cter = getCenter();
        /*float xmax = getMaxBounding().x();
        float ymax = getMaxBounding().y();
        float zmax = getMaxBounding().z();
        
        float xmin = getMinBounding().x();
        float ymin = getMinBounding().y();
        float zmin = getMinBounding().z();
        float radius = std::sqrt((xmax - xmin)*(xmax - xmin) + (ymax-ymin)*(ymax - ymin) + (zmax - zmin)*(zmax - zmin))/2;*/
        for(int i = 0; i < m_verts.size();i++)
        {
            float d = std::sqrt((cter.x()-m_verts[i].x())*(cter.x()-m_verts[i].x())+(cter.y()-m_verts[i].y())*(cter.y()-m_verts[i].y()));
            radius = std::max(radius, d);
            
        }
        return radius;
    }
    void getExteriorEdge()//get the exterior edge of a model.
    {
        int  exedge[10000][2];
        int numbertemp = 0;
        for(int i = 0; i < m_polys.size(); i++)
        {
            int temp1 = m_polys[i].at(1);
            int temp2 = m_polys[i].at(2);
            int temp3 = m_polys[i].at(3);
            
            //std::cout<<temp1<<" "<<temp2<<" "<<temp3<<"\n";
            exedge[number][0] = temp1;
            exedge[number++][1] = temp2;
            exedge[number][0] = temp2;
            exedge[number++][1] = temp3;
            exedge[number][0] = temp3;
            exedge[number++][1] = temp1;
            
        }
        for(int i = 0; i < number; i++)
        {
            if(exedge[i][0] > exedge[i][1])
            {
                int temp;
                temp = exedge[i][0];
                exedge[i][0] = exedge[i][1];
                exedge[i][1] = temp;
            }
        }
        for(int i = 0; i < number; i++)
        {
            int sign = 0;
            for(int j = i + 1; j < number; j++)
            {
                if(exedge[i][0] == exedge[j][0]&& exedge[i][1] == exedge[j][1])
                {
                    exedge[j][0] = -1;
                    exedge[j][1] = -1;
                    sign = 1;
                }
            }
            if(sign == 1)
            {
                exedge[i][0] = -1;
                exedge[i][1] = -1;
            }
        }
        for(int i = 0; i < number; i++)
        {
            
            if(exedge[i][0]!=-1 && exedge[i][1]!=-1)
            {
                ex[numbertemp][0] = exedge[i][0];
                ex[numbertemp][1] = exedge[i][1];
                numbertemp++;
            }
        }
        number = numbertemp;
//        for(int i = 0; i < numbertemp; i++)
//        {
//            for(int j = 0; j < 2; j++)
//                std::cout<<ex[i][j];
//            std::cout<<" ";
//        }
        
    }
    Vec3f getDirectionVector()
    {
    
        Vec3f temp =m_verts[0] - getCenter();
        float length = std::sqrt(temp.x()*temp.x()+temp.y()*temp.y()+temp.z()*temp.z());
        Vec3f result;
        result.set(temp.x()/length, temp.y()/length, temp.z()/length);
        return result;
    }
    Vec3f getHorizonVector()
    {
        Vec3f temp = getDirectionVector();
        Matrix temp1(4,1);
        temp1.val[0][0] = temp.x();
        temp1.val[1][0] = temp.y();
        temp1.val[2][0] = temp.z();
        Matrix change(4,4);
        change.setIdentity(4, 4);
        change.val[0][0] = 0;
        change.val[0][1] = -1;
        change.val[1][0] = 1;
        change.val[1][1] = 0;
        change = change.mlt(temp1);
        temp.x() = change.val[0][0];
        temp.y() = change.val[1][0];
        temp.z() = change.val[2][0];
        return temp;
        
        
    }
    
protected:
    std::vector<Vec3f> m_verts;
    std::vector<std::vector<int> > m_polys;
    std::vector<Vec3f> m_tex;
    

    
    
    Vec3f m_center;
    Vec3f m_min_bounding;
    Vec3f m_max_bounding;
    
    Vec3f face_normal[10000];
    Vec3f vertex_normal[10000];
    };