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
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */

#include <argos2/user/vtrianni/more/evolution.h>

#include <argos2/simulator/argos_command_line_arg_parser.h>
#include <argos2/simulator/simulator.h>
#include <argos2/user/vtrianni/loop_functions/evolutionary_lf/evolutionary_lf.h>

#include <iostream>
#include <limits.h>

#include <mpi.h>


using namespace argos;
using namespace std;


class CObjectives {
private:
   UInt32       m_unNumObjectives;
   Real         m_fFitness;
   vector<Real> m_vecObjectives;
   
public:
   CObjectives() {
      m_unNumObjectives = 0;
      m_fFitness = 0;
   }

   CObjectives( const UInt32& num_objectives ) {
      m_unNumObjectives = num_objectives;
      m_fFitness = 0;
      if( m_unNumObjectives > 0 ) {
	 m_vecObjectives.resize( num_objectives ); 
	 std::fill( m_vecObjectives.begin(), m_vecObjectives.end(), 0.0 );
      }
   }

   CObjectives( const Real& f_fitness, const UInt32& num_objectives, const Real* pf_objectives ) {
      m_unNumObjectives = num_objectives;
      m_fFitness = f_fitness;
      if( m_unNumObjectives > 0 ) {
	 m_vecObjectives.resize( num_objectives );
	 std::copy( pf_objectives, pf_objectives + num_objectives, m_vecObjectives.begin() );
      }
   }

   CObjectives( const CObjectives& c_obj ) {
      m_unNumObjectives = c_obj.m_unNumObjectives;
      m_fFitness        = c_obj.m_fFitness;
      if( m_unNumObjectives > 0 ) {
	 m_vecObjectives.resize( m_unNumObjectives );
	 std::copy( c_obj.m_vecObjectives.begin(), c_obj.m_vecObjectives.end(), m_vecObjectives.begin() );
      }
   }

   CObjectives& operator =( const CObjectives& c_obj ) {
      m_unNumObjectives = c_obj.m_unNumObjectives;
      m_fFitness        = c_obj.m_fFitness;
      if( m_unNumObjectives > 0 ) {
	 m_vecObjectives.resize( m_unNumObjectives );
	 std::copy( c_obj.m_vecObjectives.begin(), c_obj.m_vecObjectives.end(), m_vecObjectives.begin() );
      }
      return *this;
   }

   CObjectives& operator +=( const CObjectives& c_obj ) {
      m_fFitness += c_obj.m_fFitness;
      for( UInt32 i = 0; i < m_unNumObjectives; ++i ) {
	 m_vecObjectives[i] += c_obj.m_vecObjectives[i];
      }
      return *this;
   }

   CObjectives& operator *=( const Real& factor ) {
      m_fFitness *= factor;
      for( UInt32 i = 0; i < m_unNumObjectives; ++i ) {
	 m_vecObjectives[i] *= factor;
      }
      return *this;
   }

   CObjectives& operator /=( const Real& factor ) {
      *this *= 1/factor;
      return *this;
   }

   inline void SetNumObjectives( const UInt32& un_num_obj )  { m_unNumObjectives = un_num_obj; m_vecObjectives.resize( m_unNumObjectives ); }
   inline UInt32 GetNumObjectives() const { return m_unNumObjectives; }

   inline void SetFitness( const Real& f_fitness )  { m_fFitness = f_fitness; }
   inline Real GetFitness() const { return m_fFitness; }

   inline void SetObjective( const UInt32& index, const Real& obj ) { m_vecObjectives[index] = obj; }
   inline Real GetObjective( const UInt32& index ) const { return m_vecObjectives[index]; }

   inline void GetObjectives( Real** pf_objectives ) {
      std::copy( m_vecObjectives.begin(), m_vecObjectives.end(), *pf_objectives );
   }

   friend ostream& operator <<( ostream& os, const CObjectives& obj ) {
      os << obj.m_fFitness << " " << obj.m_unNumObjectives << " ";
      for( UInt32 i = 0; i < obj.m_unNumObjectives; ++i ) {
	 os << obj.m_vecObjectives[i] << " ";
      }
      return os;
   }

};


/**
 * This function computes the mean for a vector of objectives
 */
void ObjectivesMean( const vector<CObjectives>& vec_objectives, CObjectives& mean ) {
   for( UInt32 i = 0; i < vec_objectives.size(); ++i ) {
      mean += vec_objectives[i];
   }
   mean /= vec_objectives.size();
}


/**
 * This function dumps a vector of objectives into a file
 */
void DumpObjectives( const vector<CObjectives>& vec_objectives, const string& filename ) {
   ofstream out;
   out.open( filename.c_str(), ios::out );
   for( UInt32 i = 0; i < vec_objectives.size(); ++i ) {
      out << vec_objectives[i] << endl;
   }
   out.close();
}


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
   ////////////////////////////////////////////////////////////////////////////////
   // set argos isntall dir
   ////////////////////////////////////////////////////////////////////////////////
   setenv("ARGOSINSTALLDIR", "/home/vtrianni/Work/argos/", 1);

   CSimulator& cSimulator = CSimulator::GetInstance();

   ////////////////////////////////////////////////////////////////////////////////
   // parse the command line
   ////////////////////////////////////////////////////////////////////////////////

   // fitness base file
   string s_fit_file = "";

   // evolution/post-evaluation flag
   bool bPosteval = false;


   // define the class for parsing the command line
   CARGoSCommandLineArgParser cCommandLineArgs(cSimulator);
   
   // argument -f - fitness file
   cCommandLineArgs.AddArgument<std::string>('f',
					     "--fitness",
					     "Specify the fitness file",
					     s_fit_file);
   
   // argument -p - evolution/post-evaluation flag
   cCommandLineArgs.AddFlag('p',
			    "--post-evaluation",
			    "Specify the flag for evolution or post-evaluation",
			    bPosteval);
   

   ////////////////////////////////////////////////////////////////////////////////
   // Build and initialise the simulator according to the command line arguments
   ////////////////////////////////////////////////////////////////////////////////
   try {
      /* Parse command line */
      cCommandLineArgs.Parse(argc, argv);
      cSimulator.SetExperimentFileName(cCommandLineArgs.GetExperimentConfigFile());
      chdir( cSimulator.GetInstallationDirectory().c_str() );
      cSimulator.LoadExperiment();
   }
   catch(std::exception& ex) {
      /* A fatal error occurred: dispose of data, print error and exit */
      cSimulator.Destroy();
      LOGERR << "[FATAL] " << ex.what() << std::endl;
      LOG.Flush();
      LOGERR.Flush();
      return -1;
   }


   ////////////////////////////////////////////////////////////////////////////////
   // Initialise parallel environment
   ////////////////////////////////////////////////////////////////////////////////
   MPI::Intercomm parent_comm;
   try{
      MPI::Init();       
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Initialization error of MPI parallel environment", ex);
      SInt32 errorcode;
      MPI::COMM_WORLD.Abort(errorcode);
   }
   parent_comm = MPI::Comm::Get_parent();
   

   ////////////////////////////////////////////////////////////////////////////////
   // Initialise random number generation
   ////////////////////////////////////////////////////////////////////////////////
   ostringstream category;
   category << "invalid" << MPI::COMM_WORLD.Get_rank();
   CARGoSRandom::CreateCategory( category.str(), 1 );
   CARGoSRandom::CRNG* pc_RNG = CARGoSRandom::CreateRNG( category.str() );
   
   ////////////////////////////////////////////////////////////////////////////////
   // Start individual evaluation
   ////////////////////////////////////////////////////////////////////////////////
   while( true ) {
      SInt32  n_individual_number = -1;
      UInt32  un_genotype_length = 0;
      Real*   pf_genotype = NULL;
      UInt32  un_num_samples = 0;
      UInt32  un_evaluation_seed = 0;
      UInt32* pun_sample_seeds = NULL;
      UInt32  un_num_objectives = 0;

      // blocking call - waiting for the parent to send individual information
      parent_comm.Recv( &n_individual_number, 1, MPI_INT, 0, 1);
      parent_comm.Recv( &un_genotype_length, 1, MPI_INT, 0, 1);
      pf_genotype = new Real[un_genotype_length];
      parent_comm.Recv( pf_genotype, un_genotype_length, MPI_ARGOSREAL, 0, 1);
      parent_comm.Recv( &un_num_samples, 1, MPI_INT, 0, 1);
      parent_comm.Recv( &un_evaluation_seed, 1, MPI_INT, 0, 1);
      parent_comm.Recv( &un_num_objectives, 1, MPI_INT, 0, 1);

      // check wether parent sent a stop signal (ind < 0)
      if( n_individual_number < 0 ) break;
      
      // set the evaluation seed
      CARGoSRandom::SetSeedOf(category.str(), un_evaluation_seed);
      CARGoSRandom::GetCategory(category.str()).ResetRNGs();

      // compute new sample seeds for the individual evalation
      pun_sample_seeds = new UInt32[un_num_samples];
      for( UInt32 i = 0; i < un_num_samples; ++i ) {
	 pun_sample_seeds[i] = pc_RNG->Uniform( CRange<UInt32>(0, UINT_MAX) );
      }

      // vector for storing all the evaluation results
      vector<CObjectives> vec_results;

      // start evaluation
      for( UInt32 i = 0; i < un_num_samples; ++i ) {
	 // set the random seed in the simulator
	 cSimulator.SetRandomSeed(pun_sample_seeds[i]);
	 
	 // set the trial number
	 ((CEvolutionaryLoopFunctions&) cSimulator.GetLoopFunctions()).SetTrialNumber( i );

	 // resetting the experiment
	 cSimulator.Reset();
	 
	 // set the controller parameters
	 cSimulator.GetLoopFunctions().SetOnlineControlParameters( un_genotype_length, pf_genotype );
	 ((CEvolutionaryLoopFunctions&) cSimulator.GetLoopFunctions()).SetEvolution();
	 
	 // run the simulation
	 cSimulator.Execute();
	 
	 // retrieve the fitness values
	 UInt32 num_components = 0;
	 Real*  components = NULL;
	 Real   fitness = cSimulator.GetLoopFunctions().ComputePerformanceInExperiment( &num_components, &components );
	 vec_results.push_back( CObjectives(fitness, num_components, components) );

	 // delete the components vector
	 if( components != NULL ) delete [] components;
      }

      // compute average fitness
      CObjectives mean_results(un_num_objectives);
      ObjectivesMean( vec_results, mean_results );

      // send the computed fitness to the parent process
      if( !bPosteval ) {
	 Real* pf_objectives = new Real[un_num_objectives+1];
	 mean_results.GetObjectives( &pf_objectives );
	 pf_objectives[un_num_objectives] = mean_results.GetFitness();
	 parent_comm.Send(pf_objectives, un_num_objectives+1, MPI_ARGOSREAL, 0, 1);
	 delete [] pf_objectives;
      }
      else {
	 ostringstream filename;
	 filename.fill( '0' );
	 filename << s_fit_file << "_ind" << fixed << setw(4) << setfill('0') << n_individual_number << ".log";
	 DumpObjectives( vec_results, filename.str() );
	 
	 Real* pf_objectives = new Real[un_num_objectives+1];
	 mean_results.GetObjectives( &pf_objectives );
	 pf_objectives[un_num_objectives] = mean_results.GetFitness();
	 parent_comm.Send(pf_objectives, un_num_objectives+1, MPI_ARGOSREAL, 0, 1);
	 delete [] pf_objectives;
      }

      delete [] pf_genotype;
      delete [] pun_sample_seeds;
   }
  
   // no more individuls to evaluate - destroy the simulator
   cSimulator.Destroy();

   // everything's ok, exit
   MPI_Finalize(); 
   return 0;
}
