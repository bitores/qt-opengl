
/* @All Right Reserved (C), 2008-2108, OpenRs.org.*/

//!
/*!
\author   : Enel_TS
\date     : 2010/05/23
\version  : ver 1.0
\filepath : e:\CgSDK\CGExample\CGExample
\filename : EnGLWidget.h
\note     : GLWidget

\attention:

\remark   : 1.2010/05/23 10 : 58 created by Enel_TS version 1.0
\History:
   <author>      <time>      <version>         <desc>
*/


#ifndef EnGLWidget_h__
#define EnGLWidget_h__

#include <qt_windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <QWidget>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glaux.lib")

class EnGLWidget : public QWidget
{
    Q_OBJECT
public:
    EnGLWidget(QWidget* parent = NULL); //构造函数
    QPaintEngine *paintEngine() const;	//渲染位置

    BOOL WGLInit();						//初始化Wgl
    BOOL GLInit();						//初始化openGL
    void GLFrame();						//刷帧
    void GLRender();					//渲染
    void GLResize(QSize size);			//改变窗口大小
    BOOL GLRelease();					//释放OpenGL

    virtual void EnGLFrame();
    virtual void EnGLRender();

    void setFrequency(int count);

protected:
    virtual ~EnGLWidget();				//析构函数
    void timerEvent(QTimerEvent* );
    void paintEvent(QPaintEvent* );
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);

private:
    HDC			hDC;	// Private GDI Device Context
    HGLRC		hRC;	// Permanent Rendering Context
    HWND		hWnd;

    int         mFrequency;
};


#endif // EnGLWidget_h__
