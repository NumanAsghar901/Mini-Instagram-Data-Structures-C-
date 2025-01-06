#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <ctime>

using namespace std;


// Forward declaration of User structure
struct User;

// Graph structure for User Profiles
struct GraphNode {
    User* user;
    GraphNode* next;
};

struct GraphEdge {
    GraphNode* friendNode;
    string relationType; // active, blocked, pending
    GraphEdge* next;
};

// Linked List for Posts
struct PostNode {
    string content;
    string timestamp;
    PostNode* next;
};

// Queue for Friend Requests and Notifications
struct QueueNode {
    string notification;
    QueueNode* next;
};

// Stack for Messages
struct MessageNode {
    string message;
    MessageNode* next;
};

// Binary Search Tree for User Search
struct BSTNode {
    string username;
    User* user;
    BSTNode* left;
    BSTNode* right;
};
struct OneTimeMessageSender {
    string senderName;
    OneTimeMessageSender* next;
};

// User Profile
struct User {
    string name;
    string password;
    string city;
    string lastLoginTimestamp;
    PostNode* posts;
    QueueNode* friendRequests;
    QueueNode* notifications;
    string securityQuestion;
    string securityAnswer;
    GraphEdge* connections;
    MessageNode* messages;
    OneTimeMessageSender* oneTimeSenders;
};

void signup(GraphNode*& usersGraph, BSTNode*& userSearchTree);
User* login(GraphNode* usersGraph, BSTNode* userSearchTree);
void logout(User*& currentUser);
void followRequest(User* currentUser, GraphNode* usersGraph);
void cancelAcceptRequests(User* currentUser, GraphNode* usersGraph);
void viewTimeline(User* currentUser);
void viewNotifications(User* currentUser);
void sendMessage(User* currentUser, GraphNode* usersGraph);
void viewMessages(User* currentUser);
void searchUsers(BSTNode* userSearchTree, User* currentUser);
void displayFollowers(User* currentUser);
void viewNewsfeed(User* currentUser);
void addNotification(User* targetUser, const string& message);
void uploadPost(User* currentUser);
void welcome();
bool isPasswordStrong(const string& password);

