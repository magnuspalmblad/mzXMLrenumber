/*                                                                                                 */                     
/*  mzXMLrenumber - renumber mzXML scans from 1 (1, 2, 3, ...) or arbitrary starting point (-b)    */
/*                                                                                                 */
/* (c) Magnus Palmblad, Leiden University Medican Center, 2011-                                    */ 
/*                                                                                                 */
/* This program is free software; you can redistribute it and/or modify it under the terms of the  */
/* Creative Commons Attribution-Share Alike 3.0 License                                            */
/* (http://creativecommons.org/licenses/by-sa/3.0/)                                                */
/*                                                                                                 */
/* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;       */
/* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.       */
/*                                                                                                 */
/* Contact information: magnus.palmblad@gmail.com                                                  */
/*                                                                                                 */
/*                                                                                                 */
/* compile with e.g. gcc -o mzXMLrenumber mzXMLrenumber.c                                          */
/*                                                                                                 */
/*                                                                                                 */

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>


/* main program starts here */

int main(int argc, char *argv[]) 
{
  FILE *inp, *outp;
  char *p, mzXML_filename[256], renumbered_mzXML_filename[256], line[65536], token[20][256], num_string[256];
  int i, j, n_tokens;
  long old_scan, new_scan=1;
  
  /* parsing command line parameters */
  
  if( (argc==2) && ( (strcmp(argv[1],"--help")==0) || (strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h")==0)) ) /* want help? */
    {
      printf("mzXMLrenumber - (c) Magnus Palmblad 2010-\n\nusage: mzXMLrenumber -i <input mzXML file> -o <output, renumbered mzXML file> [-b <new first scan number>]. (type mzXMLrenumber --help for more information).\nFor more information, see http://www.ms-utils.org/mzXMLwarp or e-mail magnus.palmblad@gmail.com\n");
      return 0;
    }
  
  if (argc<3 || argc>7) /* incorrect number of parameters */
    {
      printf("Incorrect number of arguments. Usage: mzXMLrenumber -i <input mzXML file> -o <output, renumbered mzXML file> [-b <new first scan number>]. (type mzXMLrenumber --help for more information)\n");
      return -1;
    }
  
  
  /* set default values and parse flags */

  for(i=1;i<argc;i++) {
    if( (argv[i][0]=='-') && (argv[i][1]=='i') ) strcpy(mzXML_filename,&argv[strlen(argv[i])>2?i:i+1][strlen(argv[i])>2?2:0]);
    if( (argv[i][0]=='-') && (argv[i][1]=='o') ) strcpy(renumbered_mzXML_filename,&argv[strlen(argv[i])>2?i:i+1][strlen(argv[i])>2?2:0]);
    if( (argv[i][0]=='-') && (argv[i][1]=='b') ) new_scan=atof(&argv[strlen(argv[i])>2?i:i+1][strlen(argv[i])>2?2:0]);
  }


   /* opening alignment function */
  
  printf("reading and warping mzXML file..."); fflush(stdout);
  
  if ((inp = fopen(mzXML_filename, "r"))==NULL) {printf("error opening mzXML file %s\n",mzXML_filename);return -1;}
  if ((outp = fopen(renumbered_mzXML_filename, "w"))==NULL) {printf("error opening renumbered mzXML file %s for output\n",renumbered_mzXML_filename);return -1;}

  while (fgets(line, 65536, inp)!=NULL) {
    if (strstr(line,"num=")!=NULL) {
      i=0;
      // printf("%s",line); /* for debugging */
      p=strtok(line," ");   
      strcpy(token[i],p); i++;
      //while (p!=NULL) {strcpy(token[i],p); printf("token[%i]=%s\n",i,token[i]); i++; p=strtok('\0'," ");} /* for debugging */
      while (p!=NULL) {strcpy(token[i],p); i++; p=strtok('\0'," ");}
      n_tokens=i;
      
      for(i=1;i<=n_tokens;i++) {
	if (strstr(token[i],"num=")!=NULL) { /* tolower() and checking for = would make it more robust, but probably not needed */
	  //if ((i>0)&&(strstr(token[1],"<scan")!=NULL)) {
	    strcpy(num_string,token[i]+5); /* remove retentionTime="PT from time_string */
	    num_string[strlen(num_string)-1]='\0'; /* remove S" from time_string */
	    old_scan=atoi(num_string); /* convert to int */
	    fprintf(outp,"num=\"%i\" ",new_scan); new_scan++;
	    //}
	    //else {fprintf(outp,"%s",token[i]); if(i<n_tokens-1) fprintf(outp," ");}
       	}
	else {fprintf(outp,"%s",token[i]); if(i<n_tokens-1) fprintf(outp," ");}
      }
    }
    else fprintf(outp,"%s",line); /* lines not containing num */
  } 
  
  printf("done (wrote renumbered file %s)\n",renumbered_mzXML_filename); fflush(stdout);
  
  return 0;
}

