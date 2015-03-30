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
 * @brief This file provides the standard main() function to run the AHSS
 * simulator.
 *
 * In this file the standard main() function is defined. It provides the
 * basic functionalities to run the AHSS simulator: parses the command line,
 * loads the experiment, runs the simulation and disposes all the data.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include <argos2/simulator/factories/query_factories.h>
#include <argos2/simulator/simulator.h>
#include <argos2/simulator/dynamic_linking/loop_functions.h>
#include <argos2/simulator/argos_command_line_arg_parser.h>

// #include <google/profiler.h>
// #include <sys/time.h>

using namespace argos;
using namespace std;

/**
 * @brief The standard main() function to run the AHSS simulator.
 *
 * This main() function provides tha basic functionalities to run the AHSS
 * simulator: parses the command line, loads the experiment, runs the
 * simulation and disposes all the data.
 *
 * @param argc the number of command line arguments given at the shell.
 * @param argv the actual command line arguments.
 *
 * @return 0 if everything OK; -1 in case of errors.
 *
 */
int main(int argc, char** argv) {
   CSimulator& cSimulator = CSimulator::GetInstance();
   try {

      /* Check the presence of the environment variable ARGOSINSTALLDIR */
      char* pchARGoSInstallDir = ::getenv("ARGOSINSTALLDIR");
      if (pchARGoSInstallDir == NULL) {
         THROW_ARGOSEXCEPTION("Unable to find the required environment variable \"ARGOSINSTALLDIR\"");
      }
      cSimulator.SetInstallationDirectory(pchARGoSInstallDir);


      /* Configure the command line options */
      CARGoSCommandLineArgParser cACLAP(cSimulator);
      /* Parse command line */
      cACLAP.Parse(argc, argv);

      switch(cACLAP.GetAction()) {
         case CARGoSCommandLineArgParser::ACTION_RUN_EXPERIMENT:
            cSimulator.SetExperimentFileName(cACLAP.GetExperimentConfigFile());
            cSimulator.LoadExperiment();
	    //       ProfilerStart("argos.prof");
	    //       struct timeval t1, t2;
	    //       gettimeofday(&t1, NULL);
            cSimulator.Execute();
	    //       gettimeofday(&t2, NULL);
	    //       LOG << "elapsed time = " << (Real)((t2.tv_sec - t1.tv_sec)*1000000 + (t2.tv_usec - t1.tv_usec))/(Real)1000000 << endl;
	    //       ProfilerStop();
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

      UInt32 num_components = 0;
      Real*  components     = NULL;
      Real   fitness        = cSimulator.GetLoopFunctions().ComputePerformanceInExperiment( &num_components, &components );
      LOG << "Fitness obtained: " << fitness;
      for( UInt32 i = 0; i < num_components; ++i ) {
	 LOG << " " << components[i];
      }
      LOG << endl;
      if( components ) delete [] components;

      cSimulator.Destroy();
   }
   catch(std::exception& ex) {
      /* A fatal error occurred: dispose of data, print error and exit */
      cSimulator.Destroy();
      LOGERR << "[FATAL] " << ex.what() << std::endl;
      LOG.Flush();
      LOGERR.Flush();
      return -1;
   }
   /* Everything's ok, exit */
   return 0;
}
