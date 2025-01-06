#pragma once
#include"insta.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void uploadPost(User* currentUser) {
    if (!currentUser) {
        cout << "Please log in to upload a post.\n";
        return;
    }

    string content;
    cout << "Enter your post content: ";
    //cin.ignore();
    getline(cin, content);

    // Create a new post node
    PostNode* newPost = new PostNode();
    newPost->content = content;

    // Add timestamp
    time_t now = time(0);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &now);
    newPost->timestamp = buffer;

    // Add post to the user's stack
    newPost->next = currentUser->posts;
    currentUser->posts = newPost;

    cout << "Post uploaded successfully!\n";
}


bool isPasswordStrong(const string& password) {
    bool hasLetter = false, hasDigit = false;
    for (char c : password) {
        if (isalpha(c)) hasLetter = true;
        if (isdigit(c)) hasDigit = true;
    }
    return hasLetter && hasDigit;
}

void signup(GraphNode*& usersGraph, BSTNode*& userSearchTree) {
    User* newUser = new User();
    cout << "Enter username: ";
    cin >> newUser->name;

    // Check if username is unique in BST
    BSTNode* current = userSearchTree;
    while (current != nullptr) {
        if (current->username == newUser->name) {
            cout << "Username already exists. Please choose another.\n";
            delete newUser;
            return;
        }
        current = (newUser->name < current->username) ? current->left : current->right;
    }

    // Password validation
    string password;
    cout << "Enter password: ";
    while (true) {
        cin >> password;
        if (isPasswordStrong(password)) {
            break;
        }
        else {
            cout << "Password must contain at least one letter and one number. Try again: ";
        }
    }
    newUser->password = password;

    // Security question
    cout << "Set a security question for password recovery (e.g., What is your pet's name?): ";
    cin.ignore();  // To clear the newline character from previous input
    getline(cin, newUser->securityQuestion);
    cout << "Enter the answer to the security question: ";
    getline(cin, newUser->securityAnswer);

    cout << "Enter city: ";
    cin >> newUser->city;

    // Initialize user attributes
    newUser->lastLoginTimestamp = "";
    newUser->posts = nullptr;
    newUser->friendRequests = nullptr;
    newUser->notifications = nullptr;
    newUser->connections = nullptr;
    newUser->messages = nullptr;

    // Insert user into graph
    GraphNode* newGraphNode = new GraphNode();
    newGraphNode->user = newUser;
    newGraphNode->next = usersGraph;
    usersGraph = newGraphNode;

    // Insert into BST
    BSTNode* newBSTNode = new BSTNode();
    newBSTNode->username = newUser->name;
    newBSTNode->user = newUser;
    newBSTNode->left = newBSTNode->right = nullptr;

    if (userSearchTree == nullptr) {
        userSearchTree = newBSTNode;
    }
    else {
        BSTNode* parent = nullptr;
        BSTNode* current = userSearchTree;
        while (current != nullptr) {
            parent = current;
            if (newUser->name < current->username)
                current = current->left;
            else
                current = current->right;
        }
        if (newUser->name < parent->username)
            parent->left = newBSTNode;
        else
            parent->right = newBSTNode;
    }

    cout << "Signup successful! Welcome, " << newUser->name << ".\n";
}



User* login(GraphNode* usersGraph, BSTNode* userSearchTree) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Search for user in BST
    BSTNode* current = userSearchTree;
    while (current != nullptr) {
        if (current->username == username) {
            if (current->user->password == password) {
                // Update last login time
                time_t now = time(0);
                char buffer[26];
                ctime_s(buffer, sizeof(buffer), &now);
                current->user->lastLoginTimestamp = buffer;

                cout << "Login successful! Welcome back, " << username << ".\n";
                return current->user;
            }
            else {
                cout << "Incorrect password. Would you like to reset your password? (y/n): ";
                char choice;
                cin >> choice;

                if (choice == 'y' || choice == 'Y') {
                    string answer;
                    cout << "Security question: " << current->user->securityQuestion << endl;
                    cout << "Answer: ";
                    cin.ignore();  // To clear the newline character
                    getline(cin, answer);

                    if (answer == current->user->securityAnswer) {
                        cout << "Answer is correct. Enter new password: ";
                        string newPassword;
                        while (true) {
                            cin >> newPassword;
                            if (isPasswordStrong(newPassword)) {
                                current->user->password = newPassword;
                                cout << "Password reset successful. You can now log in with your new password.\n";
                                return nullptr;
                            }
                            else {
                                cout << "Password must contain at least one letter and one number. Try again: ";
                            }
                        }
                    }
                    else {
                        cout << "Incorrect answer to security question. Cannot reset password.\n";
                    }
                }
                return nullptr;
            }
        }
        current = (username < current->username) ? current->left : current->right;
    }

    cout << "Username not found. Please signup first.\n";
    return nullptr;
}





void logout(User*& currentUser) {
    if (currentUser) {
        cout << "Goodbye, " << currentUser->name << "!\n";
        currentUser = nullptr;
    }
    else {
        cout << "No user is currently logged in.\n";
    }
}




void followRequest(User* currentUser, GraphNode* usersGraph) {
    string friendName;
    cout << "Enter the username to send a follow request: ";
    cin >> friendName;

    if (friendName == currentUser->name) {
        cout << "You cannot send a follow request to yourself.\n";
        return;
    }

    // Search for the friend in the graph
    GraphNode* temp = usersGraph;
    while (temp != nullptr && temp->user->name != friendName)
        temp = temp->next;

    if (temp == nullptr) {
        cout << "User not found.\n";
        return;
    }

    // Check if already connected
    GraphEdge* edge = currentUser->connections;
    while (edge != nullptr) {
        if (edge->friendNode->user->name == friendName) {
            cout << "Friend request already sent or you are already connected.\n";
            return;
        }
        edge = edge->next;
    }

    // Create a new edge for currentUser to friendName
    GraphEdge* newEdge = new GraphEdge();
    newEdge->friendNode = temp; // The friend's node
    newEdge->relationType = "pending"; // Connection status
    newEdge->next = currentUser->connections; // Link to existing connections
    currentUser->connections = newEdge; // Update head of connections

    // Create a reverse edge for friendName to currentUser
    GraphEdge* reverseEdge = new GraphEdge();
    reverseEdge->friendNode = new GraphNode();
    reverseEdge->friendNode->user = currentUser; // Point back to current user
    reverseEdge->relationType = "pending"; // Same connection status
    reverseEdge->next = temp->user->connections; // Link to their existing connections
    temp->user->connections = reverseEdge; // Update head of their connections

    // Add a friend request to the target user's queue
    QueueNode* newRequest = new QueueNode();
    newRequest->notification = currentUser->name + " sent you a follow request.";
    newRequest->next = temp->user->friendRequests;
    temp->user->friendRequests = newRequest;
    addNotification(temp->user, currentUser->name + " sent you a follow request.");

    cout << "Follow request sent to " << friendName << ".\n";
}





void cancelAcceptRequests(User* currentUser, GraphNode* usersGraph) {
    // Check if there are any friend requests
    if (currentUser->friendRequests == nullptr) {
        cout << "No friend requests.\n";
        return;
    }

    // Display friend requests
    QueueNode* request = currentUser->friendRequests;
    cout << "Friend requests:\n";
    int index = 1;
    while (request != nullptr) {
        cout << index++ << ". " << request->notification << "\n";
        request = request->next;
    }

    // Ask if the user wants to accept all requests or specific ones
    cout << "Would you like to accept all requests? (yes/no): ";
    string response;
    cin >> response;

    if (response == "yes") {
        // Accept all requests
        QueueNode* currentRequest = currentUser->friendRequests;
        while (currentRequest != nullptr) {
            // Extract sender's name from the notification
            string senderName = currentRequest->notification.substr(0, currentRequest->notification.find(" "));

            // Prevent self-connections
            if (senderName == currentUser->name) {
                cout << "You cannot accept a request from yourself.\n";
                currentRequest = currentRequest->next;
                continue;
            }

            // Locate sender in the graph
            GraphNode* temp = usersGraph;
            while (temp != nullptr && temp->user->name != senderName)
                temp = temp->next;

            if (temp) {
                // Add connections between the current user and sender
                GraphEdge* newEdge = new GraphEdge();
                newEdge->friendNode = temp;
                newEdge->relationType = "active";
                newEdge->next = currentUser->connections;
                currentUser->connections = newEdge;

                GraphEdge* reciprocalEdge = new GraphEdge();
                reciprocalEdge->friendNode = usersGraph; // Current user's graph node
                reciprocalEdge->relationType = "active";
                reciprocalEdge->next = temp->user->connections;
                temp->user->connections = reciprocalEdge;

                // Add notification to the sender
                addNotification(temp->user, currentUser->name + " accepted your friend request.");

                cout << "Accepted friend request from " << senderName << ".\n";
            }

            // Move to the next request
            QueueNode* toDelete = currentRequest;
            currentRequest = currentRequest->next;
            delete toDelete;
        }
        // Clear all friend requests after accepting
        currentUser->friendRequests = nullptr;
    }
    else {
        // Allow user to accept specific requests
        cout << "Enter the number of the request you want to accept (or 0 to cancel): ";
        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "No requests accepted.\n";
            return;
        }

        // Find the selected request
        QueueNode* selectedRequest = currentUser->friendRequests;
        QueueNode* prevRequest = nullptr;
        int count = 1;

        while (selectedRequest != nullptr && count < choice) {
            prevRequest = selectedRequest;
            selectedRequest = selectedRequest->next;
            count++;
        }

        if (selectedRequest == nullptr) {
            cout << "Invalid choice. No request accepted.\n";
            return;
        }

        // Extract sender's name from the notification
        string senderName = selectedRequest->notification.substr(0, selectedRequest->notification.find(" "));

        // Prevent self-connections
        if (senderName == currentUser->name) {
            cout << "You cannot accept a request from yourself.\n";
            return;
        }

        // Locate sender in the graph
        GraphNode* temp = usersGraph;
        while (temp != nullptr && temp->user->name != senderName)
            temp = temp->next;

        if (temp) {
            // Add connections between the current user and sender
            GraphEdge* newEdge = new GraphEdge();
            newEdge->friendNode = temp;
            newEdge->relationType = "active";
            newEdge->next = currentUser->connections;
            currentUser->connections = newEdge;

            GraphEdge* reciprocalEdge = new GraphEdge();
            reciprocalEdge->friendNode = usersGraph; // Current user's graph node
            reciprocalEdge->relationType = "active";
            reciprocalEdge->next = temp->user->connections;
            temp->user->connections = reciprocalEdge;

            // Add notification to the sender
            addNotification(temp->user, currentUser->name + " accepted your friend request.");

            cout << "Accepted friend request from " << senderName << ".\n";
        }

        // Remove the selected request from the queue
        if (prevRequest != nullptr) {
            prevRequest->next = selectedRequest->next;
        }
        else {
            currentUser->friendRequests = selectedRequest->next;
        }

        delete selectedRequest;
    }
}










void viewTimeline(User* currentUser) {

    if (!currentUser) {
        cout << "Please log in to view your newsfeed.\n";
        return;
    }

    if (currentUser->connections == nullptr) {
        cout << "No friends' posts to display.\n";
        return;
    }

    cout << "Your Newsfeed:\n";
    GraphEdge* connection = currentUser->connections;
    bool hasPosts = false;

    while (connection != nullptr) {
        if (connection->relationType == "active") {
            PostNode* post = connection->friendNode->user->posts;
            while (post != nullptr) {
                cout << " -" << connection->friendNode->user->name << " - " << post->timestamp;
                cout << post->content << "\n";
                post = post->next;
                hasPosts = true;
            }
        }
        connection = connection->next;
    }

    if (!hasPosts) {
        cout << "No posts to display in your Timeline.\n";
    }


    //////////////////////////////////
  
}






void viewNotifications(User* currentUser) {
    if (!currentUser) {
        cout << "Please log in to view notifications.\n";
        return;
    }

    if (currentUser->notifications == nullptr) {
        cout << "No new notifications.\n";
        return;
    }

    cout << "Notifications for " << currentUser->name << ":\n";

    // Traverse the queue and display notifications
    QueueNode* temp = currentUser->notifications;
    while (temp != nullptr) {
        cout << "- " << temp->notification << "\n";
        temp = temp->next;
    }

    // Clear the notification queue
    while (currentUser->notifications != nullptr) {
        QueueNode* toDelete = currentUser->notifications;
        currentUser->notifications = currentUser->notifications->next;
        delete toDelete;
    }

    cout << "All notifications cleared.\n";
}


// Function to add notifications
void addNotification(User* targetUser, const string& message) {
    if (!targetUser) {
        cout << "Target user does not exist.\n";
        return;
    }

    // Create a new notification node
    QueueNode* newNotification = new QueueNode();
    newNotification->notification = message;
    newNotification->next = nullptr;

    // Add the notification to the end of the queue
    if (targetUser->notifications == nullptr) {
        targetUser->notifications = newNotification;
    }
    else {
        QueueNode* temp = targetUser->notifications;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNotification;
    }

    cout << "Notification added for " << targetUser->name << ".\n";
}






void sendMessage(User* currentUser, GraphNode* usersGraph) {
    string friendName, messageContent;
    cout << "Enter the username to send a message: ";
    cin >> friendName;

    // Find the friend in the graph
    GraphNode* temp = usersGraph;
    while (temp != nullptr && temp->user->name != friendName)
        temp = temp->next;

    if (temp == nullptr) {
        cout << "User not found.\n";
        return;
    }

    // Check if the user is blocked
    GraphEdge* connection = currentUser->connections;
    while (connection != nullptr) {
        if (connection->friendNode->user->name == friendName) {
            if (connection->relationType == "blocked") {
                cout << "Cannot send a message. You have blocked this user or they have blocked you.\n";
                return;
            }
            break;
        }
        connection = connection->next;
    }

    // Check for no connection
    if (connection == nullptr || connection->relationType == "pending") {
        // Search the one-time sender list
        OneTimeMessageSender* sender = temp->user->oneTimeSenders;
        while (sender != nullptr) {
            if (sender->senderName == currentUser->name) {
                cout << "You can only send one message to this user without a connection.\n";
                return;
            }
            sender = sender->next;
        }

        // Allow sending a one-time message
        cout << "You are not connected to this user. This message will be sent as a one-time message.\n";
        cout << "Enter your message: ";
        cin.ignore();
        getline(cin, messageContent);

        // Add the message to the friend's stack
        MessageNode* newMessage = new MessageNode();
        newMessage->message = currentUser->name + ": " + messageContent;
        newMessage->next = temp->user->messages;
        temp->user->messages = newMessage;

        // Add current user to the friend's one-time sender list
        OneTimeMessageSender* newSender = new OneTimeMessageSender();
        newSender->senderName = currentUser->name;
        newSender->next = temp->user->oneTimeSenders;
        temp->user->oneTimeSenders = newSender;

        // Add a notification for the receiver
        addNotification(temp->user, "One-time message received from " + currentUser->name + ".");

        cout << "One-time message sent to " << friendName << ".\n";
        return;
    }

    // Normal messaging for connected users
    cout << "Enter your message: ";
    cin.ignore();
    getline(cin, messageContent);

    // Add the message to the friend's stack
    MessageNode* newMessage = new MessageNode();
    newMessage->message = currentUser->name + ": " + messageContent;
    newMessage->next = temp->user->messages;
    temp->user->messages = newMessage;

    // Add a notification for the receiver
    addNotification(temp->user, "New message received from " + currentUser->name + ".");

    cout << "Message sent to " << friendName << ".\n";
}







void viewMessages(User* currentUser) {
    if (currentUser->messages == nullptr) {
        cout << "No messages to display.\n";
        return;
    }

    cout << "Your Messages:\n";
    MessageNode* message = currentUser->messages;
    while (message != nullptr) {
        cout << "- " << message->message << "\n";
        message = message->next;
    }
}






void searchUsers(BSTNode* userSearchTree, User* currentUser) {
    if (currentUser == nullptr) {
        cout << "Please log in to search for users.\n";
        return;
    }

    string username;
    cout << "Enter username to search: ";
    cin >> username;

    BSTNode* current = userSearchTree;
    while (current != nullptr) {
        if (current->username == username) {
            cout << "User found: " << username << "\n";

            // Check connection status with the current user
            GraphEdge* connection = currentUser->connections;
            bool foundStatus = false;

            while (connection != nullptr) {
                if (connection->friendNode->user->name == username) {
                    foundStatus = true;
                    string relation = connection->relationType;

                    if (relation == "active") {
                        cout << "Status: Friend\n";
                    }
                    else if (relation == "blocked") {
                        cout << "Status: Blocked\n";
                    }
                    else if (relation == "pending") {
                        cout << "Status: Pending Request\n";
                    }
                    else {
                        cout << "Status: " << relation << "\n";
                    }
                    break;
                }
                connection = connection->next;
            }

            if (!foundStatus) {
                cout << "Status: No connection\n";
            }

            return;
        }

        // Navigate the BST
        current = (username < current->username) ? current->left : current->right;
    }

    cout << "User not found.\n";
}






void displayFollowers(User* currentUser) {
    if (!currentUser) {
        cout << "Please log in to view your followers.\n";
        return;
    }

    if (currentUser->connections == nullptr) {
        cout << "You have no followers.\n";
        return;
    }

    GraphEdge* connection = currentUser->connections;
    GraphEdge* activeConnections = nullptr; // Temporary list for active followers
    GraphEdge* blockedConnections = nullptr; // Temporary list for blocked followers

    // Separate active and blocked followers
    while (connection != nullptr) {
        if (connection->relationType == "active") {
            GraphEdge* temp = new GraphEdge(*connection);
            temp->next = activeConnections;
            activeConnections = temp;
        }
        else if (connection->relationType == "blocked") {
            GraphEdge* temp = new GraphEdge(*connection);
            temp->next = blockedConnections;
            blockedConnections = temp;
        }
        connection = connection->next;
    }

    // Display active followers
    cout << "Your Active Followers:\n";
    if (activeConnections == nullptr) {
        cout << "No active followers.\n";
    }
    else {
        int index = 1;
        GraphEdge* temp = activeConnections;
        while (temp != nullptr) {
            User* follower = temp->friendNode->user;
            cout << index++ << ". " << follower->name;

            if (!follower->lastLoginTimestamp.empty()) {
                cout << " -- Last Login: " << follower->lastLoginTimestamp;
            }
            else {
                cout << " (Last Login: Unknown)";
            }
            cout << "\n";
            temp = temp->next;
        }
    }

    // Display blocked followers
    cout << "\nYour Blocked Followers:\n";
    if (blockedConnections == nullptr) {
        cout << "No blocked followers.\n";
    }
    else {
        int index = 1;
        GraphEdge* temp = blockedConnections;
        while (temp != nullptr) {
            User* follower = temp->friendNode->user;
            cout << index++ << ". " << follower->name << "\n";
            temp = temp->next;
        }
    }

    // Menu for blocking/unblocking
    int choice;
    while (true) {
        cout << "\nOptions:\n";
        cout << "1. Block an active follower\n";
        cout << "2. Unblock a blocked follower\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter the number of the follower to block: ";
            int num;
            cin >> num;

            // Block the chosen follower
            GraphEdge* temp = activeConnections;
            for (int i = 1; temp != nullptr; ++i, temp = temp->next) {
                if (i == num) {
                    temp->relationType = "blocked";
                    cout << "Blocked " << temp->friendNode->user->name << ".\n";

                    // Update both users' connections
                    GraphEdge* currentConn = currentUser->connections;
                    while (currentConn != nullptr) {
                        if (currentConn->friendNode->user == temp->friendNode->user) {
                            currentConn->relationType = "blocked";
                            break;
                        }
                        currentConn = currentConn->next;
                    }

                    GraphEdge* friendConn = temp->friendNode->user->connections;
                    while (friendConn != nullptr) {
                        if (friendConn->friendNode->user == currentUser) {
                            friendConn->relationType = "blocked";
                            break;
                        }
                        friendConn = friendConn->next;
                    }
                    break;
                }
            }
        }
        else if (choice == 2) {
            cout << "Enter the number of the follower to unblock: ";
            int num;
            cin >> num;

            // Unblock the chosen follower
            GraphEdge* temp = blockedConnections;
            for (int i = 1; temp != nullptr; ++i, temp = temp->next) {
                if (i == num) {
                    temp->relationType = "active";
                    cout << "Unblocked " << temp->friendNode->user->name << ".\n";

                    // Update both users' connections
                    GraphEdge* currentConn = currentUser->connections;
                    while (currentConn != nullptr) {
                        if (currentConn->friendNode->user == temp->friendNode->user) {
                            currentConn->relationType = "active";
                            break;
                        }
                        currentConn = currentConn->next;
                    }

                    GraphEdge* friendConn = temp->friendNode->user->connections;
                    while (friendConn != nullptr) {
                        if (friendConn->friendNode->user == currentUser) {
                            friendConn->relationType = "active";
                            break;
                        }
                        friendConn = friendConn->next;
                    }
                    break;
                }
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    // Cleanup temporary lists
    while (activeConnections != nullptr) {
        GraphEdge* toDelete = activeConnections;
        activeConnections = activeConnections->next;
        delete toDelete;
    }
    while (blockedConnections != nullptr) {
        GraphEdge* toDelete = blockedConnections;
        blockedConnections = blockedConnections->next;
        delete toDelete;
    }
}







void viewNewsfeed(User* currentUser) {
   



    ///////////////////////////////////

    if (currentUser->posts == nullptr) {
        cout << "No posts to display in your NewsFeed.\n";
        return;
    }

    cout << "Your NewsFeed:\n";
    PostNode* post = currentUser->posts;
    while (post != nullptr) {
        cout << " -" << post->timestamp;

        cout << post->content << "\n";
        post = post->next;
    }
}

void welcome() {
    SetConsoleTextAttribute(hConsole, 13); // 13 is for magenta/pink
    cout << "\n\n\n\n\n\n\n\n\n\n";
    cout << setw(100) << "  d8,                                                                               " << endl;
    Sleep(100);
    cout << setw(100) << " `8P                      d8P                                                     " << endl;
    Sleep(100);
    cout << setw(100) << "                       d888888P                                                   " << endl;
    Sleep(100);
    cout << setw(100) << "  88b  88bd88b  .d888b,  ?88'   d888b8b   d888b8b    88bd88b d888b8b    88bd8b,d88b" << endl;
    Sleep(100);
    cout << setw(100) << "  88P  88P' ?8b ?8b,     88P   d8P' ?88  d8P' ?88    88P'  `d8P' ?88    88P'`?8P'?8b" << endl;
    Sleep(100);
    cout << setw(100) << " d88  d88   88P   `?8b   88b   88b  ,88b 88b  ,88b  d88     88b  ,88b  d88  d88  88P" << endl;
    Sleep(100);
    cout << setw(100) << "d88' d88'   88b`?888P'   `?8b  `?88P'`88b`?88P'`88bd88'     `?88P'`88bd88' d88'  88b" << endl;
    Sleep(100);
    cout << setw(100) << "                                                )88                                 " << endl;
    Sleep(100);
    cout << setw(100) << "                                               ,88P                                 " << endl;
    Sleep(100);
    cout << setw(100) << "                                           `?8888P                                  " << endl;
    // Reset console text color to the default
    SetConsoleTextAttribute(hConsole, 7); // 7 is the default color (white on black)
    Sleep(3000);
    system("cls");
}