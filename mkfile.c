#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void
usage(char *argv0,char *error){
	if(error)
		fprintf(stderr,"ERROR: %s\n",error);
	fprintf(stderr,"Usage: %s [options] size file\n"
		"Create a sparse file of specific size.\n"
		"If unit is not specified, size is in bytes.\n",argv0);
	fprintf(stderr,"Options:\n"
		"  -v : verbose\n"
		"  -h : this help\n"
		"Size factors (specify only one of -k,-M,-G,-T):\n"
		"  -k : size is in kibibyte (2^10=1024 byte)\n"
		"  -M : size is in Mebibyte (2^20=1048576 byte)\n"
		"  -G : size is in Gibibyte (2^30=1073741824 byte)\n"
		"  -T : size is in Tebibyte (2^40=1099511627776 byte)\n");
	exit(1);
}

off_t
get_size_factor(char c){
	switch(c){
	case 'k':
		return 1ULL<<10; /* kibibyte */
	case 'M':
		return 1ULL<<20; /* Mebibyte */
	case 'G':
		return 1ULL<<30; /* Gibibyte */
	case 'T':
		return 1ULL<<40; /* Tebibyte */
	default:
		return -1;
	}
}

int main(int argc,char **argv){
	int fd,i;
	int verbose=0;
	char *fname,*p;
	off_t fsize,factor=1;

	while(-1 != (i=getopt(argc,argv,"vhbkMGT"))){
		switch(i){
		case 'v':
			verbose++;
			break;
		case 'h':
			usage(argv[0],NULL);
			break;
		case 'k':
		case 'M':
		case 'G':
		case 'T':
			if(factor != 1)
				usage(argv[0],"Specify only one size factor!");
			factor = get_size_factor(i);
			break;
		default:
			exit(1); /* invalid option specified */
		}
	}

	if(argc - optind != 2)
		usage(argv[0],"Please specify file and size.");

	fname = argv[optind+1];
	fsize = strtoull(argv[optind],&p,0);

	if(*p)
		usage(argv[0],"Cannot parse size.");

	fsize *= factor;

	if(-1 == (fd=open(fname,O_RDWR|O_EXCL|O_CREAT,0666))){
		perror(fname);
		exit(1);
	}

	if(verbose)
		fprintf(stderr,"File %s opened, setting length to %lld byte.\n",
			fname,fsize);

	if(-1 == ftruncate(fd,fsize)){
		perror(fname);
		exit(1);
	}
	
	if(-1 == close(fd)){
		perror(fname);
		exit(1);
	}
	
	exit(0);
}
