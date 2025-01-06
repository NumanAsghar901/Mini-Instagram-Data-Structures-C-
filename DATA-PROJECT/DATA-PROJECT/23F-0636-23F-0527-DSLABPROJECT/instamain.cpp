#pragma once
#include"insta.h"
int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    welcome();
    GraphNode* usersGraph = nullptr;      // Graph for user connections
    BSTNode* userSearchTree = nullptr;   // BST for user search
    User* currentUser = nullptr;         // Currently logged-in user
    int choice;
    do {
        SetConsoleTextAttribute(hConsole, 4); // is for red
        cout << endl;
        cout << setw(85) << "  88bd8b,d88b      d8888b      88bd88b     ?88   d8P" << endl;
        cout << setw(85) << "  88P'`?8P'?8b    d8b_,dP      88P' ?8b    d88   88 " << endl;
        cout << setw(85) << " d88  d88  88P    88b         d88   88P    ?8(  d88 " << endl;
        cout << setw(85) << "d88' d88'  88b    `?888P'    d88'   88b    `?88P'?8b" << endl;
        cout << setw(85) << "                                                     " << endl;
        cout << setw(85) << "                                                     " << endl;
        cout << setw(85) << "                                                     " << endl;
        cout << setw(85) << "                                                     " << endl;
        SetConsoleTextAttribute(hConsole, 11);
        cout << "                                                1. Signup\n";
        cout << "                                                2. Login\n";
        cout << "                                                3. Logout\n";
        cout << "                                                4. Send Follow Request\n";
        cout << "                                                5. Cancel/Accept Friend Requests\n";
        cout << "                                                6. View Timeline\n";
        cout << "                                                7. View Notifications\n";
        cout << "                                                8. Send Message\n";
        cout << "                                                9. View Messages\n";
        cout << "                                                10. Search Users\n";
        cout << "                                                11. View Followers\n";
        cout << "                                                12. View Newsfeed\n";
        cout << "                                                13. Upload post\n";
        cout << "                                                0. Exit\n";
        cout << "                                                Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            signup(usersGraph, userSearchTree);
            break;
        case 2:
            currentUser = login(usersGraph, userSearchTree);
            break;
        case 3:
            logout(currentUser);
            break;
        case 4:
            if (currentUser)
                followRequest(currentUser, usersGraph); // Pass usersGraph
            else
                cout << "Please login first.\n";
            break;

        case 5:
            if (currentUser)
                cancelAcceptRequests(currentUser, usersGraph); // Pass usersGraph
            else
                cout << "Please login first.\n";
            break;

        case 6:
            if (currentUser)
                viewTimeline(currentUser);
            else
                cout << "Please login first.\n";
            break;
        case 7:
            if (currentUser)
                viewNotifications(currentUser);
            else
                cout << "Please login first.\n";
            break;
        case 8:
            if (currentUser)
                sendMessage(currentUser, usersGraph); // Pass usersGraph
            else
                cout << "Please login first.\n";
            break;
        case 9:
            if (currentUser)
                viewMessages(currentUser);
            else
                cout << "Please login first.\n";
            break;
        case 10:
            searchUsers(userSearchTree,currentUser);
            break;
        case 11:
            if (currentUser)
                displayFollowers(currentUser);
            else
                cout << "Please login first.\n";
            break;
        case 12:
            if (currentUser)
                viewNewsfeed(currentUser);
            else
                cout << "Please login first.\n";
            break;
        case 13:
            if (currentUser)
                uploadPost(currentUser);
            else
                cout << "Please login first.\n";
            break;
        case 0:
            cout << "Exiting Mini Instagram...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
      //  system("cls");
    } while (choice != 0);
    system("pause");
    return 0;
}