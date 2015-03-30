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
 * @file <simulator/main.cpp>
 *
 * @brief This file provides the standard main() function to run the ARGoS
 * simulator.
 *
 * In this file the standard main() function is defined. It provides the
 * basic functionalities to run the ARGoS simulator: parses the command line,
 * loads the experiment, runs the simulation and disposes all the data.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include <argos2/simulator/simulator.h>
#include <argos2/simulator/factories/query_factories.h>
#include <argos2/simulator/argos_command_line_arg_parser.h>
#include <cstdlib>

using namespace argos;

/**
 * @brief The standard main() function to run the ARGoS simulator.
 *
 * This main() function provides tha basic functionalities to run the ARGoS
 * simulator: parses the command line, loads the experiment, runs the
 * simulation and disposes all the data.
 *
 * @param argc the number of command line arguments given at the shell.
 * @param argv the actual command line arguments.
 *
 * @return 0 if everything OK; 1 in case of errors.
 *
 */
int main(int n_argc, char** ppch_argv) {
   /* Create a new instance of the simulator */
   CSimulator& cSimulator = CSimulator::GetInstance();
   /* Configure the command line options */
   CARGoSCommandLineArgParser cACLAP(cSimulator);

   try {
      /* Check the presence of the environment variable ARGOSINSTALLDIR */
      char* pchARGoSInstallDir = ::getenv("ARGOSINSTALLDIR");
      if (pchARGoSInstallDir == NULL) {
         THROW_ARGOSEXCEPTION("Unable to find the required environment variable \"ARGOSINSTALLDIR\"");
      }
      cSimulator.SetInstallationDirectory(pchARGoSInstallDir);

      /* Parse command line */
      cACLAP.Parse(n_argc, ppch_argv);

      switch(cACLAP.GetAction()) {
         case CARGoSCommandLineArgParser::ACTION_RUN_EXPERIMENT:
            cSimulator.SetExperimentFileName(cACLAP.GetExperimentConfigFile());
            cSimulator.LoadExperiment();
            cSimulator.Execute();
            break;

         case CARGoSCommandLineArgParser::ACTION_QUERY:
            QueryPlugins(cACLAP.GetQuery());
            break;

         case CARGoSCommandLineArgParser::ACTION_SHOW_HELP:
            cACLAP.PrintUsage(LOG);
            break;

         case CARGoSCommandLineArgParser::ACTION_UNKNOWN:
            /* Should never get here */
            break;
      }

      /* Done, destroy stuff */
      cSimulator.Destroy();
   }
   catch(std::exception& ex) {
      /* A fatal error occurred: dispose of data, print error and exit */
      cSimulator.Destroy();
      LOGERR << "[FATAL] " << ex.what() << std::endl;
      LOG.Flush();
      LOGERR.Flush();
      return 1;
   }
   /* Everything's ok, exit */
   return 0;
}
