#include "Http/Server/Http.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

Http::Http(int clientSocket):
ServerClient(clientSocket)
{
}

Http::~Http() {
}

void Http::thread()
{
    char buffer[1024];
    bzero(buffer,sizeof(buffer));
    
    close(_clientSocket);
}