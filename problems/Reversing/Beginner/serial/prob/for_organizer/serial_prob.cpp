#include <cstdio>
#include <cstring>

const char* bb = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void encode_serial(unsigned int p, char* output) {
    char num_str[20];
    snprintf(num_str, sizeof(num_str), "%u", p);
    size_t len = strlen(num_str);
    int out_idx = 0;

    for (size_t i = 0; i < len; i += 3) {
        unsigned char b1 = num_str[i] - '0';
        unsigned char b2 = (i + 1 < len) ? num_str[i + 1] - '0' : 0;
        unsigned char b3 = (i + 2 < len) ? num_str[i + 2] - '0' : 0;

        unsigned int triple = (b1 << 8) | (b2 << 4) | b3;

        output[out_idx++] = bb[(triple >> 12) & 0x3F];
        output[out_idx++] = bb[(triple >> 6) & 0x3F];
        output[out_idx++] = (i + 1 < len) ? bb[triple & 0x3F] : '=';
        output[out_idx++] = (i + 2 < len) ? bb[(triple << 6) & 0x3F] : '=';
    }
    output[out_idx] = '\0';
}

bool input_check(unsigned int user_input, unsigned int p, const char* name) {
    if (user_input != p || strcmp(name, "hello") != 0) return false;

    char encoded[32];
    encode_serial(p, encoded);

    char flag[128];
    snprintf(flag, sizeof(flag), "hspace{%s", encoded);

    for (const char* k = name; *k; k++) {
        char buf[4];
        snprintf(buf, sizeof(buf), "%hhu", (unsigned char)*k);
        strcat(flag, buf);
    }
    strcat(flag, "}");
    puts(flag);
    return true;
}

int main() {
    char name[64];
    unsigned int p = 0, user_input = 0;

    printf("Enter name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    for (const char* t = name; *t; t++) p += *t;

    printf("Enter serial: ");
    scanf("%u", &user_input);

    if (!input_check(user_input, p, name)) {
        puts("Invalid serial!");
    }

    printf("Press enter to exit...");
    return 0;
}
