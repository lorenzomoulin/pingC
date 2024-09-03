#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int is_valid_ipv4_octet(const char *octet) {
    int num = atoi(octet);
    if (num < 0 || num > 255) {
        return 0;
    }
    return 1;
}

int is_ipv4(const char *ip) {
    char ip_copy[100];
    char *token;
    int count = 0;

    // Faz uma cópia da string de IP para não modificar o original
    strcpy(ip_copy, ip);

    // Split da string por "."
    token = strtok(ip_copy, ".");
    while (token != NULL) {
        count++;
        // Verifica se cada parte é um número válido entre 0 e 255
        if (!is_valid_ipv4_octet(token)) {
            return 0;
        }
        token = strtok(NULL, ".");
    }

    // O IPv4 deve ter exatamente 4 octetos
    return count == 4;
}

int is_ipv6(const char *ip) {
    // A implementação do IPv6 permanece a mesma
    regex_t regex;
    int ret;
    const char *pattern = "^[0-9a-fA-F:]+$";
    
    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    
    ret = regexec(&regex, ip, 0, NULL, 0);
    regfree(&regex);
    
    return !ret;
}

void ping_ip(const char *ip) {
    char command[100];
    FILE *fp;
    char result[1024];
    
    snprintf(command, sizeof(command), "ping -c 4 %s", ip);
    
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }
    
    while (fgets(result, sizeof(result), fp) != NULL) {
        printf("%s", result);
    }
    
    pclose(fp);
}

int main() {
    char ip[100];
    printf("Estas são as informações do monitor de rede do Vinicius soares:\n");
    printf("Enter IP address (IPv4 or IPv6): ");
    scanf("%99s", ip);

    if (is_ipv4(ip)) {
        printf("IP type: IPv4\n");
    } else if (is_ipv6(ip)) {
        printf("IP type: IPv6\n");
    } else {
        printf("Invalid IP address format\n");
        return 1;
    }

    printf("Pinging %s...\n", ip);
    ping_ip(ip);

    return 0;
}
