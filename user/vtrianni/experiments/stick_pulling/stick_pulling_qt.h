#ifndef STICKPULLING_QTOPENGL_USER_FUNCTIONS_H
#define STICKPULLING_QTOPENGL_USER_FUNCTIONS_H

#include <argos2/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>

using namespace argos;

class CStickPullingQt : public CQTOpenGLUserFunctions {

public:
   CStickPullingQt();
   virtual ~CStickPullingQt();

   virtual void Draw(CFootBotEntity& c_entity);

};

#endif
