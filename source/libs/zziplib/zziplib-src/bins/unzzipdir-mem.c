/*
 *	Copyright (c) 2003 Guido Draheim <guidod@gmx.de>
 *      Use freely under the restrictions of the ZLIB license.
 *
 *      This file is used as an example to clarify zzipmmap api usage.
 */

#include <zzip/memdisk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unzzip.h"

#ifdef ZZIP_HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef ZZIP_HAVE_IO_H
#include <io.h>
#endif

#ifdef ZZIP_HAVE_FNMATCH_H
#include <fnmatch.h>
#else
#define fnmatch(x,y,z) strcmp(x,y)
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif


int 
unzzip_list (int argc, char ** argv)
{
    int argn;
    ZZIP_MEM_DISK* disk;
    
    if (argc == 1)
    {
        printf (__FILE__" version "ZZIP_PACKAGE" "ZZIP_VERSION"\n");
        return -1; /* better provide an archive argument */
    }
    
    disk = zzip_mem_disk_open (argv[1]);
    if (! disk) {
	perror(argv[1]);
	return -1;
    }

    if (argc == 2)
    {  /* list all */
	ZZIP_MEM_ENTRY* entry = zzip_mem_disk_findfirst(disk);
	for (; entry ; entry = zzip_mem_disk_findnext(disk, entry))
	{
	    char* name = zzip_mem_entry_to_name (entry);
	    int compr = entry->zz_compr;
	    long long usize = entry->zz_usize;
	    long long csize = entry->zz_csize;
	    char* defl = compr ? "deflated" : "stored";
	    printf ("%lli/%lli %s %s \n", csize, usize, defl, name);
	}
	return 0;
    }

    if (argc == 3)
    {  /* list from one spec */
	ZZIP_MEM_ENTRY* entry = 0;
	while ((entry = zzip_mem_disk_findmatch(disk, argv[2], entry, 0, 0)))
	{
	    char* name = zzip_mem_entry_to_name (entry);
	    int compr = entry->zz_compr;
	    long long usize = entry->zz_usize;
	    long long csize = entry->zz_csize;
	    char* defl = compr ? "deflated" : "stored";
	    printf ("%lli/%lli %s %s \n", csize, usize, defl, name);
	}
	return 0;
    }

    {   /* list only the matching entries - in order of zip directory */
	ZZIP_MEM_ENTRY* entry = zzip_mem_disk_findfirst(disk);
	for (; entry ; entry = zzip_mem_disk_findnext(disk, entry))
	{
	    char* name = zzip_mem_entry_to_name (entry);
	    for (argn=1; argn < argc; argn++)
	    {
		if (! fnmatch (argv[argn], name, 
			       FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD))
		{
		    int compr = entry->zz_compr;
		    long long usize = entry->zz_usize;
		    long long csize = entry->zz_csize;
		    char* defl = compr ? "deflated" : "stored";
	    	    printf ("%lli/%lli %s %s \n", csize, usize, defl, name);
		    break; /* match loop */
		}
	    }
	}
	return 0;
    }
} 

/* 
 * Local variables:
 * c-file-style: "stroustrup"
 * End:
 */
