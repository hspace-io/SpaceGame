// gcc -o crpc_v1 crpc_v1.c -rdynamic
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <dlfcn.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define MAX_FUNCS 10

typedef struct {
    uint32_t header;
    uint8_t type;
    uint32_t request_id;
    char* func_name;
    uint32_t name_length;
    char name[32];
    uint32_t param_length;
    char params[256];
} RequestMessage;

typedef struct {
    uint32_t length;
    uint8_t type;
    uint32_t request_id;
    uint8_t status;
    uint32_t result_length;
    char result[256];
} ResponseMessage;


void handle_echo(const char* params, char* result) {
    strcpy(result, params);
}

void handle_reverse(const char* params, char* result) {
    int len = strlen(params);
    for (int i = 0; i < len; i++) {
        result[i] = params[len - i - 1];
    }
    result[len] = '\0';
}

void handle_write(const char* params, char* result) {
    write(1, params, strlen(params));
    strcpy(result, "Done");
}

void handle_open(const char* params, char* result) {
    int fd = open(params, O_RDONLY);
    if (fd == -1) {
        strcpy(result, "Error: Could not open file");
    } else {
        sprintf(result, "File descriptor: %d", fd);
    }
}

void handle_close(const char* params, char* result) {
    int fd;
    sscanf(params, "%d", &fd);
    if (close(fd) == -1) {
        strcpy(result, "Error: Could not close file");
    } else {
        strcpy(result, "Done");
    }
}

void handle_upper(const char* params, char* result) {
    int len = strlen(params);
    for (int i = 0; i < len; i++) {
        if (params[i] >= 'a' && params[i] <= 'z') {
            result[i] = params[i] - 'a' + 'A';
        } else {
            result[i] = params[i];
        }
    }
    result[len] = '\0';
}

void handle_read(const char* params, char* result) {
    char tmp[256];
    read(0, tmp, 256);
    strcpy(result, tmp);
}

void handle_sum(const char* params, char* result) {
    int a, b;
    sscanf(params, "%d %d", &a, &b);
    int sum = a + b;
    sprintf(result, "%d", sum);
}

void handle_multiply(const char* params, char* result) {
    int a, b;
    sscanf(params, "%d %d", &a, &b);
    int product = a * b;
    sprintf(result, "%d", product);
}

void handle_divide(const char* params, char* result) {
    int a, b;
    sscanf(params, "%d %d", &a, &b);
    if (b == 0) {
        strcpy(result, "Error: Division by zero");
    } else {
        int quotient = a / b;
        sprintf(result, "%d", quotient);
    }
}

void handle_pow(const char* params, char* result) {
    int a, b;
    sscanf(params, "%d %d", &a, &b);
    int power = 1;
    for (int i = 0; i < b; i++) {
        power *= a;
    }
    sprintf(result, "%d", power);
}

void handle_mod(const char* params, char* result) {
    int a, b;
    sscanf(params, "%d %d", &a, &b);
    if (b == 0) {
        strcpy(result, "Error: Division by zero");
    } else {
        int remainder = a % b;
        sprintf(result, "%d", remainder);
    }
}

void handle_gcd(const char* params, char* result) {
    int a, b;
    sscanf(params, "%d %d", &a, &b);
    int gcd = 1;
    for (int i = 1; i <= a && i <= b; i++) {
        if (a % i == 0 && b % i == 0) {
            gcd = i;
        }
    }
    sprintf(result, "%d", gcd);
}

void handle_is_prime(const char* params, char* result) {
    int n;
    sscanf(params, "%d", &n);
    if (n <= 1) {
        strcpy(result, "NO");
        return;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            strcpy(result, "NO");
            return;
        }
    }
    strcpy(result, "YES");
}

void handle_is_palindrome(const char* params, char* result) {
    int n;
    sscanf(params, "%d", &n);
    int temp = n;
    int rev = 0;
    while (temp > 0) {
        rev = rev * 10 + temp % 10;
        temp /= 10;
    }
    if (n == rev) {
        strcpy(result, "YES");
    } else {
        strcpy(result, "NO");
    }
}

void handle_is_armstrong(const char* params, char* result) {
    int n;
    sscanf(params, "%d", &n);
    int temp = n;
    int sum = 0;
    while (temp > 0) {
        int digit = temp % 10;
        sum += digit * digit * digit;
        temp /= 10;
    }
    if (n == sum) {
        strcpy(result, "YES");
    } else {
        strcpy(result, "NO");
    }
}

typedef void (*func_ptr)(const char*, char*);

typedef struct {
    char *name;
    func_ptr func;
} FuncEntry;

enum res_type{
    ERROR = 0x01,
    REGISTER,
    UNREGISTER,
    PING,
    PONG,
    EXECUTE
};

enum res_status{
    OK = 0x01,
    NO,
};

FuncEntry func_list[MAX_FUNCS];
int func_count = 0;

void register_function(const char *name) {
    if (func_count < MAX_FUNCS) {
        func_list[func_count].name = strdup(name);
        func_list[func_count].func = (func_ptr)dlsym(RTLD_DEFAULT, name);
        if (func_list[func_count].func == NULL) {
            fprintf(stderr, "Error: Function %s not found\n", name);
            free(func_list[func_count].name);
        } else {
            func_count++;
        }
    } else {
        fprintf(stderr, "Error: Maximum number of functions reached\n");
    }
}

void unregister_function(const char *name) {
    int i;
    for (i = 0; i < func_count; i++) {
        if (strcmp(name, func_list[i].name) == 0) {
            free(func_list[i].name);
            func_list[i].name = NULL;
            func_list[i].func = NULL;
            break;
        }
    }
    if (i == func_count) {
        fprintf(stderr, "Error: Function %s not found\n", name);
    }
}

void process_request(RequestMessage* req) {
    ResponseMessage res;

    memset(&res, 0, sizeof(res));
    
    if(req->header != 0xe0f1d2c3) {
        res.type = ERROR;
        res.status = NO;
        res.request_id = req->request_id;
        res.length = sizeof(res);
        strcpy(res.result, "Error: Invalid header");
        res.result_length = strlen(res.result);
        write(1, &res, res.length);
        return;
    }
    if(req->name_length != strlen(req->func_name)) {
        printf("name_length: %d, strlen: %ld\n", req->name_length, strlen(req->func_name));
        res.type = ERROR;
        res.status = NO;
        res.request_id = req->request_id;
        res.length = sizeof(res);
        strcpy(res.result, "Error: Invalid name length");
        res.result_length = strlen(res.result);
        write(1, &res, res.length);
        return;
    }
    if(req->name_length > 32 || req->param_length > 256) {
        res.type = ERROR;
        res.status = NO;
        res.request_id = req->request_id;
        res.length = sizeof(res);
        strcpy(res.result, "Error: Invalid length");
        res.result_length = strlen(res.result);
        write(1, &res, res.length);
        return;
    }

    switch(req->type) {
        case REGISTER:
            register_function(req->func_name);
            res.type = REGISTER;
            res.status = OK;
            res.result_length = 0;
            res.request_id = req->request_id;
            res.length = sizeof(res);
            write(1, &res, res.length);
            return;
        case UNREGISTER:
            unregister_function(req->func_name);
            res.type = UNREGISTER;
            res.status = OK;
            res.result_length = 0;
            res.request_id = req->request_id;
            res.length = sizeof(res);
            write(1, &res, res.length);
            return;
        case PING:
            res.type = PONG;
            res.status = OK;
            res.request_id = req->request_id;
            res.length = sizeof(res);
            strcpy(res.result, "PongPong!");
            res.result_length = strlen(res.result);
            write(1, &res, res.length);
            return;
        case PONG:
            res.type = ERROR;
            res.status = NO;
            res.request_id = req->request_id;
            res.length = sizeof(res);
            strcpy(res.result, "Error: Invalid request type");
            res.result_length = strlen(res.result);
            write(1, &res, res.length);
            return;
        case EXECUTE:
            res.type = EXECUTE;
            res.status = OK;
            res.request_id = req->request_id;
            int i;
            for (i = 0; i < func_count; i++) {
                if (strcmp(req->func_name, func_list[i].name) == 0) {
                    // printf("Calling function %s\n", func_list[i].name);
                    func_list[i].func(req->params, res.result);
                    // printf("Result: %s\n", res.result);
                    break;
                }
            }
            if (i == func_count) {
                res.status = NO;
                strcpy(res.result, "Error: Function not found");
                res.result_length = strlen(res.result);
            } else {
                res.result_length = strlen(res.result);
            }
            res.length = sizeof(res);
            // printf("res.length : %d\n\n\n", res.length);
            write(1, &res, res.length);
            return;
        default:
            res.type = ERROR;
            res.status = NO;
            res.request_id = req->request_id;
            res.length = sizeof(res);
            strcpy(res.result, "Error: Invalid request type");
            res.result_length = sizeof(res.result);
            write(1, &res, res.length);
            return;
    }
}

int main() {
    char buffer[BUF_SIZE];

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    // register_function("handle_sum");
    // register_function("handle_multiply");
    // register_function("handle_divide");

    while (1) {
        read(0, buffer, BUF_SIZE);
        RequestMessage* req = (RequestMessage*) buffer;
        
        req->func_name = malloc(0x20);
        memcpy(req->func_name, req->name, 0x20);
        process_request(req);

        free(req->func_name);
    }
    return 0;
}
