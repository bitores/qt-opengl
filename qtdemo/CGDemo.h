#ifndef CGDemo_h__
#define CGDemo_h__

#include "../Common/EnGLWidget.h"

class CGDemo : public EnGLWidget
{
	Q_OBJECT
public:
	CGDemo(EnGLWidget* parent = NULL);
	~CGDemo();

protected:
	void EnGLInit();
	void EnGLFrame();
	void EnGLRender();

public:
    float rTri;
    float rQuads;
};
#endif // CGDemo_h__

