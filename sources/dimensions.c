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

#include "dimensions.h"
#include "macros.h"

/******************************************************************************/

void init_cell_dimensions (CellDimensions_t *this)
{
    this->FirstWallLength = (CellDimension_t) 0.0 ;
    this->LastWallLength  = (CellDimension_t) 0.0 ;
    this->WallLength      = (CellDimension_t) 0.0 ;
    this->ChannelLength   = (CellDimension_t) 0.0 ;
    this->Width           = (CellDimension_t) 0.0 ;
    this->NHeights        = (Quantity_t) 0u ;
    this->Heights         = NULL ;
}

/******************************************************************************/

void copy_cell_dimensions (CellDimensions_t *dst, CellDimensions_t *src)
{
    dst->FirstWallLength = src->FirstWallLength ;
    dst->LastWallLength  = src->LastWallLength ;
    dst->WallLength      = src->WallLength ;
    dst->ChannelLength   = src->ChannelLength ;
    dst->Width           = src->Width ;
    dst->NHeights        = src->NHeights ;

    if (src->NHeights == 0)
    {
        dst->Heights = NULL ;

        return ;
    }

    dst->Heights = (CellDimension_t *)

        malloc (src->NHeights * sizeof (CellDimension_t)) ;

    if (dst->Heights == NULL)
    {
        fprintf (stderr, "Malloc heights error\n") ;

        return ;
    }

    memcpy (dst->Heights, src->Heights,
            src->NHeights * sizeof (CellDimension_t) ) ;
}

/******************************************************************************/

void init_grid_dimensions (GridDimensions_t *this)
{
    this->NLayers      = (CellIndex_t) 0u ;
    this->NRows        = (CellIndex_t) 0u ;
    this->NColumns     = (CellIndex_t) 0u ;
    this->NCells       = (CellIndex_t) 0u ;
    this->NConnections = (CellIndex_t) 0u ;
}

/******************************************************************************/

void copy_grid_dimensions (GridDimensions_t *dst, GridDimensions_t *src)
{
    dst->NLayers      = src->NLayers ;
    dst->NRows        = src->NRows ;
    dst->NColumns     = src->NColumns ;
    dst->NCells       = src->NCells ;
    dst->NConnections = src->NConnections ;
}

/******************************************************************************/

void init_chip_dimensions (ChipDimensions_t *this)
{
    this->Length = (CellDimension_t) 0.0 ;
    this->Width  = (CellDimension_t) 0.0 ;
}

/******************************************************************************/

void copy_chip_dimensions (ChipDimensions_t *dst, ChipDimensions_t *src)
{
    dst->Length = src->Length ;
    dst->Width  = src->Width ;
}

/******************************************************************************/

void init_dimensions (Dimensions_t *this)
{
    init_cell_dimensions (&this->Cell) ;
    init_grid_dimensions (&this->Grid) ;
    init_chip_dimensions (&this->Chip) ;
}

/******************************************************************************/

void copy_dimensions (Dimensions_t *dst, Dimensions_t *src)
{
    copy_cell_dimensions (&dst->Cell, &src->Cell) ;
    copy_grid_dimensions (&dst->Grid, &src->Grid) ;
    copy_chip_dimensions (&dst->Chip, &src->Chip) ;
}

/******************************************************************************/

Dimensions_t *calloc_dimensions (void)
{
    Dimensions_t *dimensions = (Dimensions_t *) malloc (sizeof(Dimensions_t)) ;

    if (dimensions != NULL)

        init_dimensions (dimensions) ;

    return dimensions ;
}

/******************************************************************************/

Dimensions_t *clone_dimensions (Dimensions_t *this)
{
    if (this == NULL)

        return NULL ;

    Dimensions_t *dimensions = calloc_dimensions ( ) ;

    if (dimensions != NULL)

        copy_dimensions (dimensions, this) ;

    return dimensions ;
}

/******************************************************************************/

void free_dimensions (Dimensions_t *this)
{
    if (this == NULL)

        return ;

    if (this->Cell.Heights != NULL)

        FREE_POINTER (free, this->Cell.Heights) ;

    FREE_POINTER (free, this) ;
}

/******************************************************************************/

void print_formatted_dimensions
(
    Dimensions_t *this,
    FILE         *stream,
    String_t      prefix
)
{
    fprintf (stream,
            "%sdimensions : \n",
            prefix) ;

    fprintf (stream,
            "%s   chip length %7.1f , width %7.1f ;\n",
            prefix, this->Chip.Length, this->Chip.Width) ;

    fprintf (stream,
            "%s   cell length %7.1f , width %7.1f ;\n",
            prefix, this->Cell.WallLength, this->Cell.Width) ;
}

/******************************************************************************/

void print_detailed_dimensions
(
  Dimensions_t *this,
  FILE         *stream,
  String_t      prefix
)
{
    fprintf (stream,
            "%sdimensions                  = %p\n",
            prefix, this) ;

    fprintf (stream,
            "%s  Cell.FirstWallLength      = %.1f\n",
            prefix, this->Cell.FirstWallLength) ;

    fprintf (stream,
            "%s  Cell.WallLength           = %.1f\n",
            prefix, this->Cell.WallLength) ;

    fprintf (stream,
            "%s  Cell.LastWallLength       = %.1f\n",
            prefix, this->Cell.LastWallLength) ;

    fprintf (stream,
            "%s  Cell.ChannelLength        = %.1f\n",
            prefix, this->Cell.ChannelLength) ;

    fprintf (stream,
            "%s  Cell.Width                = %.1f\n",
            prefix, this->Cell.Width) ;

    fprintf (stream,
            "%s  Cell.NHeigths             = %d\t",
            prefix, this->Cell.NHeights) ;

    fprintf (stream,
            "%s  Cell.Heigths              = %p\t",
            prefix, this->Cell.Heights) ;

    CellIndex_t layer ;

    for (layer = 0 ; layer != this->Cell.NHeights ; layer++)

    fprintf (stream, "%.1f ", this->Cell.Heights [layer]) ;

    fprintf (stream, "\n") ;


    fprintf (stream,
            "%s  Grid.NLayers              = %d\n",
            prefix, this->Grid.NLayers) ;

    fprintf (stream,
            "%s  Grid.NRows                = %d\n",
            prefix, this->Grid.NRows) ;

    fprintf (stream,
            "%s  Grid.NColumns             = %d\n",
            prefix, this->Grid.NColumns) ;

    fprintf (stream,
            "%s  Grid.NCells               = %d\n",
            prefix, this->Grid.NCells) ;

    fprintf (stream,
            "%s  Grid.Nconnections         = %d\n",
            prefix, this->Grid.NConnections) ;

    fprintf (stream,
            "%s  Chip.Length               = %.1f\n",
            prefix, this->Chip.Length) ;

    fprintf (stream,
            "%s  Chip.Width                = %.1f\n",
            prefix, this->Chip.Width) ;
}

/******************************************************************************/

void print_axes (Dimensions_t *this)
{
    FILE *file = fopen ("xaxis.txt", "w") ;

    if (file == NULL)
    {
        fprintf (stderr, "Cannot create text file for x axis\n") ;
        return ;
    }

    FOR_EVERY_COLUMN (column_index, this)

        fprintf (file, "%5.2f\n", get_cell_center_x (this, column_index)) ;

    fclose (file) ;

    file = fopen ("yaxis.txt", "w") ;

    if (file == NULL)
    {
        fprintf (stderr, "Cannot create text file for y axis\n") ;
        return ;
    }

    FOR_EVERY_ROW (row_index, this)

        fprintf (file, "%5.2f\n", get_cell_center_y (this, row_index)) ;

    fclose (file) ;
}

/******************************************************************************/

void compute_number_of_connections
(
    Dimensions_t   *this,
    Quantity_t      num_channels,
    ChannelModel_t  channel_model,
    HeatSinkModel_t sink_model
)
{
    CellIndex_t nlayers  = this->Grid.NLayers ;
    CellIndex_t nrows    = this->Grid.NRows ;
    CellIndex_t ncolumns = this->Grid.NColumns ;

    CellIndex_t nlayers_for_channel    = num_channels * NUM_LAYERS_CHANNEL_2RM ;
    CellIndex_t nlayers_except_channel = nlayers - nlayers_for_channel ;

    CellIndex_t nlayers_heatsink = 0u ;

    switch (sink_model)
    {
        case TDICE_HEATSINK_MODEL_NONE :

            nlayers_heatsink = 0u ;

            break ;

        case TDICE_HEATSINK_MODEL_CONNECTION_TO_AMBIENT :

            nlayers_heatsink = NUM_LAYERS_HEATSINK_CONNECTION_TO_AMBIENT ;

            break ;

        case TDICE_HEATSINK_MODEL_TRADITIONAL :

            nlayers_heatsink = NUM_LAYERS_HEATSINK_TRADITIONAL ;

            break ;

        default :

            fprintf (stderr, "Error: unknown sihk model %d\n", sink_model) ;
    }

    CellIndex_t tmp = 2u ;

    switch (channel_model)
    {
        case TDICE_CHANNEL_MODEL_NONE :
        case TDICE_CHANNEL_MODEL_MC_4RM :
        {
            this->Grid.NConnections =

                // All Normal Cells

                // number of coefficients in the diagonal
                nlayers * nrows * ncolumns
                +
                // number of coefficients bottom <-> top
                2 * (nlayers - 1) * nrows * ncolumns
                +
                // Number of coefficients North <-> South
                2 * (nlayers - nlayers_heatsink) * (nrows - 1) * ncolumns
                +
                // Number of coefficients East <-> West
                2 * (nlayers - nlayers_heatsink) * nrows * (ncolumns - 1) ;

            break ;
        }
        case TDICE_CHANNEL_MODEL_MC_2RM :

            tmp += 2 ;

        case TDICE_CHANNEL_MODEL_PF_INLINE :
        case TDICE_CHANNEL_MODEL_PF_STAGGERED :
        {
            this->Grid.NConnections =

                // For Normal Cells

                // Number of coefficients in the diagonal
                nlayers_except_channel * nrows * ncolumns
                +
                // Number of coefficients Bottom <-> Top
                2 * nlayers_except_channel * nrows * ncolumns
                +
                // Number of coefficients North <-> South
                2 * (nlayers_except_channel - nlayers_heatsink) * (nrows - 1) * ncolumns
                +
                // Number of coefficients East <-> West
                2 * (nlayers_except_channel - nlayers_heatsink) * nrows * (ncolumns - 1)
                +

                // For Channel Cells

                // Number of coefficients in the diagonal
                nlayers_for_channel * nrows * ncolumns
                +
                // Number of coefficients Bottom <-> Top
                2 * (nlayers_for_channel + num_channels) * nrows * ncolumns
                +
                // Number of coefficients North <-> South
                tmp * num_channels * (nrows - 1) * ncolumns
                +
                // Number of coefficients East <-> West
                0
                ;

            break ;
        }
        default :

            fprintf (stderr, "Error: unknown channel model %d\n", channel_model) ;
    }
}

/******************************************************************************/

CellDimension_t get_cell_length
(
  Dimensions_t *this,
  CellIndex_t   column_index
)
{
    // column_index < 0 not tested since CellIndex_t is unsigned

    if (column_index > LAST_COLUMN_INDEX (this))
    {
        fprintf (stderr,
            "ERROR: column index %d is out of range\n", column_index) ;

        return 0.0 ;
    }

    if (IS_FIRST_COLUMN (column_index))

        return this->Cell.FirstWallLength ;

    else if (IS_LAST_COLUMN (column_index, this))

        return this->Cell.LastWallLength ;

    else

        if (column_index & 1)

        return this->Cell.ChannelLength ;

        else

        return this->Cell.WallLength ;
}

/******************************************************************************/

CellDimension_t get_cell_width
(
    Dimensions_t *this,
    CellIndex_t   row_index
)
{
    // column_index < 0 not tested since CellIndex_t is unsigned

    if (row_index > LAST_ROW_INDEX (this))
    {
        fprintf (stderr,
            "ERROR: row index %d is out of range\n", row_index) ;

        return 0.0 ;
    }

    return this->Cell.Width ;
}

/******************************************************************************/

CellDimension_t get_cell_height
(
    Dimensions_t *this,
    CellIndex_t   layer_index
)
{
    if (this->Cell.Heights == NULL)
    {
        fprintf (stderr, "ERROR: vector of heigths does not exist in memory\n") ;

        return 0.0 ;
    }

    // layer_index < 0 not tested since CellIndex_t is unsigned

    if (layer_index > this->Cell.NHeights)
    {
        fprintf (stderr,
            "ERROR: layer index %d is out of range\n", layer_index) ;

        return 0.0 ;
    }

    return this->Cell.Heights [ layer_index ] ;
}

/******************************************************************************/

ChipDimension_t get_cell_center_x
(
    Dimensions_t *this,
    CellIndex_t   column_index
)
{
    if (IS_FIRST_COLUMN (column_index))

        return this->Cell.FirstWallLength / 2.0 ;

    else if (IS_LAST_COLUMN (column_index, this))

        return   (this->Cell.FirstWallLength      )
               + (this->Cell.ChannelLength   / 2.0) * (column_index    )
               + (this->Cell.WallLength      / 2.0) * (column_index - 2)
               + (this->Cell.LastWallLength  / 2.0) ;

    else

        return   (this->Cell.FirstWallLength      )
               + (this->Cell.ChannelLength   / 2.0) * (column_index    )
               + (this->Cell.WallLength      / 2.0) * (column_index - 1) ;
}

/******************************************************************************/

ChipDimension_t get_cell_center_y
(
    Dimensions_t *this,
    CellIndex_t   row_index
)
{
    return    this->Cell.Width / 2.0
           +  this->Cell.Width * row_index ;
}

/******************************************************************************/

ChipDimension_t get_cell_location_x
(
    Dimensions_t *this,
    CellIndex_t   column_index
)
{
    if (IS_FIRST_COLUMN (column_index))

        return 0.0 ;

    else

        return   (this->Cell.FirstWallLength)
               + (this->Cell.ChannelLength  ) * ((column_index    ) / 2u)
               + (this->Cell.WallLength     ) * ((column_index - 1) / 2u) ;
}

/******************************************************************************/

ChipDimension_t get_cell_location_y
(
    Dimensions_t *this,
    CellIndex_t   row_index
)
{
    return this->Cell.Width * row_index ;
}

/******************************************************************************/

CellIndex_t get_number_of_layers (Dimensions_t *this)
{
    return this->Grid.NLayers ;
}

/******************************************************************************/

CellIndex_t get_number_of_rows (Dimensions_t *this)
{
    return this->Grid.NRows ;
}

/******************************************************************************/

CellIndex_t get_number_of_columns (Dimensions_t *this)
{
    return this->Grid.NColumns ;
}

/******************************************************************************/

CellIndex_t get_number_of_cells (Dimensions_t *this)
{
    return this->Grid.NCells ;
}

/******************************************************************************/

CellIndex_t get_number_of_connections (Dimensions_t *this)
{
    return this->Grid.NConnections ;
}

/******************************************************************************/

CellIndex_t get_layer_area (Dimensions_t *this)
{
    return this->Grid.NRows * this->Grid.NColumns ;
}

/******************************************************************************/

CellIndex_t get_cell_offset_in_layer
(
    Dimensions_t *this,
    CellIndex_t   row_index,
    CellIndex_t   column_index
)
{
    return row_index * get_number_of_columns (this) + column_index ;
}

/******************************************************************************/

CellIndex_t get_cell_offset_in_stack
(
    Dimensions_t *this,
    CellIndex_t   layer_index,
    CellIndex_t   row_index,
    CellIndex_t   column_index
)
{
    return layer_index * get_layer_area (this)
           + get_cell_offset_in_layer (this, row_index, column_index) ;
}

/******************************************************************************/

ChipDimension_t get_chip_length (Dimensions_t *this)
{
    return this->Chip.Length ;
}

/******************************************************************************/

ChipDimension_t get_chip_width (Dimensions_t *this)
{
    return this->Chip.Width ;
}

/******************************************************************************/

ChipDimension_t get_chip_area (Dimensions_t *this)
{
    return get_chip_length (this) * get_chip_width (this) ;
}

/******************************************************************************/
