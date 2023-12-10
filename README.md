# library-management-system

This is a simple Library Management System implemented in C++ that allows librarians to manage books, users, and fines efficiently. The system provides features for book issuance, return, renewal, and fine management.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Usage](#usage)
- [File Structure](#file-structure)
- [How to Run](#how-to-run)
- [Contributing](#contributing)
- [License](#license)

## Introduction<a name="introduction"></a>

This C++ project is a Library Management System designed to simplify the management of books, user details, and fines. It provides functionalities for librarians and users, allowing them to interact with the system seamlessly.

## Features<a name="features"></a>

- Librarian Options:
  - Issue Book
  - Collect Book
  - Renew Book
  - Fines Options
  - Books Options
  - Add User

- User Options:
  - Borrow Book
  - Return Book
  - Renew Book
  - Pay Fines
  - See All Available Books
  - Check Book Availability

## Usage<a name="usage"></a>

To use the Library Management System, make sure you have C++ compiler installed and follow these steps:

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/your-username/library-management-system.git
   cd library-management-system
   ```
2. Compile the C++ code:
   ```bash
   g++ main.cpp -o library_management_system
   ```
3. Run the compiled executable:
   ```bash
   ./library_management_system
   ```

## File Structure<a name="file-structure"></a>

| File | Description |
| ---- | ----------- |
| main.cpp | Contains the main code for the Library Management System. |
| all_books.txt | Stores information about all available books. |
| user_login_details.txt | Stores user login details. |
| admin_login_details.txt | Stores admin (librarian) login details. |
| user_all_details.txt | Stores details about all users. |
| issue_dates.txt | Stores book issue dates for users. |
| paid_fines.txt | Stores information about paid fines. |
