#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct TrieNode {
    struct TrieNode *children[128];
    int isEndOfWord;
    char *description;
};

void clearTerminal();
void pressEnterToContinue();
void displayTitle(int titleId);
int isValidSlangWord(char *word);
int isValidSlangDescription(char *description);
int isValidPrefix(char *prefix);
struct TrieNode *releaseMenu(struct TrieNode *root);
void searchMenu(struct TrieNode *root);
void viewMenu(struct TrieNode *root, int isViewPrefix);
struct TrieNode *exitMenu(struct TrieNode *root);
struct TrieNode *createNode();
struct TrieNode *insertWord(struct TrieNode *root, char *word, char *description);
struct TrieNode *searchWord(struct TrieNode *root, char *word);
void viewWordsWithPrefix(struct TrieNode *root, char *prefix, int *counter);
void viewAllWords(struct TrieNode *root, char *buffer, int depth, int *counter);
struct TrieNode *freeTrie(struct TrieNode *root);

int main() {
    struct TrieNode *root = NULL;

    while (1) {
        clearTerminal();
        displayTitle(0);
        printf("\n");
        printf("[1] Release a New Slang Word\n");
        printf("[2] Search a Slang Word\n");
        printf("[3] View All Slang Words Starting with a Certain Prefix Word\n");
        printf("[4] View All Slang Words\n");
        printf("[5] Exit\n");
        printf("\n");

        int userChoice;
        printf("Enter your choice to navigate [1-5]: ");
        scanf("%d", &userChoice); getchar();

        switch (userChoice) {
            case 1:
                root = releaseMenu(root);
                break;
            case 2:
                searchMenu(root);
                break;
            case 3:
                viewMenu(root, 1);
                break;
            case 4:
                viewMenu(root, 0);
                break;
            case 5:
                root = exitMenu(root);
                return 0;
            default:
                printf("Invalid choice, please try again.");
                pressEnterToContinue();
                break;
        }
    }
    
    return 0;
}

void clearTerminal() {
    system("cls"); // system("cls"); to clear the terminal in Windows OS.
}

void pressEnterToContinue() {
    printf("\nPress Enter to Continue...");
    getchar(); // getchar(); to waits for user to press Enter key.
}

void displayTitle(int titleId) {
    switch (titleId) {
        case 0:
            system("chcp 65001 > nul"); // system("chcp 65001 > nul"); to switch to UTF-8 encoding for box characters.
            printf("██████╗  ██████╗  ██████╗  ██████╗ ██╗     ███████╗\n");
            printf("██╔══██╗██╔═══██╗██╔═══██╗██╔════╝ ██║     ██╔════╝\n");
            printf("██████╔╝██║   ██║██║   ██║██║  ███╗██║     █████╗  \n");
            printf("██╔══██╗██║   ██║██║   ██║██║   ██║██║     ██╔══╝  \n");
            printf("██████╔╝╚██████╔╝╚██████╔╝╚██████╔╝███████╗███████╗\n");
            printf("╚═════╝  ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝╚══════╝\n");
            system("chcp 437 > nul"); // system("chcp 437 > nul"); to switch back to default encoding.
            break;
        case 1:
            system("chcp 65001 > nul");
            printf("████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗    ██╗   ██╗ ██████╗ ██╗   ██╗\n");
            printf("╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║ ██╔╝    ╚██╗ ██╔╝██╔═══██╗██║   ██║\n");
            printf("   ██║   ███████║███████║██╔██╗ ██║█████╔╝      ╚████╔╝ ██║   ██║██║   ██║\n");
            printf("   ██║   ██╔══██║██╔══██║██║╚██╗██║██╔═██╗       ╚██╔╝  ██║   ██║██║   ██║\n");
            printf("   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗       ██║   ╚██████╔╝╚██████╔╝\n");
            printf("   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗       ██║   ╚██████╔╝╚██████╔╝\n");
            printf("   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝       ╚═╝    ╚═════╝  ╚═════╝ \n");
            system("chcp 437 > nul");
            break;
        default:
            break;
    }
}

int isValidSlangWord(char *word) {
    // Check length of slang word, reject if the length is less than or equal to 1.
    if (strlen(word) <= 1) {
        printf("Slang word must be more than 1 character.\n");
        return 0;
    }

    // Check for spaces in the slang word, reject if it contains any.
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == ' ') {
            printf("Slang word cannot contain spaces.\n");
            return 0;
        }
    }

    // Check for valid characters (letters, hyphens, underscores), reject if it contains any symbols.
    for (int i = 0; word[i] != '\0'; i++) {
        if (!((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || (word[i] == '-') || (word[i] == '_'))) {
            printf("Slang word should only contain letters, hyphens, or underscores.\n");
            return 0;
        }
    }

    return 1;
}

int isValidSlangDescription(char *description) {
    // Check the description length of slang word, reject if the length is equal to 0 (empty).
    if (strlen(description) == 0) {
        printf("Slang word description cannot be empty.\n");
        return 0;
    }

    // Count the total word count of the description.
    int wordCount = 0, inWord = 0;
    for (int i = 0; description[i] != '\0'; i++) {
        if (description[i] == ' ' || description[i] == '\n') {
            inWord = 0;
        } else if (!inWord) {
            inWord = 1;
            wordCount++;
        }
    }
    
    // Check the word count of the description, reject if the word count is less than or equal to 2.
    if (wordCount <= 2) {
        printf("Slang word description must contain more than 2 words.\n");
        return 0;
    }

    return 1;
}

int isValidPrefix(char *prefix) {
    // Check the prefix word length, reject if the length is less than or equal to 1 (same as the minimum length of each slang word).
    if (strlen(prefix) <= 1) {
        printf("Prefix must be more than 1 character.\n");
        return 0;
    }

    // Check for spaces in the prefix word, reject if it contains any.
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (prefix[i] == ' ') {
            printf("Prefix cannot contain spaces.\n");
            return 0;
        }
    }

    return 1;
}

struct TrieNode *releaseMenu(struct TrieNode *root) {
    clearTerminal();
    displayTitle(0);
    printf("\n");

    char word[128], description[256];
    int isValid = 0;

    // Prompt the user to input a new slang word, repeat until the conditions (>1 character, no space) are met.
    while (!isValid) {
        printf("Enter the new slang word (must be more than 1 character and contains no space): ");
        scanf("%[^\n]", word); getchar();
        isValid = isValidSlangWord(word);
    }

    isValid = 0;

    // Prompt the user to input the description of new slang word, repeat until the condition (>2 words) is met.
    while (!isValid) {
        printf("Enter the new slang word description (must be more than 2 words): ");
        scanf("%[^\n]", description); getchar();
        isValid = isValidSlangDescription(description);
    }

    // Search for existing slang word; If exist (wordExist = 1), update the description of slang word; If not exist (wordExist = 0), insert a new slang word.
    struct TrieNode *wordExist = searchWord(root, word);
    if (wordExist) {
        free(wordExist->description);
        wordExist->description = (char *) malloc(strlen(description) + 1); // +1 length for NULL terminator (\0).
        strcpy(wordExist->description, description);
        printf("\n\"%s\" has been updated!", word);
    } else {
        root = insertWord(root, word, description);
        printf("\n\"%s\" has been released!", word);
    }

    pressEnterToContinue();
    return root;
}

void searchMenu(struct TrieNode *root) {
    clearTerminal();
    displayTitle(0);
    printf("\n");

    char word[128];
    int isValid = 0;

    // Prompt the user to input a slang word, repeat until the conditions (>1 character, no space) are met.
    while (!isValid) {
        printf("Enter the slang word to search (must be more than 1 character and contains no space): ");
        scanf("%[^\n]", word); getchar();
        isValid = isValidSlangWord(word);
    }

    // Search the requested slang word from user; If found (wordFound = 1), display slang word and description; If not found (wordFound = 0), display message.
    struct TrieNode *wordFound = searchWord(root, word);
    if (wordFound) {
        printf("\nSlang Word\t: %s\n", word);
        printf("Description\t: %s\n", wordFound->description);
    } else {
        printf("\nThere is no slang word \"%s\" in the dictionary.", word);
    }

    pressEnterToContinue();
}

void viewMenu(struct TrieNode *root, int isViewPrefix) {
    clearTerminal();
    displayTitle(0);
    printf("\n");

    // If isViewPrefix = 1, handle for option [3] View All Slang Words Starting with a Certain Prefix Word; If isViewPrefix = 0, handle for option [4] View All Slang Words.
    int counter = 1;
    if (isViewPrefix) {
        char prefix[128];
        int isValid = 0;

        // Prompt the user to input a prefix word, repeat until the conditions (>1 character, no space) are met (same as the condition of each slang word).
        while (!isValid) {
            printf("Enter the prefix word to view all slang words (must be more than 1 character and contains no space): ");
            scanf("%[^\n]", prefix); getchar();
            isValid = isValidPrefix(prefix);
        }

        printf("\nList of Slang Words with Prefix \"%s\" in The Dictionary:\n", prefix);
        viewWordsWithPrefix(root, prefix, &counter);
    } else {
        char buffer[128];
        printf("List of All Slang Words in The Dictionary:\n");
        viewAllWords(root, buffer, 0, &counter);
    }

    pressEnterToContinue();
}

struct TrieNode *exitMenu(struct TrieNode *root) {
    root = freeTrie(root);
    clearTerminal();
    displayTitle(1);
    printf("\n");
    return root;
}

struct TrieNode *createNode() {
    struct TrieNode *newNode = (struct TrieNode *) malloc(sizeof(struct TrieNode));
    newNode->isEndOfWord = 0;
    newNode->description = NULL;
    for (int i = 0; i < 128; i++) {
        newNode->children[i] = NULL;
    }

    return newNode;
}

struct TrieNode *insertWord(struct TrieNode *root, char *word, char *description) {
    if (!root) root = createNode();

    // Traverse the trie and create a new nodes (paths) for characters that don't exist.
    struct TrieNode *temp = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int arrayIndex = word[i];
        if (!temp->children[arrayIndex]) {
            temp->children[arrayIndex] = createNode();
        }
        temp = temp->children[arrayIndex];
    }
    
    temp->isEndOfWord = 1; // isEndOfWord = 1 to flag the current node as the end of a word.
    temp->description = (char *) malloc(strlen(description) + 1); // +1 length for NULL terminator (\0).
    strcpy(temp->description, description);

    return root;
}

struct TrieNode *searchWord(struct TrieNode *root, char *word) {
    if (!root) return NULL;

    // Traverse the trie by following the path for each character of the word.
    struct TrieNode *temp = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int arrayIndex = word[i];
        if (!temp->children[arrayIndex]) return NULL;
        temp = temp->children[arrayIndex];
    }
    
    // Return the node if it marks the end of a complete word; otherwise, return NULL (not found).
    return temp->isEndOfWord ? temp : NULL;
}

void viewWordsWithPrefix(struct TrieNode *root, char *prefix, int *counter) {
    if (!root) return;

    // Traverse the Trie to reach the node corresponding to the end of the given prefix word.
    struct TrieNode *temp = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int arrayIndex = prefix[i];
        if (!temp->children[arrayIndex] && temp->isEndOfWord) {
            printf("\nThere is no slang word with prefix \"%s\" in the dictionary.\n", prefix);
            return;
        }
        temp = temp->children[arrayIndex];
    }

    char buffer[128];
    strcpy(buffer, prefix);
    viewAllWords(temp, buffer, strlen(prefix), counter);
}

void viewAllWords(struct TrieNode *root, char *buffer, int depth, int *counter) {
    if (!root) {
        printf("There is no slang word (yet) in the dictionary.\n");
        return;
    };

    if (root->isEndOfWord) {
        buffer[depth] = '\0';
        printf("[%d] %s\n", (*counter)++, buffer);
    }

    for (int i = 0; i < 128; i++) {
        if (root->children[i]) {
            buffer[depth] = i;
            viewAllWords(root->children[i], buffer, depth + 1, counter);
        }
    }
}

struct TrieNode *freeTrie(struct TrieNode *root) {
    if (!root) return NULL;

    for (int i = 0; i < 128; i++) {
        if (root->children[i]) {
            root->children[i] = freeTrie(root->children[i]);
        }
    }

    if (root->description) free(root->description);
    free(root);
    return NULL;
}