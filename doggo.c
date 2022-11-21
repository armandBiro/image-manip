#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	int w;
	int h;
	int max;
	int** data;
} pgm;

pgm load_image(char* input){
	pgm res;
	FILE* in = fopen(input, "r");
	if(in == NULL){
		fprintf(stderr, "Epic fail.\n");
		return res; 
	}

	fscanf(in, "P2\n");

	if(fscanf(in, "%d %d\n %d", &res.w, &res.h, &res.max) != 3){
		fprintf(stderr, "Epic fail: w & h\n");
		return res;
	} 

	//printf("w=%d h=%d max=%d\n", res.w, res.h, res.max);

	res.data = malloc(res.h * sizeof(int*));

	for(int i=0; i<res.h; i++){
		res.data[i] = malloc(res.w * sizeof(int*));
	}

	for(int i=0; i<res.h; i++){
		for(int j=0; j<res.w; j++){
			if(fscanf(in, "%d", &res.data[i][j]) != 1) fprintf(stderr, "Epic fail: %d x %d pixel\n", i, j);
		}
	}

	fclose(in);

	//printf("\nwin\n");

	return res;
}

void free_image(pgm img){
	for(int i=0; i<img.h; i++){
		free(img.data[i]);
	}
	free(img.data);
}

void save_image(pgm img, char* output){
	FILE* out = fopen(output, "w");
	if(out == NULL) { fprintf(stderr, "Epic fail: %s", output); return; }
	fprintf(out, "P2\n");
	fprintf(out, "%d %d\n %d\n", img.w, img.h, img.max);

	for(int i=0; i<img.h; i++){
		for(int j=0; j<img.w; j++){
			fprintf(out, "%d\n", img.data[i][j]);
		}
	}

	fclose(out);
}

void brightness(pgm img, float factor){
	for(int i=0; i<img.h; i++){
		for(int j=0; j<img.w; j++){
			img.data[i][j] *= factor;
		}
	}
}

void threshold(pgm img, int threshold){
	for(int i=0; i<img.h; i++){
		for(int j=0; j<img.w; i++){
			img.data[i][j] = img.data[i][j] > threshold ? img.max : 0;
		}
	}
}

int contrast_transform(int value, int ratio){
	float contrast = pow((100.0 + ratio) / 100.0, 2);
	value = round(((((value / 255.0) - 0.5) * contrast) + 0.5) * 255.0);
	if(value < 0) return 0;
	else if(value > 255) return 255;
	else return value;
}

void contrast(pgm img, int ratio){
	for(int i=0; i<img.h; i++){
		for(int j=0; j<img.w; j++){
			img.data[i][j] = contrast_transform(img.data[i][j], ratio);
		}
	}
}

int main(){
	char* in = (char*)malloc(101*sizeof(char));
	printf("Mi a kezelendo fajl neve? (.pgm) \n");
	scanf("%s", in);
	pgm img = load_image(in);
	free(in);
	
	char choice = 'h';
	printf("Valassz modositast!\n - (b) fenyero modositasa\n - (c) kontraszt modositasa\n - (t) hatarertek filter\n");

	do {
		
		scanf(" %c", &choice);

		int factor, ratio, threshold_ratio;
		switch(choice){
			case 'b':
				printf("A modositas merteke (0.0 - 1.0): \n");
				scanf("%d", &factor);
				brightness(img, factor);
				break;
			case 'c':
				printf("A modositas merteke (-50 - 50): \n");
				scanf("%d", &ratio);
				contrast(img, ratio);
				break;
			case 't':
				printf("A modositas merteke (0 - 255): \n");
				scanf("%d", &threshold_ratio);
				threshold(img, threshold_ratio);
				break;
			default:
				printf("Legyszi letezo modositast valassz!\n");
				break;
		}
	} while(choice != 'b' && choice != 'c' && choice != 't');

	char* out = (char*)malloc(101*sizeof(char));
	printf("Mi legyen a kimeno fajl neve? (.pgm) \n");
	scanf(" %s", out);
	save_image(img, out);
	free(out);

	free_image(img);

	return 0;
}