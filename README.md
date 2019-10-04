# Library-System
C Project
Functionalities:
- Read and Write to Books and Users files
- Log-in System, basic String matching from file
- To main interfaces: for Users and Librarians
- Users can look up books by title, or by author
- Users can check if a book is in stock or not
- Librarians can add and remove users
- Librarians can check out books for users and set due dates
- Librarians can add and remove books from the library

Technical Challenges:
- Due Dates were the hardest part of the program actually. The task was to set the due date to 2 weeks from the date of check-out. A few things needed to be accommodated for, such as if the user checked out a book on February 28, a month would have to pass and an additional 14 days. To handle this problem, I converted the date into days and performed date calculations based on absolute days only.
- Determining how we would read from the text files was also a challenge. Using primitive char arrays would prove to be inefficient. For example, retrieving a Book's "ID" would be a monumental task to calculate. Instead, I used a struct called "Book" with certain members variables that are supposed to represent a certain aspect of the book.
- Struct also helped with keeping users organized. Accessing passwords and usernames was as simple as calling User[i].password or User[i].username. This was crucial because otherwise, we would have to create some temporary char arrays and we would have to constantly store and modify them. Instead, having an array of struct User makes things exceptionally easy, almost akin to Java's Class design.

Credits:
- Rachel Abraham for implementing the logic for the User side of the system
- Oghenero Ekwevugbe for implementing the logic for the Librarian side of the system
- Marjon Santiago (me) for implementing the I/O, the structure data type, and combining their code
