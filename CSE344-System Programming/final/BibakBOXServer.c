#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libgen.h>


typedef struct{

	char filename [512];
    mode_t mode;
    char content [4096];
    int bytesRead;
    int flag;
    time_t lastModification;
	
} FileEntry;


typedef struct {
	char filename[512];
} ModifiedFile;


void delay(double milliseconds);
void getCurrentTime(char *buffer);
void writeLog(char *logFilePath, char *entryType, char *status, char *name);
int isFileInList(char *filename, ModifiedFile *modifiedFilesList, int count);
void * handleClient(void *threadArg);
void threadErr(int errorCode, char *errorMessage);
void signalHandler(int signal);
int isClientConnected(char * clientName);
void removeClientFromOnlineList(char * clientName);
void sendServerToClient(int fd, char *name);
void sendServerFiles(int fd, char *param);
void removeRest(ModifiedFile *list,int count, char* path , char* logFile);


int *clientFD;
int serverFD;
pthread_t *serverThreads;
int threadPoolSize;
char *serverPath;
ModifiedFile * connectedCLients;
int onlineCount=0;		
pthread_t mainThread;	
int baseIndex = 0;
pthread_mutex_t onlineMutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char **argv){


	if(argc != 4){
		printf("Usage: %s [serverDirPath] [threadPoolSize] [portnumber]\n",argv[0] );
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in serverAddress;
	socklen_t serverLen;

	threadPoolSize = atoi(argv[2]);
	int portnumber = atoi(argv[3]);
	DIR *pDir = opendir (argv[1]);

	 baseIndex = strlen(argv[1]);
    ++baseIndex;

	if(threadPoolSize>0 && portnumber>2000 && pDir != NULL){
		closedir(pDir);
	}
	else{
		printf("Usage: %s [serverDirPath] [threadPoolSize] [portnumber]\n",argv[0] );
		exit(EXIT_FAILURE);
	}

	mainThread = pthread_self();

	signal(SIGINT,signalHandler);
	signal(SIGTERM,signalHandler);

	// SERVER PATH ARRANGEMENT
	serverPath = (char*)malloc(512*sizeof(char)); 
	char * serverBase = (char*)malloc(512*sizeof(char));

	strcpy(serverBase,argv[1]); // /home/bulut/Documents/final

	strcpy(serverPath,dirname(argv[1])); 


	if(strcmp(serverPath,".") == 0)
		strcpy(serverPath,"");
	else
		strcat(serverPath,"/");
	

	strcat(serverPath, basename(serverBase));

	printf("serverPath: %s\n", serverPath); // /home/bulut/Documents/final/Server
	free(serverBase);

	// SOCKET - PORT ETC
	memset(&serverAddress,0,sizeof(struct sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portnumber);

	serverLen = sizeof(serverAddress);

	if((serverFD = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("socket : ");
		exit(EXIT_FAILURE);
	}

	if(bind(serverFD,(struct sockaddr*)&serverAddress,serverLen) == -1){
		perror("bind : ");
		exit(EXIT_FAILURE);
	}

	if(listen(serverFD,0) == -1){
		perror("listen : ");
		exit(EXIT_FAILURE);
	}

	// BAĞLANAN CLİENT LİSTESİ
	connectedCLients = (ModifiedFile*)malloc(4096*sizeof(ModifiedFile));


	int *numbers = (int*)malloc(threadPoolSize*sizeof(int));
	for (int i = 0; i < threadPoolSize; ++i){
		numbers[i]=i; 
	}

	clientFD = (int*)malloc(threadPoolSize*sizeof(int));
	serverThreads = (pthread_t *)malloc(threadPoolSize * sizeof(pthread_t));


	for (int i = 0; i < threadPoolSize; ++i){
		clientFD[i] = -1;

		// İŞLEM BURDA YAPILIYOR
		int err = pthread_create(&serverThreads[i], NULL, handleClient, (void*)&numbers[i]);
		if(err) threadErr(err,"thread creation");
	}

	for (int i = 0; i < threadPoolSize; ++i){
    	int err = pthread_join (serverThreads[i], NULL);	
        if(err) threadErr(err,"thread join");
    }

    free(numbers);
    free(clientFD);
    free(serverThreads);
    free(serverPath);
    free(connectedCLients);
    close(serverFD);	
	return 0;
}

void * handleClient(void* p){

	struct sockaddr_in clientAddress;
	struct stat fileStat;
	socklen_t clientLen;
	int clientFdIndex = *(int*) p;
	int wfd = -1 , log = -1;
	int touchedCount=0;
	char logFileName[512],clientDirName[512],buf[50];
	char * logInfo;
	
	FileEntry each;
	FileEntry resp;
    memset(&resp, 0, sizeof resp);
    memset(&each, 0, sizeof each);


	memset(&clientAddress,0,sizeof(struct sockaddr_in));
	clientLen = sizeof(clientAddress);

	for(;;){
		
		// Connection kontrolü
		clientFD[clientFdIndex] = accept(serverFD, (struct sockaddr*)&clientAddress, &clientLen);
		if (clientFD[clientFdIndex] == -1) { 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		}


		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

	    read(clientFD[clientFdIndex], &each, sizeof(FileEntry));		//read client dir name : client 122 101r

		strcpy(clientDirName, each.filename);
	

		// ONLİNE İSE HATA VERDİR.
    	if(isClientConnected(each.filename)){

			strcpy(each.content, "error");
			write(clientFD[clientFdIndex], &each, sizeof(FileEntry)); //response // 101w
    		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    		continue;
		}


		// İSMİ ONLİNE LİSTE EKLE
    	strcpy(connectedCLients[onlineCount++].filename, each.filename);

		// IP ADRESS BASTIR
    	char str[clientLen];
		inet_ntop( AF_INET, &clientAddress, str, clientLen);
		printf("Client connected with ip: %s\n",str );

		// CLİENT A DEVAM ET MESAJI GÖNDER
		strcpy(resp.content, "Ok");
		write(clientFD[clientFdIndex], &resp, sizeof(FileEntry));
		memset(&resp, 0, sizeof resp);

    	strcpy(logFileName,serverPath);
    	strcat(logFileName,"/");		
    	strcat(logFileName,each.filename);
    	strcat(logFileName,".log");
    
    	//LOG A YAZ
    	log = open(logFileName, O_WRONLY | O_CREAT | O_APPEND, 0666);
    	getCurrentTime(buf);		//time
  		logInfo = (char*)malloc(512*sizeof(char));
    	sprintf(logInfo, "Client %s connected. \t %s\n",each.filename, buf);	
    	write(log, logInfo, strlen(logInfo));
    	close(log);
    	free(logInfo); 

    	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	

		ModifiedFile * touchedList = (ModifiedFile*)malloc(1024*sizeof(ModifiedFile)); 	
	    

		while(read(clientFD[clientFdIndex], &each, sizeof(FileEntry))>0){


			if (each.flag == 2)
			{
				strcpy(resp.content, "Serve sync Starting..");
				write(clientFD[clientFdIndex], &resp, sizeof(FileEntry)); 
			    memset(&resp, 0, sizeof resp);
				for (int i = 0; i < onlineCount; i++)
				{
					sendServerToClient(clientFD[i], serverPath);
				}
				
				continue;
			}

			if (each.flag == 5)
			{
				removeRest(touchedList,touchedCount, serverPath, logFileName);

				strcpy(resp.content, "Rest files handled..");
				write(clientFD[clientFdIndex], &resp, sizeof(FileEntry)); 
			    memset(&resp, 0, sizeof resp); 
				continue;
			}
			
			
	    	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); 

	    	char *name = (char *) malloc(512*sizeof(char));
	    	
	    	strcpy(name,serverPath);
	    	strcat(name,"/");
	    	strcat(name,each.filename);
			
			// name: /home/bulut/Documents/Client/deneme2
			// mode: 16893
			// flag: 0

			
	    	if(S_ISDIR(each.mode)){
	    		if(!isFileInList(name,touchedList,touchedCount))
	    			strcpy(touchedList[touchedCount++].filename,name);
	    		
	    		if(mkdir(name,0700) != -1){
	    			writeLog(logFileName,"Directory","ADDED  ",name);
	    		}
	    	}
	    	else{
				
				if (!isFileInList(name, touchedList, touchedCount))
                strcpy(touchedList[touchedCount++].filename, name);

	    		if(lstat(name,&fileStat) != -1){		//file exist
					if(wfd == -1){		//open only once
						if(!isFileInList(name,touchedList,touchedCount))
							strcpy(touchedList[touchedCount++].filename,name);

						if(each.lastModification > fileStat.st_mtime){	//file modified
							wfd = open(name, O_WRONLY | O_TRUNC, each.mode);
							
		    				writeLog(logFileName,"File    ","UPDATED",name);
						}
						else {
							free(name);
						
							strcpy(resp.content, "continue");
							write(clientFD[clientFdIndex], &resp, sizeof(FileEntry)); //response // 101w
							memset(&resp, 0, sizeof resp); //respo

							pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
							continue;
						}
					}
					write(wfd,each.content,each.bytesRead);
    				
	    		}
	    		else{	//file not exist
	    			if(wfd == -1){		//open only once
	    				if(!isFileInList(name,touchedList,touchedCount)){
	    					strcpy(touchedList[touchedCount++].filename,name);
						}

	    				wfd = open(name, O_WRONLY | O_CREAT | O_TRUNC, each.mode);
	    				
	    				writeLog(logFileName,"File    ","ADDED  ",name);

	    			}
	    			write(wfd,each.content,each.bytesRead);
	    		}
	    	}

	    	if(each.flag == 1){	//file done
	    		if(!S_ISDIR(each.mode)){
		    		close(wfd);		//clise file
		    		wfd = -1;		//reset wfd
		    	}
	    	}
	    	free(name);

			strcpy(resp.content, "continue");
			write(clientFD[clientFdIndex], &resp, sizeof(FileEntry)); //response // 101w
			memset(&resp, 0, sizeof resp); //respo

	    	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);	//unlock
	    }

	    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);	//lock


	    //disconnect log
	    log = open(logFileName, O_WRONLY | O_APPEND, 0666);	
	    getCurrentTime(buf);
	    logInfo = (char*)malloc(512*sizeof(char));
	    sprintf(logInfo, "Client %s disconnected. \t %s\n", basename(dirname(logFileName)),buf );
	    write(log, logInfo, strlen(logInfo));
	    close(log);
	    free(logInfo);
	    free(touchedList);


	    removeClientFromOnlineList(basename(clientDirName));

	    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);	//unlock

	}

	return NULL;
}
void removeRest(ModifiedFile *list,int count, char* path , char* logFile){
	struct dirent *pDirent;
    DIR *pDir;
    struct stat fileStat;
    char name [512];        

    pDir = opendir (path);
    if (pDir != NULL){ 
        while ((pDirent = readdir(pDir)) != NULL){
            if(0!=strcmp(pDirent->d_name,".") && 0!=strcmp(pDirent->d_name,"..")){
                strcpy(name,path);
                strcat(name,"/");
                strcat(name,pDirent->d_name);

                if(strcmp(name,logFile)==0)
                	continue;
                
                lstat(name,&fileStat);
	            if(S_ISDIR(fileStat.st_mode)){
	            	removeRest(list,count,name,logFile);
	            	
	            	if(!isFileInList(name,list,count)){	//remove dir
	            		if(remove(name) != -1){
		    				writeLog(logFile,"Directory","REMOVED",name);	
	            		}
	            	}
	            }
	            else if(!isFileInList(name,list,count)){
                	remove(name);	//remove file
                	writeLog(logFile,"File    ","REMOVED",name);
	            }
            }
        }
    }

    closedir(pDir);
}
void writeLog(char * logFile, char *type, char * status,char * name){
	int log;
	char buf[50];			
	getCurrentTime(buf);

	char * logInfo = (char*)malloc(512*sizeof(char));
	sprintf(logInfo, "%s\t %s\t %s \t\t %s\n", type, status ,buf ,name );	
	log = open(logFile, O_WRONLY | O_APPEND, 0666);

	write(log, logInfo, strlen(logInfo));
	close(log);
	free(logInfo);
}

int isFileInList(char *name, ModifiedFile *list, int count){
	for (int i = 0; i < count; ++i)
		if(strcmp(list[i].filename,name)==0)
			return 1;
	
	return 0;
}

int isClientConnected(char * name){

	for (int i = 0; i < onlineCount; ++i){
		pthread_mutex_lock(&onlineMutex);
		if(strcmp(connectedCLients[i].filename,name) == 0){
			pthread_mutex_unlock(&onlineMutex);
			return 1;
		}
		pthread_mutex_unlock(&onlineMutex);
	}
	return 0;
}

void removeClientFromOnlineList(char * name){
	for (int i = 0; i < onlineCount; ++i){
		pthread_mutex_lock(&onlineMutex);
		if(strcmp(connectedCLients[i].filename,name) == 0)
			strcpy(connectedCLients[i].filename,"");
		pthread_mutex_unlock(&onlineMutex);
	}
}

void delay(double msec){
    double dif = 0.0;
    struct timeval start, end;

    gettimeofday(&start, NULL);
    while(dif <= msec){
        gettimeofday(&end, NULL);
        dif = (double) (end.tv_usec - start.tv_usec) / 1000 + (double) (end.tv_sec - start.tv_sec)*1000;
    } 
}

void getCurrentTime(char *buf){
	strcpy(buf,"");
	time_t now = time(0);
	struct tm now_t = *localtime(&now);
	strftime (buf, 50, "%x - %I:%M:%S %p", &now_t);
}

void threadErr(int err,char* msg){
    errno = err;
    perror(msg);
    exit(EXIT_FAILURE);
}

void signalHandler(int signo){
	FileEntry resp;
	memset(&resp, 0, sizeof resp);

	if(pthread_self() == mainThread){

		if(signo == SIGINT)
			printf("SIGINT handled.\n");
		else if(signo == SIGTERM)
			printf("SIGTERM handled.\n");

		for (int i = 0; i < threadPoolSize; ++i){
			if(clientFD[i]!=-1){
				strcpy(resp.content, "shutdown");
				write(clientFD[i], &resp, sizeof(FileEntry));
				memset(&resp, 0, sizeof resp);
			}
		}

		delay(1000);

		for (int i = 0; i < threadPoolSize; ++i){
			pthread_cancel(serverThreads[i]);	
		}
	}

}

void sendServerToClient(int fd, char *name)
{
    char message[50];
    sendServerFiles(fd, name);
    FileEntry each, resp;
    memset(&each, 0, sizeof each);
    memset(&resp, 0, sizeof resp);

    each.flag = 3;
    write(fd, &each, sizeof(FileEntry));
	strcpy(resp.content, "kvp");
    read(fd, &resp, sizeof(FileEntry)); // response message from server
}

void sendServerFiles(int fd, char *param)
{
    struct dirent *pDirent;
    DIR *pDir;
    struct stat fileStat;
	FileEntry resp;

    char name[512], message[10];
    int size, rSize;

    pDir = opendir(param);
    if (pDir != NULL)
    {

        while ((pDirent = readdir(pDir)) != NULL)
        {

            if (0 != strcmp(pDirent->d_name, ".") && 0 != strcmp(pDirent->d_name, ".."))
            {

                strcpy(name, param);
                strcat(name, "/");
                strcat(name, pDirent->d_name);

                lstat(name, &fileStat);

                if ((S_ISDIR(fileStat.st_mode)))
                {

                    FileEntry each;
                    memset(&each, 0, sizeof each);
                    memset(each.filename, '\0', sizeof(each.filename));
                    memcpy(each.filename, &name[baseIndex], strlen(name) - baseIndex);

					each.flag = 0;
                    each.mode = fileStat.st_mode;
					strcpy(resp.content, "Directory sent.");
                    write(fd, &each, sizeof(FileEntry)); //103w
                    read(fd, &resp, sizeof(FileEntry)); // 103r ... response message from server

					memset(&resp, 0, sizeof resp); //respo

                    strcpy(message, "");
                    sendServerFiles(fd, name);
                }

                else
                {
                    int rfd;
                    if (S_ISREG(fileStat.st_mode))
                    {
                        rfd = open(name, O_RDONLY);
                    }
                    else if (S_ISFIFO(fileStat.st_mode))
                    {
                        rfd = open(name, O_RDONLY | O_NONBLOCK);
                    }
                    else
                    {
                        continue;
                    }
                    if (rfd == -1)
                    {
                        continue;
                    }

                    FileEntry each;
                    memset(&each, 0, sizeof each);
                    size = (int)fileStat.st_size;
                    memset(each.filename, '\0', sizeof(each.filename));
                    memcpy(each.filename, &name[baseIndex], strlen(name) - baseIndex);

                    each.mode = fileStat.st_mode;
                    each.flag = 0;
                    each.lastModification = fileStat.st_mtime;

                    do
                    {
                        rSize = read(rfd, each.content, sizeof(each.content));
                        if (rSize < 0)
                            break;
                        size -= sizeof(each.content);
                        if (size <= 0)
                            each.flag = 1; // this file done

                        each.bytesRead = rSize;




                        write(fd, &each, sizeof(FileEntry)); //103w
                        read(fd, &resp, sizeof(FileEntry)); // 103r ... response message from server
						memset(&resp, 0, sizeof resp); //respo

						strcpy(each.content, "");           // clear content

                        strcpy(message, ""); // clear message
                    } while (rSize > 0);
                    close(rfd);
                }
            }
        }
    }

    closedir(pDir);
}

