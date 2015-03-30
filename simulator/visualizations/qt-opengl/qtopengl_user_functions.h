/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <argos2/simulator/visualizations/qtopengl_user_functions.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef QTOPENGL_USER_FUNCTIONS_H
#define QTOPENGL_USER_FUNCTIONS_H

namespace argos {
   class CQTOpenGLUserFunctions;
}

class QPainter;

#include <argos2/simulator/visualizations/qt-opengl/qtopengl_widget.h>
#include <argos2/common/utility/datatypes/color.h>
#include <argos2/common/utility/math/quaternion.h>

namespace argos {

   class CQTOpenGLUserFunctions {

   public:

      CQTOpenGLUserFunctions() :
         m_pcQTOpenGLWidget(NULL) {}

      virtual ~CQTOpenGLUserFunctions() {}

      /**
       * @brief Drawing hook executed after each booth is drawn.
       * When this function is called, the rototranslation to
       * place the booth has already been done. Therefore, all
       * drawing commands in this function are relative to the center of the cylinder.
       */
      inline virtual void Draw(CBoothEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after each cylinder is drawn.
       * When this function is called, the rototranslation to
       * place the cylinder has already been done. Therefore, all
       * drawing commands in this function are relative to the center of the cylinder.
       */
      inline virtual void Draw(CCylinderEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after each tile is drawn.
       * When this function is called, the rototranslation to
       * place the tile has already been done. Therefore, all
       * drawing commands in this function are relative to the center of the tile.
       */
      inline virtual void Draw(CTileEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after each foot-bot is drawn.
       * When this function is called, the rototranslation to
       * place the foot-bot has already been done. Therefore, all
       * drawing commands in this function are relative to the robot.
       */
      inline virtual void Draw(CFootBotEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after each eye-bot is drawn.
       * When this function is called, the rototranslation to
       * place the eye-bot has already been done. Therefore, all
       * drawing commands in this function are relative to the robot.
       */
      inline virtual void Draw(CEyeBotEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after each micro quadrotor is drawn.
       * When this function is called, the rototranslation to
       * place the quadrotor has already been done. Therefore, all
       * drawing commands in this function are relative to the robot.
       */
      inline virtual void Draw(CKumarMicroQuadrotorEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after each e-puck is drawn.
       * When this function is called, the rototranslation to
       * place the e-puck has already been done. Therefore, all
       * drawing commands in this function are relative to the robot.
       */
      inline virtual void Draw(CEPuckEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after each bluebot is drawn.
       * When this function is called, the rototranslation to
       * place the bluebot has already been done. Therefore, all
       * drawing commands in this function are relative to the robot.
       */
      inline virtual void Draw(CBluebotEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after the floor is drawn.
       */
      inline virtual void Draw(CFloorEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after all objects have been drawn.
       * Use this hook to draw your own stuff in the world.
       */
      inline virtual void DrawInWorld() {}

      /**
       * @brief Drawing hook to put graphics on top of the OpenGL window.
       * Extend this method to draw stuff on top of the 3D graphical window.
       * Use the methods in the Qt4 QPainter class to add all the
       * stuff you want, such as text, shapes and so on.
       * @param c_painter The QPainter object to draw the overlay.
       */
      inline virtual void DrawOverlay(QPainter& c_painter) {}

      inline CQTOpenGLWidget& GetOpenGLWidget() {
         return *m_pcQTOpenGLWidget;
      }

      inline void SetOpenGLWidget(CQTOpenGLWidget& c_widget) {
         m_pcQTOpenGLWidget = &c_widget;
      }

      /**
       * @brief Draws a triangle, parallel to the XY plane.
       * By default the triangle is equilateral (edge length: 0.2m), red, with the height along the X axis, positioned in the origin.
       * The triangle reference system is positioned in (h/2,b/2,0), with the X axis along the height of the triangle.
       * @param v_center_offset triangle's center offset with respect to the origin.
       * @param c_color color of the triangle.
       * @param b_fill when true (default) the triangle is filled with color
       * @param c_orientation defines the 3D rotation of the triangle
       * @param f_base base of the the triangle
       * @param f_height height of the triangle
       */
      virtual void DrawTriangle(const CVector3& c_center_offset = CVector3::ZERO,
                                const CColor& c_color = CColor::RED,
                                const bool b_fill = true,
                                const CQuaternion& c_orientation = CQuaternion(),
                                Real f_base = 0.2f,
                                Real f_height = 0.1732050808f);

      /**
       * @brief Draws a circle, parallel to the XY plane.
       * By default the circle is red, with a radius of 0.1m and positioned in the origin.
       * The circle reference system is positioned the center of the circle.
       * @param f_radius radius of the circle.
       * @param v_center_offset circle's center offset with respect to the origin.
       * @param c_color color of the circle.
       * @param b_fill when true (default) the circle is filled with color
       * @param c_orientation defines the 3D rotation of the circle
       * @param un_vertices number of vertices to be used for approximating the circle.
       */
      virtual void DrawCircle(Real f_radius = 0.1,
                              const CVector3& c_center_offset = CVector3::ZERO,
                              const CColor& c_color = CColor::RED,
                              const bool b_fill = true,
                              const CQuaternion& c_orientation = CQuaternion(),
                              GLuint un_vertices = 20);


      /**
       * @brief Draws a cylinder, with the height perpendicular to the XY plane.
       * By default the cylinder is red, with a radius of 0.1m, an height of 0.1m and positioned in the origin.
       * The cylinder reference system is positioned at half the way along the segment connecting the center of the 2 circular faces, with the z axis along the segment.
       * @param f_radius radius of the cylinder.
       * @param f_height height of the cylinder.
       * @param v_center_offset offset with respect to the origin where the circle will be drawn.
       * @param c_color color of the cylinder.
       * @param c_orientation defines the 3D rotation of the cylinder
       * @param un_vertices number of vertices to be used for approximating the circle.
       */
      virtual void DrawCylinder(Real f_radius=0.1,
                                Real f_height=0.1,
                                const CVector3& c_center_offset = CVector3::ZERO,
                                const CColor& c_color = CColor::RED,
                                const CQuaternion& c_orientation = CQuaternion(),
                                GLuint un_vertices = 20);


      /**
       * @brief Draws a segment, with optional endpoint markers.
       * By default the segment starts in the origin and ends in (1,0,1), it is drawn in red without end points markers. The end point markers default color is red.
       * @param c_end_point vector specifying the end point of the segment.
       * @param c_start_point vector specifying the start point of the segment.
       * @param c_segment_color color of the segment.
       * @param b_draw_end_point when true the end point of the segment is drawn.
       * @param b_draw_start_point when true the start point of the segment is drawn.
       * @param c_end_point_color color of the end point if drawn.
       * @param c_start_point_color color of the end point if drawn.
       */
      virtual void DrawSegment(const CVector3& c_end_point = CVector3(1.0,0.0,1.0),
                               const CVector3& c_start_point = CVector3::ZERO,
                               const CColor& c_segment_color = CColor::RED,
                               const Real& f_line_width = 1.0,
                               bool b_draw_end_point = false,
                               bool b_draw_start_point = false,
                               const CColor& c_end_point_color = CColor::RED,
                               const CColor& c_start_point_color = CColor::RED);

      /**
       * @brief Draws a poligon.
       * By default the poligon's color is red.
       * @param vec_points vector of vectors defining the vertices of the poligon
       * @param c_color color of the poligon.
       */
      virtual void DrawPoligon(const std::vector<CVector3>& vec_points,
                               const CColor& c_color = CColor::RED);


      /**
       * @brief Draws a point
       * By default the point is positioned in the origin, drawn in red.
       * @param c_position vector specifying the position of the point
       * @param c_color color of the point.
       * @param f_point_diameter diameter of the point.
       */
      virtual void DrawPoint(const CVector3& c_position = CVector3(1.0,0.0,1.0),
                             const CColor& c_color = CColor::RED,
                             const Real f_point_diameter  = 5.0);

   private:

      CQTOpenGLWidget* m_pcQTOpenGLWidget;

   };

}

/* Definitions useful for dynamic linking of user functions */

/* typedef to make it easier to set up controller factory */
typedef argos::CQTOpenGLUserFunctions *CQTOpenGLUserFunctions_Maker();

/* the actual controller factory */
extern std::map<std::string, CQTOpenGLUserFunctions_Maker *, std::less<std::string> > mapQTOpenGLUserFunctionFactory;

/* to add the controller to the factory */
#define REGISTER_QTOPENGL_USER_FUNCTIONS(class_name, label)             \
   extern "C" {                                                         \
      CQTOpenGLUserFunctions* class_name ## _maker(){                   \
         return new class_name;                                         \
      }                                                                 \
                                                                        \
      class class_name ## _proxy {                                      \
      public:                                                           \
      class_name ## _proxy(){                                           \
         LOG << "[INFO] Registering QTOpenGL user function <" #class_name ", " label ">" << std::endl; \
         mapQTOpenGLUserFunctionFactory[ label ] = class_name ## _maker; \
      }                                                                 \
      };                                                                \
      class_name ## _proxy class_name ## _p;                            \
   }

#endif
