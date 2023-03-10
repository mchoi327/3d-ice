/******************************************************************************
 * This file is part of 3D-ICE, version 3.1.0 .                               *
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
 *                             Copyright (C) 2021                             *
 *   Embedded Systems Laboratory - Ecole Polytechnique Federale de Lausanne   *
 *                            All Rights Reserved.                            *
 *                                                                            *
 * Authors: Arvind Sridhar              Alessandro Vincenzi                   *
 *          Giseong Bak                 Martino Ruggiero                      *
 *          Thomas Brunschwiler         Eder Zulian                           *
 *          Federico Terraneo           Darong Huang                          *
 *          Luis Costero                Marina Zapater                        *
 *          David Atienza                                                     *
 *                                                                            *
 * For any comment, suggestion or request  about 3D-ICE, please  register and *
 * write to the mailing list (see http://listes.epfl.ch/doc.cgi?liste=3d-ice) *
 * Any usage  of 3D-ICE  for research,  commercial or other  purposes must be *
 * properly acknowledged in the resulting products or publications.           *
 *                                                                            *
 * EPFL-STI-IEL-ESL                     Mail : 3d-ice@listes.epfl.ch          *
 * Batiment ELG, ELG 130                       (SUBSCRIPTION IS NECESSARY)    *
 * Station 11                                                                 *
 * 1015 Lausanne, Switzerland           Url  : http://esl.epfl.ch/3d-ice      *
 ******************************************************************************/

#include "gridmapper.h"

using namespace std;

struct Coord
{
    Coord(double x, double y) : x(x), y(y) {}
    double x,y;
};

static FragmentGrid mapGridsRowSwap(Grid source, Grid target, double featurePerUnitArea)
{
    double sourceWidth =source.x/source.Nc;
    double sourceHeight=source.y/source.Nr;
    double targetWidth =target.x/target.Nc;
    double targetHeight=target.y/target.Nr;
    FragmentGrid result(source.Nr,source.Nc);
    for(int i=0;i<source.Nc;i++)
    {
        for(int j=0;j<source.Nr;j++)
        {
            //Clip rectangle coordinates
            Coord a(source.x0+i*sourceWidth,     source.y0+j*sourceHeight);
            Coord b(source.x0+(i+1)*sourceWidth, source.y0+(j+1)*sourceHeight);
            auto& frags=result.at(j,i);
            for(int h=0;h<target.Nc;h++)
            {
                for(int k=0;k<target.Nr;k++)
                {
                    //Candidate fragment coordinates
                    Coord p(target.x0+h*targetWidth,     target.y0+k*targetHeight);
                    Coord q(target.x0+(h+1)*targetWidth, target.y0+(k+1)*targetHeight);
                    //Find rectangle which is the non-empty intersection of the
                    //fragment rectangle with the clip rectangle
                    double xa=max(a.x,p.x);
                    double xb=min(b.x,q.x);
                    if(xa>=xb) continue; //Empty intersection

                    double ya=max(a.y,p.y);
                    double yb=min(b.y,q.y);
                    if(ya>=yb) continue; //Empty intersection

                    double fragmentArea=(xb-xa)*(yb-ya);
                    frags.push_back(Fragment(k,h,featurePerUnitArea*fragmentArea));
                }
            }
        }
    }
    return result;
}

static FragmentGrid mapGridsNoRowSwap(Grid source, Grid target, double featurePerUnitArea)
{
    double sourceWidth =source.x/source.Nc;
    double sourceHeight=source.y/source.Nr;
    double targetWidth =target.x/target.Nc;
    double targetHeight=target.y/target.Nr;
    FragmentGrid result(source.Nr,source.Nc);
    for(int i=0;i<source.Nc;i++)
    {
        for(int j=0;j<source.Nr;j++)
        {
            //Clip rectangle coordinates
            Coord a(source.x0+i*sourceWidth,     source.y0+(source.Nr-1-j)*sourceHeight);
            Coord b(source.x0+(i+1)*sourceWidth, source.y0+(source.Nr-j)*sourceHeight);
            auto& frags=result.at(j,i);
            for(int h=0;h<target.Nc;h++)
            {
                for(int k=0;k<target.Nr;k++)
                {
                    //Candidate fragment coordinates
                    Coord p(target.x0+h*targetWidth,     target.y0+(target.Nr-1-k)*targetHeight);
                    Coord q(target.x0+(h+1)*targetWidth, target.y0+(target.Nr-k)*targetHeight);
                    //Find rectangle which is the non-empty intersection of the
                    //fragment rectangle with the clip rectangle
                    double xa=max(a.x,p.x);
                    double xb=min(b.x,q.x);
                    if(xa>=xb) continue; //Empty intersection

                    double ya=max(a.y,p.y);
                    double yb=min(b.y,q.y);
                    if(ya>=yb) continue; //Empty intersection

                    double fragmentArea=(xb-xa)*(yb-ya);
                    frags.push_back(Fragment(k,h,featurePerUnitArea*fragmentArea));
                }
            }
        }
    }
    return result;
}

FragmentGrid mapGrids(Grid source, Grid target, double featurePerUnitArea, bool swapRows)
{
    if(swapRows) return mapGridsRowSwap(source,target,featurePerUnitArea);
    else         return mapGridsNoRowSwap(source,target,featurePerUnitArea);
}

ostream& operator<<(ostream& os, const FragmentGrid& fg)
{
    for(int r=0;r<fg.row();r++)
    {
        for(int c=0;c<fg.col();c++)
        {
            os<<"source["<<r<<","<<c<<"]=";
            auto& frags=fg.at(r,c);
            if(frags.empty()) os<<"0\n";
            else {
                for(unsigned int f=0;f<frags.size();f++)
                {
                    os<<frags.at(f).feature<<"*target["
                      <<frags.at(f).r<<","<<frags.at(f).c
                      <<(f==frags.size()-1 ? "]\n" : "]+");
                }
            }
        }
    }
    return os;
}
