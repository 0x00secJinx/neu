#!/usr/bin/python
#from NNetwork import NeuralNetwork
from fixednn import NeuralNetwork
import random
import pygame
from time import sleep


black = (0,0,0)
white = (255,255,255)
gray = (150,150,150)
width = 1000
height = 700
sixth = width/6
radius = 25

def getHeightDivisions(nodes):
	return height/(nodes + 1)

def getWeightArray(matrix):
	weights = matrix.getCols()
	new_weights = []
	for i in range(len(weights)):
		new_weights.append(int((weights[i])+10))
	for i in range(len(weights)):
		if (new_weights[i] < 1):
			new_weights[i] = 1
	return new_weights

def main():
	training_data = [
	{
		'inputs': [0,0],
		'output': [0]
	},
	{
		'inputs': [0,1],
		'output': [1]
	},
	{
		'inputs': [1,0],
		'output': [1]
	},
	{
		'inputs': [1,1],
		'output': [0]
	}]

	nn = NeuralNetwork(2,6,1)
	print("Before Training:")
	print("\t[1,1]: Output should be close to 0")
	print("\t\t" + str(nn.predict([1,1])))
	print("\n\t[1,0]: Output should be close to 1")
	print("\t\t"+ str(nn.predict([1,0])))
	print("\n\t[0,1]: Output should be close to 1")
	print("\t\t"+ str(nn.predict([0,1])))
	print("\n\t[0,0]: Output should be close to 0")
	print("\t\t"+ str(nn.predict([0,0])))

	secure_random = random.SystemRandom()
	pygame.init()
	font = pygame.font.SysFont("comicsansms", 30)
	text = font.render("0", True, gray)
	gameDisplay = pygame.display.set_mode((width, height))
	pygame.display.set_caption("Neural Network")
	clock = pygame.time.Clock()
	progExit = False

	# Get the points for the nodes
	input_text = []
	input_text_points = []
	input_points = []
	hidden_points = []
	output_points = []
	output_text_points = []
	output_text = []
	input_division = getHeightDivisions(nn.i_nodes)
	hidden_division = getHeightDivisions(nn.h_nodes)
	output_division = getHeightDivisions(nn.o_nodes)
	for i in range(nn.i_nodes):
		input_text_points.append([(1*sixth)-(text.get_width()/2), ((i+1)*input_division)-(text.get_height()/2)])
	for i in range(nn.i_nodes):
		input_points.append([2*sixth, (i+1)*input_division])
	for i in range(nn.h_nodes):
		hidden_points.append([3*sixth, (i+1)*hidden_division])
	for i in range(nn.o_nodes):
		output_points.append([4*sixth, (i+1)*output_division])
	for i in range(nn.o_nodes):
		output_text_points.append([(5*sixth)-(text.get_width()/2), ((i+1)*output_division)-(text.get_height()/2)])

	# loop
	frames = 0
	while not progExit:
		for event in pygame.event.get():
			if (event.type == pygame.QUIT):
				progExit = True
		gameDisplay.fill(white)

		top = font.render("Train rotations: " + str(frames), True, black)

		for pos in input_points:
			pygame.draw.circle(gameDisplay, black, (pos[0], pos[1]), radius)
		for pos in hidden_points:
			pygame.draw.circle(gameDisplay, black, (pos[0], pos[1]), radius)
		for pos in output_points:
			pygame.draw.circle(gameDisplay, black, (pos[0], pos[1]), radius)


		counter = 0
		weight_array = getWeightArray(nn.ih_weights)
		for j in hidden_points:
			for i in input_points:
				pygame.draw.line(gameDisplay, black, (i[0], i[1]), (j[0], j[1]), weight_array[counter])
				counter += 1

		counter = 0
		weight_array = getWeightArray(nn.ho_weights)
		for i in hidden_points:
			for j in output_points:
				pygame.draw.line(gameDisplay, black, (i[0], i[1]), (j[0], j[1]), weight_array[counter])
				counter += 1

		data = secure_random.choice(training_data)
		for i in data['inputs']:
			input_text.append(i)

		for i in data['output']:
			output_text.append(i)

		for pos in range(len(input_text)):
			input_text[pos] = font.render(str(input_text[pos]), True, black)

		for pos in range(len(output_text)):
			output_text[pos] = font.render(str(output_text[pos]), True, black)

		for i in range(len(input_text_points)):
			gameDisplay.blit(input_text[i],input_text_points[i])

		for i in range(len(output_text_points)):
			gameDisplay.blit(output_text[i],output_text_points[i])

		gameDisplay.blit(top, (0,0))

		nn.train(data['inputs'], data['output'])
		frames += 1


		input_text = []
		output_text = []
		pygame.display.flip()
		#sleep(1)
		#clock.tick(60)

	print("After Training:")
	print("\t[1,1]: Output should be close to 0")
	print("\t\t" + str(nn.predict([1,1])))
	print("\n\t[1,0]: Output should be close to 1")
	print("\t\t"+ str(nn.predict([1,0])))
	print("\n\t[0,1]: Output should be close to 1")
	print("\t\t"+ str(nn.predict([0,1])))
	print("\n\t[0,0]: Output should be close to 0")
	print("\t\t"+ str(nn.predict([0,0])))

if __name__ == "__main__":
	main()
