/******************************************************************************
 * This file is part of 3D-ICE, version 1.0.1 .                               *
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
 *          Martino Ruggiero                                                  *
 *          Thomas Brunschwiler                                               *
 *          David Atienza                                                     *
 *                                                                            *
 * For any comment, suggestion or request  about 3D-ICE, please  register and *
 * write to the mailing list (see http://listes.epfl.ch/doc.cgi?liste=3d-ice) *
 *                                                                            *
 * EPFL-STI-IEL-ESL                                                           *
 * Batiment ELG, ELG 130                Mail : 3d-ice@listes.epfl.ch          *
 * Station 11                                  (SUBSCRIPTION IS NECESSARY)    *
 * 1015 Lausanne, Switzerland           Url  : http://esl.epfl.ch/3d-ice.html *
 ******************************************************************************/

#ifndef _3DICE_ANALYSIS_H_
#define _3DICE_ANALYSIS_H_

/*! \file analysis.h */

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/

#include <stdio.h>
#include <stdint.h>

#include "types.h"
#include "print_output.h"
#include "stack_element.h"

/******************************************************************************/

    /*! \struct Analysis
     *
     *  \brief Informations about the type of thermal simulation to be run and its initial settings
     */

    struct Analysis
    {
        /*! The analysis type */

        AnalysisType_t AnalysisType ;

        /*! The stepping time for Transient Analysis */

        double StepTime ;

        /*! The slot time for Transient Analysis */

        double SlotTime ;

        /*! Number of steps to do to complete a slot */

        uint32_t SlotLength ;

        /*! Number of steps simulated so far ... */

        uint32_t CurrentTime ;

        /*! Initial Temperature if the IC stack */

        double InitialTemperature ;

        /*! Pointer to the list of inspection points to print
         *  at the end of the simulation */

        PrintOutput *PrintOutputListFinal ;

        /*! Pointer to the list of inspection points to print
         *  at the end of the time slot */

        PrintOutput *PrintOutputListSlot ;

        /*! Pointer to the list of inspection points to print
         *  at every time step */

        PrintOutput *PrintOutputListStep ;

    } ;

    /*! Definition of the type Analysis */

    typedef struct Analysis Analysis ;

/******************************************************************************/



    /*! Sets all the fields of \a analysis to a default value (zero or \c NULL ).
     *
     * \param analysis the address of the structure to initialize
     */

    void init_analysis (Analysis *analysis) ;



    /*! Allocates a structure Analysis in memory and sets its fields to
     *  their default value with #init_analysis
     *
     * \return the pointer to a new Analysis
     * \return \c NULL if the memory allocation fails
     */

    Analysis *alloc_and_init_analysis (void) ;



    /*! Frees the memory related to \a analysis
     *
     *  \a analysis must be the address of an automatic variable
     *
     * \param analysis the address of the structure to free
     */

    void free_analysis (Analysis *analysis) ;



    /*! Returns the time, in seconds, sum of the time steps emulated so far
     *
     * \param analysis the address of the analysis structure
     * \return the simulated amount of time in seconds
     */

    double get_simulated_time (Analysis *analysis) ;


    /*! Returns the state of the slot simulation
     *
     * \param analysis the address of the analysis structure
     *
     * \return \c TRUE if simulation for a slot is completed
     * \return \c FALSE otherwise
     */

    bool slot_completed (Analysis *analysis) ;

    /*! Prints the analysis informations as they look in the stack file
     *
     * \param stream   the output stream (must be already open)
     * \param prefix   a string to be printed as prefix at the beginning of each line
     * \param analysis the analysis information to print
     */

    void print_formatted_analysis

        (FILE *stream, char *prefix, Analysis *analysis) ;



    /*! Prints detailed information about all the fields of analysis
     *
     * \param stream   the output stream (must be already open)
     * \param prefix   a string to be printed as prefix at the beginning of each line
     * \param analysis the analysis information to print
     */

    void print_detailed_analysis

        (FILE *stream, char *prefix, Analysis *analysis) ;


    /*! Inserts a print output instance into the corresponding print output queue
     *
     * \param analysis     pointer to the analysis structure
     * \param print_output pointer to print output structure to add
     */

    void add_print_output_to_analysis

        (Analysis *analysis, PrintOutput *print_output) ;


    /*! Initializes output files
     *
     * Generates, for every inspection point, the output file and print the header
     * If the target output file is already there, it will be overwritten.
     *
     * \param analysis pointer to the analysis structure
     * \param list FIXME can we remove it ?????
     *
     * \return \c TDICE_SUCCESS if the operation terminates with success
     * \return \c TDICE_FAILURE if one of the files can not be created
     */

    Error_t initialize_print_output_instructions (Analysis* analysis, StackElement *list) ;


/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _3DICE_CHANNEL_H_ */
