/********************** ********************************************************
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

#include <stdlib.h>

#include "thermal_cell.h"
#include "macros.h"

/******************************************************************************/

void fill_solid_cell_bottom
(
# ifdef PRINT_THERMAL_CELLS
  Dimensions*           dimensions,
  GridDimension_t       layer_index,
  GridDimension_t       row_index,
  GridDimension_t       column_index,
# endif
  ThermalCell*          thermalcell,
  CellDimension_t       cell_length,
  CellDimension_t       cell_width,
  CellDimension_t       cell_height,
  SolidTC_t             solid_tc,
  SolidVHC_t            solid_vhc,
  Time_t                delta_time
)
{
  thermalcell->North = thermalcell->South
    = (solid_tc * cell_length * cell_height )
      / ( cell_width / (CellDimension_t) 2);

  thermalcell->East = thermalcell->West
    = (solid_tc * cell_width * cell_height )
      / (cell_length / (CellDimension_t) 2) ;

  thermalcell->Bottom = (Conductance_t) 0 ;

  thermalcell->Top
    = (solid_tc * cell_length * cell_width) / cell_height ;

  thermalcell->Capacity
    = ((cell_length * cell_width * cell_height) * solid_vhc) / delta_time ;

# ifdef PRINT_THERMAL_CELLS
  fprintf (stderr,

    "  l %2d r %4d c %4d [%7d] | l %6.1f w %6.1f h %6.1f "  \
    "| N % .5e  S % .5e  E % .5e  W % .5e  T % .5e  B % .5e |\n",

    layer_index, row_index, column_index,
    get_cell_offset_in_stack (dimensions,
                              layer_index, row_index, column_index),
    cell_length, cell_width, cell_height,
    thermalcell->North, thermalcell->South, thermalcell->East,
    thermalcell->West, thermalcell->Top, thermalcell->Bottom
  ) ;
# endif
}

/******************************************************************************/

void fill_solid_cell_central
(
# ifdef PRINT_THERMAL_CELLS
  Dimensions*           dimensions,
  GridDimension_t       layer_index,
  GridDimension_t       row_index,
  GridDimension_t       column_index,
# endif
  ThermalCell*          thermalcell,
  CellDimension_t       cell_length,
  CellDimension_t       cell_width,
  CellDimension_t       cell_height,
  SolidTC_t             solid_tc,
  SolidVHC_t            solid_vhc,
  Time_t                delta_time
)
{
  thermalcell->North = thermalcell->South
    = (solid_tc * cell_length * cell_height )
      / ( cell_width / (CellDimension_t) 2);

  thermalcell->East = thermalcell->West
    = (solid_tc * cell_width * cell_height )
      / (cell_length / (CellDimension_t) 2) ;

  thermalcell->Top = thermalcell->Bottom
    = (solid_tc * cell_length * cell_width )
      / (cell_height / (CellDimension_t) 2);

  thermalcell->Capacity
    = ((cell_length * cell_width * cell_height) * solid_vhc) / delta_time ;

# ifdef PRINT_THERMAL_CELLS
  fprintf (stderr,

    "  l %2d r %4d c %4d [%d] | l %6.1f w %6.1f h %6.1f "  \
    "| N % .5e  S % .5e  E % .5e  W % .5e  T % .5e  B % .5e |\n",

    layer_index, row_index, column_index,
    get_cell_offset_in_stack (dimensions,
                              layer_index, row_index, column_index),
    cell_length, cell_width, cell_height,
    thermalcell->North, thermalcell->South, thermalcell->East,
    thermalcell->West, thermalcell->Top, thermalcell->Bottom
  ) ;
# endif
}

/******************************************************************************/

void fill_solid_cell_top
(
# ifdef PRINT_THERMAL_CELLS
  Dimensions*           dimensions,
  GridDimension_t       layer_index,
  GridDimension_t       row_index,
  GridDimension_t       column_index,
# endif
  ThermalCell*          thermalcell,
  CellDimension_t       cell_length,
  CellDimension_t       cell_width,
  CellDimension_t       cell_height,
  SolidTC_t             solid_tc,
  SolidVHC_t            solid_vhc,
  Time_t                delta_time
)
{
  thermalcell->North = thermalcell->South
    = (solid_tc * cell_length * cell_height )
      / ( cell_width / (CellDimension_t) 2);

  thermalcell->East = thermalcell->West
    = (solid_tc * cell_width * cell_height )
      / (cell_length / (CellDimension_t) 2) ;

  thermalcell->Bottom
    = (solid_tc * cell_length * cell_width) / cell_height ;

  thermalcell->Top = (Conductance_t) 0 ;

  thermalcell->Capacity
    = ((cell_length * cell_width * cell_height) * solid_vhc) / delta_time ;

# ifdef PRINT_THERMAL_CELLS
  fprintf (stderr,

    "  l %2d r %4d c %4d [%7d] | l %6.1f w %6.1f h %6.1f "  \
    "| N % .5e  S % .5e  E % .5e  W % .5e  T % .5e  B % .5e |\n",

    layer_index, row_index, column_index,
    get_cell_offset_in_stack (dimensions,
                              layer_index, row_index, column_index),
    cell_length, cell_width, cell_height,
    thermalcell->North, thermalcell->South, thermalcell->East,
    thermalcell->West, thermalcell->Top, thermalcell->Bottom
  ) ;
# endif
}

/******************************************************************************/

void fill_solid_cell_conventional_heat_sink
(
# ifdef PRINT_THERMAL_CELLS
  Dimensions*           dimensions,
  GridDimension_t       layer_index,
  GridDimension_t       row_index,
  GridDimension_t       column_index,
# endif
  ThermalCell*          thermalcell,
  CellDimension_t       cell_length,
  CellDimension_t       cell_width,
  CellDimension_t       cell_height,
  SolidTC_t             thermal_conductivity,
  AmbientHTC_t          ambient_htc
)
{
  thermalcell->Bottom
    = (thermal_conductivity * cell_length * cell_width)
      / (cell_height / (CellDimension_t) 2) ;

  thermalcell->Top
    = (
        (Conductance_t) 2 * thermal_conductivity
          * ambient_htc * cell_length * cell_width
      )
      /
      (
        cell_height * ambient_htc
        +
        (Conductance_t) 2 * thermal_conductivity
      ) ;

# ifdef PRINT_THERMAL_CELLS
  fprintf (stderr,

    "  l %2d r %4d c %4d [%7d] | l %6.1f w %6.1f h %6.1f "  \
    "|                                     T % .5e  B % .5e |\n",

    layer_index, row_index, column_index,
    get_cell_offset_in_stack (dimensions,
                              layer_index, row_index, column_index),
    cell_length, cell_width, cell_height,
    thermalcell->Top, thermalcell->Bottom
  ) ;
# endif

}

/******************************************************************************/

void fill_liquid_cell
(
# ifdef PRINT_THERMAL_CELLS
  GridDimension_t        layer_index,
  GridDimension_t        row_index,
  GridDimension_t        column_index,
# endif
  Dimensions*            dimensions,
  ThermalCell*           thermalcell,
  CellDimension_t        cell_length,
  CellDimension_t        cell_width,
  CellDimension_t        cell_height,
  CoolantHTCs_t          coolant_htcs,
  CoolantVHC_t           coolant_vhc,
  CoolantFR_t            coolant_fr,
  Time_t                 delta_time
)
{
  Cconv_t C = CCONV (get_number_of_columns (dimensions),
                     coolant_vhc, coolant_fr) ;

  thermalcell->North =  C ;
  thermalcell->South = -C ;

  thermalcell->East = thermalcell->West
    = coolant_htcs.Side * cell_width * cell_height ;

  thermalcell->Top = coolant_htcs.Top * cell_width * cell_length ;

  thermalcell->Bottom = coolant_htcs.Bottom * cell_width * cell_length ;

  thermalcell->Capacity
    = ((cell_length * cell_width * cell_height) * coolant_vhc) / delta_time ;

# ifdef PRINT_THERMAL_CELLS
  fprintf (stderr,

    "  l %2d r %4d c %4d [%7d] | l %6.1f w %6.1f h %6.1f "  \
    "| N % .5e  S % .5e  E % .5e  W % .5e  T % .5e  B % .5e |\n",

    layer_index, row_index, column_index,
    get_cell_offset_in_stack (dimensions,
                              layer_index, row_index, column_index),
    cell_length, cell_width, cell_height,
    thermalcell->North, thermalcell->South, thermalcell->East,
    thermalcell->West, thermalcell->Top, thermalcell->Bottom
  ) ;
# endif

}

/******************************************************************************/
