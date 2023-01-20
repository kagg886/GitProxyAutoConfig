#include <stdio.h>
#include <stdlib.h>
#include<winsock2.h>

void executeCMD(const char *cmd, char *result);

char* chooseIP(int ipCount,char** ips);

int main() {
    system("mode con cols=50 lines=30 ");
    printf("--------------Git Proxy Auto Config---------------\n");
    printf(
            "This tool can be used to quickly set the Git proxy as the port opened by Clash.\n\n"
            "To get the latest version of this program at:\n\n"
            "https://github.com/kagg886/GitProxyAutoConfig.\n\n"
            "--------------------------------------------------\n\n"
            "now,please give me your Clash LAN port(Enter -1 to represent the default value of 7890):"
    );

    fflush(stdin);
    int port;
    if (scanf("%d", &port) == 0) {
        printf("\n\nThe value you entered is wrong! please enter again!\n");
        system("pause");
        main();
        return 0;
    }
    if (port <= 0 || port >= 65353) {
        if (port == -1) {
            port = 7890;
        } else {
            printf("\n\nThe value you enter must meet: 0 <= port <= 65353! please enter again!\n");
            system("pause");
            main();
            return 0;
        }
    }
    printf("\n--------------------------------------------------\n\n");
    printf("start Config,Please wait a moment...\n\n");

    char result[1000] = {'\0'};
    executeCMD("ipconfig", result);

    int ipCount = 0;
    char **ips = malloc(sizeof(char**) * 1000);
    //IPv4 Address. . . . . . . . . . . : 192.168.137.1
    for (int i = 0; i < 987; ++i) {
        if (result[i] == '\0') {
            break;
        }
        if (result[i] == 'I' && result[i + 1] == 'P' && result[i + 2] == 'v' && result[i + 3] == '4') {
            char *c = &result[i+36];
            char *ip = malloc(13 * sizeof(char));
            for (int j = 0; j < 13; ++j) {
                ip[j] = *c++;
            }
            ip[12] = '\0';
            ips[ipCount] = ip;
            ipCount++;
        }
    }
    char* finallyIp = chooseIP(ipCount,ips);
    printf("\nOK!You choose:%s.Now please a wait...",finallyIp);
    char *httpProxySet = "git config --global http.proxy ";
    char buf[1000],buf1[5];
    strcat(buf,httpProxySet);
    strcat(buf,finallyIp);
    strcat(buf,":");
    itoa(port,buf1,10);
    strcat(buf, buf1);
    printf("%s\n",buf);
    system(buf);
    memset(buf,'\0', sizeof(buf));
    memset(buf,'\0', sizeof(buf1));
    char *httpsProxySet = "git config --global https.proxy ";
    strcat(buf,httpsProxySet);
    strcat(buf,finallyIp);
    strcat(buf,":");
    itoa(port,buf1,10);
    strcat(buf, buf1);
    printf("%s\n",buf);
    system(buf);
    printf("\nProxy set is complete! Thank you for using, bye!");
    system("pause");
    return 0;
}

char* chooseIP(int ipCount,char** ips) {
    if (ipCount != 1) {
        printf("We have found %d ip addresses, please enter the serial number represented before the ip address to select it\n\n",ipCount);
        for (int j = 0; j < ipCount; ++j) {
            printf("%d:%s\n\n",j,ips[j]);
        }
        int choice;
        printf("You input:");
        if (scanf("%d", &choice) == 0) {
            printf("\n\nThe value you entered is wrong! please enter again!\n");
            system("pause");
            return chooseIP(ipCount,ips);
        }

        if (choice >= ipCount) {
            printf("\n\nThe number you entered exceeds the upper limit of the found ip!please enter again!\n");
            system("pause");
            return chooseIP(ipCount,ips);
        }
        return ips[choice];
    } else {
        return ips[0];
    }
}

void executeCMD(const char *cmd, char *result) {
    char buf_ps[1024];
    char ps[1024] = {0};
    FILE *ptr;
    strcpy(ps, cmd);
    if ((ptr = popen(ps, "r")) != NULL) {
        while (fgets(buf_ps, 1024, ptr) != NULL) {
            strcat(result, buf_ps);
            if (strlen(result) > 1024)
                break;
        }
        pclose(ptr);
        ptr = NULL;
    } else {
        printf("popen %s error\n", ps);
    }
}
