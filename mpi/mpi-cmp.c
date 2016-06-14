/* mpi-cmp.c  -  MPI functions
 * Copyright (C) 1998, 1999 Free Software Foundation, Inc.
 *
 * This file is part of GnuPG.
 *
 * GnuPG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * GnuPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi-internal.h"

int
mpi_cmp_ui( MPI u, unsigned long v )
{
    mpi_limb_t limb = v;

    mpi_normalize( u );
    if( !u->nlimbs && !limb )
	return 0;
    if( u->sign )
	return -1;
    if( u->nlimbs > 1 )
	return 1;

    if( u->d[0] == limb )
	return 0;
    else if( u->d[0] > limb )
	return 1;
    else
	return -1;
}

int
mpi_cmp( MPI u, MPI v )
{
    mpi_size_t usize, vsize;
    int cmp;

    if (mpi_is_opaque (u) || mpi_is_opaque (v))
      {
	if (mpi_is_opaque (u) && !mpi_is_opaque (v))
	  return -1;
	if (!mpi_is_opaque (u) && mpi_is_opaque (v))
	  return 1;
	if (!u->nbits && !v->nbits)
	  return 0; /* Empty buffers are identical.  */
	if (u->nbits < v->nbits)
	  return -1;
	if (u->nbits > v->nbits)
	  return 1;
	return memcmp (u->d, v->d, u->nbits);
      }

    mpi_normalize( u );
    mpi_normalize( v );
    usize = u->nlimbs;
    vsize = v->nlimbs;
    if( !u->sign && v->sign )
	return 1;
    if( u->sign && !v->sign )
	return -1;
    if( usize != vsize && !u->sign && !v->sign )
	return usize - vsize;
    if( usize != vsize && u->sign && v->sign )
	return vsize + usize;
    if( !usize )
	return 0;
    if( !(cmp=mpihelp_cmp( u->d, v->d, usize )) )
	return 0;
    if( (cmp < 0?1:0) == (u->sign?1:0))
	return 1;
    return -1;
}

