#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
int main(int argc, char **argv){
	int opt;
	opterr = 0;
	char *inputFile=NULL,*outputFile=NULL;
	if(argc != 5){
    	fprintf(stderr, "Usage: shuffle -i inputfile -o outputfile\n");
    	exit(EXIT_FAILURE);
    }
	while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
        	case 'i':
            	inputFile = optarg;
            	break;
        	case 'o':
            	outputFile = optarg;
            	break;
        	default: 
            	fprintf(stderr, "Usage: shuffle -i inputfile -o outputfile\n");
            	exit(EXIT_FAILURE);
        }
    }
	struct stat fileStat;
	char *fileText;
	char **ptr;
	FILE *inputfp,*outputfp;
	long numLines=0,k=0,i=0,j=0;
	//Getting the size of the input file.
	if(stat(inputFile,&fileStat) != 0){
		fprintf(stderr, "Error: Cannot open file %s\n",inputFile);
		exit(EXIT_FAILURE);
	}
	fileText = (char *)malloc(fileStat.st_size*sizeof(char)+1);
	if(fileText==NULL){
		fprintf(stderr, "Error: Malloc error \n");
		exit(0);
	}
	inputfp = fopen(inputFile,"r");
	if(inputfp==NULL){
		fprintf(stderr, "Error: Cannot open file %s\n",inputFile);
		exit(EXIT_FAILURE);
	}
	//Reading through the file in fileText.
	if(fread(fileText,1,fileStat.st_size,inputfp)!= fileStat.st_size){
		fprintf(stderr, "File Read was unsucessful\n");
		exit(EXIT_FAILURE);
	}
	fileText[fileStat.st_size] = '\0';
	fclose(inputfp);
	//Counting the number of lines.
	for(k=0;k<fileStat.st_size;k++){
		if(fileText[k] == '\n'){
			numLines++;
		}
	}
	ptr = (char **) malloc(sizeof(char *) * numLines);
	if(ptr==NULL){
		fprintf(stderr, "Error: Malloc error \n");
		exit(0);
	}
	//Getting pointers to every new line in ptr.
	*ptr = (char *) fileText;
	for(i = 1;i<numLines;i++){
		ptr[i]=strchr(ptr[i-1],'\n')+1;
	}
	outputfp = fopen(outputFile,"w");
	if(outputfp==NULL){
		fprintf(stderr, "Error: Cannot open file %s\n",outputFile);
		exit(EXIT_FAILURE);
	}
	i=0;
	j=numLines-1;
	//Writing into the output file using fwrite.
	while(i<=j){
		if(i!=numLines-1){
			if(fwrite(ptr[i],1,ptr[i+1]-ptr[i],outputfp)!= ptr[i+1]-ptr[i]){
				fprintf(stderr, "File write was unsucessful\n");
				exit(EXIT_FAILURE);
			}
		}
		if(j==numLines-1){
			if(fwrite(ptr[j],1,fileStat.st_size-(size_t)(ptr[j]-ptr[0]),outputfp)!= fileStat.st_size-(size_t)(ptr[j]-ptr[0])){
				fprintf(stderr, "File write was unsucessful\n");
				exit(EXIT_FAILURE);
			}	
		}
		else if(i<j){
			if(fwrite(ptr[j],1,ptr[j+1]-ptr[j],outputfp)!= ptr[j+1]-ptr[j]){
				fprintf(stderr, "File write was unsucessful\n");
				exit(EXIT_FAILURE);
			}
		}
		i++;
		j--;	
	}
	fclose(outputfp);
	return 0;
}