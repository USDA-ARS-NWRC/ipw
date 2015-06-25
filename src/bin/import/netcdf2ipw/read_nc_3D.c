/** \file
Read a sample 3D file.  Won't work for anything other than 3D
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<math.h>
#include <netcdf.h>

/* This is the name of the data file we will read. */
#define FILE_NAME "pres_temp_3D.nc"
//#define FILE_NAME "../../WRF/RealTimeData/wrfout_d02_2015-02-28.nc"

/* Define what variables to return */
#define VAR "temperature"
//#define VAR "T2"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}

/* Debug level */
#define DEBUG 2

int
main()
{
	/* This will be the netCDF ID for the file and data variable. */
	int ncid;

	/* We will learn about the data file and store results in these
         program variables. */
	int ndims, nvars, ngatts, unlimdimid;
	int i, j, retval, rec;

	/* Open the file. NC_NOWRITE tells netCDF we want read-only access
	 * to the file.*/
	if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
		ERR(retval);

	/* There are a number of inquiry functions in netCDF which can be
      used to learn about an unknown netCDF file. NC_INQ tells how
      many netCDF variables, dimensions, and global attributes are in
      the file; also the dimension id of the unlimited dimension, if
      there is one. */
	if ((retval = nc_inq(ncid, &ndims, &nvars, &ngatts, &unlimdimid)))
		ERR(retval);

	if (DEBUG > 0)
	{
		printf("Number of dimensions: %d\n",ndims);
		printf("Number of variables: %d\n",nvars);
		printf("Number of attributes: %d\n",ngatts);
	}


	/*
	 * Get information about the user variables
	 */
	int varid, vdims;

	/* Get the variable id for the desired variable */
	if (( retval = nc_inq_varid (ncid, VAR, &varid) ))
		ERR(retval);

	// get the number of dimensions for the variable
	if (( retval = nc_inq_varndims(ncid, varid, &vdims) ))
		ERR(retval);

	/* Get the dimension id's */
	int dimid[vdims];
	if (( retval = nc_inq_vardimid (ncid, varid, dimid) ))
		ERR(retval);

	if (DEBUG > 0)
		printf("Variable ID for '%s': %i, Dimensions %i \n", VAR, varid, vdims);


	/* Determine how large the dimensions are */
	size_t tmp, dlen[vdims];
	char dimname[NC_MAX_NAME+1];

	for (j = 0; j < vdims; j++)
	{
		if (( retval = nc_inq_dimlen (ncid, dimid[j], &tmp) ))
			ERR(retval);
		dlen[j] = tmp;

		if (DEBUG > 0)
		{
			if (( retval = nc_inq_dimname (ncid, dimid[j], &dimname) ))
					ERR(retval);
			printf("Dimension id: %i, %s = %i \n",dimid[j], dimname, dlen[j]);
		}
	}

	/* Allocate space for the data */
	float data[dlen[1]][dlen[2]];

	/* The start and count arrays will tell the netCDF library where to read our data. */
	size_t start[vdims], count[vdims];
	count[0] = 1;		// read in one time step
	count[1] = dlen[1];	// read in full length
	count[2] = dlen[2];
	start[1] = 0;
	start[2] = 0;

	/* Read and check one record at a time. */
	for (rec = 0; rec < dlen[0]; rec++)
	{
		start[0] = rec;
		if ((retval = nc_get_vara_float(ncid, varid, start, count, &data[0][0])))
			ERR(retval);

		if (DEBUG > 1)
		{
			printf("Data record %i:\n",rec);
			for (i = 0; i < dlen[1]; i++) {
				for (j = 0; j < dlen[2]; j++) {
					printf("%2.1f ", data[i][j]);
				}
				printf("\n");
			}
		}


	} /* next record */

	/* Close the file, freeing all resources. */
	if ((retval = nc_close(ncid)))
		ERR(retval);

	printf("*** SUCCESS reading example file %s!\n", FILE_NAME);
	return 0;
}

