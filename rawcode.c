#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define FILENAME "accounts.dat"
#define TRANSACTION_FILE "transactions.dat"
#define ADMIN_PASSWORD "iitj1234"
#define HASH_MULTIPLIER 31

typedef struct
{
    int accountNumber;
    char name[50];
    float balance;
    char password[20];
} Account;

typedef struct
{
    int accountNumber;
    char type[10];
    float amount;
} Transaction;

void hashPassword(char *password, char *hashed)
{
    unsigned int hash = 0;
    while (*password != '\0')
    {
        hash = hash * HASH_MULTIPLIER + *password;
        password++;
    }
    sprintf(hashed, "%u", hash);
}
void getPassword(char *password, int minlength, int maxlength)
{
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int i = 0;
    char ch;

    while (i < maxlength - 1)
    {
        ch = getchar();

        if (ch == '\n')
        {
            if (i < minlength)
            {
                printf("\nPassword should be minimum %d characters\n", minlength);
                i = 0;
                continue;
            }
            break;
        }

        if (ch == 127) // backspace in Linux
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            password[i++] = ch;
            printf("*");
        }
    }

    password[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    printf("\n");
}

int accountExists(int accNum)
{
    Account acc;
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL)
    {
        printf("Error opening transaction file!\n");
        return 0;
    }
    while ((fread(&acc, sizeof(Account), 1, fp)) == 1)
    {
        if (acc.accountNumber == accNum)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void recordTransaction(int accNum, const char *type, float amount)
{
    Transaction trans = {accNum, "", amount};
    strcpy(trans.type, type);
    FILE *fp = fopen(TRANSACTION_FILE, "ab");
    if (fp == NULL)
    {
        printf("Error opening transaction file!\n");
        return;
    }
    fwrite(&trans, sizeof(Transaction), 1, fp);
    fclose(fp);
}

void createAccount()
{
    Account acc;
    char password[20];
    char hashed[20];
    printf("Enter account number: ");
    scanf("%d", &acc.accountNumber);
    printf("Enter name: ");
    scanf(" %[^\n]", acc.name);
    printf("Enter initial deposit: ");
    scanf("%f", &acc.balance);
    printf("Enter password: ");
    getPassword(password, 8, 20);
    if (accountExists(acc.accountNumber))
    {
        printf("Account already exists!\n");
        return;
    }

    hashPassword(password, hashed);
    strcpy(acc.password, hashed);

    FILE *fp = fopen(FILENAME, "ab");
    if (fp == NULL)
    {
        printf("Error saving account!\n");
        return;
    }
    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);

    recordTransaction(acc.accountNumber, "Deposit", acc.balance);
    printf("Account created successfully!\n");
}

void depositMoney()
{
    int accNum;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &accNum);
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    FILE *fp = fopen(FILENAME, "rb+");
    if (fp == NULL)
    {
        printf("Error accessing account file!\n");
        return;
    }

    Account acc;
    int found = 0;
    while ((fread(&acc, sizeof(Account), 1, fp)) == 1)
    {
        if (acc.accountNumber == accNum)
        {
            found = 1;
            acc.balance += amount;
            fseek(fp, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);
            break;
        }
    }
    fclose(fp);

    if (found == 1)
    {
        recordTransaction(accNum, "Deposit", amount);
        printf("Deposit successful. New balance: %.2f\n", acc.balance);
    }
    else
    {
        printf("Account not found.\n");
    }
}

void withdrawMoney()
{
    int accNum;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &accNum);
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    FILE *fp = fopen(FILENAME, "rb+");
    if (fp == NULL)
    {
        printf("Error accessing account file!\n");
        return;
    }

    Account acc;
    int found = 0;
    while ((fread(&acc, sizeof(Account), 1, fp)) == 1)
    {
        if (acc.accountNumber == accNum)
        {
            found = 1;
            if (acc.balance < amount)
            {
                printf("Insufficient balance.\n");
                fclose(fp);
                return;
            }
            acc.balance -= amount;
            fseek(fp, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);
            break;
        }
    }
    fclose(fp);

    if (found == 1)
    {
        recordTransaction(accNum, "Withdraw", amount);
        printf("Withdrawal successful. New balance: %.2f\n", acc.balance);
    }
    else
    {
        printf("Account not found.\n");
    }
}

void checkBalance()
{
    int accNum;
    char password[20], hashed[20];
    printf("Enter account number: ");
    scanf("%d", &accNum);
    printf("Enter password: ");
    getPassword(password, 8, 20);

    hashPassword(password, hashed);

    FILE *fp = fopen(FILENAME, "rb");
    if (!fp)
    {
        printf("Error accessing file!\n");
        return;
    }

    Account acc;
    int found = 0;
    while ((fread(&acc, sizeof(Account), 1, fp)) == 1)
    {
        if (acc.accountNumber == accNum)
        {
            found = 1;
            if (strcmp(acc.password, hashed) == 0)
            {
                printf("Current balance: %.2f\n", acc.balance);
            }
            else
            {
                printf("Incorrect password.\n");
            }
            break;
        }
    }
    fclose(fp);

    if (found == 0)
        printf("Account not found.\n");
}

void viewTransactions()
{
    int accNum;
    printf("Enter account number: ");
    scanf("%d", &accNum);

    FILE *fp = fopen(TRANSACTION_FILE, "rb");
    if (fp == NULL)
    {
        printf("No transaction history found.\n");
        return;
    }

    Transaction trans;
    int found = 0;
    printf("Transaction History for Account #%d:\n", accNum);
    printf("Type       Amount\n-------------------\n");
    while (fread(&trans, sizeof(Transaction), 1, fp))
    {
        if (trans.accountNumber == accNum)
        {
            found = 1;
            printf("%-10s %.2f\n", trans.type, trans.amount);
        }
    }
    fclose(fp);
    if (!found)
    {
        printf("No transactions found.\n");
    }
}

void viewAllAccounts()
{
    char password[20];
    printf("Enter admin password: ");
    getPassword(password, 8, 20);
    if (strcmp(password, ADMIN_PASSWORD) != 0)
    {
        printf("Incorrect password.\n");
        return;
    }

    FILE *fp = fopen(FILENAME, "rb");
    if (!fp)
    {
        printf("No accounts found.\n");
        return;
    }

    Account acc;
    printf("Account Number | Name           | Balance\n");
    printf("-------------------------------------------\n");
    while (fread(&acc, sizeof(Account), 1, fp))
    {
        printf("%-15d %-15s %.2f\n", acc.accountNumber, acc.name, acc.balance);
    }
    fclose(fp);
}

void transferMoney()
{
    int fromAcc, toAcc;
    float amount;
    char password[20], hashed[20];

    printf("Enter your (sender) account number: ");
    scanf("%d", &fromAcc);
    printf("Enter your password: ");
    getPassword(password, 8, 20);
    printf("Enter recipient's account number: ");
    scanf("%d", &toAcc);
    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    if (fromAcc == toAcc)
    {
        printf("Cannot transfer to the same account.\n");
        return;
    }

    hashPassword(password, hashed);

    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp)
    {
        printf("Error accessing account file.\n");
        return;
    }

    Account acc;
    Account sender, receiver;
    int senderFound = 0, receiverFound = 0;
    long senderPos = -1, receiverPos = -1;
    long pos = 0;

    while (fread(&acc, sizeof(Account), 1, fp))
    {
        if (acc.accountNumber == fromAcc)
        {
            if (strcmp(acc.password, hashed) != 0)
            {
                printf("Incorrect password.\n");
                fclose(fp);
                return;
            }
            sender = acc;
            senderFound = 1;
            senderPos = pos;
        }
        if (acc.accountNumber == toAcc)
        {
            receiver = acc;
            receiverFound = 1;
            receiverPos = pos;
        }
        if (senderFound && receiverFound)
            break;
        pos++;
    }

    if (!senderFound || !receiverFound)
    {
        printf("One or both accounts not found.\n");
        fclose(fp);
        return;
    }

    if (sender.balance < amount)
    {
        printf("Insufficient balance in sender's account.\n");
        fclose(fp);
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    fseek(fp, senderPos * sizeof(Account), SEEK_SET);
    fwrite(&sender, sizeof(Account), 1, fp);

    fseek(fp, receiverPos * sizeof(Account), SEEK_SET);
    fwrite(&receiver, sizeof(Account), 1, fp);

    fclose(fp);

    recordTransaction(fromAcc, "Debit", amount);
    recordTransaction(toAcc, "Credit", amount);

    printf("Transfer successful. %.2f transferred from #%d to #%d.\n", amount, fromAcc, toAcc);
}

int main()
{
    int choice;
    do
    {
        printf("\033[0;32m");
        printf("\n===Welcome to IITJ NetBanking Menu ===\n");
        printf("\033[36m");
        printf("1. Create Account\n2. Deposit Money\n3. Withdraw Money\n");
        printf("4. Check Balance\n5. View Transactions\n6. View All Accounts (Admin)\n");
        printf("7. Transfer Money\n8. Exit\n");
        printf("Select option: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount();
            break;
        case 2:
            depositMoney();
            break;
        case 3:
            withdrawMoney();
            break;
        case 4:
            checkBalance();
            break;
        case 5:
            viewTransactions();
            break;
        case 6:
            viewAllAccounts();
            break;
        case 7:
            transferMoney();
            break;
        case 8:
            printf("Thank You! Please Visit again");
            break;
        default:
            printf("Please Enter a Valid option. Try again.\n");
        }
    } while (choice != 8);
    return 0;
}
    