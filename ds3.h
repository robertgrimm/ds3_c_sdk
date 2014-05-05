#ifndef __DS3_HEADER__
#define __DS3_HEADER__

#include <stdint.h>

typedef enum {
    false, true
}ds3_bool;

typedef enum {
  GET, PUT, POST, DELETE
}http_verb;

typedef struct {
    char * access_id;
    size_t access_id_len;
    char * secret_key;
    size_t secret_key_len;
}ds3_creds;

typedef struct {
    char * endpoint;
    size_t endpoint_len;
    char * proxy;
    size_t  proxy_len;
    ds3_creds * creds; 
}ds3_client;

typedef struct _ds3_request ds3_request;

typedef struct {
    char * creation_date;
    size_t creation_date_size;
    char * name;
    size_t name_size;
}ds3_bucket;

typedef struct {
    char * name;
    size_t name_size;
    char * id;
    size_t id_size;
}ds3_owner;

typedef struct {
    char * name;
    size_t name_size;
    char * etag;
    size_t etag_size;
    uint64_t size;
    ds3_owner *owner;
    char * last_modified;
    size_t last_modified_size;
    char * storage_class;
    size_t storage_class_size;
}ds3_object;

typedef struct {
    ds3_bucket * buckets;
    size_t num_buckets;
    ds3_owner *owner;
}ds3_get_service_response;

typedef struct {
    ds3_object *objects;
    size_t num_objects;
    char * creation_date;
    size_t creation_date_size;
    ds3_bool is_truncated;
    char * marker;
    size_t marker_size;
    char * delimiter;
    size_t delimiter_size;
    uint32_t max_keys;
    char * name;
    size_t name_size;
    char * next_marker;
    size_t next_marker_size;
    char * prefix;
    size_t prefix_size;
}ds3_get_bucket_response;

ds3_request * ds3_init_get_service(void);
ds3_request * ds3_init_get_bucket(const char * bucket_name);

ds3_creds * ds3_create_creds(const char * access_id, const char * secret_key);

ds3_client * ds3_create_client(const char * endpoint, ds3_creds * creds);
void ds3_client_proxy(ds3_client * client, const char * proxy);

ds3_get_service_response * ds3_get_service(const ds3_client * client, const ds3_request * request);

ds3_get_bucket_response * ds3_get_bucket(const ds3_client * client, const ds3_request * request);

void ds3_print_request(const ds3_request * request);

void ds3_free_service_response(ds3_get_service_response * response);
void ds3_free_bucket_response(ds3_get_bucket_response * response);

void ds3_free_bucket(ds3_bucket * bucket);
void ds3_free_owner(ds3_owner * owner);
void ds3_free_creds(ds3_creds * client);
void ds3_free_client(ds3_client * client);
void ds3_free_request(ds3_request * request);
void ds3_cleanup(void);
#endif
