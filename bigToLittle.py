#!/usr/bin/python
import struct

def main():

	new_image = "train-images-idx3-ubyte-new"
	new_label = "train-labels-idx1-ubyte-new"
	training_image_file = "train-images-idx3-ubyte"
	training_label_file = "train-labels-idx1-ubyte"
	new_buff = ''

	with open(new_image, "wb") as i:
		pass

	with open(training_imgage_file, "rb") as imgfile:
		digit = imgfile.read(8)
		new_buff = ""
		while True:
			digit = imgfile.read(1)
			if digit == "":
				with open(new_image, "ab") as newimgfile:
					newimgfile.write(new_buff)
				return 0
					#print(digit)
			new_buff += str(struct.unpack(">B",digit)[0])


	"""
	with open(new_image, "rb") as imgfile:
		while True:
			digit = imgfile.read(4)
			if digit == "":
				break
			#print(digit)
			new_buff += str(struct.pack(">I",struct.unpack("<I",digit)[0]))

	with open(new_label, "wb") as imgfile:
		imgfile.write(new_buff)
	"""

main()