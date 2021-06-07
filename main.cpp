#include <bits/stdc++.h>
using namespace std;
#define ALPHABET_SIZE 26
#define false 0
#define true 1
#define MAX_WORD_SIZE 500
#define MEANING_WORD_SIZE 10000
struct trienode
{
    struct trienode *children[ALPHABET_SIZE];
    bool end_of_word;
    string value;
};
int lettertoint(char ch)
{
    if (ch >= 'a' and ch <= 'z')
    {
        return ch - 'a';
    }
    else if (ch >= 'A' and ch <= 'Z')
    {
        return ch - 'A';
    }
    return -1;
}
struct trienode *getnode(void)
{
    struct trienode *pnode = new trienode;
    pnode->end_of_word = false;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        pnode->children[i] = NULL;
    }
    return pnode;
}
int insert(struct trienode *root, string word, string meaning)
{
    struct trienode *pcrawl = root;
    for (int i = 0; i < word.size(); i++)
    {
        int index = lettertoint(word[i]);
        if (!(pcrawl->children[index]))
        {
            pcrawl->children[index] = getnode();
        }
        pcrawl = pcrawl->children[index];
    }
    pcrawl->end_of_word = true;
    pcrawl->value = meaning;
    return 1;
}
int readwordsfromfile(struct trienode *root, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("could not find/open file \"%s\"\n", filename);
        return false;
    }
    char word[MAX_WORD_SIZE];
    char meaning[MEANING_WORD_SIZE];
    int count{};
    while (fscanf(file, "%s %[^\n]", word, meaning) > 1)
    {
        if (!(insert(root, word, meaning)))
        {
            fclose(file);
            return false;
        }
        else
        {
            ++count;
        }
    }
    fclose(file);
    cout << "Welcome To Our Dictionary. Our Dictionary Right Now Contains " << count << "Words ."
         << "\n";
    return 1;
}
bool search(struct trienode *root, string word)
{
    struct trienode *pcrawl = root;
    for (int i = 0; i < word.size(); i++)
    {
        int index = lettertoint(word[i]);
        if (!(pcrawl->children[index]))
        {
            return false;
        }
        pcrawl = pcrawl->children[index];
    }
    if (pcrawl != NULL and pcrawl->end_of_word)
    {
        cout << "The Meaning Of The Word Is: \n";
        cout << pcrawl->value;
    }
    return true;
}
bool deleteword(struct trienode *root, string word)
{
    struct trienode *pcrawl = root;
    for (int i = 0; i < word.size(); i++)
    {
        int index = lettertoint(word[i]);
        if (!(pcrawl->children[index]))
        {
            return false;
        }
        pcrawl = pcrawl->children[index];
    }
    if (pcrawl != NULL and pcrawl->end_of_word)
    {

        pcrawl->end_of_word = false;
    }
    return true;
}

void alphabeticalorder(struct trienode *root, char allwords[], int index)
{

    struct trienode *pcrawl = root;

    if (pcrawl != NULL && pcrawl->end_of_word == true)
    {
        for (int j = 0; j < index; j++)
        {
            cout << allwords[j];
        }

        cout << ": ";
        cout << pcrawl->value << "\n";
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (pcrawl->children[i] != NULL)
        {
            allwords[index] = 'a' + i;
            alphabeticalorder(pcrawl->children[i], allwords, index + 1);
        }
    }
}
void print_prefix_search(struct trienode *root, char allwords[], int index, string prefix)
{
    struct trienode *pcrawl = root;
    if (pcrawl != NULL and pcrawl->end_of_word == true)
    {
        cout << prefix;
        for (int i = 0; i < index; i++)
        {
            cout << allwords[i];
        }
        cout << " : ";
        cout << pcrawl->value << "\n";
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (pcrawl->children[i] != NULL)
        {
            allwords[index] = 'a' + i;
            print_prefix_search(pcrawl->children[i], allwords, index + 1, prefix);
        }
    }
}
void prefix_search(struct trienode *root, string prefix)
{
     struct trienode* pcrawl = root;

    for(int i = 0; i < prefix.size(); i++)    {
        int index = lettertoint(prefix[i]);
        pcrawl = pcrawl->children[index];
    }

    char allwords[50];
    print_prefix_search(pcrawl, allwords, 0, prefix);
}

bool searchsuggestedword(struct trienode *root, string word, int count)
{
    struct trienode *pcrawl = root;

    for (int i = 0; i < word.size(); i++)
    {
        int index = lettertoint(word[i]);

        if (!pcrawl->children[index])
            return false;

        pcrawl = pcrawl->children[index];
    }
    
    // cout << "Suggested words are: ";
        
    //     cout << word << "  ";

    if (pcrawl != NULL && pcrawl->end_of_word)
    {
       
        if (count== 0)
        {
            cout << "Suggested words are: ";
        cout << word << "  ";
        }
    return true;
    }
return false;
}

bool suggestedwords(struct trienode *root, string word, int number)
{
    int count = 0;

    for (int i = 0; i < word.size(); i++)
    {
        string temp = word;

        for (int j = 0; j < ALPHABET_SIZE; j++)
        {
            if (count < number && j != lettertoint(word[word.size() - i - 1]))
            {
                temp[word.size() - i - 1] = j + 97;

                if (searchsuggestedword(root, temp, count))
                {
                    count++;
                }
            }
        }
    }
    if (count == 0)
    {
        cout << "\nSorry We Couldn't Find Any Words Related To " << word << " In Our Dictionary.\n";
        return false;
    }
    else if (count < number)
    {
        cout << "\nWe Could Only Find " << count << " Words Related To " << word << " In Our Dictionary.\n";
        return true;
    }
    else
    {
        return true;
    }
}

int main()
{
    struct trienode *root = getnode();
    readwordsfromfile(root, "words.txt");
    string command;
    do
    {
        cout << "\nWhat would you like to do ?\n";
        cout << "1. Insert a word.\n";
        cout << "2. Search a word.\n";
        cout << "3. Delete a word.\n";
        cout << "4. Print dictionary in alphabetical order.\n";
        cout << "5. Prefix Search.\n";
        cout << "6. Show suggested words.\n";
        cout << "\n";
        // cin.ignore();

        int priority;
        cin >> priority;
        cout << "\n";

        string word;
        string meaning;

        switch (priority)
        {
        case 1:
            cout << "Enter the word you would want to insert : ";
            cin >> word;
            cout << "Enter its meaning : ";
            cin >> meaning;

            if (insert(root, word, meaning))
            {
                cout << "The word "
                     << "\'" << word << "\'"
                     << "has been inserted into the dictionary successfully.";
            }
            break;

        case 2:
            cout << "Enter the word you would like to search : ";
            cin >> word;
            if (!(search(root, word)))
            {
                cout << "Sorry, the word you searched for doesn't exist. Would you like to add it to the Dictionary.(Yes/No) ";
                string add;
                cin >> add;
                if (add == "YES" or add == "yes" or add == "Yes")
                {
                    cout << "Enter its meaning : ";
                    cin >> meaning;
                    if (insert(root, word, meaning))
                    {
                        cout << "The word "
                             << "\'" << word << "\'"
                             << "has been inserted into the dictionary successfully.";
                    }
                }
            }
            break;
        case 3:
            cout << "Which word would you like to delete? ";
            cin >> word;
            if (deleteword(root, word))
            {
                cout << "The word "
                     << "\'" << word << "\'"
                     << "has been successfully deleted from the dictionary.";
            }
            else
            {
                cout << "No such word exists in the Dictionary.\n";
            }
            break;
        case 4:
            char allwords[50];
            alphabeticalorder(root, allwords, 0);
            cout << "\n";

            break;
        case 5:
            cout << "Enter the word you would like to use as a prefix: ";
            cin >> word;
            cout << "\n";

            prefix_search(root, word);

            break;
        case 6:
            cout << "Enter the word for whose suggested words you want to see: ";
            cin >> word;
            int number;
            cout << "How many suggested words do you want to see: ";
            cin >> number;

            suggestedwords(root, word, number);

            break;

        default:
            cout << "Enter a valid entry.";
        }
        cin.ignore();
        cout << "\n\nWould you like to continue or exit?(Yes/No) ";
        cin >> command;

    } while (command == "YES" or command == "yes" or command == "Yes");
    cout << "Thanks for using our Dictionary.\n";
    return 0;
}
