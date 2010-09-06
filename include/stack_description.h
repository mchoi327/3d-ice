/******************************************************************************
 * Bison source file "3D-ICE/bison/floorplan_parser.y"                        *
 *                                                                            *
 * This file is part of 3D-ICE 1.0 (beta).                                    *
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
 * with 3D-ICe. If not, see <http://www.gnu.org/licenses/>.                   *
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
 * EPFL-STI-IEL-ESL                                                           *
 * Batiment ELG, ELG 130                                                      *
 * Station 11                                          threed-ice@esl.epfl.ch *
 * 1015 Lausanne, Switzerland                       http://esl.epfl.ch/3D-ICE *
 ******************************************************************************/

#ifndef _3DICE_STACK_DESCRIPTION_H_
#define _3DICE_STACK_DESCRIPTION_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

#include "material.h"
#include "channel.h"
#include "die.h"
#include "stack_element.h"
#include "dimensions.h"
#include "conductances.h"
#include "environment_heat_sink.h"

/******************************************************************************/

  typedef struct
  {
    /* The name of the file used to fill the stack description */

    String_t FileName ;

    /* The list of materials componing the layers and channels */

    Material* MaterialsList ;

    /* The (if present) single type of channel used to compose the 3d stack */

    Channel* Channel ;

    /* The list of dies available to compose the 3d stack */

    Die* DiesList ;

    /* Information (if present) about the heat dissipation */
    /* throught the top surface                            */

    EnvironmentHeatSink* EnvironmentHeatSink;

    /* The list of stack elements componing the 3Dstack */

    StackElement* StackElementsList ;

    /* Collection of all the dimensions (chip, grid of cells, cell) */

    Dimensions* Dimensions ;

    /* Number of remaining time slots to consume */

    Quantity_t RemainingTimeSlots ;

  } StackDescription ;

/******************************************************************************/

  void init_stack_description (StackDescription* stkd) ;

/******************************************************************************/

  int fill_stack_description (StackDescription* stkd, String_t filename) ;

/******************************************************************************/

  void free_stack_description (StackDescription* stkd) ;

/******************************************************************************/

  void print_stack_description
  (
    FILE*             stream,
    String_t          prefix,
    StackDescription* stkd
  ) ;

/******************************************************************************/

  void print_all_floorplans
  (
    FILE*             stream,
    String_t          prefix,
    StackDescription* stkd
  ) ;

/******************************************************************************/

  void fill_conductances_stack_description
  (
    StackDescription* stkd,
    Conductances*     conductances
  ) ;

/******************************************************************************/

  void fill_capacities_stack_description
  (
    StackDescription* stkd,
    Capacity_t*       capacities,
    Time_t            delta_time
  ) ;

/******************************************************************************/

  void fill_sources_stack_description
  (
    StackDescription* stkd,
    Source_t*         sources,
    Conductances*     conductances
  ) ;

/******************************************************************************/

  void fill_system_matrix_stack_description
  (
    StackDescription*    stkd,
    Conductances*        conductances,
    Capacity_t*          capacities,
    RowIndex_t*          row_pointers,
    ColumnIndex_t*       column_indices,
    SystemMatrixValue_t* values
  ) ;

/******************************************************************************/

  Quantity_t get_total_number_of_floorplan_elements
  (
    StackDescription* stkd
  ) ;

/******************************************************************************/

  Quantity_t get_number_of_floorplan_elements_in_floorplan
  (
    StackDescription* stkd,
    String_t stack_element_id
  ) ;

/******************************************************************************/

  int get_max_temperature_in_floorplan_element
  (
    StackDescription* stkd,
    String_t          stack_element_id,
    String_t          floorplan_element_id,
    Temperature_t*    temperatures,
    Temperature_t*    max_temperature
  ) ;

/******************************************************************************/

  int get_min_temperature_in_floorplan_element
  (
    StackDescription* stkd,
    String_t          stack_element_id,
    String_t          floorplan_element_id,
    Temperature_t*    temperatures,
    Temperature_t*    min_temperature
  ) ;

/******************************************************************************/

  int get_avg_temperature_in_floorplan_element
  (
    StackDescription* stkd,
    String_t          stack_element_id,
    String_t          floorplan_element_id,
    Temperature_t*    temperatures,
    Temperature_t*    avg_temperature
  ) ;

/******************************************************************************/

  int get_min_avg_max_temperatures_in_floorplan_element
  (
    StackDescription* stkd,
    String_t          stack_element_id,
    String_t          floorplan_element_id,
    Temperature_t*    temperatures,
    Temperature_t*    min_temperature,
    Temperature_t*    avg_temperature,
    Temperature_t*    max_temperature
  ) ;

/******************************************************************************/

  int get_all_max_temperatures_in_floorplan
  (
    StackDescription* stkd,
    String_t          stack_element_id,
    Temperature_t*    temperatures,
    Temperature_t*    max_temperature
  ) ;

/******************************************************************************/

  int get_all_min_temperatures_in_floorplan
  (
    StackDescription* stkd,
    String_t          stack_element_id,
    Temperature_t*    temperatures,
    Temperature_t*    min_temperature
  ) ;

/******************************************************************************/

  int get_all_avg_temperatures_in_floorplan
  (
    StackDescription* stkd,
    String_t          stack_element_id,
    Temperature_t*    temperatures,
    Temperature_t*    avg_temperature
  ) ;

/******************************************************************************/

  int get_all_min_avg_max_temperatures_in_floorplan
  (
    StackDescription* stkd,
    String_t          stack_element_id,
    Temperature_t*    temperatures,
    Temperature_t*    min_temperature,
    Temperature_t*    avg_temperature,
    Temperature_t*    max_temperature
  ) ;

/******************************************************************************/

  int print_thermal_map
  (
    StackDescription* stkd,
    Temperature_t*    temperatures,
    String_t          stack_element_id,
    String_t          file_name
  ) ;

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _3DICE_STACK_DESCRIPTION_H_ */
