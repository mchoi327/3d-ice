/******************************************************************************
 * This file is part of 3D-ICE, version 2.1 .                                 *
 *                                                                            *
 * 3D-ICE is free software: you can  redistribute it and/or  modify it  under *
 * the terms of the  GNU General  Public  License as  published by  the  Free *
 * Software  Foundation, either  version  3  of  the License,  or  any  later *
 * version.                                                                   *
 *                                                                            *
 * 3D-ICE is  distributed  in the hope  that it will  be useful, but  WITHOUT *
 * ANY  WARRANTY; without  even the  implied warranty  of MERCHANTABILITY  or *
 * FITNESS  FOR A PARTICULAR  PURPOSE. See the GNU General Public License for *
 * more details.                                                              *
 *                                                                            *
 * You should have  received a copy of  the GNU General  Public License along *
 * with 3D-ICE. If not, see <http://www.gnu.org/licenses/>.                   *
 *                                                                            *
 *                             Copyright (C) 2010                             *
 *   Embedded Systems Laboratory - Ecole Polytechnique Federale de Lausanne   *
 *                            All Rights Reserved.                            *
 *                                                                            *
 * Authors: Arvind Sridhar                                                    *
 *          Alessandro Vincenzi                                               *
 *          Giseong Bak                                                       *
 *          Martino Ruggiero                                                  *
 *          Thomas Brunschwiler                                               *
 *          David Atienza                                                     *
 *                                                                            *
 * For any comment, suggestion or request  about 3D-ICE, please  register and *
 * write to the mailing list (see http://listes.epfl.ch/doc.cgi?liste=3d-ice) *
 * Any usage  of 3D-ICE  for research,  commercial or other  purposes must be *
 * properly acknowledged in the resulting products or publications.           *
 *                                                                            *
 * EPFL-STI-IEL-ESL                                                           *
 * Batiment ELG, ELG 130                Mail : 3d-ice@listes.epfl.ch          *
 * Station 11                                  (SUBSCRIPTION IS NECESSARY)    *
 * 1015 Lausanne, Switzerland           Url  : http://esl.epfl.ch/3d-ice.html *
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "analysis.h"
#include "macros.h"

/******************************************************************************/

void init_analysis (Analysis_t *this)
{
    this->AnalysisType         = TDICE_ANALYSIS_TYPE_NONE ;
    this->StepTime             = (Time_t) 0.0 ;
    this->SlotTime             = (Time_t) 0.0 ;
    this->SlotLength           = 0u ;
    this->CurrentTime          = 0u ;
    this->InitialTemperature   = (Temperature_t) 0.0 ;
}

/******************************************************************************/

Time_t get_simulated_time (Analysis_t *this)
{
  return this->CurrentTime * this->StepTime ;
}

/******************************************************************************/

void increase_by_step_time (Analysis_t *this)
{
    this->CurrentTime++ ;
}

/******************************************************************************/

bool slot_completed (Analysis_t *this)
{
    if (this->CurrentTime % this->SlotLength == 0u)

        return true ;

    return false ;
}

/******************************************************************************/

void print_formatted_analysis
(
    Analysis_t *this,
    FILE       *stream,
    String_t    prefix
)
{
    fprintf (stream, "%ssolver : ", prefix) ;

    if (this->AnalysisType == TDICE_ANALYSIS_TYPE_STEADY)

        fprintf (stream, "steady ;\n") ;

    else

        fprintf (stream, "transient step %.2f, slot %.2f ;\n",
            this->StepTime, this->SlotTime) ;

    fprintf (stream, "%s\n", prefix) ;

    fprintf (stream, "%sinitial temperature  %.2f ;\n",
        prefix, this->InitialTemperature) ;

    fprintf (stream, "%s\n", prefix) ;
}

/******************************************************************************/

void print_detailed_analysis
(
  Analysis_t *this,
  FILE       *stream,
  String_t    prefix
)
{
    String_t new_prefix = (String_t) malloc (sizeof(*new_prefix) * (5 + strlen(prefix))) ;

    if (new_prefix == NULL) return ;

    sprintf (new_prefix, "%s    ", prefix) ;

    fprintf (stream,
        "%sAnalysis                        = %p\n",
        prefix, this) ;

    fprintf (stream,
        "%s  AnalysisType                  = %d\n",
        prefix, this->AnalysisType) ;

    fprintf (stream,
        "%s  StepTime                      = %.2f\n",
        prefix, this->StepTime) ;

    fprintf (stream,
        "%s  SlotTime                      = %.2f\n",
        prefix, this->SlotTime) ;

    fprintf (stream,
        "%s  SlotLength                    = %d\n",
        prefix, this->SlotLength) ;

    fprintf (stream,
        "%s  CurrentTime                   = %d\n",
        prefix, this->CurrentTime) ;

    fprintf (stream,
        "%s  InitialTemperature            = %.2f\n",
        prefix, this->InitialTemperature) ;

    FREE_POINTER (free, new_prefix) ;
}

/******************************************************************************/
