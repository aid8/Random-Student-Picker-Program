/*
Title: Random Student Picker
Date Created: June 26, 2020
Description: A program that helps you pick students randomly
Author: Christian Dia
*/

#include<iostream>
#include<vector>
#include<fstream>

#ifdef _WIN32 
#define cls system("cls"); 
#elif linux
#define cls system("clear"); 
#endif

using namespace std;

int menu_size = 80;
char default_tag = '#';
int default_left_spaces = 4;
string title = "Random Student Picker";
vector<string> students = {}, backup_students = {};
bool exit_program = false;

void print_line(char tags = default_tag, char mid = '='){
    cout << tags << string(menu_size-2, mid) << tags << "\n";
}

void print_text_middle(string text, char tags = default_tag){
    int i, spaces = (menu_size - 2 - text.length())/2, is_odd = text.length() % 2 != 0; 
    cout << tags << string(spaces, ' ') << text << string(spaces + is_odd, ' ') 
    << tags << '\n';
}

void print_text_left(string text, char tags = default_tag, int spaces = default_left_spaces){
    cout << tags << string(spaces, ' ') << text << string(menu_size-text.length()-2-spaces, ' ')
    << tags << '\n';
}

void print_choices(vector<string> choices, char tags = default_tag){
    print_line(default_tag, ' ');
    for(int i = 0; i < choices.size(); ++i){
        print_text_left(to_string(i+1) + ". " + choices[i]);
    }
}

string print_student_num(int n){
    string count = to_string(n) + " student";
    if(n > 1) count += "s";
    return count;
}

int get_choice(int choices){
    int input;
    cout << "Enter choice: ";
    cin >> input;
    if(input > choices || input <= 0){
        input = -1;
    }
    return input;
}

void add_students_manually(){
    cin.ignore();

    do{
        cls;
        print_line();
        print_text_middle("Input Names of Students");
        print_line();
        print_line(default_tag, ' ');
        print_text_middle("Your Students");
        print_line(default_tag, ' ');

        if(students.size() == 0){
            print_text_left("No current students");
        }
        else{
            for(int i = 0; i < students.size(); ++i){
                print_text_left(to_string(i+1) + ". " + students[i]);
            }
        }
        print_line(default_tag, ' ');
        print_line();

        string name;
        cout << "Options:\n-e = to finish input\n-r = to remove student\n";
        cout << "Input name of student: ";
        getline(cin,name);

        if(name == "-e"){
            return;
        }
        else if(name == "-r"){
            cout << "Input student index number to remove: ";
            int index; 
            cin >> index;
            cin.ignore();
            if(index > 0 && index <= students.size()){
                students.erase(students.begin() + (index - 1));
            }
        }
        else{
            students.push_back(name);
        }
    }
    while(true);
}

void show_main_menu(){
    vector<bool> errors = {false};

    do{
        cls;
        print_line();
        print_text_middle(title);
        print_line();
        print_line(default_tag, ' ');

        print_text_middle("How do you want to load students file?");
        print_choices(vector<string>{"Load from a file (students.txt)",
        "Load from a file (Enter full path of file)",
        "Input names manually in console", "Exit"});
        print_line(default_tag, ' ');
        print_line();

        if(errors[0]) cout << "Invalid Choice, please try again!\n";
        if(errors[1]) cout << "Path not found, please try again!\n";

        int choice = get_choice(4);

        if(choice == -1){
            errors[0] = true;
            continue;
        }
        else{
            errors[0] = false;
            ifstream student_data;
            if(choice == 1){
                student_data.open("students.txt");
                string name;
                while(getline(student_data,name)){
                    students.push_back(name);
                }
                student_data.close();
            }
            else if(choice == 2){
                string path, name;
                cout << "Enter full path of the file: ";
                cin.ignore();
                getline(cin,path);
                student_data.open(path);
                if(student_data.fail()){
                    errors[1] = true;
                    cls;
                    continue;
                }
                while(getline(student_data,name)){
                    students.push_back(name);
                }
                student_data.close();
            }
            else if(choice == 3){
                add_students_manually();
            }
            else{
                exit_program = true;
            }
            return;
        }
    }
    while(true);
}

void pick_random_student(){
    backup_students = students;
    int i, count;
    bool start_picking = false;
    vector<bool> errors = {false};
    vector<string> picked_students = {};

    do{
        cls;
        print_line();
        print_text_middle("Picking Students");
        print_line();

        if(picked_students.size() > 0){
            print_line(default_tag, ' ');
            print_text_left(print_student_num(picked_students.size()) + " picked");
            print_line(default_tag, ' ');

            for(i = 0; i < picked_students.size(); ++i){
                print_text_left(to_string(i+1) + ". " + picked_students[i]);
            }

            print_line(default_tag, ' ');
            print_text_left(print_student_num(students.size()) + " left");
            print_line(default_tag, ' ');
            print_line();

            picked_students.clear();
        }

        if(students.size() > 0){
            if(errors[0]){
                cout << "Please input a valid number!\n";
                cout << "You still have " << print_student_num(students.size()) << " left.\n";
            }
            cout << "How many students do you want to pick? ";
            cin >> count;
            if(count > students.size() || count < 1){
                errors[0] = true;
                continue;
            }
            errors[0] = false;
        }
        else{
            cout << "Successfully picked all students!\n";
            cout << "Do you want to repick the students again? [y/n] ";
            char choice; 
            cin >> choice;
            if(choice == 'y'){
                students = backup_students;
                continue;
            }
            break;
        }
       
        int rand_pick;
        for(i = 0; i < count; ++i){
            rand_pick = rand() % students.size();
            picked_students.push_back(students[rand_pick]);
            students.erase(students.begin()+rand_pick);
        }
    }
    while(true);
}

int main(){
    //Randomize the seed
    srand(time(NULL));
    do{
        show_main_menu();
        if(exit_program) break;
        pick_random_student();
    }
    while(true);
    return 0;
}