#ifndef FLOCKING_QTOPENGL_USER_FUNCTIONS_H
#define FLOCKING_QTOPENGL_USER_FUNCTIONS_H

#include <argos2/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>

using namespace argos;

class CFlockingQt : public CQTOpenGLUserFunctions {

public:
   CFlockingQt();
   virtual ~CFlockingQt();

   virtual void Draw(CFootBotEntity& c_entity);

   virtual void DrawInWorld();

private:
   CVector3 c_com_position;

};

#endif
