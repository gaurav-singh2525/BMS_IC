# Bank Management System (BMS)

A simple yet effective command-line bank management application built in C that allows users to create accounts, perform transactions, and manage their banking operations securely.

## Features

- **Account Management**: Create new bank accounts with secure password protection
- **Deposit & Withdrawal**: Perform deposit and withdrawal operations with real-time balance updates
- **Balance Inquiry**: Check account balance with password authentication
- **Transaction History**: View complete transaction history for any account
- **Money Transfer**: Transfer money between accounts with password verification
- **Admin Panel**: View all accounts and balances (requires admin password)
- **Secure Password Storage**: Passwords are hashed using a hash function for security
- **Data Persistence**: All account and transaction data is stored in binary files

## Project Structure

```
BMS/
├── accounts.dat          (Binary file storing account data)
├── transactions.dat      (Binary file storing transaction records)
└── bms.c                 (Main source code)
```

## Data Structures

### Account Structure
```c
typedef struct {
    int accountNumber;     // Unique account identifier
    char name[50];         // Account holder's name
    float balance;         // Current account balance
    char password[20];     // Hashed password
} Account;
```

### Transaction Structure
```c
typedef struct {
    int accountNumber;     // Associated account number
    char type[10];         // Transaction type (Deposit/Withdraw/Credit/Debit)
    float amount;          // Transaction amount
} Transaction;
```

## Installation & Setup

### Prerequisites
- GCC compiler or any C compiler
- Windows/Linux/MacOS operating system
- C standard library

### Compilation

```bash
gcc -o bms bms.c
```

### Running the Application

```bash
./bms
```

On Windows:
```bash
bms.exe
```

## Usage Guide

### 1. Create Account
- Select option **1** from the main menu
- Enter a unique account number
- Enter your full name
- Enter initial deposit amount
- Create a secure password (minimum 8 characters)
- Account is created successfully

### 2. Deposit Money
- Select option **2** from the main menu
- Enter your account number
- Enter the amount to deposit
- Balance is updated and transaction is recorded

### 3. Withdraw Money
- Select option **3** from the main menu
- Enter your account number
- Enter the amount to withdraw
- System validates sufficient balance before processing
- Balance is updated and transaction is recorded

### 4. Check Balance
- Select option **4** from the main menu
- Enter your account number
- Enter your password for authentication
- Current balance is displayed securely

### 5. View Transactions
- Select option **5** from the main menu
- Enter your account number
- Complete transaction history is displayed with transaction types and amounts

### 6. View All Accounts (Admin)
- Select option **6** from the main menu
- Enter admin password: **iitj1234**
- View all accounts with account numbers, names, and balances

### 7. Transfer Money
- Select option **7** from the main menu
- Enter your (sender) account number
- Enter your password for authentication
- Enter recipient's account number
- Enter transfer amount
- System validates password and sufficient balance
- Transfer is processed and both accounts are updated

### 8. Exit
- Select option **8** to exit the application

## Key Functions

| Function | Purpose |
|----------|---------|
| `hashPassword()` | Hashes passwords using a simple hash algorithm |
| `getPassword()` | Securely reads password input with masked characters |
| `accountExists()` | Checks if an account exists in the database |
| `recordTransaction()` | Records all transactions in transaction file |
| `createAccount()` | Creates a new bank account |
| `depositMoney()` | Handles deposit operations |
| `withdrawMoney()` | Handles withdrawal with balance validation |
| `checkBalance()` | Displays account balance with password verification |
| `viewTransactions()` | Displays transaction history for an account |
| `viewAllAccounts()` | Displays all accounts (admin only) |
| `transferMoney()` | Transfers money between accounts |

## Security Features

- **Password Hashing**: All passwords are hashed before storage
- **Password Masking**: Password input is masked with asterisks during entry
- **Authentication**: Balance checks and transfers require password verification
- **Admin Protection**: Admin functions are protected with a master password
- **Data Validation**: System validates transactions before execution

## File Format

### accounts.dat
Binary file containing Account structures with the following data:
- Account number (int)
- Name (50 characters)
- Balance (float)
- Hashed password (20 characters)

### transactions.dat
Binary file containing Transaction structures with the following data:
- Account number (int)
- Transaction type (10 characters)
- Amount (float)

## Important Configuration

The following constants can be modified in the source code:

```c
#define FILENAME "accounts.dat"          // Account data file
#define TRANSACTION_FILE "transactions.dat"  // Transaction data file
#define ADMIN_PASSWORD "iitj1234"        // Admin password
#define HASH_MULTIPLIER 31               // Hash function multiplier
```

## Limitations & Future Improvements

### Current Limitations
- Uses a simple hash function (not cryptographically secure)
- No encryption for stored data
- Command-line interface only
- No multi-user simultaneous access
- Password minimum length is 8 characters

### Suggested Improvements
- Implement stronger encryption algorithms
- Add database support (SQLite/MySQL)
- Create a GUI using libraries like GTK or Windows Forms
- Add transaction fees and interest calculations
- Implement account freezing and blocking features
- Add date/time stamps to transactions
- Multi-threading for concurrent access
- Input validation enhancements
- Backup and recovery mechanisms

## Troubleshooting

### "Error opening file"
- Ensure the program has write permissions in the current directory
- Check that disk space is available

### "Account not found"
- Verify the account number is correct
- Ensure the account exists before attempting operations

### "Incorrect password"
- Re-enter your password carefully
- Passwords are case-sensitive

### Files not being created
- Run the program from a directory where you have write permissions
- Check file system permissions

## License

This is an educational project created for learning C programming concepts including file handling, data structures, and basic banking operations.


## Support

For bugs, issues, or suggestions, please review the code and make necessary modifications. This is a learning project, and improvements are welcome.

---

**Note**: This system is for educational purposes only and should not be used in production environments without significant security enhancements and proper encryption protocols.
