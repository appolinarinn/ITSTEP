#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <ws2tcpip.h>
using namespace std;

// Подключаем необходимые библиотеки
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define PAUSE 1000

int main(int argc, char** argv) {
    setlocale(0, "Ukrainian");
    system("title CLIENT SIDE");
    cout << "Процес клiєнта запущено!\n";
    Sleep(PAUSE);

    // 1. Инициализация Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup не вдалося з помилкою: " << iResult << "\n";
        return 11;
    }

    // 2. Получение адреса и порта сервера
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    const char* ip = "localhost"; // IP-адрес сервера
    addrinfo* result = NULL;
    iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo не вдалося з помилкою: " << iResult << "\n";
        WSACleanup();
        return 12;
    }

    // 3. Создание и подключение сокета
    SOCKET ConnectSocket = INVALID_SOCKET;
    for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            cout << "socket не вдалося створити з помилкою: " << WSAGetLastError() << "\n";
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
        cout << "Неможливо пiдключитися до сервера. Перевiрте, чи запущено процес сервера!\n";
        WSACleanup();
        return 14;
    }

    // 4. Отправка данных серверу
    const char* message = "hello from client!";
    iResult = send(ConnectSocket, message, (int)strlen(message), 0);
    if (iResult == SOCKET_ERROR) {
        cout << "send не вдалося з помилкою: " << WSAGetLastError() << "\n";
        closesocket(ConnectSocket);
        WSACleanup();
        return 15;
    }

    // 5. Завершение отправки данных
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown не вдалося з помилкою: " << WSAGetLastError() << "\n";
        closesocket(ConnectSocket);
        WSACleanup();
        return 16;
    }

    // 6. Получение данных от сервера
    char answer[DEFAULT_BUFLEN];
    do {
        iResult = recv(ConnectSocket, answer, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            answer[iResult] = '\0'; // Завершаем строку
            cout << "Процес сервера надiслав вiдповiдь: " << answer << "\n";
        }
        else if (iResult == 0) {
            cout << "З'єднання з сервером закрито.\n";
        }
        else {
            cout << "recv не вдалося з помилкою: " << WSAGetLastError() << "\n";
        }
    } while (iResult > 0);

    // 7. Закрытие соединения и очистка ресурсов
    closesocket(ConnectSocket);
    WSACleanup();
    cout << "Процес клiєнта завершує свою роботу!\n";
    return 0;
}
