/*
Compile with:
gcc hello.c -o hello -O2 -O3 -Ofast -fstack-protector-strong -Wall -lm -faggressive-loop-optimizations -ffast-math
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NNetwork.h"

#define IMG_FILE "train-images-idx3-ubyte-new"
#define LBL_FILE "train-labels-idx1-ubyte-new"
#define BUFF_SIZE 4096
#define TRAINING 20000
#define TESTING 10000

int createExpectedOutput(int n, float arr[], int arr_len){
	for(int i = 0; i < arr_len; i++){
		if(n == i){
			arr[i] = 1;
		} else {
			arr[i] = 0;
		}
	}
	return 0;
}

int getExpectedOutput(float arr[], int arr_len){
	int highest = 0;
	for(int i = 0; i < arr_len; i++){
		if(arr[i] > arr[highest]){
			highest = i;
		}
	}
	return highest;
}

int parseInputs(char inputs[], float parsedInputs[]){
	//char buff[BUFF_SIZE];
	//printf("%s\n", inputs);
	char tmp[4];
	char character;
	int counter = 0;
	int parsed_counter = 0;
	int a;
	float b;
	for(int i = 0; i < BUFF_SIZE; i++){
		if(inputs[i] == '\n'){
			//printf("returning\n");
			return 0;
		}
		character = inputs[i];
		//printf("%c\n", character);
		if(character != ' '){
			tmp[counter] = character;
			counter++;
		} else {
			a = atoi(tmp);
			b = a/256.0;
			parsedInputs[parsed_counter] = b;
			parsed_counter++;
			counter = 0;
			memset(tmp, '\0', 4);
		}
	}

	return -1;
}

int main(int argc, char *argv[]){

	struct NeuralNetwork nn;
	initNeuralNetwork(&nn, 784, 7840, 10, 0.1);
	int counter, z, expected, i, correct;
	int labels[50000];
	char tmp[2];
	char inputs_buffer[BUFF_SIZE];
	float inputs[nn.input];
	float result[nn.output];
	FILE *img, *label;

	//Open the image file
	if((img = fopen(IMG_FILE, "r")) == NULL) {
		printf("Could not find the file : %s\n", IMG_FILE);
		return -1;
	}

	//Open the label file
	if((label = fopen(LBL_FILE, "r")) == NULL) {
		printf("Could not find the file : %s\n", LBL_FILE);
		return -1;
	}

	counter = 0;
	while(fgets(tmp, 2, (FILE*)label) != NULL){
		if(counter < 50000){
			labels[counter] = atoi(tmp);
			counter++;
		}
	}

	i = 0;
	correct = 0;
	while(fgets(inputs_buffer, BUFF_SIZE, (FILE*)img) != NULL){
		printf("%d\r", i);
		if(i < (TRAINING + TESTING)){
			expected = labels[i];
			createExpectedOutput(expected, result, nn.output);
			parseInputs(inputs_buffer, inputs);

			if(i < TRAINING){
				train(&nn, inputs, result);
			}

			if(i > TRAINING) {
				feedforward(&nn, inputs, result);
				z = getExpectedOutput(result, nn.output);
				if(expected == z){
					correct++;
				}
				printf("Expected: %d\n\tActual prediction: %d\n", expected, z);
			}

			i++;
		}
	}
	printf("Percent correct: %.4f\n", ((float)correct/(float)TESTING));

	fclose(img);
	fclose(label);
	deallocNNMatricies(&nn);

	return 0;
}