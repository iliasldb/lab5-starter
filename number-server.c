#include "http-server.h"
#include <string.h>

int num = 0;

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_200_OK[] = "HTTP/1.1 200 OK\r\n\r\n";

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator
    send(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND),0);
    // TODO: send response back to client?
}

void handle_shownum(int client_sock)
{
	char response[BUFFER_SIZE];
	strncpy(response,HTTP_200_OK, sizeof(response)-1);
	char line[BUFFER_SIZE];
	snprintf(line, sizeof(line), "Number: %d\n", num);
	strcat(response, line);
	printf("%s", response);
	write(client_sock, response, strlen(response));
}

void handle_increment(int client_sock)
{
	char response[BUFFER_SIZE];
	strncpy(response, HTTP_200_OK, sizeof(response)-1);
	char line[BUFFER_SIZE];
	num+=1;
	snprintf(line, sizeof(line), "Number Incremented\n");
	strcat(response, line);
	printf("%s", response);
	write(client_sock, response, strlen(response));
}

void handle_add(int client_sock, char* query)
{
	char response[BUFFER_SIZE];
	if(atoi(query))
	{
		strncpy(response, HTTP_200_OK, sizeof(HTTP_200_OK));
		num += atoi(query);
		char line[BUFFER_SIZE];
		snprintf(line, sizeof(line), "Number Incremented by %d\n", atoi(query));
		strcat(response, line);
	}
	else
	{
		strncpy(response, HTTP_404_NOT_FOUND, sizeof(response)-1);
	}
	printf("%s", response);
	write(client_sock, response, strlen(response));
}

void handle_response(char *request, int client_sock) {
    char url[256], query[256]; 

    if(strlen(request)>256)
    {
	    printf("Invalid request length\n");
	    return;
    }

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", url) != 1) {
        printf("Invalid request line\n");
        return;
    }
    else if(strncmp(url, "/shownum", 8) ==0)
    {
	    handle_shownum(client_sock);
    }
    else if(strncmp(url, "/increment", 10) == 0)
    {
	    handle_increment(client_sock);
    }
    else if(strncmp(url, "/add", 4)==0)
    {
	    strcpy(query, strchr(url, '?')+1);
	    handle_add(client_sock, query);
    }
    else
    {
	    handle_404(client_sock, url);
    }
    
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
