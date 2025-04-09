#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <ws2tcpip.h>
using namespace std;

// ���������� ����������� ����������
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define PAUSE 1000

int main(int argc, char** argv) {
    setlocale(0, "Ukrainian");
    system("title CLIENT SIDE");
    cout << "������ ��i���� ��������!\n";
    Sleep(PAUSE);

    // 1. ������������� Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup �� ������� � ��������: " << iResult << "\n";
        return 11;
    }

    // 2. ��������� ������ � ����� �������
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    const char* ip = "localhost"; // IP-����� �������
    addrinfo* result = NULL;
    iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo �� ������� � ��������: " << iResult << "\n";
        WSACleanup();
        return 12;
    }

    // 3. �������� � ����������� ������
    SOCKET ConnectSocket = INVALID_SOCKET;
    for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            cout << "socket �� ������� �������� � ��������: " << WSAGetLastError() << "\n";
            WSACleanup();
            return 13;
        }
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "��������� �i���������� �� �������. �����i���, �� �������� ������ �������!\n";
        WSACleanup();
        return 14;
    }

    // 4. �������� ������ �������
    const char* message = "hello from client!";
    iResult = send(ConnectSocket, message, (int)strlen(message), 0);
    if (iResult == SOCKET_ERROR) {
        cout << "send �� ������� � ��������: " << WSAGetLastError() << "\n";
        closesocket(ConnectSocket);
        WSACleanup();
        return 15;
    }

    // 5. ���������� �������� ������
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown �� ������� � ��������: " << WSAGetLastError() << "\n";
        closesocket(ConnectSocket);
        WSACleanup();
        return 16;
    }

    // 6. ��������� ������ �� �������
    char answer[DEFAULT_BUFLEN];
    do {
        iResult = recv(ConnectSocket, answer, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            answer[iResult] = '\0'; // ��������� ������
            cout << "������ ������� ���i���� �i����i��: " << answer << "\n";
        }
        else if (iResult == 0) {
            cout << "�'������� � �������� �������.\n";
        }
        else {
            cout << "recv �� ������� � ��������: " << WSAGetLastError() << "\n";
        }
    } while (iResult > 0);

    // 7. �������� ���������� � ������� ��������
    closesocket(ConnectSocket);
    WSACleanup();
    cout << "������ ��i���� ������� ���� ������!\n";
    return 0;
}
