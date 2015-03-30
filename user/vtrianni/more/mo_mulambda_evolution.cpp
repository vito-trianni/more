#include "mo_mulambda_evolution.h"

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
const string CONFIGURATION_EVOLUTION_POSTEVAL_TYPE   = "posteval_type";

/****************************************/
/****************************************/

CMOMuLambdaEvolution::CMOMuLambdaEvolution() :
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
   m_unNumObjectives( 0 ),
   m_tPostEvalType( POSTEVAL_SO )
{}


/****************************************/
/****************************************/

CMOMuLambdaEvolution::~CMOMuLambdaEvolution() {
   if( m_pcPopulation ) delete m_pcPopulation;
   if( m_pcOffsprings ) delete m_pcOffsprings;
}


/****************************************/
/****************************************/

void CMOMuLambdaEvolution::Init( TConfigurationNode& t_configuration_tree ) {
   ////////////////////////////////////////////////////////////////////////////////
   // standard initialisation
   ////////////////////////////////////////////////////////////////////////////////
   CEvolution::Init(t_configuration_tree);

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
   // parameters of the multi-objective evolution
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_OBJECTIVES, m_unNumObjectives );

   ////////////////////////////////////////////////////////////////////////////////
   // parameters of the multi-objective post evaluation
   ////////////////////////////////////////////////////////////////////////////////
   string s_posteval_type = "posteval_so";
   GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_POSTEVAL_TYPE, s_posteval_type, s_posteval_type );
   if( s_posteval_type == "posteval_so" ) {
      m_tPostEvalType = POSTEVAL_SO;
   }
   else if( s_posteval_type == "posteval_mo_rank" ) {
      m_tPostEvalType = POSTEVAL_MO_RANK;
   }
   else if( s_posteval_type == "posteval_mo_nd" ) {
      m_tPostEvalType = POSTEVAL_MO_ND;
   }
   else {
      std::cerr << "[MORE] post-evaluation type not recognised: '" << s_posteval_type << "'" << std::endl;
   }
}


/****************************************/
/****************************************/

void CMOMuLambdaEvolution::Evolve() {
   StartEvaluationProcess();

   ////////////////////////////////////////////////////////////////////////////////
   // initialise population of individuals
   ////////////////////////////////////////////////////////////////////////////////
   ChromosomeT<double> chrom(m_unGenotypeSize);
   IndividualMOO prototype( chrom );
   m_pcPopulation = new PopulationMOO( m_unPopulationSize, prototype );
   m_pcOffsprings = new PopulationMOO( m_unPopulationSize, prototype );
   
   // set the max/min flag
   if( m_bMaximization ) {
      m_pcPopulation->setMaximize();
      m_pcOffsprings->setMaximize();
   }
   else {
      m_pcPopulation->setMinimize();
      m_pcOffsprings->setMinimize();
   }
   

   // set the number of objectives
   m_pcPopulation->setNoOfObj( m_unNumObjectives );
   m_pcOffsprings->setNoOfObj( m_unNumObjectives );


   // initialise the genotypes within the population
   for( UInt32 i = 0; i < m_unPopulationSize; ++i ) {
      (dynamic_cast< ChromosomeT<double>& >((*m_pcPopulation)[i][0])).initialize(m_fGenotypeMinValue, m_fGenotypeMaxValue);
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
	 DumpNonDominated();
	 DumpFitness();
      }

      m_pcPopulation->SMeasure();
      std::sort(m_pcPopulation->begin(), m_pcPopulation->end(), PopulationMOO::compareRankShare);
      //      m_pcPopulation->sort();
      
      
      // reproduction with mutation only
      for( UInt32 i = 0; i < m_unPopulationSize; ++i ) {
	 if( i < m_unEliteSize) {
	    (*m_pcOffsprings)[i] = (*m_pcPopulation)[i];
	 }
	 else {
	    UInt32 index = (i-m_unEliteSize)%m_unPopulationSizeMu;
	    (*m_pcOffsprings)[i] = (*m_pcPopulation)[index];
	    (dynamic_cast< ChromosomeT<double>& >((*m_pcOffsprings)[i][0])).mutateNormal( m_fMutationVariance );
	    (dynamic_cast< ChromosomeT<double>& >((*m_pcOffsprings)[i][0])).cutOff( m_fGenotypeMinValue, m_fGenotypeMaxValue );
	 }
      }
      

      // fitness proportional selection with eltiism
      *m_pcPopulation = *m_pcOffsprings;
      
      // get a new random seed for the next generation
      m_unRandomSeed = Rng::discrete(0, UINT_MAX);
      
      m_unNumGeneration += 1;
   }

   StopEvaluationProcess();
}


/****************************************/
/****************************************/

void CMOMuLambdaEvolution::PostEvaluation() {
   // start evaluation
   StartEvaluationProcess();

   // initialise population of individuals
   ChromosomeT<double> chrom(m_unGenotypeSize);
   IndividualMOO prototype( chrom );
   m_pcPopulation = new PopulationMOO( m_unPopulationSize, prototype );
   
   // set the max/min flag
   if( m_bMaximization ) {
      m_pcPopulation->setMaximize();
   }
   else {
      m_pcPopulation->setMinimize();
   }
   
   // load the population from file
   LoadGeneration();


   // initialise random number generator for this generation
   SetRandomSeed( m_unRandomSeed );
    
   switch( m_tPostEvalType ) {
   case POSTEVAL_MO_ND: {
      // select the non-dominated individuals
      m_pcOffsprings = new PopulationMOO(*m_pcPopulation);
      m_pcOffsprings->getNonDominated(*m_pcPopulation);
      break;
   }
      
   case POSTEVAL_MO_RANK:
      // sort population according to the hypervolume metric
      m_pcPopulation->SMeasure();
      std::sort(m_pcPopulation->begin(), m_pcPopulation->end(), PopulationMOO::compareRankShare);
      // select the first Mu individuals
      m_pcPopulation->remove(m_unPopulationSizeMu,m_unPopulationSize);
      break;

   case POSTEVAL_SO:
   default:
      // sort population according to the previously computed fitness
      m_pcPopulation->sort();
      // select the first Mu individuals
      m_pcPopulation->remove(m_unPopulationSizeMu,m_unPopulationSize);
      break;
   }

   // reset the population size variable
   m_unPopulationSize = m_pcPopulation->size();

   // set the number of objectives (may be different from evolution)
   m_pcPopulation->setNoOfObj( m_unNumObjectives );

   // evaluate population
   if( m_unNumSamples > 0 )
      EvaluatePopulation();

   // dump all individuals that were post-evaluated
   ostringstream filename;
   filename.fill( '0' );
   for( UInt32 i = 0; i < m_unPopulationSize; ++i ) {
      filename.str("");
      filename << m_sWorkingDir << m_sBestIndividualBaseFilename
	       << "_gen" << setw(4) << m_unNumGeneration
	       << "_ind" << setw(4) << i << ".log";   
      DumpIndividual( i, filename.str() );
   }

   // Dump the population of non-dominated individuals
   DumpNonDominated();

   // Dump the fitness after postevaluation
   DumpFitness();

   // stop evaluation
   StopEvaluationProcess();
}


/****************************************/
/****************************************/

void CMOMuLambdaEvolution::EvaluatePopulation() {
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

void CMOMuLambdaEvolution::SendIndividualParameters( UInt32 individualNumber ) {
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
   ChromosomeT<double>& chrom = dynamic_cast< ChromosomeT<double>& >((*m_pcPopulation)[individualNumber][0]);
   Real* genes = new Real[m_unGenotypeSize];
   for( UInt32 i = 0; i < m_unGenotypeSize; i++ ) {
      genes[i] = chrom[i];
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

void CMOMuLambdaEvolution::ReceiveIndividualFitness() {
   SInt32 tid;
   MPI::Status status;
   Real*  objectives = new Real[m_unNumObjectives+1];
   m_cEvaluatorComm.Recv(objectives, m_unNumObjectives+1, MPI_ARGOSREAL, MPI_ANY_SOURCE, 1, status);
   
   tid = status.Get_source();
   
   vector<double> vec_objectives;
   vec_objectives.resize(m_unNumObjectives);
   std::copy( objectives, objectives+m_unNumObjectives, vec_objectives.begin() );
   (*m_pcPopulation)[m_mapProcessToIndividual[tid]].setMOOFitnessValues( vec_objectives );
   (*m_pcPopulation)[m_mapProcessToIndividual[tid]].setFitness( objectives[m_unNumObjectives] );

   // reset the process2Individual map
   m_mapProcessToIndividual[tid] = -1;

   delete [] objectives;
}


/****************************************/
/****************************************/

void CMOMuLambdaEvolution::DumpGeneration() {
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

void CMOMuLambdaEvolution::DumpFitness() {
   ofstream out;
   ostringstream filename;
   filename.fill( '0' );
   
//    // write the fitness values
//    filename.str("");
//    filename << m_sWorkingDir << m_sFitnessBaseFilename << ".log";
//    if( m_unNumGeneration ) {
//       out.open( filename.str().c_str(), ios::app );
//    }
//    else {
//       out.open( filename.str().c_str(), ios::out );
//    }
//    out << m_unNumGeneration << " " 
//        << m_pcPopulation->best().fitnessValue() << " " 
//        << m_pcPopulation->worst().fitnessValue() << " " 
//        << endl;
//    out.close();


   // write the current distribution of the fitness components of the whole population
   filename.str("");
   filename << m_sWorkingDir << m_sFitnessBaseFilename << "_gen" << setw(4) << m_unNumGeneration << ".log";
   out.open( filename.str().c_str(), ios::out );
   out.fill('0');

   // dump all the individual positions in the fitness space
   for( UInt32 i = 0; i < m_unPopulationSize; ++i ) { 
      vector<double> objectives = (*m_pcPopulation)[i].getMOOFitnessValues( false );
      for( UInt32 j = 0; j < objectives.size(); ++j ) {
	 out << objectives[j] << " ";
      }
      out << "ind_" << fixed << setw(3) << i << endl;
   }
   out.close();
}

/****************************************/
/****************************************/

void CMOMuLambdaEvolution::DumpBestIndividuals() {
   ofstream out, bestind_out;
   ostringstream filename, bestind_filename;
   filename.fill( '0' );
   bestind_filename.fill( '0' );
   filename << m_sWorkingDir << m_sBestIndividualBaseFilename << "_gen" << setw(4) << m_unNumGeneration << ".log";
   // bestind_filename << m_sWorkingDir << m_sBestIndividualBaseFilename << ".log";
   
   UInt32 best_ind = m_pcPopulation->bestIndex();
   DumpIndividual( best_ind, filename.str() );
   // DumpIndividual( best_ind, bestind_filename.str() );
}

/****************************************/
/****************************************/

void CMOMuLambdaEvolution::DumpNonDominated() {
   // create a temporary population
   ChromosomeT<double> chrom(m_unGenotypeSize);
   IndividualMOO prototype( chrom );
   PopulationMOO nd_population( prototype );

   // set the max/min flag
   if( m_bMaximization ) {
      nd_population.setMaximize();
   }
   else {
      nd_population.setMinimize();
   }
   
   // set the number of objectives
   nd_population.setNoOfObj( m_unNumObjectives );

   // get the non dominated individuals
   m_pcPopulation->getNonDominated(nd_population);

   // write the current distribution of the fitness components of the whole population
   ofstream out;
   ostringstream filename;
   filename.fill( '0' );
   filename.str("");
   filename << m_sWorkingDir << m_sFitnessBaseFilename << "_gen" << setw(4) << m_unNumGeneration << "_nd.log";
   out.open( filename.str().c_str(), ios::out );
   out.fill('0');

   // dump all the individual positions in the fitness space, along with the individual name
   for( UInt32 i = 0; i < nd_population.size(); ++i ) { 
      vector<double> objectives = nd_population[i].getMOOFitnessValues( false );
      for( UInt32 j = 0; j < objectives.size(); ++j ) {
	 out << objectives[j] << " ";
      }
      out << "ind_" << fixed << setw(3) << i << endl;
   }
   out.close();

   // dump all the chromosomes of the nondominated population
   for( UInt32 i = 0; i < nd_population.size(); ++i ) { 
      ChromosomeT<double>& chrom = dynamic_cast< ChromosomeT<double>& >(nd_population[i][0]);
      
      filename.str("");
      filename << m_sWorkingDir << m_sBestIndividualBaseFilename 
	       << "_gen" << setw(4) << m_unNumGeneration
	       << "_nd"  << setw(4) << i << ".log";
      DumpChromosome( chrom, filename.str() );
   }
}

/****************************************/
/****************************************/

void CMOMuLambdaEvolution::DumpIndividual( const UInt32& un_individual_number, const string& filename ) {
   ChromosomeT<double>& chrom = dynamic_cast< ChromosomeT<double>& >((*m_pcPopulation)[un_individual_number][0]);
   DumpChromosome( chrom, filename );
}


/****************************************/
/****************************************/

void CMOMuLambdaEvolution::DumpChromosome( const ChromosomeT<double>& chrom, const string& filename ) {
   ofstream out;
   out.open( filename.c_str(), ios::out );
   
   out << m_unGenotypeSize;
   for( UInt32 i = 0; i < m_unGenotypeSize; i++ ) {
      out << " " << chrom[i];
   }
   out << endl;
   out.close();
}

/****************************************/
/****************************************/

void CMOMuLambdaEvolution::LoadGeneration() {
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

void CMOMuLambdaEvolution::LoadBestIndividuals() {

}
