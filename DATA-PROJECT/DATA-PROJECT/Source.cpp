//#include <iostream>
//#include<windows.h>
//#include<iomanip>
//#include <string>
//#include <ctime>
//
//using namespace std;
//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//// Data structures for Users, Posts, Messages, Friend Requests
//struct User; // Forward declaration
//
//// Graph structure for User Profiles
//struct GraphNode {
//    User* user;
//    GraphNode* next;
//};
//
//struct GraphEdge {
//    GraphNode* friendNode;
//    string relationType; // active, blocked, pending
//    GraphEdge* next;
//};
//
//// Linked List for Posts
//struct PostNode {
//    string content;
//    string timestamp;
//    PostNode* next;
//};
//
//// Queue for Friend Requests and Notifications
//struct QueueNode {
//    string notification;
//    QueueNode* next;
//};
//
//// Stack for Messages
//struct MessageNode {
//    string message;
//    MessageNode* next;
//};
//
//// Binary Search Tree for User Search
//struct BSTNode {
//    string username;
//    User* user;
//    BSTNode* left;
//    BSTNode* right;
//};
//
//// User Profile
//struct User {
//    string name;
//    string password;
//    string city;
//    string lastLoginTimestamp;
//    PostNode* posts;
//    QueueNode* friendRequests;
//    QueueNode* notifications;
//    GraphEdge* connections;
//    MessageNode* messages;
//};
//
//// Function prototypes
//void signup(GraphNode*& usersGraph, BSTNode*& userSearchTree);
//User* login(GraphNode* usersGraph, BSTNode* userSearchTree);
//void logout(User*& currentUser);
//void followRequest(User* currentUser, GraphNode* usersGraph);
//void cancelAcceptRequests(User* currentUser);
//void viewTimeline(User* currentUser);
//void viewNotifications(User* currentUser);
//void sendMessage(User* currentUser, GraphNode* usersGraph);
//void viewMessages(User* currentUser);
//void searchUsers(BSTNode* userSearchTree);
//void displayFollowers(User* currentUser);
//void viewNewsfeed(User* currentUser);
//void addNotification(User* targetUser, const string& message);
//void uploadPost(User* currentUser);
//
//void uploadPost(User* currentUser) {
//    if (!currentUser) {
//        cout << "Please log in to upload a post.\n";
//        return;
//    }
//
//    string content;
//    cout << "Enter your post content: ";
//    //cin.ignore();
//    getline(cin, content);
//
//    // Create a new post node
//    PostNode* newPost = new PostNode();
//    newPost->content = content;
//
//    // Add timestamp
//    time_t now = time(0);
//    char buffer[26];
//    ctime_s(buffer, sizeof(buffer), &now);
//    newPost->timestamp = buffer;
//
//    // Add post to the user's stack
//    newPost->next = currentUser->posts;
//    currentUser->posts = newPost;
//
//    cout << "Post uploaded successfully!\n";
//}
//
//
//void signup(GraphNode*& usersGraph, BSTNode*& userSearchTree) {
//    User* newUser = new User();
//    cout << "Enter username: ";
//    cin >> newUser->name;
//
//    // Check if username is unique in BST
//    BSTNode* current = userSearchTree;
//    while (current != nullptr) {
//        if (current->username == newUser->name) {
//            cout << "Username already exists. Please choose another.\n";
//            delete newUser;
//            return;
//        }
//        current = (newUser->name < current->username) ? current->left : current->right;
//    }
//
//    cout << "Enter password: ";
//    cin >> newUser->password;
//
//    cout << "Enter city: ";
//    cin >> newUser->city;
//
//    // Initialize user attributes
//    newUser->lastLoginTimestamp = "";
//    newUser->posts = nullptr;
//    newUser->friendRequests = nullptr;
//    newUser->notifications = nullptr;
//    newUser->connections = nullptr;
//    newUser->messages = nullptr;
//
//    // Insert user into graph
//    GraphNode* newGraphNode = new GraphNode();
//    newGraphNode->user = newUser;
//    newGraphNode->next = usersGraph;
//    usersGraph = newGraphNode;
//
//    // Insert into BST
//    BSTNode* newBSTNode = new BSTNode();
//    newBSTNode->username = newUser->name;
//    newBSTNode->user = newUser;
//    newBSTNode->left = newBSTNode->right = nullptr;
//
//    if (userSearchTree == nullptr) {
//        userSearchTree = newBSTNode;
//    }
//    else {
//        BSTNode* parent = nullptr;
//        BSTNode* current = userSearchTree;
//        while (current != nullptr) {
//            parent = current;
//            if (newUser->name < current->username)
//                current = current->left;
//            else
//                current = current->right;
//        }
//        if (newUser->name < parent->username)
//            parent->left = newBSTNode;
//        else
//            parent->right = newBSTNode;
//    }
//
//    cout << "Signup successful! Welcome, " << newUser->name << ".\n";
//}
//
//
//User* login(GraphNode* usersGraph, BSTNode* userSearchTree) {
//    string username, password;
//    cout << "Enter username: ";
//    cin >> username;
//    cout << "Enter password: ";
//    cin >> password;
//
//    // Search for user in BST
//    BSTNode* current = userSearchTree;
//    while (current != nullptr) {
//        if (current->username == username) {
//            if (current->user->password == password) {
//                // Update last login time
//                time_t now = time(0);
//                char buffer[26];
//                ctime_s(buffer, sizeof(buffer), &now);
//                current->user->lastLoginTimestamp = buffer;
//
//                cout << "Login successful! Welcome back, " << username << ".\n";
//                return current->user;
//            }
//            else {
//                cout << "Incorrect password. Please try again.\n";
//                return nullptr;
//            }
//        }
//        current = (username < current->username) ? current->left : current->right;
//    }
//
//    cout << "Username not found. Please signup first.\n";
//    return nullptr;
//}
//
//
//
//
//
//void logout(User*& currentUser) {
//    if (currentUser) {
//        cout << "Goodbye, " << currentUser->name << "!\n";
//        currentUser = nullptr;
//    }
//    else {
//        cout << "No user is currently logged in.\n";
//    }
//}
//
//
//
//
//void followRequest(User* currentUser, GraphNode* usersGraph) {
//    string friendName;
//    cout << "Enter the username to send a follow request: ";
//    cin >> friendName;
//
//    if (friendName == currentUser->name) {
//        cout << "You cannot send a follow request to yourself.\n";
//        return;
//    }
//
//    // Search for the friend in the graph
//    GraphNode* temp = usersGraph;
//    while (temp != nullptr && temp->user->name != friendName)
//        temp = temp->next;
//
//    if (temp == nullptr) {
//        cout << "User not found.\n";
//        return;
//    }
//
//    // Check if already connected
//    GraphEdge* edge = currentUser->connections;
//    while (edge != nullptr) {
//        if (edge->friendNode->user->name == friendName) {
//            cout << "Friend request already sent or you are already connected.\n";
//            return;
//        }
//        edge = edge->next;
//    }
//
//    // Add a friend request to the target user's queue
//    QueueNode* newRequest = new QueueNode();
//    newRequest->notification = currentUser->name + " sent you a follow request.";
//    newRequest->next = temp->user->friendRequests;
//    temp->user->friendRequests = newRequest;
//
//    addNotification(temp->user, currentUser->name + " sent you a follow request.");
//
//    cout << "Follow request sent to " << friendName << ".\n";
//}
//
//
//
//
//
//void cancelAcceptRequests(User* currentUser, GraphNode* usersGraph) {
//    // Check if there are any friend requests
//    if (currentUser->friendRequests == nullptr) {
//        cout << "No friend requests.\n";
//        return;
//    }
//
//    // Display friend requests
//    QueueNode* request = currentUser->friendRequests;
//    cout << "Friend requests:\n";
//    while (request != nullptr) {
//        cout << "- " << request->notification << "\n";
//        request = request->next;
//    }
//
//    // Accept or reject friend requests
//    cout << "Accept the first request? (yes/no): ";
//    string response;
//    cin >> response;
//
//    if (response == "yes") {
//        QueueNode* acceptedRequest = currentUser->friendRequests;
//        currentUser->friendRequests = currentUser->friendRequests->next;
//
//        // Extract sender's name from the notification
//        string senderName = acceptedRequest->notification.substr(0, acceptedRequest->notification.find(" "));
//
//        // Prevent self-connections
//        if (senderName == currentUser->name) {
//            cout << "You cannot accept a request from yourself.\n";
//            delete acceptedRequest;
//            return;
//        }
//
//        // Locate sender in the graph
//        GraphNode* temp = usersGraph;
//        while (temp != nullptr && temp->user->name != senderName)
//            temp = temp->next;
//
//        if (temp) {
//            // Add connections between the current user and sender
//            GraphEdge* newEdge = new GraphEdge();
//            newEdge->friendNode = temp;
//            newEdge->relationType = "active";
//            newEdge->next = currentUser->connections;
//            currentUser->connections = newEdge;
//
//            GraphEdge* reciprocalEdge = new GraphEdge();
//            reciprocalEdge->friendNode = usersGraph; // Current user's graph node
//            reciprocalEdge->relationType = "active";
//            reciprocalEdge->next = temp->user->connections;
//            temp->user->connections = reciprocalEdge;
//
//            cout << "Accepted friend request from " << senderName << ".\n";
//        }
//
//        delete acceptedRequest;
//    }
//    else {
//        cout << "No requests accepted.\n";
//    }
//}
//
//
//
//
//
//
//
//
//
//void viewTimeline(User* currentUser) {
//    if (currentUser->posts == nullptr) {
//        cout << "No posts to display in your timeline.\n";
//        return;
//    }
//
//    cout << "Your Timeline:\n";
//    PostNode* post = currentUser->posts;
//    while (post != nullptr) {
//        cout << "[" << post->timestamp << "] ";
//     
//        cout << post->content << "\n";
//        post = post->next;
//    }
//}
//
//
//
//
//
//
//void viewNotifications(User* currentUser) {
//    if (!currentUser) {
//        cout << "Please log in to view notifications.\n";
//        return;
//    }
//
//    if (currentUser->notifications == nullptr) {
//        cout << "No new notifications.\n";
//        return;
//    }
//
//    cout << "Notifications for " << currentUser->name << ":\n";
//
//    // Traverse the queue and display notifications
//    QueueNode* temp = currentUser->notifications;
//    while (temp != nullptr) {
//        cout << "- " << temp->notification << "\n";
//        temp = temp->next;
//    }
//
//    // Clear the notification queue
//    while (currentUser->notifications != nullptr) {
//        QueueNode* toDelete = currentUser->notifications;
//        currentUser->notifications = currentUser->notifications->next;
//        delete toDelete;
//    }
//
//    cout << "All notifications cleared.\n";
//}
//
//
//// Function to add notifications
//void addNotification(User* targetUser, const string& message) {
//    if (!targetUser) {
//        cout << "Target user does not exist.\n";
//        return;
//    }
//
//    // Create a new notification node
//    QueueNode* newNotification = new QueueNode();
//    newNotification->notification = message;
//    newNotification->next = nullptr;
//
//    // Add the notification to the end of the queue
//    if (targetUser->notifications == nullptr) {
//        targetUser->notifications = newNotification;
//    }
//    else {
//        QueueNode* temp = targetUser->notifications;
//        while (temp->next != nullptr) {
//            temp = temp->next;
//        }
//        temp->next = newNotification;
//    }
//
//    cout << "Notification added for " << targetUser->name << ".\n";
//}
//
//
//
//
//
//
//void sendMessage(User* currentUser, GraphNode* usersGraph) {
//    string friendName, messageContent;
//    cout << "Enter the username to send a message: ";
//    cin >> friendName;
//
//    // Find the friend in the graph
//    GraphNode* temp = usersGraph;
//    while (temp != nullptr && temp->user->name != friendName)
//        temp = temp->next;
//
//    if (temp == nullptr) {
//        cout << "User not found.\n";
//        return;
//    }
//
//    cout << "Enter your message: ";
//    cin.ignore();
//    getline(cin, messageContent);
//
//    // Add the message to the friend's stack
//    MessageNode* newMessage = new MessageNode();
//    newMessage->message = currentUser->name + ": " + messageContent;
//    newMessage->next = temp->user->messages;
//    temp->user->messages = newMessage;
//
//    // Add a notification for the receiver
//    addNotification(temp->user, "New message received from " + currentUser->name + ".");
//
//    cout << "Message sent to " << friendName << ".\n";
//}
//
//
//
//
//
//void viewMessages(User* currentUser) {
//    if (currentUser->messages == nullptr) {
//        cout << "No messages to display.\n";
//        return;
//    }
//
//    cout << "Your Messages:\n";
//    MessageNode* message = currentUser->messages;
//    while (message != nullptr) {
//        cout << "- " << message->message << "\n";
//        message = message->next;
//    }
//}
//
//
//
//
//
//
//void searchUsers(BSTNode* userSearchTree) {
//    string username;
//    cout << "Enter username to search: ";
//    cin >> username;
//
//    BSTNode* current = userSearchTree;
//    while (current != nullptr) {
//        if (current->username == username) {
//            cout << "User found: " << username << "\n";
//            return;
//        }
//        current = (username < current->username) ? current->left : current->right;
//    }
//
//    cout << "User not found.\n";
//}
//
//
//
//
//
//void displayFollowers(User* currentUser) {
//    if (!currentUser) {
//        cout << "Please log in to view your followers.\n";
//        return;
//    }
//
//    if (currentUser->connections == nullptr) {
//        cout << "You have no followers.\n";
//        return;
//    }
//
//    cout << "Your Followers:\n";
//    GraphEdge* connection = currentUser->connections;
//    bool hasFollowers = false;
//
//    while (connection != nullptr) {
//        if (connection->relationType == "active") {
//            cout << "- " << connection->friendNode->user->name << "\n";
//            hasFollowers = true;
//        }
//        connection = connection->next;
//    }
//
//    if (!hasFollowers) {
//        cout << "You have no followers.\n";
//    }
//}
//
//
//
//
//
//void viewNewsfeed(User* currentUser) {
//    if (!currentUser) {
//        cout << "Please log in to view your newsfeed.\n";
//        return;
//    }
//
//    if (currentUser->connections == nullptr) {
//        cout << "No friends' posts to display.\n";
//        return;
//    }
//
//    cout << "Your Newsfeed:\n";
//    GraphEdge* connection = currentUser->connections;
//    bool hasPosts = false;
//
//    while (connection != nullptr) {
//        if (connection->relationType == "active") {
//            PostNode* post = connection->friendNode->user->posts;
//            while (post != nullptr) {
//                cout << "[" << connection->friendNode->user->name << " - " << post->timestamp << "] "
//                    << post->content << "\n";
//                post = post->next;
//                hasPosts = true;
//            }
//        }
//        connection = connection->next;
//    }
//
//    if (!hasPosts) {
//        cout << "No posts to display in your newsfeed.\n";
//    }
//}
//
//void welcome() {
//    SetConsoleTextAttribute(hConsole, 13); // 13 is for magenta/pink
//    cout << "\n\n\n\n\n\n\n\n\n\n";
//    cout << setw(100) << "  d8,                                                                               " << endl;
//    cout << setw(100) << " `8P                      d8P                                                     " << endl;
//    cout << setw(100) << "                       d888888P                                                   " << endl;
//    cout << setw(100) << "  88b  88bd88b  .d888b,  ?88'   d888b8b   d888b8b    88bd88b d888b8b    88bd8b,d88b" << endl;
//    cout << setw(100) << "  88P  88P' ?8b ?8b,     88P   d8P' ?88  d8P' ?88    88P'  `d8P' ?88    88P'`?8P'?8b" << endl;
//    cout << setw(100) << " d88  d88   88P   `?8b   88b   88b  ,88b 88b  ,88b  d88     88b  ,88b  d88  d88  88P" << endl;
//    cout << setw(100) << "d88' d88'   88b`?888P'   `?8b  `?88P'`88b`?88P'`88bd88'     `?88P'`88bd88' d88'  88b" << endl;
//    cout << setw(100) << "                                                )88                                 " << endl;
//    cout << setw(100) << "                                               ,88P                                 " << endl;
//    cout << setw(100) << "                                           `?8888P                                  " << endl;
//    // Reset console text color to the default
//    SetConsoleTextAttribute(hConsole, 7); // 7 is the default color (white on black)
//    Sleep(3000);
//    system("cls");
//}
//
//
//
//
//
//
//int main() {
//    welcome();
//    GraphNode* usersGraph = nullptr;      // Graph for user connections
//    BSTNode* userSearchTree = nullptr;   // BST for user search
//    User* currentUser = nullptr;         // Currently logged-in user
//    int choice;
//    do {
//        SetConsoleTextAttribute(hConsole, 4); // is for red
//        cout << endl;
//        cout << setw(85) << "  88bd8b,d88b      d8888b      88bd88b     ?88   d8P" << endl;
//        cout << setw(85) << "  88P'`?8P'?8b    d8b_,dP      88P' ?8b    d88   88 " << endl;
//        cout << setw(85) << " d88  d88  88P    88b         d88   88P    ?8(  d88 " << endl;
//        cout << setw(85) << "d88' d88'  88b    `?888P'    d88'   88b    `?88P'?8b" << endl;
//        cout << setw(85) << "                                                     " << endl;
//        cout << setw(85) << "                                                     " << endl;
//        cout << setw(85) << "                                                     " << endl;
//        cout << setw(85) << "                                                     " << endl;
//        SetConsoleTextAttribute(hConsole, 11);  
//        cout << "                                                1. Signup\n";
//        cout << "                                                2. Login\n";
//        cout << "                                                3. Logout\n";
//        cout << "                                                4. Send Follow Request\n";
//        cout << "                                                5. Cancel/Accept Friend Requests\n";
//        cout << "                                                6. View Timeline\n";
//        cout << "                                                7. View Notifications\n";
//        cout << "                                                8. Send Message\n";
//        cout << "                                                9. View Messages\n";
//        cout << "                                                10. Search Users\n";
//        cout << "                                                11. View Followers\n";
//        cout << "                                                12. View Newsfeed\n";
//        cout << "                                                13. Upload post\n";
//        cout << "                                                0. Exit\n";
//        cout << "                                                Enter your choice: ";
//        cin >> choice;
//        cin.ignore();
//
//        switch (choice) {
//        case 1:
//            signup(usersGraph, userSearchTree);
//            break;
//        case 2:
//            currentUser = login(usersGraph, userSearchTree);
//            break;
//        case 3:
//            logout(currentUser);
//            break;
//        case 4:
//            if (currentUser)
//                followRequest(currentUser, usersGraph); // Pass usersGraph
//            else
//                cout << "Please login first.\n";
//            break;
//
//        case 5:
//            if (currentUser)
//                cancelAcceptRequests(currentUser, usersGraph); // Pass usersGraph
//            else
//                cout << "Please login first.\n";
//            break;
//
//        case 6:
//            if (currentUser)
//                viewTimeline(currentUser);
//            else
//                cout << "Please login first.\n";
//            break;
//        case 7:
//            if (currentUser)
//                viewNotifications(currentUser);
//            else
//                cout << "Please login first.\n";
//            break;
//        case 8:
//            if (currentUser)
//                sendMessage(currentUser, usersGraph); // Pass usersGraph
//            else
//                cout << "Please login first.\n";
//            break;
//        case 9:
//            if (currentUser)
//                viewMessages(currentUser);
//            else
//                cout << "Please login first.\n";
//            break;
//        case 10:
//            searchUsers(userSearchTree);
//            break;
//        case 11:
//            if (currentUser)
//                displayFollowers(currentUser);
//            else
//                cout << "Please login first.\n";
//            break;
//        case 12:
//            if (currentUser)
//                viewNewsfeed(currentUser);
//            else
//                cout << "Please login first.\n";
//            break;
//        case 13:
//            if (currentUser)
//                uploadPost(currentUser);
//            else
//                cout << "Please login first.\n";
//            break;
//        case 0:
//            cout << "Exiting Mini Instagram...\n";
//            break;
//        default:
//            cout << "Invalid choice. Please try again.\n";
//        }
//    } while (choice != 0);
//    system("pause");
//    return 0;
//}