#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int data;
    struct node *next;
};
typedef struct node node;

struct message_node{
    int sender_id;
    int topic_id;
    int message_id;
    struct message_node *next;
};
typedef struct message_node message;

struct topic_node{
    char *topic_name;
    int topic_id;
    struct node *follower_list; /*hold user ID’s in data field*/
    struct topic_node *next;
    /*Add additional fields as needed*/
};
typedef struct topic_node topic;

struct user_node{
    char *user_name;
    int user_id;
    struct node *followed_topics; /*hold topic ID’s in data field*/
    struct message_node *inbox;
    /* Add additional fields as needed*/
    struct user_node *next;
};
typedef struct user_node user;

user *users = NULL;
topic *topics = NULL;
int nextUserId = 1;
int nextTopicId = 1;
int nextMsgId = 1;

/* Returns a pointer to the user with the given user name. */
user* findUser(char userName[21]) {
    user *temp_u = users;
    while(temp_u) {
        if (!strcmp(temp_u->user_name, userName))
            break;
        temp_u = temp_u->next;
    }
    return temp_u;
}

/* Returns a pointer to the topic with the given topic name. */
topic* findTopic(char topicName[21]) {
    topic *temp_t = topics;
    while (temp_t) {
        if (!strcmp(temp_t->topic_name, topicName))
            break;
        temp_t = temp_t->next;
    }
    return temp_t;
}

/* Adds a user with the given user name to the user list. */
void addUser(char* userName){
    user* newUser = malloc(sizeof(user));
    newUser->user_name = malloc(sizeof(char)*21);
    strcpy(newUser->user_name, userName);
    newUser->user_id = nextUserId++;
    newUser->followed_topics = NULL;
    newUser->inbox = NULL;
    newUser->next = NULL;

    if (!users){ /* User list is empty */
        users = newUser;
    } else { /* User list is not empty, add new user to the end. */
        user* temp;
        temp = users;
        while (temp->next)
            temp = temp->next;
        temp->next = newUser;
    }
}

/* Adds a new topic with the given topic name to topic list. */
void addTopic(char* topicName){
    topic* newTopic = malloc(sizeof(user));
    newTopic->topic_name = malloc(sizeof(char)*21);
    strcpy(newTopic->topic_name, topicName);
    newTopic->topic_id = nextTopicId++;
    newTopic->follower_list = NULL;
    newTopic->next = NULL;

    if (!topics){ /* User list is empty */
        topics = newTopic;
    } else { /* Not empty, add to the end. */
        topic* temp;
        temp = topics;
        while (temp->next)
            temp = temp->next;
        temp->next = newTopic;
    }
}

/**
 * Adds the message to user's inbox by creating a new message object.
 * usr: pointer to the user who will receive the message
 * mid: message id
 * sid: sender id
 * tid: topic id
 */
void addMessage(user* usr, int mid, int sid, int tid) {
    message *newMessage, *temp_m;
    newMessage = (message*) malloc(sizeof(message));
    newMessage->message_id = mid;
    newMessage->sender_id = sid;
    newMessage->topic_id = tid;
    newMessage->next = NULL;

    if (!(usr->inbox)) /* Inbox is empty */
        usr->inbox = newMessage;
    else { /* Inbox not empty, add the message to the end */
        temp_m = usr->inbox;
        while (temp_m) {
            if (temp_m->next == NULL) {
                temp_m->next = newMessage;
                break;
            }
            temp_m = temp_m->next;
        }
    }
}

/**
 * Deletes the node with the given data from the given list.
 * list: pointer to pointer to the first element in the list
 * data: data of the node to delete
 */
void deleteNode(node** list, int data) {
    if (*list == NULL) return;
    if ((*list)->data == data) { /* First element will be deleted */
        if ((*list)->next == NULL) { /* It is the only element in the list */
            free(*list);
            *list = NULL;
        }
        else { /* There are more elements in the list */
            node *temp = *list;
            *list = (*list)->next;
            free(temp);
        }
    }
    else { /* One of the later elements will be deleted */
        node *prev, *cur;
        prev = *list;
        if (prev->next == NULL) return;
        cur = prev->next;
        while (cur) {
            if (cur->data == data) { /* Found the node to delete */
                node *temp = cur;
                prev->next = cur->next;
                cur = cur->next;
                free(temp);
            }
            else { /* Not found yet, move on */
                prev = prev->next;
                cur = cur->next;
            }
        }
    }
}

/* Completely deletes the given list */
void deleteList(node **list) {
    if (*list == NULL) return;
    deleteList(&((*list)->next));
    free(*list);
    *list = NULL;
}

/* Deletes ALL the messages that the given user id has sent in the given list. */
void deleteMessagesOfUser(message **list, int userId) {
    message *cur = NULL, *prev = NULL;
    if (*list == NULL) return;
    /* Deletion of (possibly multiple) first elements */
    if ((*list)->sender_id == userId) { /* Head node will be deleted */
        while (1) {
            message *temp = *list;
            if ((*list)->next == NULL) { /* Only one element exits */
                free(temp);
                *list = NULL;
                return;
            }
            /* Next will be deleted as well */
            else if ((*list)->next->sender_id == userId){
                *list = (*list)->next;
                free(temp);
                continue;
            }
            else { /* Next will not be deleted */
                *list = (*list)->next;
                free(temp);
                prev = *list;
                cur = prev->next;
                break;
            }
        }
    }
    /* There is one element and it will not be deleted */
    else if ((*list)->next == NULL) return;
    else {
        prev = *list;
        cur = prev->next;
    }
    while (cur) {
        if (cur->sender_id == userId) { /* Node to delete */
            message *temp = cur;
            prev->next = cur->next;
            cur = cur->next;
            free(temp);
        }
        else { /* No delete, move on */
            prev = cur;
            cur = cur->next;
        }
    }
}

/* Deletes ALL the messages with the given topic id. */
void deleteMessagesOfTopic(message **list, int topicId) {
    message *cur = NULL, *prev = NULL;
    if (*list == NULL) return;
    /* Deletion of (possibly multiple) first elements */
    if ((*list)->topic_id == topicId) { /* Head node will be deleted */
        while (1) {
            message *temp = *list;
            if ((*list)->next == NULL) { /* Only one element exits */
                free(temp);
                *list = NULL;
                return;
            }
            /* Next will be deleted as well */
            else if ((*list)->next->topic_id == topicId){
                *list = (*list)->next;
                free(temp);
                continue;
            }
            else { /* Next will not be deleted */
                *list = (*list)->next;
                free(temp);
                prev = *list;
                cur = prev->next;
                break;
            }
        }
    }
    /* There is one element and it will not be deleted */
    else if ((*list)->next == NULL) return;
    else {
        prev = *list;
        cur = prev->next;
    }
    while (cur) {
        if (cur->topic_id == topicId) { /* Node to delete */
            message *temp = cur;
            prev->next = cur->next;
            cur = cur->next;
            free(temp);
        }
        else { /* No delete, move on */
            prev = cur;
            cur = cur->next;
        }
    }
}

/**
 * Deletes the user pointed by given pointer completely: user_name, inbox,
 * followed_topics are cleared. */
void deleteUser(user *usr) {
    deleteList(&(usr->followed_topics));
    deleteMessagesOfUser(&(usr->inbox), usr->user_id);
    free(usr->user_name);
    free(usr);
}

/* Prints messges of all users. Uses global list "users". */
void printAllMessages() {
    user *temp_u = users;
    while (temp_u) {
        message *temp_m = temp_u->inbox;
        printf("%d:", temp_u->user_id);
        while (temp_m) {
            printf(" %d", temp_m->message_id);
            temp_m = temp_m->next;
        }
        printf("\n");
        temp_u = temp_u->next;
    }
}

int main() {
    char input[21];

    while(scanf("%s ", input) > 0) { /* While unread input exists */
        if (!strcmp(input, "ADD")) { /* Command is add */
            scanf("%s", input); /* Read the name */
            addUser(input);
        }
        else if (!strcmp(input, "SUBSCRIBE")){ /* Command is subscribe */
            user *temp_u;
            topic *temp_t;
            node *newFollower, *newTopic;

            scanf("%s", input); /* Read the username */
            temp_u = findUser(input);

            scanf("%s", input); /* Read the topic name to subscribe */
            temp_t = findTopic(input);

            if (temp_t == NULL){ /* Topic not found, create a new topic */
                addTopic(input);
                temp_t = findTopic(input);
            }

            /* Add the topic id to followed topics list of user */
            newTopic = (node*) malloc(sizeof(node));
            newTopic->data = temp_t->topic_id;
            newTopic->next = NULL;
            if (temp_u->followed_topics == NULL)
                temp_u->followed_topics = newTopic;
            else {
                node *temp_n;
                temp_n = temp_u->followed_topics;
                while(temp_n) {
                    if (temp_n->next == NULL) {
                        temp_n->next = newTopic;
                        break;
                    }
                    temp_n = temp_n->next;
                }
            }

            /* Add the user id to follower list of the topic */
            newFollower = (node*) malloc(sizeof(node));
            newFollower->data = temp_u->user_id;
            newFollower->next = NULL;
            if (temp_t->follower_list == NULL){ /* Follower list is empty*/
                temp_t->follower_list = newFollower;
            }
            else { /* Add the follower node to the end */
                node *temp_n;
                temp_n = temp_t->follower_list;
                while(temp_n) {
                    if (temp_n->next == NULL){
                        temp_n->next = newFollower;
                        break;
                    }
                    temp_n = temp_n->next;
                }
            }
        }

        else if (!strcmp(input, "SEND")){ /* The command is send. */
            int senderId, topicId, messageId;
            user *temp_u;
            topic *temp_t;

            scanf("%s", input); /* Read the sender's username */
            temp_u = findUser(input);

            scanf("%s", input); /* Read the topic name */
            temp_t = findTopic(input);

            /* New message info */
            messageId = nextMsgId++;
            senderId = temp_u->user_id;
            topicId = temp_t->topic_id;

            /* Add the message to sender's inbox */
            addMessage(temp_u, messageId, senderId, topicId);

            /* Add the message to inbox of the users following the topic. */
            temp_u = users;
            while(temp_u != NULL){ /* Go through users to add the new message */
                node *temp_n;
                if (temp_u->user_id == senderId) {
                    temp_u = temp_u->next;
                    continue; /* Avoid duplicate message in sender's inbox */
                }

                /* Go through user's followed topics */
                temp_n = temp_u->followed_topics;
                while (temp_n != NULL){
                    if (temp_n->data == topicId) {
                        /* User follows the message's topic. */
                        addMessage(temp_u, messageId, senderId, topicId);
                        break;
                    }
                    temp_n = temp_n->next;
                }
                temp_u = temp_u->next; /* Continue with the next user */
            }
        }
        else if (!strcmp(input, "UNSUBSCRIBE")){ /* Command is unsubscribe. */
            user *temp_u;
            topic *temp_t;

            /* Find the user */
            scanf("%s", input); /* Read the username */
            temp_u = findUser(input);

            /* Find the topic */
            scanf("%s\n", input); /* Read the topic name */
            temp_t = findTopic(input);

            /* Remove user from the follower list */
            deleteNode(&(temp_t->follower_list), temp_u->user_id);

            /* Remove topic from user's followed_topics */
            deleteNode(&(temp_u->followed_topics), temp_t->topic_id);

            /* Remove the messages of the topic from user's inbox */
            deleteMessagesOfTopic(&(temp_u->inbox), temp_t->topic_id);
        }
        else if (!strcmp(input, "DELETEUSER")){ /* Command is delete */
            user *temp_u;
            int deletedUserId;

            scanf("%s", input); /* Read the username */
            temp_u = findUser(input); /* Find the user */
            deletedUserId = temp_u->user_id;

            if (users == NULL) continue;
            if (users->user_id == deletedUserId) { /* First user will be deleted */
                if (users->next == NULL) { /* It is the only user in the list */
                    deleteUser(users);
                    users = NULL;
                }
                else { /* There are more users in the list */
                    user *temp = users;
                    users = users->next;
                    deleteUser(temp);
                }
            }
            else { /* One of the later users will be deleted */
                user *prev, *cur;
                prev = users;
                if (prev->next == NULL) continue;
                cur = prev->next;
                while (cur) {
                    if (cur->user_id == deletedUserId) { /* Found the user to delete */
                        user *temp = cur;
                        prev->next = cur->next;
                        cur = cur->next;
                        deleteUser(temp);
                    }
                    else { /* Move on */
                        prev = prev->next;
                        cur = cur->next;
                    }
                }
            }

            /* Delete the messages of deleted user from other inboxes */
            temp_u = users;
            while (temp_u) {
                deleteMessagesOfUser(&(temp_u->inbox), deletedUserId);
                temp_u = temp_u->next;
            }
        }
    }

    printAllMessages();

    return 0;
}
