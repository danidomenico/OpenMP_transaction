/*==============================================================*/
/* aux/slc.c : smart source line counter			*/
/*==============================================================*/

#include "specific.h"

/*--------------------------------------------------------------*/
/* global variables						*/
/*--------------------------------------------------------------*/

/* controls */

bool		CountSymLinks	= FALSE;
bool		IncludeBlanks	= TRUE;
bool		IncludeComments = TRUE;
bool		IncludeParens	= TRUE;
bool		PrintOnlyTotal	= FALSE;
bool		PrintTotal	= TRUE;
bool		PrintZero	= FALSE;
bool		Recurse		= TRUE;

/* currently in multi-line comment */

bool		InComment	= FALSE;

/* total counter */

int		TotalLines	= 0; 

/*--------------------------------------------------------------*/
/* function prototypes						*/
/*--------------------------------------------------------------*/

void
countDir(
  char	      * dirName			/* directory name */
);
int
countFile(
  char	      * fileName		/* file name */
);
bool
countLine(
  char	      * textBuf			/* text of line */
);
void
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

  /* arguments */
  while ((argd < argc) && (argv[argd][0] == '-')){
    switch(argv[argd][1]){
     case 'L' :
      CountSymLinks = TRUE;
      break;
     case 'T' :
      PrintOnlyTotal = TRUE;
      break;
     case 'Z' :
      PrintZero = TRUE;
      break;
     case 'b' :
      IncludeBlanks = FALSE;
      break;
     case 'c' :
      IncludeComments = FALSE;
      break;
     case 'p' :
      IncludeParens = FALSE;
      break;
     case 'r' :
      Recurse = FALSE;
      break;
     case 't' :
      PrintTotal = FALSE;
      break;
     default :
      fprintf(stderr, "unknown flag \"%s\"\n", argv[argd]);
      usage();
      exit(1);
      break;
    }
    argd++;
  }

  /* sanity check */
  if (PrintOnlyTotal && (!PrintTotal)){
    fprintf(stderr, "error: print-only-total on, but print-total off\n");
    usage();
    exit(1);
  }

  /* no file arguments */
  if (argd == argc){
    usage();
    exit(0);
  }
  /* work */
  else {
    while (argd < argc){
      handleFile(argv[argd++]);
    }
  }

  /* total */
  if (PrintTotal){
    printf(PRINT_FORMAT, TotalLines, "total");
  }

  /* finished */
  return 0;
}

/*--------------------------------------------------------------*/
/* support functions						*/
/*--------------------------------------------------------------*/

/*
 * @ countDir : recurse into sub-directory
 * > none
 * + count lines in sub-directory files
 */

void
countDir(
  char	      * dirName			/* directory name */
){
  char		fileName[PATH_MAX];	/* what to count */
  char	      * subFile = NULL;		/* where to add file name */
  DIR	      * dirP;			/* directory pointer */
  dirent_t    * entryP;			/* directory entry */

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
      handleFile(fileName);
    }
  }

  /* tidy up */
  closedir(dirP);
  
  /* return */
}

/*
 * @ countFile : count single file
 * > number of lines
 */

int
countFile(
  char	      * fileName		/* file name */
){
  FILE	      * fp;			/* file pointer */
  int		lines = 0;		/* number of lines */
  char		textBuf[MAXLEN];	/* line buffer */

  /* open file */
  if ((fp = fopen(fileName, "r")) == NULL){
    fprintf(stderr, "slc: unable to open \"%s\"\n", fileName);
    exit(1);
  }

  /* count */
  while (fgets(textBuf, MAXLEN, fp) != NULL){
    if (countLine(textBuf)) lines++;
  }

  /* tidy up */
  fclose(fp);

  return lines;
}

/*
 * @ countLine : should line be counted?
 * > TRUE or FALSE
 * + mangle contents of buffer
 */

bool
countLine(
  char	      * textBuf			/* text of line */
){
  bool		result = TRUE;		/* count unless disqualified */
  char	      * ptr;			/* pointer into buffer */

  if ((ptr = strtok(textBuf, BLANKS)) == NULL){
    result = IncludeBlanks;
  } else if ((*ptr == '}') && (*(ptr+1) == '\n')){
    result = IncludeParens;
  } else if (InComment){
    result = IncludeComments;
    InComment = strncmp(ptr, "*/", 2) == 0;
  } else if (strncmp(ptr, "/*", 2) == 0){
    result = IncludeComments;
    ptr += strlen(ptr) - 1;
    if (*ptr == '\n') ptr--;
    InComment = strncmp(ptr, "*/", 2) != 0;
  }

  return result;
}

/*
 * @ handleFile : launch counting of single file
 * > none
 * + print count for file, incrementing total
 */

void
handleFile(
  char	      * fileName		/* file name */
){
  struct stat	fileStat;		/* file info buffer */
  int		err;			/* error code */
  int		nl = 0;			/* number of lines */
  bool		directory = FALSE;	/* to suppress printing */

  /* open file */
  if ((err = lstat(fileName, &fileStat)) != 0){
    fprintf(stderr, "slc: unable to stat \"%s\"\n", fileName);
    exit(1);
  }

  /* count? */
  switch(fileStat.st_mode & S_IFMT){
   case S_IFDIR :
    if (Recurse){
     countDir(fileName);
     directory = TRUE;
    } else {
     fprintf(stderr, "skipping \"%s\": directory\n", fileName);
    }
    break;
   case S_IFLNK :
    if (CountSymLinks){
      nl = countFile(fileName);
    }
    break;
   case S_IFREG :
    nl = countFile(fileName);
    break;
   default :
    fprintf(stderr, "skipping \"%s\": bad file type\n", fileName);
    break;
  }
  TotalLines += nl;

  /* report */
  if ((!directory) && (!PrintOnlyTotal)){
    if ((nl > 0) || PrintZero){
      printf(PRINT_FORMAT, nl, fileName);
    }
  }

  /* return */
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
  fprintf(stderr, "slc: smart source line counter\n");
  fprintf(stderr, "-L\tcount lines in symbolically-linked files\n");
  fprintf(stderr, "-T\tprint only total number of lines\n");
  fprintf(stderr, "-b\tdo not count blank lines\n");
  fprintf(stderr, "-c\tdo not count lines containing only comments\n");
  fprintf(stderr, "-p\tdo not count lines containing only closing parentheses\n");
  fprintf(stderr, "-r\tdo not recurse into sub-directories\n");
  fprintf(stderr, "-t\tdo not print total\n");
  /* return */
}
