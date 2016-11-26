#include "CGDemo.h"
#include "gl/glut.h"
#include <iostream>

#pragma comment(lib,"glut32.lib")

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

void CGDemo::EnGLInit()
{
    rTri = 0;
    rQuads = 0;
//    glClearColor(1,0,0,1);
    std::cout<<"init"<<std::endl;
}

void CGDemo::EnGLFrame()
{
    std::cout<<"EnGLRender"<<std::endl;
}

void CGDemo::EnGLRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear Screen And Depth Buffer
    glShadeModel(GL_SMOOTH);
    glLoadIdentity();                               // Reset The Current Modelview Matrix
    glTranslatef(-1.5f,0.0f,-6.0f);//物体左移1.5,向内移6,相当于移动镜头一样，让物体进入镜头中
    glRotatef(rTri,0.0f,1.0f,0.0f);
    glBegin(GL_TRIANGLES);                            // 绘制三角形
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);                    // 上顶点
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);                    // 左下
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);                    // 右下
    glEnd();                                // 三角形绘制结束
    glShadeModel(GL_FLAT);
    glLoadIdentity();
    glTranslatef(1.5f,0.0f,-6.0f);
    glRotatef(rQuads,1.0f,0.0f,0.0f);
    glBegin(GL_QUADS);                            //  绘制正方形
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);                    // 左上
        glColor3f(1.0,1.0f,1.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);                    // 右下

        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);                    // 左下
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);                    // 右上
    glEnd();                                // 正方形绘制结束
    glFlush();
    rTri+=1.2f;
    if(rTri>=360.0f)
    {
        rTri-=360.0f;
    }
    rQuads-=1.15f;
    if(rQuads<=-360.0f)
    {
        rQuads+=360.0f;
    }

    std::cout<<"frame "<<width()<<":"<<height()<<std::endl;

}

CGDemo::CGDemo( EnGLWidget* parent ) : EnGLWidget(parent)
{
    EnGLInit();
}

CGDemo::~CGDemo()
{

}


