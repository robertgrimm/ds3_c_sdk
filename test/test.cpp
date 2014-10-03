// The BOOST_TEST_MODULE should be set only in this file
// so that a main function is only generated once
#define BOOST_TEST_MODULE DS3_Tests
#include <stdio.h>
#include <stdlib.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

ds3_client * get_client() {
    char* endpoint = getenv("DS3_ENDPOINT");
    char* access_key = getenv("DS3_ACCESS_KEY");
    char* secret_key = getenv("DS3_SECRET_KEY");
    char* proxy = getenv("http_proxy");

    ds3_client* client;

    if (endpoint == NULL) {
        fprintf(stderr, "DS3_ENDPOINT must be set.\n");
        exit(1);
    }

    if (access_key == NULL) {
        fprintf(stderr, "DS3_ACCESS_KEY must be set.\n");
        exit(1);
    }

    if (secret_key == NULL) {
        fprintf(stderr, "DS3_SECRET_KEY must be set.\n");
        exit(1);
    }

    ds3_creds* creds = ds3_create_creds(access_key, secret_key);
    
    client = ds3_create_client(endpoint, creds);
    
    if (proxy != NULL) {
        fprintf(stderr, "Setting proxy: %s\n", proxy);
        ds3_client_proxy(client, proxy); 
    }
    
    return client; 
}

void clear_bucket(const ds3_client* client, const char* bucket_name) {
    uint64_t i;
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* bucket_response;
    
    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &bucket_response);
    ds3_free_request(request);

    BOOST_CHECK(error == NULL);

    for (i = 0; i < bucket_response->num_objects; i++) {
        request = ds3_init_delete_object(bucket_name, bucket_response->objects[i].name->value);
        error = ds3_delete_object(client, request);
        ds3_free_request(request);
        
        if (error != NULL) {
            fprintf(stderr, "Failed to delete object %s\n", bucket_response->objects[i].name->value);
            ds3_free_error(error);
        } 
    }
    
    request = ds3_init_delete_bucket(bucket_name);
    error = ds3_delete_bucket(client, request);
    ds3_free_request(request);
    
    BOOST_CHECK(error == NULL);
}