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
 * @file <simulator/visualizations/qtopengl_render.cpp>
 *
 * @brief This file contains the definition of the CRender interface.
 *
 * This file contains the definition of the CRender interface. Such interface
 * is the base for all the visualisations (OpenGL, OGRE, gnuplot, blender,
 * etc.) in ARGOS.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "qtopengl_render.h"

#include <argos2/common/utility/logging/argos_log.h>
#include <argos2/common/utility/string_utilities.h>
#include <cstring>

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

namespace argos {

   /****************************************/
   /****************************************/

   void CQTOpenGLRender::Init(TConfigurationNode& t_tree) {
      /* Parse options from the XML */
      /* Splash screen */
      GetNodeAttributeOrDefault(t_tree, "splash", m_bShowSplashScreen, true);
      /* Intel fix */
      bool bIntelFix;
      GetNodeAttributeOrDefault(t_tree, "intel_fix", bIntelFix, false);
      /* Save the configuration for later */
      m_tConfTree = t_tree;
      /* Set up dummy arguments for QApplication */
      m_nOptionNum = 1;
      if(bIntelFix) {
         m_nOptionNum += 2;
      }
      m_ppcOptions = new char*[m_nOptionNum];
      m_ppcOptions[0] = new char[6];
      ::strcpy(m_ppcOptions[0], "argos");
      if(bIntelFix) {
         m_ppcOptions[1] = new char[16];
         ::strcpy(m_ppcOptions[1], "-graphicssystem");
         m_ppcOptions[2] = new char[7];
         ::strcpy(m_ppcOptions[2], "raster");
      }
      /* Create the QT application */
      m_pcApplication = new QApplication(m_nOptionNum, m_ppcOptions);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLRender::Execute() {
      try {
         /* Set some data about the application */
         m_pcApplication->setApplicationName("ARGoS");
         m_pcApplication->setApplicationVersion("2.0");
         m_pcApplication->setOrganizationName("Iridia-ULB");
         m_pcApplication->setOrganizationDomain("iridia.ulb.ac.be");
         /* Draw the splash screen, if needed */
         QSplashScreen* pcSplashScreen = NULL;
         if(m_bShowSplashScreen) {
            std::string strSTDBaseDirectory(CSimulator::GetInstance().GetInstallationDirectory());
            strSTDBaseDirectory += "/simulator/visualizations/qt-opengl/resources/";
            QString strBaseDirectory(strSTDBaseDirectory.c_str());
            QPixmap cPixmap(strBaseDirectory + "argos_logo.png");
            pcSplashScreen = new QSplashScreen(cPixmap);
            pcSplashScreen->show();
         }
         /* Draw the main window */
         m_pcMainWindow = new CQTOpenGLMainWindow(m_tConfTree);
         m_pcMainWindow->show();
         LOG.Flush();
         LOGERR.Flush();
         /* This effectively starts the experiment */
         m_pcApplication->exec();
         /* Dispose of the splash screen */
         if(m_bShowSplashScreen) {
            pcSplashScreen->finish(m_pcMainWindow);
            delete pcSplashScreen;
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while executing the experiment.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLRender::Destroy() {
      /* Destroy the QT application */
      delete m_pcMainWindow;
      delete m_pcApplication;
   }

   /****************************************/
   /****************************************/

   REGISTER_RENDER(CQTOpenGLRender,
                   "qtopengl_render",
                   "An interactive graphical renderer based on QT and OpenGL",
                   "Carlo Pinciroli [cpinciro@ulb.ac.be]",
                   "The QT-OpenGL renderer is a graphical renderer based on QT >= 4.5 and OpenGL.\n"
                   "It allows the user to watch and modify the simulation as it's running in an\n"
                   "intuitive way.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <visualizations>\n"
                   "    <qtopengl_render />\n"
                   "  </visualizations>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "The <qtopengl_render> tag has two optional attributes: 'id' and 'splash'.\n"
                   "The attribute 'id' is ignored. It is there for compatibility with the plugin\n"
                   "system, but it's never used.\n"
                   "The attribute 'splash' can take as value 'true' or 'false'. When 'true' is\n"
                   "specified, a splash screen is shown while ARGoS is loading. When 'false', the\n"
                   "splash screen is not shown. The default, if the attribute is unspecified, is\n"
                   "'true'. Example:\n\n"
                   "  <visualizations>\n"
                   "    <qtopengl_render splash=\"false\"/>\n"
                   "  </visualizations>\n\n"
                   "Furthermore, it is possible to set some camera parameters. There are 10 available\n"
                   "cameras to use. You can switch from one to the other by clicking on the\n"
                   "graphical view (to give it focus) and then pressing the keys 0-9.\n"
                   "To configure position and orientation of specific cameras, say cameras 0 to 3,\n"
                   "you have to include the following XML code:\n\n"
                   "  <visualizations>\n"
                   "    <qtopengl_render>\n"
                   "      <camera>\n"
                   "        <placement idx=\"0\" position=\"2,2,2\" look_at=\"1,1,1\" />\n"
                   "        <placement idx=\"1\" position=\"1,0,7\" look_at=\"1,0,0\" />\n"
                   "        <placement idx=\"2\" position=\"3,3,4\" look_at=\"1,6,0\" />\n"
                   "        <placement idx=\"3\" position=\"2,3,2\" look_at=\"0,1,0\" />\n"
                   "      </camera>\n"
                   "    </qtopengl_render>\n"
                   "  </visualizations>\n\n"
                   "The 'idx' attribute specifies the camera index (and the key to press to switch\n"
                   "to that camera).\n"
                   "The 'position' attribute contains the position of the camera in the arena.\n"
                   "The 'look_at' attribute sets the point the camera is looking at.\n"
                   "It also possible to set some optical parameters of real cameras, namely the\n"
                   "focal length and the length of the frame diagonal. For example:\n\n"
                   "  <visualizations>\n"
                   "    <qtopengl_render>\n"
                   "      <camera>\n"
                   "        ...\n"
                   "        <placement idx=\"4\"\n"
                   "                   position=\"4,1,4\"\n"
                   "                   look_at=\"2,1,0\"\n"
                   "                   lens_focal_length=\"50\"\n"
                   "                   frame_diagonal=\"40\" />\n"
                   "        ...\n"
                   "      </camera>\n"
                   "    </qtopengl_render>\n"
                   "  </visualizations>\n\n"
                   "The 'lens_focal_length' attribute controls the focal length of the lens of the\n"
                   "simulated camera. The value is in millimeters and it defaults, if not set in\n"
                   "XML, to 20mm.\n"
                   "The 'frame_diagonal' attribute specifies the length of the frame diagonal of\n"
                   "the image film. The value is in millimeters and it defaults, if not set in\n"
                   "XML, to 35mm.\n"
                   "This visualization also allows for user customization. In a similar fashion to\n"
                   "the loop functions, you can set a plug-in that derives from the\n"
                   "CQTOpenGLUserFunctions class. To load it in the system, follow this example:\n\n"
                   "  <visualizations>\n"
                   "    <qtopengl_render>\n"
                   "      <user_functions library=\"/path/to/libmyuserfunctions.so\"\n"
                   "                      label=\"my_user_functions\" />\n"
                   "    </qtopengl_render>\n"
                   "  </visualizations>\n\n"
                   "The 'library' attribute points to the library where the user functions are\n"
                   "stored. This library can be the same as the loop functions, or a new one.\n"
                   "There is no limitation to where the code is to be found.\n"
                   "The 'label' attribute identifies the user function class to use. In this way,\n"
                   "in a single library you can have multiple user function implementations, if\n"
                   "you wish.\n"
                   "You can also grab frames and store them into image files, for example to create\n"
                   "videos in a fast way. To do it, you just need to press the red capture button\n"
                   "and frame grabbing will be on. By default, the frames are named\n"
                   "'frame_NNNNN.png' and are stored in the current directory, i.e. the directory\n"
                   "where you run the 'argos' command. If you want to override this behavior, you\n"
                   "can add the optional 'frame_grabbing' section as follows:\n\n"
                   "  <visualizations>\n"
                   "    <qtopengl_render>\n"
                   "      <frame_grabbing directory=\"frames\"\n"
                   "                      base_name=\"myframe_\"\n"
                   "                      format=\"png\"\n"
                   "                      quality=\"100\" />\n"
                   "    </qtopengl_render>\n"
                   "  </visualizations>\n\n"
                   "All the attributes in this section are optional. If you don't specify one of\n"
                   "them, the default is taken.\n"
                   "The 'directory' attribute stores the directory where the frames are saved. If\n"
                   "the directory does not exist, a fatal error occurs. The directory must exist\n"
                   "and be writable. Both absolute and relative paths are allowed. The default\n"
                   "value is '.'\n"
                   "The 'base_name' attribute is the string to prepend to the file name. After this\n"
                   "string, the frame number (padded to 5 digits) is added. The default value is\n"
                   "'frame_', so a typical resulting name is 'frame_00165'.\n"
                   "The 'format' attribute specifies the format. The default value is 'png' but you\n"
                   "can put any format supported by Qt>=4.5. Refer to the Qt documentation for the\n"
                   "complete list of supported formats.\n"
                   "The 'quality' attribute dictates the quality of the image. Its value is in the\n"
                   "range [0:100] where 0 means maximum compression and minimum quality, and 100\n"
                   "means maximum quality and no compression at all. The default value is '-1',\n"
                   "which means to use Qt's default quality. For videos, it's best to use 100 to\n"
                   "avoid artifacts due to compression. For a normal screenshot, the default is the\n"
                   "safest choice.\n",
                   "Under development"
      );

}
