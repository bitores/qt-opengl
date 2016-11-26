#include "EnGLWidget.h"

#include <QtGUI>
#include <qmessagebox.h>
//////////////////////////////////////////////////////////////////////////
//初始化openGL
BOOL EnGLWidget::GLInit()
{
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    startTimer(mFrequency);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//刷帧
void EnGLWidget::GLFrame()
{
    EnGLFrame();
}

//////////////////////////////////////////////////////////////////////////
//渲染
void EnGLWidget::GLRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    glLoadIdentity();									// Reset The Current Modelview Matrix

    EnGLRender();
}

/////////////////////////////////////////////////////////////////
/// \brief EnGLWidget::setFrequency
/// \param count
///设置刷新频率
void EnGLWidget::setFrequency(int count)
{
    mFrequency = count;
}

//////////////////////////////////////////////////////////////////////////
//构造
EnGLWidget::EnGLWidget( QWidget* parent /*= NULL*/ )
: QWidget(parent),hDC(NULL),hRC(NULL),mFrequency(30)
{
    hWnd = (HWND)winId();

    setAttribute(Qt::WA_PaintOnScreen, true);

    // 1.Set Up Out wgl with HRC
    if (!WGLInit())
    {
        QMessageBox::warning(this, tr("Error"),tr("WGLInit()"),QMessageBox::Ok);
    }

    // 2.Set Up Our Perspective GL Screen
    GLResize(size());

    // 3.Initialize Our Newly Created GL Window
    if (!GLInit())
    {
        QMessageBox::warning(this, tr("Error"),tr("GLInit()"),QMessageBox::Ok);
    }
}

//////////////////////////////////////////////////////////////////////////
//析构
EnGLWidget::~EnGLWidget()
{
    GLRelease();
}

/////////////////////////////////////////////////////////////////////////
/// \brief EnGLWidget::timerEvent
///定时刷新
void EnGLWidget::timerEvent(QTimerEvent* )
{
    update();
}

//////////////////////////////////////////////////////////////////////////
//渲染循环
void EnGLWidget::paintEvent( QPaintEvent* )
{
    if (updatesEnabled())
    {
        GLFrame();
        GLRender();
        SwapBuffers(hDC);
    }
}

//////////////////////////////////////////////////////////////////////////
//渲染引擎空实现.说明不用Qt渲染引擎渲染
QPaintEngine * EnGLWidget::paintEngine() const
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
//改变窗口大小事件
void EnGLWidget::resizeEvent( QResizeEvent * e)
{
    GLResize(e->size());
    QWidget::resizeEvent(e);
}

//////////////////////////////////////////////////////////////////////////
//关闭窗口事件
void EnGLWidget::closeEvent( QCloseEvent * e)
{
    GLRelease();
    QWidget::closeEvent(e);
}

//////////////////////////////////////////////////////////////////////////
//改变窗口大小函数
void EnGLWidget::GLResize( QSize size )
{
    int height = size.height();
    int width = size.width();

    if (height==0)										// Prevent A Divide By Zero By
    {
        height=1;										// Making Height Equal One
    }

    glViewport(0,0,(GLsizei)width,(GLsizei)height);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();									// Reset The Modelview Matrix
}

BOOL EnGLWidget::WGLInit()
{
    GLuint PixelFormat;			// Holds The Results After Searching For A Match
    static PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
        1,											// Version Number
        PFD_DRAW_TO_WINDOW |						// Format Must Support Window
        PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
        PFD_DOUBLEBUFFER,							// Must Support Double Buffering
        PFD_TYPE_RGBA,								// Request An RGBA Format
        32,										// Select Our Color Depth
        0, 0, 0, 0, 0, 0,							// Color Bits Ignored
        0,											// No Alpha Buffer
        0,											// Shift Bit Ignored
        0,											// No Accumulation Buffer
        0, 0, 0, 0,									// Accumulation Bits Ignored
        16,											// 16Bit Z-Buffer (Depth Buffer)
        0,											// No Stencil Buffer
        0,											// No Auxiliary Buffer
        PFD_MAIN_PLANE,								// Main Drawing Layer
        0,											// Reserved
        0, 0, 0										// Layer Masks Ignored
    };

    if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
    {
        QMessageBox::warning(this, tr("Error"),tr("GetDC()"),QMessageBox::Ok);
        return false;
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
    {
        QMessageBox::warning(this, tr("Error"),tr("ChoosePixelFormat()"),QMessageBox::Ok);
        return false;
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
    {
        QMessageBox::warning(this, tr("Error"),tr("SetPixelFormat()"),QMessageBox::Ok);
        return false;
    }

    if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
    {
        QMessageBox::warning(this, tr("Error"),tr("wglCreateContext()"),QMessageBox::Ok);
        return false;
    }

    if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
    {
        QMessageBox::warning(this, tr("Error"),tr("wglMakeCurrent()"),QMessageBox::Ok);
        return false;
    }

    ShowWindow(hWnd,SW_SHOW);						// Show The Window
    SetForegroundWindow(hWnd);						// Slightly Higher Priority
    SetFocus(hWnd);									// Sets Keyboard Focus To The Window

    return true;
}

BOOL EnGLWidget::GLRelease()
{
    if (hRC)											// Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
        {
            QMessageBox::warning(this, tr("Error"),tr("wglMakeCurrent()"),QMessageBox::Ok);
        }

        if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
        {
            QMessageBox::warning(this, tr("Error"),tr("wglDeleteContext()"),QMessageBox::Ok);
        }
        hRC=NULL;										// Set RC To NULL
    }

    if (hDC)
    {
        if (!ReleaseDC(hWnd,hDC))
        {
            QMessageBox::warning(this, tr("Error"),tr("ReleaseDC()"),QMessageBox::Ok);
        }
        hDC=NULL;										// Set DC To NULL
    }
    return true;
}

void EnGLWidget::EnGLFrame()
{

}

void EnGLWidget::EnGLRender()
{

}
