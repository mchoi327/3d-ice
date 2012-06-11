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
#include <math.h>

#include "heat_sink.h"
#include "macros.h"

/******************************************************************************/

void init_heat_sink (HeatSink_t *this)
{
    this->SinkModel          = TDICE_HEATSINK_MODEL_NONE ;
    this->NLayers            = 0u ;
    this->SourceLayerOffset  = 0u ;
    this->AmbientHTC         = 0.0 ;
    this->AmbientTemperature = 0.0 ;
    this->SinkHeight         = 0.0 ;
    this->SinkArea           = 0.0 ;
    this->SinkMaterial       = NULL ;
    this->SpreaderHeight     = 0.0 ;
    this->SpreaderArea       = 0.0 ;
    this->SpreaderMaterial   = NULL ;
}

/******************************************************************************/

HeatSink_t *alloc_and_init_heat_sink (void)
{
    HeatSink_t *heat_sink = (HeatSink_t *) malloc (sizeof(HeatSink_t)) ;

    if (heat_sink != NULL)

        init_heat_sink (heat_sink) ;

    return heat_sink ;
}

/******************************************************************************/

void free_heat_sink (HeatSink_t *this)
{
    FREE_POINTER (free, this) ;
}

/******************************************************************************/

void print_formatted_heat_sink
(
  HeatSink_t *this,
  FILE       *stream,
  String_t    prefix
)
{
    if (this->SinkModel == TDICE_HEATSINK_MODEL_CONNECTION_TO_AMBIENT)

        fprintf (stream, "%sconnection to ambient :\n", prefix) ;

    else if (this->SinkModel == TDICE_HEATSINK_MODEL_TRADITIONAL)
    {
        fprintf (stream, "%sheat sink :\n", prefix) ;

        fprintf (stream,
            "%s   sink     height %.2f, area %.1e, material %s ;\n",
            prefix,
            this->SinkHeight,
            this->SinkArea,
            this->SinkMaterial->Id) ;

        fprintf (stream,
            "%s   spreader height %.2f, area %.1e, material %s ;\n",
            prefix,
            this->SpreaderHeight,
            this->SpreaderArea,
            this->SpreaderMaterial->Id) ;
    }

    fprintf (stream,
        "%s   heat transfer coefficient %.4e ;\n",
        prefix, this->AmbientHTC) ;

    fprintf (stream,
        "%s   ambient temperature       %.2f ;\n",
        prefix, this->AmbientTemperature) ;
}

/******************************************************************************/

void print_detailed_heat_sink
(
  HeatSink_t *this,
  FILE       *stream,
  String_t    prefix
)
{
    fprintf (stream,
        "%sheat_sink                   = %p\n",
        prefix,   this) ;

    fprintf (stream,
        "%s  SinkModel                 = %d\n",
        prefix,   this->SinkModel) ;

    fprintf (stream,
        "%s  NLayers                   = %d\n",
        prefix,   this->NLayers) ;

    fprintf (stream,
        "%s  SourceLayerOffset         = %d\n",
        prefix,   this->SourceLayerOffset) ;

    fprintf (stream,
        "%s  AmbientHTC                = %.4e\n",
        prefix,   this->AmbientHTC) ;

    fprintf (stream,
        "%s  AmbientTemperature        = %.2f\n",
        prefix,   this->AmbientTemperature) ;

    fprintf (stream,
        "%s  SinkHeight                = %.2f\n",
        prefix,   this->SinkHeight) ;

    fprintf (stream,
        "%s  SinkArea                  = %.1e\n",
        prefix,   this->SinkArea) ;

    fprintf (stream,
        "%s  SinkMaterial              = %p\n",
        prefix,   this->SinkMaterial) ;

    fprintf (stream,
        "%s  SpreaderHeight            = %.2f\n",
        prefix,   this->SpreaderHeight) ;

    fprintf (stream,
        "%s  SpreaderArea              = %.1e\n",
        prefix,   this->SpreaderArea) ;

    fprintf (stream,
        "%s  SpreaderMaterial          = %p\n",
        prefix,   this->SpreaderMaterial) ;
}

/******************************************************************************/

SolidTC_t get_spreader_volumetric_heat_capacity
(
    HeatSink_t      *this,
    CellDimension_t  chip_area
)
{
    if (this->SinkModel != TDICE_HEATSINK_MODEL_TRADITIONAL)
    {
        fprintf (stderr,
            "Cannot compute spreader volumetric heat capacity (bad model)\n") ;

        return 0.0 ;
    }

    return this->SpreaderMaterial->VolumetricHeatCapacity
           * (this->SpreaderArea / chip_area) ;
}

/******************************************************************************/

SolidTC_t get_sink_volumetric_heat_capacity
(
    HeatSink_t      *this,
    CellDimension_t  chip_area
)
{
    if (this->SinkModel != TDICE_HEATSINK_MODEL_TRADITIONAL)
    {
        fprintf (stderr,
            "Cannot compute sink volumetric heat capacity (bad model)\n") ;

        return 0.0 ;
    }

    return this->SinkMaterial->VolumetricHeatCapacity
           * (this->SinkArea / chip_area) ;
}

/******************************************************************************/

#   define LAMBDA(_e)                     \
                                          \
        ( PI + (1.0 / (sqrt (PI) * _e)) )

#   define PHI(_lambda, _bi, _tau)                          \
                                                            \
        ( (tanh (_lambda * _tau) + (_lambda / _bi))         \
          /                                                 \
          (1.0 + (_lambda / _bi) * tanh (_lambda * _tau)) )

SolidTC_t get_spreader_thermal_conductivity
(
    HeatSink_t      *this,
    ChipDimension_t  chip_area
)
{
    if (this->SinkModel != TDICE_HEATSINK_MODEL_TRADITIONAL)
    {
        fprintf (stderr,
            "Cannot compute spreader thermal conductivity (bad model)\n") ;

        return 0.0 ;
    }

    // FIXME: check varible types (all doubles so far...)

    // To get the equivalent thermal conductivity, some values coming from
    // the sink are needed ...

    CellDimension_t e_sink = sqrt (this->SpreaderArea / this->SinkArea) ;

    CellDimension_t tau_sink = this->SinkHeight / sqrt (this->SinkArea / PI) ;

    SolidTC_t Bi_sink =   (this->AmbientHTC * sqrt (this->SinkArea / PI))

                        / this->SinkMaterial->ThermalConductivity ;

    SolidTC_t g_sink = (  2.0
                        * this->SinkMaterial->ThermalConductivity
                        * sqrt(this->SpreaderArea)
                       )
                       /
                       (pow(1.0 - e_sink, 1.5) * PHI (LAMBDA(e_sink), Bi_sink, tau_sink)) ;

    SolidTC_t gm_sink = this->SinkMaterial->ThermalConductivity

                      * (this->SinkArea / this->SinkHeight) ;

    // Values just for the spreader

    CellDimension_t e   = sqrt (chip_area / this->SpreaderArea) ;

    CellDimension_t tau = this->SpreaderHeight / sqrt (this->SpreaderArea / PI) ;

    AmbientHTC_t R_air  = 1.0 / (this->AmbientHTC * this->SinkArea) ;

    AmbientHTC_t R_sink = 1.0 / PARALLEL (g_sink, gm_sink) ;

    AmbientHTC_t H_sink =   (1.0 / (R_air + R_sink))

                          * (1.0 / this->SpreaderArea) ;

    SolidTC_t Bi =   (H_sink * sqrt (this->SpreaderArea / PI))

                   / this->SpreaderMaterial->ThermalConductivity ;

    SolidTC_t g = (  2.0
                   * this->SpreaderMaterial->ThermalConductivity
                   * sqrt(chip_area)
                  )
                  /
                  (pow(1.0 - e, 1.5) * PHI (LAMBDA(e), Bi, tau)) ;

    SolidTC_t gm = this->SpreaderMaterial->ThermalConductivity

                   * (this->SpreaderArea / this->SpreaderHeight) ;

    return PARALLEL (g, gm) * (this->SpreaderHeight / chip_area) ;
}

/******************************************************************************/

SolidTC_t get_sink_thermal_conductivity
(
    HeatSink_t      *this,
    ChipDimension_t  chip_area
)
{
    if (this->SinkModel != TDICE_HEATSINK_MODEL_TRADITIONAL)
    {
        fprintf (stderr,
            "Cannot compute spreader thermal conductivity (bad model)\n") ;

        return 0.0 ;
    }

    // FIXME: check varible types (all doubles so far...)

    CellDimension_t e   = sqrt (this->SpreaderArea / this->SinkArea) ;

    CellDimension_t tau = this->SinkHeight / sqrt (this->SinkArea / PI) ;

    SolidTC_t Bi =   (this->AmbientHTC * sqrt (this->SinkArea / PI))

                   / this->SinkMaterial->ThermalConductivity ;

    SolidTC_t g = (  2.0
                   * this->SinkMaterial->ThermalConductivity
                   * sqrt(this->SpreaderArea)
                  )
                  /
                  (pow(1.0 - e, 1.5) * PHI (LAMBDA(e), Bi, tau)) ;

    SolidTC_t gm = this->SinkMaterial->ThermalConductivity

                   * (this->SinkArea / this->SinkHeight) ;

    return PARALLEL (g, gm) * (this->SinkHeight / chip_area) ;
}
