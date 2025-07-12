#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include "erc20.h"
#include "tiny-json.h"

using namespace std;

ERC20 token;

const char *banner = 
"\n\n                                    ██              ██                \n"                  
"                                  ██  ██          ██  ██              \n"                  
"                                  ██  ░░██      ██░░  ██              \n"                  
"                                  ██  ░░░░██████░░░░  ██              \n"                  
"                                  ██  ████░░░░░░████  ██              \n"                  
"                                  ████░░░░░░░░░░░░░░████              \n"                  
"                                  ██░░░░░░░░░░░░░░░░░░██              \n"                  
"                                  ██░░██░░░░░░░░░░██░░██              \n"                  
"                                ██░░░░██░░██████░░██░░░░██            \n"                  
"                              ░░██░░░░░░░░  ██  ░░░░░░░░██            \n"                  
"                              ██░░░░░░░░          ░░░░░░░░██          \n"                  
"                                ████                  ████            \n"                  
"                                    ██████████████████                \n\n\n";

void init_proc() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);

    srand(time(NULL));
}

void connect_wallet() {
    cout << banner;
    cout << "Your private key : ";

    uint32_t privkey;
    scanf("%u", &privkey);
    cin.ignore();
    
    USER = privkey_to_wallet(privkey);
    token = ERC20(USER);

    cout << "Now connected to HSpace Mainnet." << endl;
    cout << "Your wallet address : 0x" << hex << USER << endl;

    return;
}

char *tx_fetch_method(json_t const* parent) {
    json_t const* field = json_getProperty(parent, "method");
    if ( field == NULL ) {
        cout << "[!] Missing required 'method' field" << endl;
        exit(0);
    }
    if ( json_getType(field) != JSON_TEXT ) {
        cout << "[!] Invalid type for 'method' field" << endl;
        exit(0);
    }    
    return (char *)json_getValue(field);
}

address tx_fetch_address(json_t const* parent, const char *field_name) {
    json_t const* field = json_getProperty(parent, field_name);
    if ( field == NULL ) {
        cout << "[!] Missing required '" << field_name << "' field" << endl;
        exit(0);
    }
    if ( json_getType(field) != JSON_INTEGER  ) {
        cout << "[!] Invalid type for '" << field_name << "' field" << endl;
        exit(0);
    }    
    return (address)json_getInteger(field);
}

uint32_t tx_fetch_amount(json_t const* parent, const char *field_name) {
    json_t const* field = json_getProperty(parent, field_name);
    if ( field == NULL ) {
        cout << "[!] Missing required '" << field_name << "' field" << endl;
        exit(0);
    }
    if ( json_getType(field) != JSON_INTEGER  ) {
        cout << "[!] Invalid type for '" << field_name << "' field" << endl;
        exit(0);
    }    
    return (uint32_t)json_getInteger(field);
}

int main(int argc, char **argv) {
    init_proc();
    connect_wallet();

    while (1) {
        string input;
        cout << "Tx : ";
        getline(cin, input);

        json_t pool[4];
        json_t const* parent = json_create(const_cast<char*>(input.c_str()), pool, 10);
        if ( parent == NULL ) {
            cout << "[!] Invalid JSON format" << endl;
            exit(0);
        }
        
        char *method = tx_fetch_method(parent);
        
        if (strcmp(method, "balanceOf") == 0) {
            address account = tx_fetch_address(parent, "account");
            
            uint32_t res = token.balanceOf(account);
            cout << "Balance of 0x" << hex << account << ": " << dec << res << endl;
        } else if (strcmp(method, "approve") == 0) {
            address spender = tx_fetch_address(parent, "spender");
            uint32_t amount = tx_fetch_amount(parent, "amount");

            token.approve(spender, amount);
            cout << "Approved 0x" << hex << spender << "of " << amount << "tokens" << endl;
        } else if (strcmp(method, "allowance") == 0) {
            address owner = tx_fetch_address(parent, "owner");
            address spender = tx_fetch_address(parent, "spender");            

            uint32_t res = token.allowance(owner, spender);
            cout << "Allowance for spender 0x" << hex << spender << " by owner 0x" << owner << ": " << res << endl;
        } else if (strcmp(method, "transfer") == 0) {
            address to = tx_fetch_address(parent, "to");
            uint32_t amount = tx_fetch_amount(parent, "amount");

            token.transfer(to, amount);
        } else if (strcmp(method, "transferFrom") == 0) {
            address sender = tx_fetch_address(parent, "sender");
            address recipient = tx_fetch_address(parent, "recipient");
            uint32_t amount = tx_fetch_amount(parent, "amount");            

            token.transferFrom(sender, recipient, amount);
        } else if (strcmp(method, "check") == 0) {
            address account = tx_fetch_address(parent, "account");

            token.check(account);
        } else {
            cout << "[!] Unknown method '" << method << "'" << endl;
        }

    }

    return 0;
}