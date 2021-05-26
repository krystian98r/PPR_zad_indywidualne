/* ********************************************** */
/* Na podstawie: http://xmlrpc-c.sourceforge.net  */
/* ********************************************** */
#define WIN32_LEAN_AND_MEAN  /* wymagane przez xmlrpc-c/server_abyss.h */

#include <stdlib.h>
#include <stdio.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
 
void socketClient(char *str) {
    int  	port   = 2222;
    char 	host[] = "127.0.0.1";

    int 	fd;
    struct 	sockaddr_in serv_addr;
    struct 	hostent *server;
     /* tworzymy gniazdo ***********************************************/
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (fd < 0) 
        printf("blad socket()");
  
	/* pobieramy informacje o serwerze ********************************/
    server = gethostbyname( host );
  
    if( server == NULL )
		printf("blad gethostbyname()" );
    
    /* zapelniamy strukture zerami (inicjujemy) ***********************/
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    /* przypisujemy parametry *****************************************/
    serv_addr.sin_family = AF_INET;			// typ gniazda 
    serv_addr.sin_port = htons(port);		// port
    
    bcopy( (char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length );
    
    /* nawiazujemy polaczenie *****************************************/
    if( connect( fd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) 
        printf( "connect() failed ");

    char buf[1024];
    bzero(buf, sizeof(buf));
    strcpy(buf, str);
    /* przesylamy string */
    write(fd, buf, sizeof(buf));

    close(fd);
} 
   
//======================================================================
static xmlrpc_value * getMess (
			xmlrpc_env *   const envP,
			xmlrpc_value * const paramArrayP,
			void *         const serverInfo,
			void *         const channelInfo) {

    char * str;

    /* Parsujemy tablice z argumentami ********************************/
    /* ("s") oznacza format danych, tutaj: string *******************/
    xmlrpc_decompose_value( envP, paramArrayP, "(s)", &str);
    if (envP->fault_occurred)
        return NULL;

    /* Dodajemy argumenty *********************************************/

    /* Zwracamy wynik *************************************************/
    // printf("Message: %s\n", str);
    printf("Sent forward\n");
    socketClient(strcat(str, "_Ruczynski"));

    return xmlrpc_build_value(envP, "s", str);
}
//======================================================================
int main( void ){
    printf("*** PROC 2 ****\n");
	/* zmienne predefiniowane *****************************************/
	int  port   = 12345;
	//------------------------------------------------------------------
    struct xmlrpc_method_info3 const methodInfo = {
        /* .methodName     = */ "getMess",
        /* .methodFunction = */ &getMess,
    };
    
	/* zmienne niezainicjowane ****************************************/
    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry * registryP;
    xmlrpc_env env;

    /* inicjujemy serwer **********************************************/
    xmlrpc_env_init(&env);

	/* rejestrujemy zmienne srodowiskowe ******************************/
    registryP = xmlrpc_registry_new(&env);
    if (env.fault_occurred){
        printf( "xmlrpc_registry_new(): %s\n", env.fault_string);
        exit(1);
    }

	/* rejestrujemy nasza metode/funkcje ******************************/
    xmlrpc_registry_add_method3( &env, registryP, &methodInfo );
    if (env.fault_occurred) {
        printf( "xmlrpc_registry_add_method3(): %s\n", env.fault_string );
        exit(1);
    }

	/* ustawiamy parametry serwera ************************************/
    serverparm.config_file_name = NULL;
    serverparm.registryP        = registryP;
    serverparm.port_number      = port;
    serverparm.log_file_name    = "/tmp/xmlrpc_log";

	/* uruchamiamy serwer *********************************************/
    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));
    if( env.fault_occurred ){
        printf("xmlrpc_server_abyss(): %s\n", env.fault_string);
        exit(1);
    }

	/* konczymy dzialanie *********************************************/
    return 0;
}
//======================================================================


