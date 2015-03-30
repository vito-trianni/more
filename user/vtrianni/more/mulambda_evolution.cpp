#include "mulambda_evolution.h"

using namespace more;

const string CONFIGURATION_POPULATION_SIZE           = "population_size";
const string CONFIGURATION_POPULATION_SIZE_MU        = "population_size_mu";
const string CONFIGURATION_POPULATION_ELITE_SIZE     = "elite_size";
const string CONFIGURATION_GENOTYPE_TYPE             = "genotype_type";

const string CONFIGURATION_GENOTYPE_SIZE             = "genotype_size";
const string CONFIGURATION_GENOTYPE_MINVALUE         = "gene_min_value";
const string CONFIGURATION_GENOTYPE_MAXVALUE         = "gene_max_value";
const string CONFIGURATION_MUTATION_VARIANCE         = "mutation_variance";
const string CONFIGURATION_EVOLUTION_OBJECTIVES      = "num_objectives";


/****************************************/
/****************************************/

CMuLambdaEvolution::CMuLambdaEvolution() :
   CEvolution(),
   m_unPopulationSize( 0 ),
   m_unPopulationSizeMu( 0 ),
   m_unEliteSize( 0 ),

   m_unGenotypeSize( 0 ),
   m_fGenotypeMinValue( 0 ),
   m_fGenotypeMaxValue( 0 ),
   m_fMutationVariance( 0 ),
   m_pcPopulation( NULL ),
   m_pcOffsprings( NULL ),
   m_unNumObjectives( 0 )
{}


/****************************************/
/****************************************/

CMuLambdaEvolution::~CMuLambdaEvolution() {
   if( m_pcPopulation ) delete m_pcPopulation;
   if( m_pcOffsprings ) delete m_pcOffsprings;
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::Init( TConfigurationNode& t_configuration_tree ) {
   ////////////////////////////////////////////////////////////////////////////////
   // evolutionary paramters
   ////////////////////////////////////////////////////////////////////////////////
   CEvolution::Init( t_configuration_tree );

   ////////////////////////////////////////////////////////////////////////////////
   // parameters of the population of individuals
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_POPULATION_SIZE, m_unPopulationSize );
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_POPULATION_SIZE_MU, m_unPopulationSizeMu );
   GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_POPULATION_ELITE_SIZE, m_unEliteSize, m_unEliteSize );

   ////////////////////////////////////////////////////////////////////////////////
   // parameters of the genotype
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_GENOTYPE_SIZE, m_unGenotypeSize );
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_GENOTYPE_MINVALUE, m_fGenotypeMinValue );
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_GENOTYPE_MAXVALUE, m_fGenotypeMaxValue );
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_MUTATION_VARIANCE, m_fMutationVariance );

   ////////////////////////////////////////////////////////////////////////////////
   // number of the objectives in the fitness function (components)
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_OBJECTIVES, m_unNumObjectives, m_unNumObjectives );
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::Evolve() {
   StartEvaluationProcess();

   ////////////////////////////////////////////////////////////////////////////////
   // initialise population of individuals
   ////////////////////////////////////////////////////////////////////////////////
   m_pcPopulation = new PopulationT<double>( m_unPopulationSize, ChromosomeT<double>(m_unGenotypeSize) );
   m_pcOffsprings = new PopulationT<double>( m_unPopulationSize, ChromosomeT<double>(m_unGenotypeSize) );
   

   if( m_bMaximization ) {
      m_pcPopulation->setMaximize();
      m_pcOffsprings->setMaximize();
   }
   else {
      m_pcPopulation->setMinimize();
      m_pcOffsprings->setMinimize();
   }
   
   // initialise the genotypes within the population
   for( UInt32 i = 0; i < m_unPopulationSize; ++i ) {
      (*m_pcPopulation)[i][0].initialize(m_fGenotypeMinValue, m_fGenotypeMaxValue);
   }


   ////////////////////////////////////////////////////////////////////////////////
   // start loop
   ////////////////////////////////////////////////////////////////////////////////
   while( m_unNumGeneration < m_unMaxGeneration ) {
      // initialise random number generator for this generation
      SetRandomSeed( m_unRandomSeed );
    
      // evaluate popoulation
      std::cout << "[MORE] Evaluating population for generation " << m_unNumGeneration << endl; std::cout.flush();

      EvaluatePopulation();

      std::cout << "[MORE] Generation " << m_unNumGeneration << " - best fitness: " << m_pcPopulation->best().fitnessValue() << endl; std::cout.flush();
    
      // dump population
      if( (m_unNumGeneration % m_unSaveGenerationStep == 0) ||
	  (m_unNumGeneration == m_unMaxGeneration -1) ) {
	 DumpGeneration();
	 DumpBestIndividuals();
	 DumpFitness();
      }

      m_pcPopulation->sort();
      
      // reproduction with mutation only
      for( UInt32 i = 0; i < m_unPopulationSize; ++i ) {
	 if( i < m_unEliteSize) {
	    (*m_pcOffsprings)[i] = (*m_pcPopulation)[i];
	 }
	 else {
	    UInt32 index = (i-m_unEliteSize)%m_unPopulationSizeMu;
	    (*m_pcOffsprings)[i] = (*m_pcPopulation)[index];
	    (*m_pcOffsprings)[i][0].mutateNormal( m_fMutationVariance );
	    (*m_pcOffsprings)[i][0].cutOff( m_fGenotypeMinValue, m_fGenotypeMaxValue );
	 }
      }
      

      // fitness proportional selection with eltiism
      *m_pcPopulation = *m_pcOffsprings;
      
      // get a new random seed for the next generation
      m_unRandomSeed = Rng::discrete(0,UINT_MAX);
      
      m_unNumGeneration += 1;
   }

   StopEvaluationProcess();

}


/****************************************/
/****************************************/

void CMuLambdaEvolution::PostEvaluation() {
   // start evaluation
   StartEvaluationProcess();

   // initialise population of individuals
   m_pcPopulation = new PopulationT<double>( m_unPopulationSize, ChromosomeT<double>(m_unGenotypeSize) );
   LoadGeneration();

   // initialise random number generator for this generation
   SetRandomSeed( m_unRandomSeed );
    
   // sort population according to the previously computed fitness
   m_pcPopulation->sort();

   // select the first Mu individuals
   m_pcPopulation->remove(m_unPopulationSizeMu,m_unPopulationSize);

   // reset the population size variable
   m_unPopulationSize = m_pcPopulation->size();

   // evaluate population
   EvaluatePopulation();

   // dump all individuals that were post-evaluated and the best one
   ostringstream filename;
   filename.fill( '0' );
   for( UInt32 i = 0; i < m_unPopulationSize; ++i ) {
      filename.str("");
      filename << m_sWorkingDir << m_sBestIndividualBaseFilename 
	       << "_gen" << setw(4) << m_unNumGeneration
	       << "_ind" << setw(4) << i << ".log";   
      DumpIndividual( i, filename.str() );
   }

   // dump best individual
      filename.str("");
      filename << m_sWorkingDir << m_sBestIndividualBaseFilename 
	       << "_gen" << setw(4) << m_unNumGeneration
	       << "_bestind.log";   
      DumpIndividual( m_pcPopulation->bestIndex(), filename.str() );

   // stop evaluation
   StopEvaluationProcess();
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::EvaluatePopulation() {
   // fist, send numProcess individuals in the queue. The queueing script
   // will write in a file the results, a file different for each
   // individual sample. 
   UInt32 queued    = 0;
   UInt32 ind       = 0;
   UInt32 remaining = m_unPopulationSize;
   while( remaining > 0 ) {    
      // every time there are less queued evaluations than the maximum
      // number, stated in numProcesses, start a new one
      while( (queued < m_unNumProcesses) && (ind < m_unPopulationSize) ) {
	 SendIndividualParameters( ind ); // send the individual genotype to the evaluation program
	 queued  += 1;
	 ind += 1;
      }
    
      // wait for any results: receive message and add fitness values
      ReceiveIndividualFitness();
      remaining -= 1;
      queued    -= 1;
   }
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::SendIndividualParameters( UInt32 individualNumber ) {
   // search for the first available process
   SInt32 tid = -1;
   for( TMapProcessToIndividualIterator it = m_mapProcessToIndividual.begin(); 
	it != m_mapProcessToIndividual.end(); 
	++it ) {
      if( it->second == -1 ) {
	 tid = it->first;
	 break;
      }
   }

   if( tid == -1 ) {
      THROW_ARGOSEXCEPTION( "error: no process available to start evaluation" );
   }

   // get the individual genotype
   Real* genes = new Real[m_unGenotypeSize];
   for( UInt32 i = 0; i < m_unGenotypeSize; i++ ) {
      genes[i] = (*m_pcPopulation)[individualNumber][0][i];
   }
   
   m_cEvaluatorComm.Send(&individualNumber, 1, MPI_INT, tid, 1);
   m_cEvaluatorComm.Send(&m_unGenotypeSize, 1, MPI_INT, tid, 1);
   m_cEvaluatorComm.Send(genes, m_unGenotypeSize, MPI_ARGOSREAL, tid, 1);
   m_cEvaluatorComm.Send(&m_unNumSamples, 1, MPI_INT, tid, 1);
   m_cEvaluatorComm.Send(&m_unRandomSeed, 1, MPI_INT, tid, 1);
   m_cEvaluatorComm.Send(&m_unNumObjectives, 1, MPI_INT, tid, 1);

   // record the individual into the map, for later retrieval
   m_mapProcessToIndividual[tid] = individualNumber;
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::ReceiveIndividualFitness() {
   SInt32 tid;
   MPI::Status status;
   Real*  objectives = new Real[m_unNumObjectives+1];
   m_cEvaluatorComm.Recv(objectives, m_unNumObjectives+1, MPI_ARGOSREAL, MPI_ANY_SOURCE, 1, status);
   
   tid = status.Get_source();
   
   vector<double> vec_objectives;
   vec_objectives.resize(m_unNumObjectives);
   std::copy( objectives, objectives+m_unNumObjectives, vec_objectives.begin() );
   UInt32 un_individual = m_mapProcessToIndividual[tid];
   (*m_pcPopulation)[un_individual].setFitness( objectives[m_unNumObjectives] );
   m_mapPopulationFitness[un_individual] = vec_objectives;

   // reset the process2Individual map
   m_mapProcessToIndividual[tid] = -1;

   delete [] objectives;
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::DumpGeneration() {
   // Write the population
   ofstream out;
   ostringstream filename;
   filename.fill( '0' );

   filename.str("");
   filename << m_sWorkingDir << m_sEvolutionBaseFilename << "_gen" << setw(4) << m_unNumGeneration << ".log";
   out.open( filename.str().c_str(), ios::out );
   out << (*m_pcPopulation);
   out.close();
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::DumpFitness() {
   ofstream out;
   ostringstream filename;
   filename.fill( '0' );

   // write the fitness value
   filename.str("");
   filename << m_sWorkingDir << m_sFitnessBaseFilename << ".log";
   if( m_unNumGeneration ) {
      out.open( filename.str().c_str(), ios::app );
   }
   else {
      out.open( filename.str().c_str(), ios::out );
   }
   out << m_unNumGeneration << " " 
       << m_pcPopulation->best().fitnessValue() << " " 
       << m_pcPopulation->worst().fitnessValue() << " " 
       << endl;
   out.close();


   // write the current distribution of the fitness components of the whole population
   filename.str("");
   filename << m_sWorkingDir << m_sFitnessBaseFilename << "_gen" << setw(4) << m_unNumGeneration << ".log";
   out.open( filename.str().c_str(), ios::out );
   out.fill('0');

   // dump all the individual positions in the fitness space
   for( map< int, vector<double> >::iterator it = m_mapPopulationFitness.begin();
	it != m_mapPopulationFitness.end(); ++it ) {
      for( UInt32 j = 0; j < it->second.size(); ++j ) {
	 out << (it->second)[j] << " ";
      }
      out << "ind_" << fixed << setw(3) << it->first << endl;
   }
   out.close();
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::DumpBestIndividuals() {
   ostringstream filename, bestind_filename;
   filename.fill( '0' );
   bestind_filename.fill( '0' );
   filename << m_sWorkingDir << m_sBestIndividualBaseFilename << "_gen" << setw(4) << m_unNumGeneration << ".log";
   bestind_filename << m_sWorkingDir << "bestind.log";
   
   UInt32 best_ind = m_pcPopulation->bestIndex();
   DumpIndividual( best_ind, filename.str() );
   DumpIndividual( best_ind, bestind_filename.str() );
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::DumpIndividual( const UInt32& un_individual_number, const string& filename ) {
   ofstream out;
   out.open( filename.c_str(), ios::out );
   
   ChromosomeT<double> chrom = (*m_pcPopulation)[un_individual_number][0];
   out << m_unGenotypeSize;
   for( UInt32 i = 0; i < m_unGenotypeSize; i++ ) {
      out << " " << chrom[i];
   }
   out << endl;
   out.close();
}

/****************************************/
/****************************************/

void CMuLambdaEvolution::LoadGeneration() {
   // Write the population
   ifstream in;
   ostringstream filename;
   filename.fill( '0' );

   filename.str("");
   filename << m_sWorkingDir << m_sEvolutionBaseFilename << "_gen" << setw(4) << m_unNumGeneration << ".log";
   cout << "[MORE] loading population from file " << filename.str() << endl;
   in.open( filename.str().c_str(), ios::in );
   if( !in ) {
      cerr << "[MORE] FATAL: cannot open file " << filename.str() << endl;
      exit( 1 );
   }
   in >> (*m_pcPopulation);
   in.close();
}


/****************************************/
/****************************************/

void CMuLambdaEvolution::LoadBestIndividuals() {

}
