/*==============================================================*/
/* aux/sdu.c : smart disk usage counter				*/
/*==============================================================*/

#include "specific.h"

bool		PrintOnlyTotal	= FALSE;
int
handleDir(
  char	      * dirName			/* directory name */
);
int
handleFile(
  char	      * fileName		/* file name */
);
void
usage(
  void
);

/*--------------------------------------------------------------*/
/* main driver							*/
/*--------------------------------------------------------------*/

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  int		argd = 1;		/* argument index */
  int		total;			/* total usage */

  /* arguments */
  while ((argd < argc) && (argv[argd][0] == '-')){
    switch(argv[argd][1]){
     case 'T' :
      PrintOnlyTotal = TRUE;
      break;
     default :
      fprintf(stderr, "unknown flag \"%s\"\n", argv[argd]);
      usage();
      exit(1);
      break;
    }
    argd++;
  }

  /* no file arguments */
  if (argd == argc){
    usage();
    exit(0);
  }
  /* work */
  else {
    for (total=0; argd<argc; argd++){
      total += handleFile(argv[argd]);
    }
  }

  /* total */
  printf(PRINT_FORMAT, total, "total");

  /* finished */
  return 0;
}

/*--------------------------------------------------------------*/
/* support functions						*/
/*--------------------------------------------------------------*/

/*
 * @ handleDir : recurse into sub-directory
 * > total usage
 * + report contents' sizes
 */

int
handleDir(
  char	      * dirName			/* directory name */
){
  char		fileName[PATH_MAX];	/* what to count */
  char	      * subFile = NULL;		/* where to add file name */
  DIR	      * dirP;			/* directory pointer */
  dirent_t    * entryP;			/* directory entry */
  int		count = 0;		/* total count */

  /* open directory stream */
  if ((dirP = opendir(dirName)) == NULL){
    fprintf(stderr, "unable to open directory \"%s\"\n", dirName);
    exit(1);
  }

  /* set up stem of filename */
  strcpy(fileName, dirName);
  subFile = fileName + strlen(fileName);
  *subFile++ = '/';

  /* handle entries */
  while ((entryP = readdir(dirP)) != NULL){
    if ((strcmp(entryP->d_name, ".") != 0) &&
	(strcmp(entryP->d_name, "..") != 0)){
      strcpy(subFile, entryP->d_name);
      count += handleFile(fileName);
    }
  }

  /* tidy up */
  closedir(dirP);
  
  return count;
}

/*
 * @ handleFile : launch counting of single file
 * > none
 * + print count for file, incrementing total
 */

int
handleFile(
  char	      * fileName		/* file name */
){
  struct stat	fileStat;		/* file info buffer */
  int		err;			/* error code */
  int		count = 0;		/* item count */

  /* open file */
  if ((err = lstat(fileName, &fileStat)) != 0){
    fprintf(stderr, "slc: unable to stat \"%s\"\n", fileName);
    exit(1);
  }

  /* count? */
  switch(fileStat.st_mode & S_IFMT){
   case S_IFDIR :
    count = handleDir(fileName);
    break;
   case S_IFLNK :
    break;
   case S_IFREG :
    count = fileStat.st_blocks;
    break;
   default :
    fprintf(stderr, "skipping \"%s\": bad file type\n", fileName);
    break;
  }

  /* report */
  if ((count != 0) && (!PrintOnlyTotal)){
    printf(PRINT_FORMAT, count, fileName);
  }

  return count;
}

/*
 * @ usage : print usage message
 * > none
 * + print proper usage to stderr
 */

void
usage(
  void
){
  fprintf(stderr, "sdu: smart disk usage counter\n");
  fprintf(stderr, "-T\tprint only total usage\n");
  /* return */
}
