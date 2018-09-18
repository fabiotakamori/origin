#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 12
// define = constante ou seja , variaveis imutaveis dentro do progama
int
main(int argc, char *argv[])
{
//struct, definim tipos de dados que agrupo variaveis sob um mesmo tipo, aplicado em estrutura de dados , podemos utilizar parentes e delcara seus respectivos atributos
//ex
// struct pais {
//atributos
//  }    e depois     struct nome comp; atribuindo todos os dados de nome a comp, assim podemos utilizar 
//pais.estado e utilizar de uma forma semelhante a orientacao a objeto
    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];
    // aqui ira conter o valor BUFFER_SIZE DEFINIDO NO INICIO
    /* Create data socket. */

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	//AF_UNIX = UNIX domain socket, comunicação local
	//ha varios tipos de unix domain , ao sockt stream, na teoria stram faz comunicao
	//byte a byte

    if (data_socket == -1) {
	// em caso de erro ira ter o retorno -1
        perror("socket");
        exit(EXIT_FAILURE);
    }
    //1- se não retornar -1, sigifica que foi criado com suceso o Master socket
    printf("Master Socket created\n")
    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     * */

    memset(&addr, 0, sizeof(struct sockaddr_un));// iniializa o sockaddr_un

    /* Connect socket to socket address */

    addr.sun_family = AF_UNIX;// com addr do codig acima, iremos atribuir ao sun_family = AF_UNIX
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);//será o nome do socket
    // aqui tambem será feito uma copia do socket addr.s

    ret = connect (data_socket, (const struct sockaddr *) &addr,
            sizeof(struct sockaddr_un));

    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    /* Send arguments. */
    do{
        printf("Enter number to send to server :\n");
        scanf("%d", &i);
        ret = write(data_socket, &i, sizeof(int));
        if (ret == -1) {
            perror("write");
            break;
        }
        printf("No of bytes sent = %d, data sent = %d\n", ret, i); 
    } while(i);

    /* Request result. */
    
    memset(buffer, 0, BUFFER_SIZE);
    strncpy (buffer, "RES", strlen("RES"));
    buffer[strlen(buffer)] = '\0';
    printf("buffer = %s\n", buffer);

    ret = write(data_socket, buffer, strlen(buffer));
    if (ret == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    /* Receive result. */
    memset(buffer, 0, BUFFER_SIZE);
    
    ret = read(data_socket, buffer, BUFFER_SIZE);
    if (ret == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    /* Ensure buffer is 0-terminated. */

    buffer[BUFFER_SIZE - 1] = 0;

    printf("Result = %s\n", buffer);

    /* Close socket. */

    close(data_socket);

    exit(EXIT_SUCCESS);
}
