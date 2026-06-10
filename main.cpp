#include "DatabaseManager.h"
#include "DatabaseInitializer.h"
#include "AppController.h"
#include "Restaurant.h"
#include <iostream>
#include <vector>
#include <cstdlib> //baray tabe system
#include <conio.h> //ezafe shodan baray khandan password be sorat ****

using namespace std;


int main() 
{
    // farsi dar windows
    system("chcp 65001 > nul");

    // etesal b database
    DatabaseManager db("food_system.db");

    if (!db.open()) {
        cerr << "Error: Could not open database connection. Did you break it?! 💥" << endl;
        return 1;
    }

    // rah andazi va sakht jadval ha
    DatabaseInitializer::initialize(db);
    
    // sakht ghalb tapande barname ma
    AppController appController(db);

    int mainChoice;
    do {
        cout << "\n=================================\n";
        cout << "   Welcome to the Food System! 🍽️\n";
        cout << "=================================\n";
        cout << "1. Customer Portal 👨‍👩‍👧‍👦\n";
        cout << "2. Manager Portal 🧓\n";
        cout << "3. Admin Portal 👨‍💻\n";
        cout << "0. Exit 🏃‍♂️\n";
        cout << "Enter your role: ";
        
        // jologiri az crash age b jay adad horof vared shavad
        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Numbers only, my friend! We don't speak alphabet here. 🧐\n";
            continue;
        }

        switch (mainChoice) 
        {
            case 1: 
                // vagozari karhaye moshtari b method makhsos khodesh to controller
                appController.customerLoginMenu(); 
                break;
                
            case 2: 
                appController.managerLoginMenu(); 
                break;
                
            case 3: // system taeid hoviyat addmin
                {
                    string password = "";
                    char ch;
                    cout << "\n--- 🔐 Admin Security Check ---" << endl;
                    cout << "Enter Admin Password: ";
                    
                    // halghe daryaft charactor ha b sorat makhfi ***
                    while (true) {
                        ch = _getch();//daryaft kelid bedon chap roy safhe
                        
                        if (ch == 13) { //dokme enter
                            break;
                        }
                        else if (ch == 8) { // // baray pak kardan eshtebahat backspace
                            if (!password.empty()) {
                                password.pop_back();
                                cout << "\b \b"; // pak kardan * az terminal
                            }
                        } 
                        else if (ch >= 32 && ch <= 126) { // charactor hay mojaz va estandard keyboard
                            password.push_back(ch);
                            cout << "*"; //chap * b jay horof asli
                        }
                    }

                    //ramz vorod admin
                    if (password == "admin123") 
                    {
                        cout << "\n\n✅ Access Granted! Welcome Admin.\n";
                        appController.runSystemAdminPanel(); // vorod b panel admin kol system
                    } 
                    else 
                    {
                        cout << "\n\n❌ Access Denied! Wrong password. Don't try to hack us! 🧐\n";
                    }
                }
                break;
                
            case 0: 
                cout << "Goodbye! Have a delicious day! 👋\n"; 
                break;
                
            default: 
                cout << "Invalid choice! Try again, my friend. 🧐\n";
        } 
        
    } while (mainChoice != 0);
    
    return 0;
}

