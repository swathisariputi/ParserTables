#include<bits/stdc++.h>
using namespace std;

#define TERMSIZE 128

// This array contains production strings.
vector<string> productions(20);

// parse_table[i][j] stores
// the index of production that must be applied on
// ith varible if the input is
// jth nonterminal
int parse_table[100][TERMSIZE];

// stores all list of terminals
// the ASCII value if use to index terminals
// terminal[i] = 1 means the character with
// ASCII value is a terminal
vector<char> terminal(TERMSIZE);

// stores all list of terminals
// only Upper case letters from 'A' to 'Z'
// can be nonterminals
// nonterminal[i] means ith alphabet is present as
// nonterminal is the grammar
vector<char> nonterminal(26);

int no_productions;
char first[26][TERMSIZE];
char follow[26][TERMSIZE];
// stores first of each production in form A->ß
char first_of_each_pro[100][TERMSIZE];

// check if the symbol is nonterminal
int isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

// Function to read data from the file
void readFile() {
    FILE* fptr;
    fptr = fopen("text.txt", "r");
    char buffer[255];
    int i;
    int j;
    int cnt=0;
    while (fgets(buffer, sizeof(buffer), fptr)) {
        if(cnt < 3){
            cnt++;
            continue;
        }
        if(buffer[0]=='#'&&buffer[1]=='#'){
            continue;
        }
        nonterminal[buffer[0] - 'A'] = 1;
        for (i = 0; i < strlen(buffer) - 1; ++i) {
            if (buffer[i] == '|') {
                ++no_productions;
                productions[no_productions].push_back(productions[no_productions - 1][0]);
                productions[no_productions].push_back(productions[no_productions - 1][1]);
                productions[no_productions].push_back(productions[no_productions - 1][2]);
                j = 3;
            }
            else {
                productions[no_productions].push_back(buffer[i]);
                ++j;
                if (!isNonTerminal(buffer[i]) && buffer[i] != '-' && buffer[i] != '>') {
                    terminal[buffer[i]] = 1;
                }
            }
        }
        no_productions++;
    }
}

void print_Productions(){
    for(int i = 0; i < no_productions; i++){
        cout << productions[i] << "\n";
    }
}

void add_FIRST_A_to_FIRST_B(char A, char B) {
    int i;
    for (i = 0; i < TERMSIZE; ++i) {
        if (i != '^') {
            first[B - 'A'][i] = first[A - 'A'][i] || first[B - 'A'][i];
        }
    }
}
// Function to Calculate First 
void FIRST() {
    int i, j;
    int t = 0;
    while (t < no_productions) {
        for (i = 0; i < no_productions; ++i) {
            int len = productions[i].length();
            for (j = 3; j < len; ++j) {
                char sc = productions[i][j];
                if (isNonTerminal(sc)) {
                    add_FIRST_A_to_FIRST_B(sc, productions[i][0]);
                    if (first[sc - 'A']['^'])
                        continue;
                }
                else {
                    first[productions[i][0] - 'A'][sc] = 1;
                }
                break;
            }
            if (j == len)
                first[productions[i][0] - 'A']['^'] = 1;
        }
        ++t;
    }
}

void print_First(){
    int i,j;
    cout << "\n";
    for (i = 0; i < no_productions; ++i) {
        if (i == 0 || (productions[i - 1][0] != productions[i][0])) {
            char c = productions[i][0];
            cout << "FIRST OF" <<" : " << c;
            for (j = 0; j < TERMSIZE; ++j) {
                if (first[c - 'A'][j]) {
                    printf("%c ", j);
                }
            }
            cout << "\n";
        }
    }
}

void add_FIRST_A_to_FOLLOW_B(char A, char B) {
    int i;
    for (i = 0; i < TERMSIZE; ++i) {
        if (i != '^')
            follow[B - 'A'][i] = follow[B - 'A'][i] || first[A - 'A'][i];
    }
}
void add_FOLLOW_A_to_FOLLOW_B(char A, char B) {
    int i;
    for (i = 0; i < TERMSIZE; ++i) {
        if (i != '^')
            follow[B - 'A'][i] = follow[B - 'A'][i] || follow[A - 'A'][i];
    }
}

// Function to calculate Follow
void FOLLOW() {
    follow[productions[0][0] - 'A']['$'] = 1;
    int t = 0;
    int i, j, k, x;
    while (t++ < no_productions) {
        for (k = 0; k < 26; ++k) {
            if (!nonterminal[k])    continue;
            char nt = k + 'A';
            for (i = 0; i < no_productions; ++i) {
                int len = productions[i].length();
                for (j = 3; j < len; ++j) {
                    if (nt == productions[i][j]) {
                        for (x = j + 1; x < len; ++x) {
                            char sc = productions[i][x];
                            if (isNonTerminal(sc)) {
                                add_FIRST_A_to_FOLLOW_B(sc, nt);
                                if (first[sc - 'A']['^'])
                                    continue;
                            }
                            else {
                                follow[nt - 'A'][sc] = 1;
                            }
                            break;
                        }
                        if (x == len)
                            add_FOLLOW_A_to_FOLLOW_B(productions[i][0], nt);
                    }
                }
            }
        }
    }
}

void print_Follow(){
    int i,j;
    cout << "\n";
    for (i = 0; i < no_productions; ++i) {
        if (i == 0 || (productions[i - 1][0] != productions[i][0])) {
            char c = productions[i][0];
            cout << "FOLLOW OF" << " : " << c;
            for (j = 0; j < TERMSIZE; ++j) {
                if (follow[c - 'A'][j]) {
                    printf("%c ", j);
                }
            }
            cout << "\n";
        }
    }
}

void add_FIRST_A_to_FIRST_of_each_pro__B(char A, int B) {
    int i;
    for (i = 0; i < TERMSIZE; ++i) {
        if (i != '^')
            first_of_each_pro[B][i] = first[A - 'A'][i] || first_of_each_pro[B][i];
    }
}
// Function to Calculate FIRST(B) for each A->B
void FIRST_of_each_pro() {
    int i, j;
    int t = 0;
    while (t < no_productions) {
        for (i = 0; i < no_productions; ++i) {
            int len = productions[i].length();
            for (j = 3; j < len; ++j) {
                char sc = productions[i][j];
                if (isNonTerminal(sc)) {
                    add_FIRST_A_to_FIRST_of_each_pro__B(sc, i);
                    if (first[sc - 'A']['^'])
                        continue;
                }
                else {
                    first_of_each_pro[i][sc] = 1;
                }
                break;
            }
            if (j == len)
                first_of_each_pro[i]['^'] = 1;
        }
        ++t;
    }
}
void print_Fisrt_of_each_pro() {
    cout << "\n";
    int i,j;
    for (i = 0; i < no_productions; ++i) {
        cout << "FIRST OF" << " : " <<  productions[i];
        for (j = 0; j < TERMSIZE; ++j) {
            if (first_of_each_pro[i][j]) {
                cout << (char)j << " ";
            }
        }
        cout << "\n";
    }
}

// Function to print Parse Table
void print_LL_parse_table(){
    // the parse parse_table contains '$'
    // set terminal['$'] = 1
    // to include '$' in the parse parse_table
    terminal['$'] = 1;
    
    // the parse parse_table do not read '^'
    // as input
    // so we set terminal['^'] = 0
    // to remove '^' from terminals
    terminal['^'] = 0;
    int i,j,k;
    // printing parse parse_table
    cout << "\n";
    cout << "\n\t**************** LL(1) PARSING parse_table *******************\n";
    cout << "\n";

    cout << "\t  ";
    for (i = 0; i < TERMSIZE; ++i) {
        if (terminal[i])    cout << (char)i << "\t  ";
    }
    cout << "\n";
    int p = 0;
    for (i = 0; i < no_productions; ++i) {
        if (i != 0 && (productions[i][0] != productions[i - 1][0]))
            p = p + 1;
        for (j = 0; j < TERMSIZE; ++j) {
            if (first_of_each_pro[i][j] && j != '^') {
                parse_table[p][j] = i + 1;
            }
            else if (first_of_each_pro[i]['^']) {
                for (k = 0; k < TERMSIZE; ++k) {
                    if (follow[productions[i][0] - 'A'][k]) {
                        parse_table[p][k] = i + 1;
                    }
                }
            }
        }
    }
    k = 0;
    for (i = 0; i < no_productions; ++i) {
        if (i == 0 || (productions[i - 1][0] != productions[i][0])) {
            cout << productions[i][0] << "\t";
            for (j = 0; j < TERMSIZE; ++j) {
                if (parse_table[k][j]) {
                    cout << productions[parse_table[k][j] - 1] << "    ";
                }
                else if (terminal[j]) {
                    cout << "    \t";
                }
            }
            ++k;
            cout << "\n";
        }
    }
}

// Function to Check acceptance of the string
bool accept_string(){
    string input;
    cin >> input;
    cout << "Given Input String" << input ;
    stack<char> left, right;
    left.push('$'); right.push('$');
    left.push(productions[0][0]);
    for(auto it=rbegin(input); it!=rend(input); it++){
        right.push(*it);
    }
    while(!left.empty() && !right.empty()){
        char l = left.top();
        char r = right.top();

        if(!isNonTerminal(l) && l != '^'){
            if(l != r) {
                string erroMsg = "Not";
                return false;
            }
            left.pop();
            right.pop();
        }
        else if(l == '$' && r == '$' && left.size()==1 && right.size()==1){
            left.pop();
            right.pop();
        }
        else {
            left.pop();
            int flag=0;
            for(int i = 0; i < 26; i++){
                if(parse_table[i][r]){
                    if(productions[parse_table[i][r]-1][0]==l){
                        flag=1;
                        string s = productions[parse_table[i][r]-1];
                        int lenth = productions[parse_table[i][r]-1].length();
                        if(lenth==4 && s[3]=='^'){
                            break;
                        }
                        for(int i = lenth-1; i >=3; i--){
                            left.push(s[i]);
                        }
                        break;
                    }
                }   
            }
            if(!flag)
            {
                return false;
            }
        }
    }
    if(left.empty() && right.empty()){
        return true;
    }
    else{
        return false;
    }
}
int main() {
    readFile();
    print_Productions(); // To print productions
    FIRST();   // To calculate First
    print_First();  // To print First
    FOLLOW();       // To Claculate Follow
    print_Follow(); // To print Follow
    FIRST_of_each_pro();    
    print_LL_parse_table();     // To print Parse Table
    bool answ = accept_string();
    if(answ)
        cout << "\n" << "STRING IS ACCEPTED" << "\n";
    else
        cout << "\n" << "STRING IS NOT ACCEPTED" << "\n";
}