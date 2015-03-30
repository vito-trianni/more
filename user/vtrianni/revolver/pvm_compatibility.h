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
 * @file <argos2/user/vtrianni/revolver/pvm_compatibility.h>
 *
 * @brief This file provides
 *
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */


#ifndef PVM_COMPATIBILITY_H
#define PVM_COMPATIBILITY_H

#include <argos2/common/utility/datatypes/datatypes.h>

#include <stdio.h>

namespace argos {

  /****************************************/
  /* REAL                                 */
  /****************************************/

#ifdef ARGOS_DOUBLE_PRECISION
#define MPI_ARGOSREAL MPI_DOUBLE
#else
#define MPI_ARGOSREAL MPI_FLOAT
#endif

};

#endif


