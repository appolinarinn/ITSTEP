#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

size_t write_callback(void* contents, size_t size, size_t nmemb, string* s)
{
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    }
    catch (bad_alloc& e) {
        
        return 0;
    }
    return newLength;
}

void horoscope(const string& sign, const string& day)
{
    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize cURL!\n";
        return;
    }

    string readBuffer;
    string url = "https://horoscope-app-api.vercel.app/api/v1/get-horoscope/daily?sign=" + sign + "&day=" + day;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "cURL error: " << curl_easy_strerror(res) << "\n";
    }
    else {
        try {
            auto js = json::parse(readBuffer);
            string date = js["data"]["date"].get<string>();
            string horoscope = js["data"]["horoscope_data"].get<string>();

            cout << "\n--- Horoscope for " << date << " (" << day << ") ---\n";
            cout << horoscope << "\n";
        }
        catch (exception& e) {
            cerr << "Failed to parse JSON: " << e.what() << "\n";
        }
    }

    curl_easy_cleanup(curl);
}

vector<string> starSigns = {
    "Aries", "Taurus", "Gemini", "Cancer", "Leo", "Virgo",
    "Libra", "Scorpio", "Sagittarius", "Capricorn", "Aquarius", "Pisces"
};

string getSign()
{
    cout << "Pick a zodiac sign:\n";

    for (size_t i = 0; i < starSigns.size(); ++i) {
        cout << i + 1 << ". " << starSigns[i] << "\n";
    }

    int pickedSign = -1;
    while (pickedSign < 1 || pickedSign > 12) {
        cin >> pickedSign;
    }
    return starSigns[pickedSign - 1];
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);

    string sign = getSign();

   horoscope(sign, "TODAY");
   horoscope(sign, "TOMORROW");

    cout << "\nMay the odds be in your favour!\n";

    curl_global_cleanup();
    return 0;
}
