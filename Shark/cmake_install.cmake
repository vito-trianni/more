# Install script for directory: /home/vtrianni/Work/MORE/Shark

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libshark.so.2.3.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libshark.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/vtrianni/Work/MORE/Shark/libshark.so.2.3.0"
    "/home/vtrianni/Work/MORE/Shark/libshark.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libshark.so.2.3.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libshark.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/home/vtrianni/Work/MORE/Shark/include/SharkDefs.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Array" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/Array/Array.h"
    "/home/vtrianni/Work/MORE/Shark/include/Array/Array2D.h"
    "/home/vtrianni/Work/MORE/Shark/include/Array/ArrayIo.h"
    "/home/vtrianni/Work/MORE/Shark/include/Array/ArrayOp.h"
    "/home/vtrianni/Work/MORE/Shark/include/Array/ArraySort.h"
    "/home/vtrianni/Work/MORE/Shark/include/Array/ArrayTable.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Rng" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Bernoulli.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Binomial.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Cauchy.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/DiffGeometric.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Dirichlet.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/DiscreteUniform.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Erlang.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Gamma.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Geometric.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/GlobalRng.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/HyperGeometric.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/LogNormal.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/NegExponential.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Normal.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Poisson.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/RNG.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/RandomVar.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Uniform.h"
    "/home/vtrianni/Work/MORE/Shark/include/Rng/Weibull.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/LinAlg" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/LinAlg/LinAlg.h"
    "/home/vtrianni/Work/MORE/Shark/include/LinAlg/VecMat.h"
    "/home/vtrianni/Work/MORE/Shark/include/LinAlg/arrayoptimize.h"
    "/home/vtrianni/Work/MORE/Shark/include/LinAlg/fft.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/FileUtil" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/FileUtil/FileUtil.h"
    "/home/vtrianni/Work/MORE/Shark/include/FileUtil/IOTools.h"
    "/home/vtrianni/Work/MORE/Shark/include/FileUtil/Params.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/EALib" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/EALib/CMA.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/CMSA.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/Chromosome.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/ChromosomeCMA.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/ChromosomeCMACholesky.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/ChromosomeFactory.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/ChromosomeT.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/ElitistCMA.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/Individual.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/IndividualT.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/Interval.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/MultiObjectiveFunctions.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/ObjectiveFunction.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/ObjectiveFunctions.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/OnePlusOneES.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/PVMinterface.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/Population.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/PopulationT.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/SearchAlgorithm.h"
    "/home/vtrianni/Work/MORE/Shark/include/EALib/UncertaintyQuantification.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MOO-EALib" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/ArchiveMOO.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/Hypervolume.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/IndividualMOO.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/MO-CMA.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/MOOMeasures.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/MOOTestFunctions.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/NSGA2.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/PopulationMOO.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/README"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/RMMEDA.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/SelectionMOO-Impl.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/SelectionMOO.h"
    "/home/vtrianni/Work/MORE/Shark/include/MOO-EALib/TestFunction.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ReClaM" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/AdpBP.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ArtificialDistributions.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/BFGS.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/BinaryCriterion.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/CG.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/CMAOptimizer.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ClassificationError.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/CMAC.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ComponentWiseModel.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ConcatenatedModel.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/CoverTree.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/CrossEntropy.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/CrossEntropyIndependent.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/CrossValidation.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/DF_CrossEntropy.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/DF_CrossEntropyIndependent.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/DF_MeanSquaredError.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Dataset.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/EarlyStopping.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ErrorFunction.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ErrorPercentage.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/FFNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/FFNetSource.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/GaussKernel.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/GaussianProcess.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/GridSearch.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/InverseClassSeparability.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/JaakkolaHeuristic.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/KTA.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/KalmanFilter.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/KernelFunction.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/KernelKMeans.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/KernelMeanClassifier.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/KernelNearestNeighbor.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/KernelNearestNeighborRegression.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/LDA.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/LMSEFFNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/LOO.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/LinOutFFNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/LinOutMSEBFFNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/LinearEquation.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/LinearModel.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/LinearRegression.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/MSEFFNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/MSERBFNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/MSERNNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/McSvmApproximation.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/MeanSquaredError.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Model.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ModelInterface.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ModelWithErrorFunction.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/NegativeLogLikelihood.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/NegativePolarization.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/NetParams.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/NoisyRprop.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/NoisySvmLikelihood.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/NormedModels.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Optimizer.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/PCA.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/FisherLDA.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Paraboloid.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Perceptron.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ProbenBNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ProbenNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/QuadraticProgram.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/KernelMatrices.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Utilities.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/QpSvmDecomp.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/QpBoxDecomp.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/QpMcDecomp.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/QpMcStzDecomp.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/QpEbCsDecomp.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Quickprop.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/RBFNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ROC.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/RadiusMargin.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Rprop.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/SigmoidFit.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/SigmoidModel.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Softmax.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/SpanBound1.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/SpanBound2.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/SpanBoundA.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/SquaredError.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/SteepestDescent.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/StochasticGradientDescent.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Svm.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/SvmApproximation.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/TanhNet.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/ValidationError.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/Variance.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/VarianceEstimator.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/WTA.h"
    "/home/vtrianni/Work/MORE/Shark/include/ReClaM/createConnectionMatrix.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Mixture" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/CodeBook.h"
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/KernelDensity.h"
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/LocalLinearRegression.h"
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/MixtureLinearRegression.h"
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/MixtureModel.h"
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/MixtureOfGaussians.h"
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/RBFN-PTO.h"
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/RBFN.h"
    "/home/vtrianni/Work/MORE/Shark/include/Mixture/RandomVector.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/TimeSeries" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/BimodalBrownianProcess.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/Counter.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/DiscreteMackeyGlass.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/Embedding.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/Generator.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/IOGenerator.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/IOSamples.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/Lorenz63.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/Lorenz84.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/MackeyGlass.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/NoisyIOSamples.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/NoisyMackeyGlass.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/RK4-1D.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/RK4.h"
    "/home/vtrianni/Work/MORE/Shark/include/TimeSeries/SelectComponent.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Fuzzy" TYPE FILE FILES
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/BellFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/BellLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/ComposedFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/ComposedLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/ComposedNDimFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/ConstantFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/ConstantLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/CustomIM.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/CustomizedFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/CustomizedLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/FuzzyException.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/FuzzyIncludes.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/FuzzyRelation.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/FuzzySet.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/GeneralizedBellFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/GeneralizedBellLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/HomogenousNDimFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/Implication.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/InferenceMachine.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/InfinityFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/InfinityLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/LinguisticTerm.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/LinguisticVariable.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/MamdaniIM.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/NDimFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/Operators.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/RCObject.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/RCPtr.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/Rule.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/RuleBase.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/RuleBaseGenerator.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/SigmoidalFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/SigmoidalLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/SingletonFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/SingletonLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/SugenoIM.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/SugenoRule.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/TrapezoidFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/TrapezoidLT.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/TriangularFS.h"
    "/home/vtrianni/Work/MORE/Shark/include/Fuzzy/TriangularLT.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/vtrianni/Work/MORE/Shark/examples/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/vtrianni/Work/MORE/Shark/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/vtrianni/Work/MORE/Shark/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
