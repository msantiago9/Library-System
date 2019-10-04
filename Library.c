/**
*Author Marjon Santiago, Rachel Abraham, and Oghenero Ekwevugbe
*Bonus Topic 2 - Library System
*Version E - See bottom for revisions
*Spring 2019
**/

/**
*Table of Contents:
*Lines 0-24:     Imports, Definitions, and Table of Contents
*Lines 26-43:    Structures
*Lines 45-50:    Global Variables
*Lines 52-187:   Utility Functions
*Lines 189-349:  Read and Write Functions (Marjon's Code)
*Lines 351-544:  Borrower Functions (Rachel's Code)
*Lines 546-921:  Librarian Functions (Oghenero's Code)
*Lines 924-1074: Main Function
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define small 100
#define large 10000

/** Structures **/

struct book{
    int BookID; //Holds the Book ID
    char Title[100]; //Holds the Book Title
    char Author[40]; //Holds the Book's Author
    char Possession[20]; //Holds the username of the borrower
    char DateOut[11]; //Holds the date checked out
    char DateIn[11]; //Holds the due date
};
struct user{
    int UserID; //Holds the User's ID
    char FirstName[20]; //Holds the User's First Name
    char LastName[20]; //Holds the User's Last Name
    char UserName[20]; //Holds the User's login username
    char Password[20]; //Holds the User's login password
    char Role; //Value of 'L' or 'B', for librarian or borrower
};

/** Global Variables **/

struct user Users[small]; //Contains all Users
struct book Books[small]; //Contains all Books
int userID,usersTailPointer,booksTailPointer; //Stores stuff--see their names
char role; //holds the current logged-in user's role

/** Utility Functions **/
/** Creates lines, visual separator **/
void spacer(){
    printf("------------------------\n");
}

/** Removes newline from a character. Used because fgets captures newline. **/
void removeNewLine(char array[]){
    array[strcspn(array, "\n")] = 0; //basically removes the \n
}

/** Prints a specific book. **/
void printBook(struct book Book){
    printf("\"%s\" by %s, ID: %d, Checked Out: %s, Due Date: %s", Book.Title,Book.Author,Book.BookID,Book.DateOut,Book.DateIn);
}

/** Prints a specific user. **/
void printUser(struct user User){
    char *role =(User.Role=='L'? "Librarian" : "Borrower");
    printf("%d. %s %s, %s, %s",User.UserID,User.FirstName,User.LastName,User.UserName,role);
}

/** Converts a given date into a number of days **/
int getDateInDays(char date[]){
    int today = 0,first,second;
    char year[4],month[2],day[2];

    /** date[] is assumed to be "YYYY-MM-DD**/
    /** the first four elements of date (YYYY) are stored into year[]**/
    year[0]=date[0]; //(2)019-04-28
    year[1]=date[1]; //2(0)19-04-28
    year[2]=date[2]; //20(1)9-04-28
    year[3]=date[3]; //201(9)-04-28

    /** converts years into days **/
    today+=(atoi(year)*365);

    /** stores MM into month[], and converts months into integers **/
    month[0]=date[5]; //2019-(0)4-28
    first=month[0]-'0';
    first*=10;
    month[1]=date[6]; //2019-0(4)-28
    second=month[1]-'0';

    int months = first + second;

    if(months>=2) //February
        today+=31;
    if(months>=3) //March
        today+=28;
    if(months>=4) //April
        today+=31;
    if(months>=5) //May
        today+=30;
    if(months>=6) //June
        today+=31;
    if(months>=7) //July
        today+=30;
    if(months>=8) //August
        today+=31;
    if(months>=9) //September
        today+=31;
    if(months>=10) //October
        today+=30;
    if(months>=11) //November
        today+=31;
    if(months>=12) //December
        today+=30;

    /** stores DD into day[], and converts days into integers **/
    day[0]=date[8]; //2019-04-(2)8
    first = day[0] - '0';
    first*=10;
    day[1]=date[9]; //2019-04-2(8)
    second = day[1] - '0';

    today+=(first + second);

    return today;
}

/** Performs the anti-operation of getDateIntDays **/
/** gets a string from an integer value **/
void getStringFromDays(int days, char date[]){
    int years, months;
    years = days/365;
    days%=365;
    if(days<=31){
        months=1;
    }
    else if(days<=59){
        months=2;
        days-=31;
    }
    else if(days<=90){
        months=3;
        days-=59;
    }
    else if(days<=120){
        months=4;
        days-=90;
    }
    else if(days<=151){
        months=5;
        days-=120;
    }
    else if(days<=181){
        months=6;
        days-=151;
    }
    else if(days<=212){
        months=7;
        days-=181;
    }
    else if(days<=243){
        months=8;
        days-=212;
    }
    else if(days<=273){
        months=9;
        days-=243;
    }
    else if(days<=304){
        months=10;
        days-=273;
    }
    else if(days<=334){
        months=11;
        days-=304;
    }
    else{
        months=12;
        days-=334;
    }
    sprintf(date,"%04d-%02d-%02d",years,months,days);
}

/** Read and Write Functions **/

/** Stores contents from UserInfo.txt and MyLibrary.txt into Users[] and Books[]**/
void load(struct book Books[], struct user Users[],int *first, int *second){
    FILE *UserInfoRead, *MyLibraryRead;
    char a[large],b[large];
    char c;
    int i,j,k;

    /** File Pointers to the text files**/
    UserInfoRead = fopen("UserInfo.txt","r");
    MyLibraryRead = fopen("MyLibrary.txt","r");

    /**Store contents of the file pointers into temp arrays**/
    /**Replaces NewLine Characters with commas**/
    i=0;
    while((c = fgetc(UserInfoRead)) != EOF){
        if(c!='\n'){
            a[i]=c;
        }
        else{
            a[i]=',';
        }
        i++;
    }

    i=0;
    while((c = fgetc(MyLibraryRead)) != EOF){
        if(c!='\n'){
            b[i]=c;
        }
        else{
            b[i]=',';
        }
        i++;
    }
    fclose(UserInfoRead);
    fclose(MyLibraryRead);


    /** Tokenizes the characters of the temporary arrays into strings **/
    /** Using comma as delimiters **/
    i=0;
    j=0;
    char* token2 = strtok(a,",");
    while (token2) {
        switch(i) {
        case 0:
            Users[j].UserID = atoi(token2);
            i++;
            break;
        case 1:
            strcpy(Users[j].FirstName,token2);
            i++;
            break;
        case 2:
            strcpy(Users[j].LastName,token2);
            i++;
            break;
        case 3:
            strcpy(Users[j].UserName,token2);
            i++;
            break;
        case 4:
            strcpy(Users[j].Password,token2);
            i++;
            break;
        case 5:
            Users[j].Role = token2[0];
            i=0;
            j++;
            break;
        }
        token2 = strtok(NULL, ",");
    }

    i=0;
    k=0;
    char* token1 = strtok(b,",");
    while (token1) {
        switch(i) {
        case 0:
            Books[k].BookID = atoi(token1);
            i++;
            break;
        case 1:
            strcpy(Books[k].Title,token1);
            i++;
            break;
        case 2:
            strcpy(Books[k].Author,token1);
            i++;
            break;
        case 3:
            strcpy(Books[k].Possession,token1);
            i++;
            break;
        case 4:
            strcpy(Books[k].DateOut,token1);
            i++;
            break;
        case 5:
            strcpy(Books[k].DateIn,token1);
            i=0;
            k++;
            break;
        }
        token1 = strtok(NULL, ",");
    }
    *first = j;
    *second = k;

}

/** Saves the contents of Users[] and Books[] into the text files again **/
void save(struct book Books[], struct user Users[], int first, int second,char role){

    /** There is no need to write back when logged in as borrower **/
    /** This is because no changes to the database occur when borrower **/
    if(role=='B'){
        exit(0);
    }

    int i;
    int j = first, k = second;
    FILE *UserWrite, *LibraryWrite;

    /** Opens pointers to the text files **/
    UserWrite = fopen("UserInfo.txt","w");
    LibraryWrite = fopen("MyLibrary.txt","w");

    /**Prints every line stored in the arrays**/
    for(i = 0; i < j; i++){
        fprintf(UserWrite,
                "%d,%s,%s,%s,%s,%c\n",
                Users[i].UserID,
                Users[i].FirstName,
                Users[i].LastName,
                Users[i].UserName,
                Users[i].Password,
                Users[i].Role);
    }
    for(i = 0; i < k; i++){
        fprintf(LibraryWrite,
                "%d,%s,%s,%s,%s,%s\n",
                Books[i].BookID,
                Books[i].Title,
                Books[i].Author,
                Books[i].Possession,
                Books[i].DateOut,
                Books[i].DateIn
                );

    }

    fflush(UserWrite);
    fflush(LibraryWrite);
    fclose(UserWrite);
    fclose(LibraryWrite);

}

/** Borrower Functions **/

/** Searches books by one specific author **/
void authorQuery(struct book Books[], int *first){

    fflush(stdin);
    struct book Queried[small];
    int k = *first;
    int i,j,l,min;
    char author[40],choice;

    /** Asks user to input the author's name **/
    /** The author's name is stored in char array author[] **/
    printf("Please enter the author.\n");
    fgets(author,40,stdin);
    removeNewLine(author);
    printf("Books written by: %s\n",author);

    /** If the user enters "everyone" or "all" then the system prints all books**/
    if(strcmp(author,"everyone")==0||strcmp(author,"all")==0){
        for(j=0;j<k;j++){
            Queried[j].BookID=Books[j].BookID;
            strcpy(Queried[j].Title,Books[j].Title);
            strcpy(Queried[j].Author,Books[j].Author);
            strcpy(Queried[j].Possession,Books[j].Possession);
            strcpy(Queried[j].DateOut,Books[j].DateOut);
            strcpy(Queried[j].DateIn,Books[j].DateIn);
        }
    }

    else{
        for(i=0,j=0; i<k; i++){

            /** Stores hits into a temporary array **/
            /** This array contains all books written by the author **/
            if(!strcmp(Books[i].Author,author)){
                Queried[j].BookID=Books[i].BookID;
                strcpy(Queried[j].Title,Books[i].Title);
                strcpy(Queried[j].Author,Books[i].Author);
                strcpy(Queried[j].Possession,Books[i].Possession);
                strcpy(Queried[j].DateOut,Books[i].DateOut);
                strcpy(Queried[j].DateIn,Books[i].DateIn);
                j++;
            }
        }
    }

    /** Selection Sort the books **/
    struct book temp;
    for(i=0; i<j; i++){
        min = i;
        for(l=i; l<j; l++){
            if(strcmp(Queried[l].Title,Queried[min].Title)<0){
                min=l;
            }
        }

        /** SWAP OPERATIONS **/
        temp.BookID = Queried[min].BookID;
        strcpy(temp.Title,Queried[min].Title);
        strcpy(temp.Author,Queried[min].Author);
        strcpy(temp.Possession,Queried[min].Possession);
        strcpy(temp.DateOut,Queried[min].DateOut);
        strcpy(temp.DateIn,Queried[min].DateIn);

        /** NOTHING REAL FANCY GOING ON HERE, JUST SWAPPING **/
        Queried[min].BookID = Queried[i].BookID;
        strcpy(Queried[min].Title,Queried[i].Title);
        strcpy(Queried[min].Author,Queried[i].Author);
        strcpy(Queried[min].Possession,Queried[i].Possession);
        strcpy(Queried[min].DateOut,Queried[i].DateOut);
        strcpy(Queried[min].DateIn,Queried[i].DateIn);

        /** FOR REAL, THIS JUST SWAPS TWO BOOKS JEEZ **/
        Queried[i].BookID = temp.BookID;
        strcpy(Queried[i].Title,temp.Title);
        strcpy(Queried[i].Author,temp.Author);
        strcpy(Queried[i].Possession,temp.Possession);
        strcpy(Queried[i].DateOut,temp.DateOut);
        strcpy(Queried[i].DateIn,temp.DateIn);
    }

    /** Prints the sorted result **/
    for(i=0; i<j; i++){
        printf("%d.%s (ID: %d)\n",i+1,Queried[i].Title,Queried[i].BookID);
    }

    /** Restart or quit query **/
    printf("\nTry again? (t try again or b to return.)\n");
    while(1){
        scanf("%c",&choice);
        if(choice=='t'){
            authorQuery(Books, first);
            break;
        }
        else if (choice=='b'){
            spacer();
            break;
        }
    }
}

/** Searches for a book by title **/
void titleQuery(struct book Books[], int *first){
    fflush(stdin);
    int k = *first;
    int i, found = 0;
    char choice;
    char title[100];

    /** Asks for the user to input the book title **/
    printf("Please Enter the title of the book.\n");
    fgets(title,50,stdin);
    removeNewLine(title);

    /** Prints the books found in the library **/
    /** Also prints whether they are checked out or not **/
    for(i=0; i<k; i++){
        if(strcmp(Books[i].Title,title)==0){
            printf("%s by %s, ",Books[i].Title,Books[i].Author);
            if(strcmp(Books[i].DateOut,"null")==0){
                printf("currently available in the library.\n");
            }
            else{
                printf("checked out by %s, Due back by %s\n",Books[i].Possession,Books[i].DateIn);
            }
            found++;
        }
    }

    /** True if no books were found with the title **/
    if(!found){
        printf("\nBook not found.");
    }

    /** Restart Query **/
    printf("\nTry again? (t try again or b to return.)\n");
    while(1){
        scanf("%c",&choice);
        if(choice=='t'){
            titleQuery(Books, first);
            break;
        }
        else if (choice=='b'){
            spacer();
            break;
        }
    }

}

/** Search for books checked out by the user **/
void userQuery(struct book Books[], struct user Users[], int *first, int currentUser){
    fflush(stdin);
    int j = *first;
    int i,today,due;
    char date[10],choice;

    /** Asks for the current date **/
    /** To check if a book is overdue **/
    printf("Please enter today's date. (YYYY-MM-DD Format)\n");
    fgets(date,10,stdin);
    removeNewLine(date);

    today = getDateInDays(date);
    due = 0;

    /** Prints all of the books checked out by the user **/
    for(i = 0; i < j; i++){
        if(strcmp(Books[i].Possession,Users[currentUser].UserName)==0){
                printBook(Books[i]);
                due = getDateInDays(Books[i].DateIn);
                if(today > due){
                    printf(" - Overdue");
                }
                printf("\n");
        }
    }

    /** Restart Query **/
    printf("\nTry again? (t try again or b to return.)\n");
    while(1){
        scanf("%c",&choice);
        if(choice=='t'){
            userQuery(Books, Users, first, currentUser);
            break;
        }
        else if (choice=='b'){
            spacer();
            break;
        }
    }

}

/** Librarian Functions **/

/** Adds a book to the Books[] array **/
void createBook(struct book Books[], int *numberOfBooks){

    fflush(stdin);

    int k = *numberOfBooks;

    /** Check if the library is full **/
    if(k>=small){
        printf("\nCannot add anymore books. increase the library size.");
        return;
    }

    char title[100],author[40],choice;

    Books[k].BookID = k+1;

    /** Assigns a title/author to the new book **/
    printf("Enter Title: ");
    fgets(title,50,stdin); //replaced scanf with fgets from Oghenero's code
    removeNewLine(title);
    strcpy(Books[k].Title,title);

    fflush(stdin);
    printf("Enter Author: ");
    fgets(author,40,stdin); //replaced scanf with fgets from Oghenero's code
    removeNewLine(author);
    strcpy(Books[k].Author,author);

    /** By default, the book is not checked out **/
    strcpy(Books[k].Possession,"library");
    strcpy(Books[k].DateOut,"null");
    strcpy(Books[k].DateIn,"null");

    k++;
    *numberOfBooks = k;

    printf("\nBook added successfully.");

    /** Restart Query **/
    printf("\nTry again? (t try again or b to return.)\n");
    while(1){
        scanf("%c",&choice);
        if(choice=='t'){
            createBook(Books, numberOfBooks);
            break;
        }
        else if (choice=='b'){
            spacer();
            break;
        }
    }
}

/** Removes a book from Books[]**/
void deleteBook(struct book Books[], int *numberOfBooks){

    fflush(stdin);

    int location = -1,k = *numberOfBooks, ID, i;
    char choice;

    /** Checks if there are no books in the library**/
    if(k<=0){
        printf("\nCannot delete--there are no books in the library.");
        return;
    }

    /** Asks for the ID of the book to be deleted.**/
    printf("Please Enter the ID of the book you would like to delete.\n");
    scanf("%d",&ID);

    /** Checks to see if any Book contains the id. **/
    for(i = 0; i < k; i++){
        if(Books[i].BookID==ID){
            location=i;
            break;
        }
    }

    if(location!=-1){
        printf("Deleted: \"%s\"",Books[location].Title);

        /** Shifts all of the books PAST the location to the left one. **/
        /** Example: Array - [a,b,c,d,e] **/
        /** Delete: b **/
        /**    O < < <
        /** [a,c,d,e,e]**/
        /** The last element repeats, we move the tail pointer to avoid it. **/
        for(i = location; i < k-1; i++){
            Books[i].BookID=Books[i+1].BookID;
            strcpy(Books[i].Title,Books[i+1].Title);
            strcpy(Books[i].Author,Books[i+1].Author);
            strcpy(Books[i].Possession,Books[i+1].Possession);
            strcpy(Books[i].DateOut,Books[i+1].DateOut);
            strcpy(Books[i].DateIn,Books[i+1].DateIn);
        }

        /** Updates the IDs of the books **/
        for(i = 0; i < k; i++){
            Books[i].BookID=i+1;
        }

        /** The number of books in the library decreases by one **/
        k--;
        *numberOfBooks=k;
    }
    else{
        printf("Not found. No such book!");
    }

    /** Restart Query **/
    printf("\nTry again? (t try again or b to return.)\n");
    while(1){
        scanf("%c",&choice);
        if(choice=='t'){
            deleteBook(Books, numberOfBooks);
            break;
        }
        else if (choice=='b'){
            spacer();
            break;
        }
    }
}

/** Assigns a Possession, DateOut, and DateIn to a Book **/
void checkOut(struct book Books[], struct user Users[], int *numberOfBooks){

    fflush(stdin);
    int i,count = 0,k=*numberOfBooks;
    char userName[20],date[11],choice;

    /** Asks for the username of the borrower. (Can also be a new user)**/
    printf("Enter the borrower's username.\n");
    fgets(userName,20,stdin);
    removeNewLine(userName);

    /** Checks if the user has already checked out three or more books **/
    for(i=0; i<k; i++){
        if(strcmp(Books[i].Possession,userName)==0){
            count++;
        }
    }

    if(count>=3){
        printf("\nThis user has checked out 3 or more books already.");
    }

    else{
        /** Asks for the ID of the Book to be checked out**/
        printf("\nEnter the ID of the Book.\n");

        scanf("%d",&i);
        int index = i-1;
        if(index>=k||index<0){
            printf("No such book!\n");
        }
        else{
            fflush(stdin);

            strcpy(Books[index].Possession,userName);

            /** Asks for today's date **/
            printf("\nEnter Today's Date (YYYY-MM-DD).\n");
            fgets(date,11,stdin);
            removeNewLine(date);
            strcpy(Books[index].DateOut,date);

            /** Assigns the due to 30 days from today **/
            getStringFromDays(getDateInDays(date)+30,date);
            strcpy(Books[index].DateIn,date);

            printf("\nCheck Out Successful.");
            printf("\nCheck Out Date: %s",Books[index].DateOut);
            printf("\nDue Date: %s",Books[index].DateIn);
        }
    }

    /** Restart Query **/
    printf("\nTry again? (t try again or b to return.)\n");
    while(1){
        scanf("%c",&choice);
        if(choice=='t'){
            checkOut(Books,Users,numberOfBooks);
            break;
        }
        else if (choice=='b'){
            spacer();
            break;
        }
    }

}

/** Returns a book's possession to library, DateIn to null, and DateOut to null **/
void returnBook(struct book Books[], int *numberOfBooks){
    int i,k = *numberOfBooks;
    char date[10],choice;

    /** Asks for the ID of the book to be checked back in **/
    printf("Enter the ID of the Book.\n");
    scanf("%d",&i);
    int index = i-1;
    if(index>=k||index<0){
        printf("No such book!\n");
    }

    else{
        fflush(stdin);

        /** Asks for today's date to see if it is overdue **/
        printf("Enter Today's Date (YYYY-MM-DD).\n");
        fgets(date,10,stdin);
        removeNewLine(date);

        int dueDate = getDateInDays(Books[index].DateIn);
        int returnDate = getDateInDays(date);

        /** The overdue fee is 10 cents per day, subject to change as per request by professor**/
        if(returnDate>dueDate){
            printf("\nBook is Overdue by %d days. The fee is $%2.2f (10 cents per day)",returnDate-dueDate,0.1*(returnDate-dueDate));
        }
        strcpy(Books[index].Possession,"library");
        strcpy(Books[index].DateOut,"null");
        strcpy(Books[index].DateIn,"null");
        printf("\nBook returned successfully!");
    }

    /** Restart Query **/
    printf("\nTry again? (t try again or b to return.)\n");
    while(1){
        scanf("%c",&choice);
        if(choice=='t'){
            returnBook(Books,numberOfBooks);
            break;
        }
        else if (choice=='b'){
            spacer();
            break;
        }
    }

}

/** Optional Feature: Add a new user to the UserInfo.txt **/
void createUser(struct user Users[], int *first){
    int j = *first,i,taken = 0;
    char firstName[20],lastName[20],userName[20],password[20],type;

    fflush(stdin);

    /** Creating a new user requires: First Name, Last Name, UserName, Password, and user type **/
    printf("Please Enter the user's first name.\n");
    fgets(firstName,20,stdin);
    removeNewLine(firstName);

    if(strcmp(firstName,"cancel")==0){
        spacer();
        return;
    }

    printf("Please Enter the user's last name.\n");
    fgets(lastName,20,stdin);
    removeNewLine(lastName);

    printf("Please Enter the desired Username.\n");
    fgets(userName,20,stdin);
    removeNewLine(userName);

    /** Checks if a UserName is taken **/
    for(i = 0; i < j; i++){
        if(strcmp(Users[i].UserName,userName)==0){
            taken = 1;
            break;
        }
    }

    if(!taken){
        printf("Please Enter the desired Password.\n");
        fgets(password,20,stdin);
        removeNewLine(password);

        printf("Please Enter the user's type, 'L' for Librarian, 'B' for Borrower.\n");
        fflush(stdin);
        scanf("%c",&type);

        Users[j].UserID=j+1;
        strcpy(Users[j].FirstName,firstName);
        strcpy(Users[j].LastName,lastName);
        strcpy(Users[j].UserName,userName);
        strcpy(Users[j].Password,password);
        Users[j].Role=type;

        printf("User has been successfully created.");
        j++;
        *first=j;
    }
    else{
        printf("UserName is already taken.\n");
    }

    /** Restart Query**/
    char choice;
    printf("\nTry again? (t try again or b to exit.)\n");
    while(1){
        scanf("%c",&choice);
        if(choice=='t'){
                spacer();
                createUser(Users,first);
                break;
        }
        else if (choice=='b'){
            spacer();
            break;
        }
    }
}

/** Logs in to a user listed in UserInfo.txt **/
void logIn(struct user Users[],int *UserOrder, int *first, char *userRole){

    fflush(stdin);

    /** u stores the UserName, p stores the Password **/
    char u[20],p[20];
    char role = ' ',choice;
    int i = 0, j = *first, success = 0;

    printf("Enter your Username: ");
    fgets(u,20,stdin);
    removeNewLine(u);

    fflush(stdin);

    printf("Enter your Password: ");
    fgets(p,20,stdin);
    removeNewLine(p);

    /** Checks to see if any of the users match the entered UserName and Password **/
    while(i<j){
        if(!strcmp(u,Users[i].UserName)&&!strcmp(p,Users[i].Password)){
            printf("\nSuccessfully logged into: %s %s",Users[i].FirstName,Users[i].LastName);
            role = Users[i].Role;
            *UserOrder = i;
            success++;
            break;
        }
        i++;
    }

    printf("\n");

    /** Restart Query **/
    if(!success){
        printf("Log-in Fail.");
        printf("\nTry again? (t try again or b to exit.)\n");
        while(1){
            scanf("%c",&choice);
            if(choice=='t'){
                spacer();
                logIn(Users,UserOrder,first,userRole);
                break;
            }
            else if (choice=='b'){
                exit(1);
            }
        }
    }

    else{
        /** Passes the User's Role to the global variable userRole **/
        *userRole=role;
    }
}

/** Main Function and Main Interface **/

/** Interaction with the system is in this function. **/
void enterMainInterface(struct book Books[], struct user Users[], int *i, int *j, int *k, char role){
    char choice;

    fflush(stdin);

    spacer();

    /** True if the User is a borrower **/
    if(role == 'B'){
        printf("Please enter an option:");
        printf("\n - l to list all books in the library");
        printf("\n - q to search for a specific author");
        printf("\n - s to search for a specific book title");
        printf("\n - u to search for books checked out by you");
        printf("\n - x to exit\n");
        while(1){

            /** Checks for a char input **/
            scanf("%c",&choice);
            if(choice=='l'){ //Marjon's Utility code - prints all books
                spacer();
                printf("ALL BOOKS:\n");
                int row;
                for(row=0; row < *k; row++){
                    printBook(Books[row]);
                    printf("\n");
                }
                spacer();
            }
            if(choice=='q'){ //Moved Rachel's code to authorQuery()
                spacer();
                authorQuery(Books,k);
            }
            if(choice=='s'){ //Moved Rachel's code to titleQuery()
                spacer();
                titleQuery(Books,k);
            }
            if(choice=='u'){ //Moved Rachel's code to userQuery()
                spacer();
                userQuery(Books,Users,j,*i);
            }
            if(choice=='x'){ //Rachel's exit(0) function call
                return;
            }
            printf("\nPlease enter another option:");
            printf("\n - l to list all books in the library");
            printf("\n - q to search for a specific author");
            printf("\n - s to search for a specific book title");
            printf("\n - u to search for books checked out by you");
            printf("\n - x to exit\n");
            fflush(stdin);
        }

    }

    /** True if a user is a Librarian **/
    else if(role == 'L'){
        printf("Please enter an option:");
        printf("\n - a to add a book");
        printf("\n - d to delete a book");
        printf("\n - k to print all users");
        printf("\n - l to list all books in the library");
        printf("\n - o to check-out a book");
        printf("\n - r to return a book");
        printf("\n - u to create a new user");
        printf("\n - x to exit\n");
        while(1){

            /** Checks for char input **/
            scanf("%c",&choice);
            if(choice=='a'){ //Moved Oghenero's code to createBooks()
                spacer();
                createBook(Books,k);
            }
            if(choice=='d'){ //Moved Oghenero's code to deleteBooks()
                spacer();
                deleteBook(Books,k);
            }
            if(choice=='k'){ //Marjon's Utility code - prints all users
                spacer();
                printf("ALL USERS:\n");
                int row;
                for(row=0; row < *j; row++){
                    printUser(Users[row]);
                    printf("\n");
                }
                spacer();
            }
            if(choice=='l'){ //Marjon's Utility code - prints all books
                spacer();
                printf("ALL BOOKS:\n");
                int row;
                for(row=0; row < *k; row++){
                    printBook(Books[row]);
                    printf("\n");
                }
                spacer();
            }
            if(choice=='o'){ //Moved Oghenero's code to checkOut()
                spacer();
                checkOut(Books,Users,k);
            }
            if(choice=='r'){ //Moved Oghenero's code to returnBook()
                spacer();
                returnBook(Books,k);
            }
            if(choice=='u'){ //Marjon's Utility code - adds new user
                spacer();
                createUser(Users,j);
            }
            if(choice=='x'){ //lines 104-107 on Oghenero's code
                return;
            }

            fflush(stdin);

            printf("Please enter another option:");
            printf("\n - a to add a book");
            printf("\n - d to delete a book");
            printf("\n - k to print all users");
            printf("\n - l to list all books in the library");
            printf("\n - o to check-out a book");
            printf("\n - r to return a book");
            printf("\n - u to create a new user");
            printf("\n - x to exit\n");
        }

    }
    /** Cannot interact with the system if there is a usertype error. **/
    else{
        printf("\nRole Unknown. %c", role);
        exit(2);
    }
}

/** Main Function **/
/** Consist of: **/
/** 1. Reading the Text Files **/
/** 2. Logging in **/
/** 3. Interacting with our program **/
/** 4. Saving any changes done to the database **/
int main(void){
    load(Books,Users,&usersTailPointer,&booksTailPointer);
    logIn(Users,&userID,&usersTailPointer,&role);
    enterMainInterface(Books,Users,&userID,&usersTailPointer,&booksTailPointer,role);
    save(Books,Users,usersTailPointer,booksTailPointer,role);
    return 0;
}

/**Concatenations
Rachel's code: Please see enterMainInterface(), lines 934-980.
Oghenero's code: Please see enterMainInterface(), lines 982-1052
**/
