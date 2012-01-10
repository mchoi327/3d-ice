/******************************************************************************
 * This file is part of 3D-ICE, version 2.0 .                                 *
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include "types.h"

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf (stderr, "Usage: \"%s file.stk\n", argv[0]) ;

        return EXIT_FAILURE ;
    }

    /**************************************************************************/

    NetworkSocket_t server_socket = socket (AF_INET, SOCK_STREAM, 0) ;

    if (server_socket < 0)
    {
        perror ("ERROR :: server socket creation") ;

        return TDICE_FAILURE ;
    }

    /**************************************************************************/

    struct sockaddr_in server_address ;

    memset ((void *) &server_address, 0, sizeof (struct sockaddr_in)) ;

    server_address.sin_family      = AF_INET ;
    server_address.sin_port        = htons (10024) ;
    server_address.sin_addr.s_addr = htonl (INADDR_ANY) ;

    /**************************************************************************/

    int result = bind (server_socket,
                       (struct sockaddr *) &server_address,
                       sizeof (struct sockaddr_in)) ;

    if (result < 0)
    {
        perror ("ERROR :: server bind") ;

        close (server_socket) ;

        return EXIT_FAILURE ;
    }

    /**************************************************************************/

    result = listen (server_socket, 1) ;

    if (result < 0)
    {
        perror ("ERROR :: server listen") ;

        close (server_socket) ;

        return EXIT_FAILURE ;
    }

    /**************************************************************************/

    NetworkSocket_t client_socket = accept (server_socket, ( struct sockaddr *) NULL , NULL ) ;

    if (client_socket < 0)
    {
        perror ("ERROR :: server accept") ;

        close (server_socket) ;

        return EXIT_FAILURE ;
    }

    /**************************************************************************/

    /* receive and send ... */

    /**************************************************************************/

    close (client_socket) ;
    close (server_socket) ;

    return EXIT_SUCCESS ;
}
