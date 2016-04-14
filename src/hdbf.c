#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hdbf.h"
#include "binarytree.h"

void run(const char* code){
	/* Set up interpretation variables */
	unsigned int dpointer=0;/* dimensional pointer */
	unsigned int dim=1; /* Number of dimensions working with*/
	int* coord=malloc(sizeof(int)*dim); /* current coordinates*/
	int* loopqueue=malloc(sizeof(int));/* loop list*/
	char* buffer=malloc(sizeof(char));/* alloc now so it doesn't fail on `free`*/
	int loops=0; /* Number of loops*/
	int offness=-1;/*Indicates offness (value is -1 or location of loop)*/
	
	/* Binary tree variables*/
	struct Leaf* root=malloc(sizeof(struct Leaf));
	struct Leaf* cell=root;
	
	/* Loop and temporary variables*/
	unsigned int i;int v;
	
	/* Set initial coordinates*/
	coord[0]=0;
	
	/* Set up binary tree */
	root->right=NULL;
	root->left=NULL;
	root->value=0;
	root->coord=malloc(sizeof(int)*dim);root->coord[0]=0;
	root->dim=dim;
	
	/* Start interpretation */
	for (i=0;i<strlen(code);i++){
		if(offness==-1 || code[i]==']' || code[i]=='['){
			switch(code[i]){
				case '[':
					/* Increment size of loop queue*/
					loops++;
					
					/* Safely reallocate memory*/
					/*printf("About to reallocate in '['\n");*/
					buffer=malloc(loops*sizeof(int));
					if(buffer==NULL){
							fprintf(stderr,"Memory allocation error\n");
							exit(1);
					}
					memcpy(buffer,loopqueue,(loops-1)*sizeof(int));
					loopqueue=malloc(loops*sizeof(int));
					memcpy(loopqueue,buffer,loops*sizeof(int));
					if(loops<=0){
						fprintf(stderr,"loops below bounds\n");
						exit(1);
					}
					
					loopqueue[loops-1]=i;

					/* Check if need to turn off*/
					if(offness==-1 && cell->value==0){
						offness=i;
					}
					break;
				case ']':
					/* Store last loopqueue value before deleting it*/
					v = loopqueue[loops-1];
					
					/* Delete last loopqueue value*/
					loops--;
					buffer=malloc(sizeof(int)*loops);
					memcpy(buffer,loopqueue,sizeof(int)*loops);
					loopqueue=malloc(sizeof(int)*loops);
					memcpy(loopqueue,buffer,sizeof(int)*loops);
					
					/* Turn on if necessary*/
					if(offness!=-1){
						if(offness==v)offness=-1;
					}else{
						/* Go back to matching `[`*/
						i=v-1;
					}
					break;
				case '+':
					/* Increase value of cell*/
					cell->value++;
					break;
				case '-':
					/* Decrease value of cell*/
					cell->value--;
					break;
				case '^':
					/* Increase dimensional pointer*/
					if((++dpointer)>=dim){
						dim++;
						buffer=malloc(sizeof(int)*dim);
						memcpy(buffer,coord,dim-1);
						coord=malloc(sizeof(int)*dim);
						memcpy(coord,buffer,dim);
						coord[dim-1]=0;
					}
					break;
				case 'V':
					/* Decrease dimentional pointer*/
					--dpointer;
					break;
				case '>':
					/* Go in positive direction of current vector*/
					coord[dpointer]+=1;
					cell=traverse(root,coord,dim);
					break;
				case '<':
					/* Go in negative direction of current vector*/
					coord[dpointer]-=1;
					cell=traverse(root,coord,dim);
					break;
				case '.':
					printf("%c",cell->value);
					break;
			}
		}
	}
	
	/* Print out final coordinates*/
	printf("\nCoordinates{");
	for(i=0;i<dim;i++){printf("%s%d",(i==0?"":","),coord[i]);}
	printf("}\n");
	
	/* Free stuff*/
	deleteTree(root);
	free(root->coord);
	free(root);
	free(coord);
	free(loopqueue);
	free(buffer);
	
}
