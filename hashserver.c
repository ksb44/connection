#include <apr.h>
#include <apr_network_io.h>
#include<apr_pools.h>
#include <apr_hash.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<apr_errno.h>


int main() {
  apr_status_t rv;
  apr_pool_t *pool;
  apr_socket_t *sockfd,*clientsocket;
  apr_sockaddr_t *serveraddr,*clientaddr;
char *key1 = "hi";
  char *val1 = "hello";
  char *key2 = "what";
  char *val2 = "up";
    char *key3= "feel";
  char *val3 = "good";
  apr_hash_t *hash;
  char buff[1024];

  rv = apr_initialize();
  if (rv != APR_SUCCESS) {
    fprintf(stderr, "Error initializing APR\n");
    return 1;
  }

  rv = apr_pool_create(&pool, NULL);
  if (rv != APR_SUCCESS) {
    fprintf(stderr, "Error creating memory pool\n");
    return 1;
  }

  rv = apr_sockaddr_info_get(&serveraddr, "127.0.0.1", APR_INET,5000,0, pool);
  if (rv != APR_SUCCESS) {
    fprintf(stderr, "Error getting server address\n");
    return 1;
  }

  rv = apr_socket_create(&sockfd, serveraddr->family, SOCK_STREAM, APR_PROTO_TCP, pool);
  if (rv != APR_SUCCESS) {
    fprintf(stderr, "Error creating server socket\n");
    return 1;
  }

  rv = apr_socket_bind(sockfd, serveraddr);
  if (rv != APR_SUCCESS) {
    fprintf(stderr, "Error binding server socket\n");
    return 1;
  }

  rv = apr_socket_listen(sockfd, 10);
  if (rv != APR_SUCCESS) {
    fprintf(stderr, "Error listening on server socket\n");
    return 1;
  }

  hash = apr_hash_make(pool);
  apr_hash_set(hash, key1, APR_HASH_KEY_STRING, val1);
  apr_hash_set(hash, key2, APR_HASH_KEY_STRING, val2);
  apr_hash_set(hash, key3, APR_HASH_KEY_STRING, val3);
 apr_size_t len = 1024;
  while (1) {
 
    rv = apr_socket_accept(&clientsocket, sockfd, pool);
    if (rv != APR_SUCCESS) {
      fprintf(stderr, "Error accepting client socket\n");
      break;
    }

    apr_socket_addr_get(&clientaddr, APR_LOCAL,clientsocket);
    printf("Accepted connection from %s:%d\n", clientaddr->hostname, clientaddr->port);
   while(1){
   memset(buff,0,sizeof(buff));
    rv = apr_socket_recv(clientsocket, buff, &len);
    if (rv != APR_SUCCESS) {
      fprintf(stderr, "Error receiving data from client\n");
      break;
    }
      if (strncmp(buff, "exit", 4) == 0) {
    printf("Closing connection from %s:%d\n", clientaddr->hostname, clientaddr->port);
apr_socket_close(clientsocket);
exit(1);
  }
    
   char *value = apr_hash_get(hash, buff, strlen(buff)); 
   if(value==NULL){
   
   value="value not found in hash";
   }

   
   rv = apr_socket_send(clientsocket,value,&len);
      if (rv != APR_SUCCESS) {
        fprintf(stderr, "Error sending data to client\n");
        continue;
      }

      }
       
    apr_socket_close(clientsocket);
  }
  
  apr_socket_close(sockfd);
  apr_pool_destroy(pool);
  apr_terminate();
  return 0;
}
