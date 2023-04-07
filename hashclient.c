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
  apr_socket_t *sockfd;
  apr_sockaddr_t *serveraddr;
  char buff[1024];
 apr_size_t len = 1024;
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

rv = apr_sockaddr_info_get(&serveraddr, "127.0.0.1", APR_INET, 5000, 0, pool);
if (rv != APR_SUCCESS) {
fprintf(stderr, "Error getting server address\n");
return 1;
}

rv = apr_socket_create(&sockfd, serveraddr->family, SOCK_STREAM, APR_PROTO_TCP, pool);
if (rv != APR_SUCCESS) {
fprintf(stderr, "Error creating client socket\n");
return 1;
}

rv = apr_socket_connect(sockfd, serveraddr);
if (rv != APR_SUCCESS) {
fprintf(stderr, "Error connecting to server\n");
return 1;
}
while(1){
printf("Enter a value to send to server:\n");
fgets(buff, sizeof(buff), stdin);
buff[strcspn(buff, "\n")] = '\0';
rv = apr_socket_send(sockfd, buff, &len);
if (rv != APR_SUCCESS) {
fprintf(stderr, "Error sending data to server\n");
return 1;
}

rv = apr_socket_recv(sockfd, buff, &len);
if (rv != APR_SUCCESS) {
fprintf(stderr, "Error receiving data from server\n");
return 1;
}

printf("Server response: %s\n", buff);
}

apr_socket_close(sockfd);
apr_pool_destroy(pool);
apr_terminate();
return 0;
}
