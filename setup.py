from distutils.core import setup, Extension

extra_comp_args = ["-Ofast"]

module1 = Extension("Matrix",
				   	sources = ['matrix.c'],
				   	extra_compile_args=extra_comp_args)

module2 = Extension("activation",
					sources=['activation.c'],
					extra_compile_args=extra_comp_args)

setup(name = "Matrix",
	version = '1.0',
	description = "This is a module to perform matrix operations",
	ext_modules = [module1])

setup(name = "activation",
	version='1.0',
	description='This is a module to have activation functions for a neural network',
	ext_modules = [module2])