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

#include "stack_description.h"
#include "thermal_grid_data.h"
#include "macros.h"
#include "layer.h"
#include "system_matrix.h"

#include "../bison/stack_description_parser.h"
#include "../flex/stack_description_scanner.h"

/******************************************************************************/

extern int  stack_description_parse (StackDescription* stkd, yyscan_t scanner) ;
static int  fill_floorplans         (StackDescription* stkd) ;

/******************************************************************************/

void init_stack_description (StackDescription* stkd)
{
  stkd->FileName             = STRING_I ;
  stkd->MaterialsList        = NULL ;
  stkd->ConventionalHeatSink = NULL ;
  stkd->Channel              = NULL ;
  stkd->DiesList             = NULL ;
  stkd->StackElementsList    = NULL ;
  stkd->Dimensions           = NULL ;
}

/******************************************************************************/

int fill_stack_description
(
  StackDescription* stkd,
  String_t          filename
)
{
  FILE*    input ;
  int      result ;
  yyscan_t scanner ;

  input = fopen (filename, "r") ;
  if(input == NULL)
  {
    perror(filename) ;
    return -1 ;
  }

  stkd->FileName = strdup (filename) ;

  stack_description_lex_init (&scanner) ;
  stack_description_set_in (input, scanner) ;

  result = stack_description_parse (stkd, scanner) ;

  stack_description_lex_destroy (scanner) ;
  fclose (input) ;

  if (result == 1) return result ;

  return fill_floorplans (stkd) ;
}

/******************************************************************************/

void free_stack_description (StackDescription* stkd)
{
  free_materials_list         (stkd->MaterialsList) ;
  free_channel                (stkd->Channel) ;
  free_dies_list              (stkd->DiesList) ;
  free_conventional_heat_sink (stkd->ConventionalHeatSink) ;
  free_stack_elements_list    (stkd->StackElementsList) ;
  free_dimensions             (stkd->Dimensions) ;
  free                        (stkd->FileName) ;
}

/******************************************************************************/

void print_formatted_stack_description
(
  FILE*             stream,
  String_t          prefix,
  StackDescription* stkd
)
{
  print_formatted_materials_list (stream, prefix, stkd->MaterialsList) ;
  fprintf (stream, STRING_F "\n", prefix) ;

  if (stkd->ConventionalHeatSink != NULL)
  {
    print_formatted_conventional_heat_sink (stream, prefix,
                                            stkd->ConventionalHeatSink) ;
    fprintf (stream, STRING_F "\n", prefix) ;
  }

  if (stkd->Channel != NULL)
  {
    print_formatted_channel (stream, prefix, stkd->Channel, stkd->Dimensions) ;
    fprintf (stream, STRING_F "\n", prefix) ;
  }

  print_formatted_dies_list (stream, prefix, stkd->DiesList) ;
  fprintf (stream, STRING_F "\n", prefix) ;

  print_formatted_stack_elements_list (stream, prefix,
                                       stkd->StackElementsList) ;
  fprintf (stream, STRING_F "\n", prefix) ;

  print_formatted_dimensions (stream, prefix, stkd->Dimensions) ;
}

/******************************************************************************/

void print_detailed_stack_description
(
  FILE*             stream,
  String_t          prefix,
  StackDescription* stkd
)
{
  String_t new_prefix = malloc (sizeof(*new_prefix) * (4 + strlen(prefix))) ;
  if (new_prefix == NULL) return ;
  sprintf (new_prefix, STRING_F "    ", prefix) ;

  fprintf (stream,
           STRING_F "stkd                            = %p\n",
           prefix, stkd) ;

  fprintf (stream,
           STRING_F "  FileName                      = " STRING_F "\n",
           prefix, stkd->FileName) ;

  fprintf (stream,
           STRING_F "  MaterialsList                 = %p\n",
           prefix, stkd->MaterialsList) ;

  fprintf (stream, STRING_F "\n", prefix) ;
  print_detailed_materials_list (stream, new_prefix, stkd->MaterialsList) ;
  fprintf (stream, STRING_F "\n", prefix) ;

  fprintf (stream,
           STRING_F "  ConventionalHeatSink          = %p\n",
           prefix, stkd->ConventionalHeatSink) ;

  if (stkd->ConventionalHeatSink != NULL)
  {
    fprintf (stream, STRING_F "\n", prefix) ;
    print_detailed_conventional_heat_sink (stream, new_prefix,
                                           stkd->ConventionalHeatSink) ;
    fprintf (stream, STRING_F "\n", prefix) ;
  }

  fprintf (stream,
           STRING_F "  Channel                       = %p\n",
           prefix, stkd->Channel) ;

  if (stkd->Channel != NULL)
  {
    fprintf (stream, STRING_F "\n", prefix) ;
    print_detailed_channel (stream, new_prefix, stkd->Channel) ;
    fprintf (stream, STRING_F "\n", prefix) ;
  }

  fprintf (stream,
           STRING_F "  DiesList                      = %p\n",
           prefix, stkd->DiesList) ;

  fprintf (stream, STRING_F "\n", prefix) ;
  print_detailed_dies_list (stream, new_prefix, stkd->DiesList) ;
  fprintf (stream, STRING_F "\n", prefix) ;

  fprintf (stream,
           STRING_F "  StackElementsList             = %p\n",
           prefix, stkd->StackElementsList) ;

  fprintf (stream, STRING_F "\n", prefix) ;
  print_detailed_stack_elements_list (stream, new_prefix,
                                      stkd->StackElementsList) ;
  fprintf (stream, STRING_F "\n", prefix) ;

  fprintf (stream,
           STRING_F "  Dimensions                    = %p\n",
           prefix, stkd->Dimensions) ;

  fprintf (stream, STRING_F "\n", prefix) ;
  print_detailed_dimensions (stream, new_prefix, stkd->Dimensions) ;

  free (new_prefix) ;
}

/******************************************************************************/

void print_all_floorplans
(
  FILE*             stream,
  String_t          prefix,
  StackDescription* stkd
)
{
  FOR_EVERY_ELEMENT_IN_LIST_FORWARD (StackElement, stk_el, stkd->StackElementsList)
  {
    if (stk_el->Type == TDICE_STACK_ELEMENT_DIE)

      print_floorplan(stream, prefix, stk_el->Floorplan) ;
  }
}

/******************************************************************************/

int fill_floorplans (StackDescription* stkd)
{
  FOR_EVERY_ELEMENT_IN_LIST_FORWARD (StackElement, stk_el, stkd->StackElementsList)
  {
    if (stk_el->Type == TDICE_STACK_ELEMENT_DIE)

      if (fill_floorplan (stk_el->Floorplan, stkd->Dimensions) == 1)

        return 1 ;
  }

  return 0 ;
 }

/******************************************************************************/

void fill_thermal_grid_data_stack_description
(
  ThermalGridData*  thermalgriddata,
  StackDescription* stkd
)
{
#ifdef PRINT_THERMAL_GRID_DATA
  fprintf (stderr,

    "Fill thermal grid data stack description ( %d layers )\n",

    get_number_of_layers  (stkd->Dimensions)) ;
#endif

  FOR_EVERY_ELEMENT_IN_LIST_FORWARD (StackElement, stack_element,
                                     stkd->StackElementsList)

    fill_thermal_grid_data_stack_element (thermalgriddata, stack_element) ;

}

/******************************************************************************/

void fill_sources_stack_description
(
  Source_t*         sources,
  ThermalGridData*  thermalgriddata,
  StackDescription* stkd
)
{
#ifdef PRINT_SOURCES
  fprintf (stderr,
    "fill_sources_stack_description ( l %d r %d c %d )\n",
    get_number_of_layers  (stkd->Dimensions),
    get_number_of_rows    (stkd->Dimensions),
    get_number_of_columns (stkd->Dimensions)) ;
#endif

  FOR_EVERY_ELEMENT_IN_LIST_FORWARD (StackElement, stack_element,
                                     stkd->StackElementsList)

    sources = fill_sources_stack_element
              (
                sources, stkd->Dimensions,
                thermalgriddata, stkd->ConventionalHeatSink,
                stack_element
              ) ;
}

/******************************************************************************/

void update_sources_stack_description
(
  Source_t*         sources,
  StackDescription* stkd
)
{
#ifdef PRINT_SOURCES
  fprintf (stderr,
    "update_sources_stack_description ( l %d r %d c %d )\n",
    get_number_of_layers  (stkd->Dimensions),
    get_number_of_rows    (stkd->Dimensions),
    get_number_of_columns (stkd->Dimensions)) ;
#endif

  FOR_EVERY_ELEMENT_IN_LIST_FORWARD (StackElement, stack_element,
                                     stkd->StackElementsList)

    update_sources_stack_element (sources, stkd->Dimensions, stack_element) ;
}

/******************************************************************************/

void fill_system_matrix_stack_description
(
  SystemMatrix          system_matrix,
  ThermalGridData*      thermalgriddata,
  StackDescription*     stkd
)
{
#ifdef PRINT_SYSTEM_MATRIX
  fprintf
  (
    stderr,
    "fill_system_matrix_stack_description ( l %d r %d c %d )\n",
    get_number_of_layers  (stkd->Dimensions),
    get_number_of_rows    (stkd->Dimensions),
    get_number_of_columns (stkd->Dimensions)) ;
#endif
  *system_matrix.ColumnPointers++ = SYSTEMMATRIXCOLUMN_I ;

  FOR_EVERY_ELEMENT_IN_LIST_FORWARD (StackElement, stack_element,
                                     stkd->StackElementsList)

    system_matrix = fill_system_matrix_stack_element
                    (
                      system_matrix, stkd->Dimensions,
                      thermalgriddata,
                      stack_element
                    ) ;
}

/******************************************************************************/

Quantity_t get_number_of_remaining_power_values (StackDescription* stkd)
{
  StackElement* stk_el = stkd->StackElementsList ;
  while (stk_el != NULL && stk_el->Type != TDICE_STACK_ELEMENT_DIE)
    stk_el = stk_el->Next ;

  // if stk_el == NULL then BUG !!!

  return stk_el->Floorplan->ElementsList->PowerValues->Length ;
}

/******************************************************************************/

Quantity_t get_number_of_floorplan_elements
(
  StackDescription* stkd,
  String_t          floorplan_id
)
{
  StackElement* stk_el = find_stack_element_in_list
                         (
                           stkd->StackElementsList,
                           floorplan_id
                         ) ;
  if (stk_el == NULL)

    return -1 ;

  if (stk_el->Type != TDICE_STACK_ELEMENT_DIE || stk_el->Floorplan == NULL)

    return -2 ;

  return stk_el->Floorplan->NElements ;
}

/******************************************************************************/

Quantity_t get_number_of_channel_outlets (StackDescription* stkd)
{
  return (get_number_of_columns(stkd->Dimensions) - 2) / (Quantity_t) 2 ;
}

/******************************************************************************/
