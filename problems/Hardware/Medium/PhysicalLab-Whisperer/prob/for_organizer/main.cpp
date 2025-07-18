#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <cctype>
#include <sstream>
using namespace std;

vector<pair<pair<int, string>, pair<int, string>> > myConnection;
bool pin_flag = false;
bool conf_flag = false;

// ChipInfo 클래스
class ChipInfo {
public:
    string model;
    vector<string> pinConnection;

    ChipInfo(const string& model, const vector<string>& pins)
        : model(model), pinConnection(pins) {}
};

// STM32 chip
vector<ChipInfo> chipDatabase = {
    ChipInfo("STM32F205", {"VSS", "VDD", "VCAP", "NRST", "PA14", "PA13", "PB3"})
};

// SWD
vector<ChipInfo> swdPin = {
    ChipInfo("SWD", {"VREF", "GND", "KEY", "RST", "NC", "SWO", "CLK", "SWIO"})
};

vector<pair<int, string> > devices = {
    {1, "ChipWhisperer" },
    {2, "SWD"},
    {3, "Trezor"}
};

void showMenu() {
    cout << "\n====== Choose an action ======\n";
    cout << "1. Show ChipWhisperer\n";
    cout << "2. Connect Pins\n";
    cout << "3. Setup glitch configuration\n";
    cout << "4. Start glitching\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

void showChipWhisperer() {
    cout << "  _______________________________\n";
    cout << " /                               \\\n";
    cout << "|   Crowbar      Neg    Pos       |\n";
    cout << "|    [---]      [---]  [---]      |\n";
    cout << "|                                 |\n";
    cout << "|   _________________________     |\n";
    cout << "|  |                        |     |\n";
    cout << "|  |  ChipWhisperer-Husky   |     |\n";
    cout << "|  |                        |     |\n";
    cout << "|  |   [   ]  ADC           |     |\n";
    cout << "|  |   [   ]  Armed         |     |\n";
    cout << "|  |   [   ]  Glitch        |     |\n";
    cout << "|  |   [   ]  Capturing     |     |\n";
    cout << "|  |   [   ]  Status        |     |\n";
    cout << "|  |   [   ]  Error         |     |\n";
    cout << "|  |________________________|     |\n";
    cout << "|                                 |\n";
    cout << "|  Trigger/Glitch Out             |\n";
    cout << "|  ----------------------------   |\n";
    cout << "|  | 1: 3.3V      |  2: 3.3V   |  |\n";
    cout << "|  | 3: Clock     |  4: GND    |  |\n";
    cout << "|  | 5: D7        |  6: GND    |  |\n";
    cout << "|  | 7: D6        |  8: GND    |  |\n";
    cout << "|  | 9: D5        | 10: GND    |  |\n";
    cout << "|  |11: D4        | 12: GND    |  |\n";
    cout << "|  |13: D3        | 14: GND    |  |\n";
    cout << "|  |15: D2        | 16: GND    |  |\n";
    cout << "|  |17: D1        | 18: GND    |  |\n";
    cout << "|  |19: D0        | 20: GND    |  |\n";
    cout << "|  ----------------------------   |\n";
    cout << "|                                 |\n";
    cout << "|  20-pin Connector               |\n";
    cout << "|  ----------------------------   |\n";
    cout << "|  | 1: +VUSB (5V)  |  2: GND   | |\n";
    cout << "|  | 3: +3.3V       |  4: HS1   | |\n";
    cout << "|  | 5: nRST        |  6: HS2   | |\n";
    cout << "|  | 7: MISO        |  8: N/C   | |\n";
    cout << "|  | 9: MOSI        | 10: TIO1  | |\n";
    cout << "|  |11: SCK         | 12: TIO2  | |\n";
    cout << "|  |13: PDIC/CS     | 14: TIO3  | |\n";
    cout << "|  |15: PDID        | 16: TIO4  | |\n";
    cout << "|  |17: GND         | 18: +3.3V | |\n";
    cout << "|  |19: GND         | 20: +VUSB | |\n";
    cout << "|  ----------------------------   |\n";
    cout << "|                                 |\n";
    cout << "|   USB Port                      |\n";
    cout << "|    [====]                       |\n";
    cout << " \\_______________________________/\n\n";
}

// 타겟 칩 검색
ChipInfo* findChipInfo(const string& model) {
    for (auto& chip : chipDatabase) {
        if (chip.model == model) {
            return &chip;
        }
    }
    return nullptr;
}

void connectPin() {
    int device1, device2;

    cout << "Select a device to connect with Trezor (1.ChipWhisperer, 2.SWD):" << endl;

    cout << "Enter the device num: ";
    cin >> device1;

    if (device1 != 1 && device1 != 2) {
        cout << "Invalid devices selected..." << endl;
        exit(0);
    }

    string pin1, pin2;
    cout << "Enter the pin on " << devices[device1 - 1].second << " to connect: ";
    cin >> pin1;
    for (char& c : pin1) {
        c = toupper(c);
    }

    cout << "Enter the pin on Trezor to connect: ";
    cin >> pin2;
    for (char& c : pin2) {
        c = toupper(c);
    }

    cout << "Connecting " << devices[device1 - 1].second << " (Pin: " << pin1 << ") with Trezor (Pin: " << pin2 << ").\n" << endl;
    myConnection.push_back(make_pair(make_pair(device1 - 1, pin1), make_pair(2, pin2)));
    
    cout << "======================myConnection======================" << endl;
    for (int i = 0; i < myConnection.size(); i++) {
        cout << devices[myConnection[i].first.first].second << ":" << myConnection[i].first.second << " <---> ";
        cout << devices[myConnection[i].second.first].second << ":" << myConnection[i].second.second << endl;
    }
}

// glitching 설정 세팅
void configureGlitching() {
    map<string, string> m;
    int glitchWidth, glitchOffset;
    string basic_mode, clkgen_src, enabled, clk_src, trigger_src, output, triggers, nrst;
    string glitch_lp, glitch_hp;

    m["basic_mode"] = "rising_edge";
    m["clkgen_src"] = "system";
    m["enabled"] = "True";
    m["clk_src"] = "pll";
    m["trigger_src"] = "ext_single";
    m["output"] = "enable_only";
    m["triggers"] = "nrst";
    m["nrst"] = "high_z";
    m["glitch_lp"] = "True";
    m["glitch_hp"] = "False";

    cout << "\nimport chipwhisperer as cw\n";
    cout << "scope = cw.scope()\n" << endl;
    cout << "------ glitch configure ------" << endl;

    conf_flag = true;
    cout << "scope.adc.basic_mode = ";
    cin >> basic_mode;
    if (m["basic_mode"] != basic_mode) conf_flag = false;
    cout << "scope.clock.clkgen_src = ";
    cin >> clkgen_src;
    if (m["clkgen_src"] != clkgen_src) conf_flag = false;

    cout << "scope.glitch.enabled = ";
    cin >> enabled;
    if (m["enabled"] != enabled) conf_flag = false;
    cout << "scope.glitch.clk_src = ";
    cin >> clk_src;
    if (m["clk_src"] != clk_src) conf_flag = false;
    cout << "scope.glitch.trigger_src = ";
    cin >> trigger_src;
    if (m["trigger_src"] != trigger_src) conf_flag = false;
    cout << "scope.glitch.output = ";
    cin >> output;
    if (m["output"] != output) conf_flag = false;
    cout << "scope.trigger.triggers = ";
    cin >> triggers;
    if (m["triggers"] != triggers) conf_flag = false;
    cout << "scope.io.nrst = ";
    cin >> nrst;
    if (m["nrst"] != nrst) conf_flag = false;

    cout << "scope.io.glitch_lp = ";
    cin >> glitch_lp;
    if (m["glitch_lp"] != glitch_lp) conf_flag = false;
    cout << "scope.io.glitch_hp = ";
    cin >> glitch_hp;
    if (m["glitch_hp"] != glitch_hp) conf_flag = false;
    

}

// 1.ChipWhisperer, 2.SWD, 3.Trezor
void checkFlag() {
    if (myConnection.size() != 6) {
        pin_flag = false;
        return;
    }

    int con_num = 0;
    for (auto con : myConnection) {
        if (con.first.first == 0) {
            if (con.first.second == "CROWBAR") {
                if (con.second.second == "VCAP") {
                    con_num++;
                }
            } else if (con.first.second == "NRST") {
                if (con.second.second == "NRST") {
                    con_num++;
                }
            }
        }
        else if (con.first.first == 1) {
            if (con.first.second == "SWIO") {
                if (con.second.second == "PA13") {
                    con_num++;
                }
            }
            else if (con.first.second == "CLK") {
                if (con.second.second == "PA14") {
                    con_num++;
                }
            }
            else if (con.first.second == "SWO") {
                if (con.second.second == "PB3") {
                    con_num++;
                }
            }
            else if (con.first.second == "VREF") {
                if (con.second.second == "VDD") {
                    con_num++;
                }
            }
        }
        else {
            pin_flag = false;
            return;
        }
    }

    if (con_num == 6) {
        pin_flag = true;
    }
    else {
        pin_flag = false;
    }

}

void start() {
    cout << "\n= = = = = = = = = = = = = = = = = = =\n";
    cout << "= = = = . . . Glitching . . . = = = =\n";
    cout << "= = = = = = = = = = = = = = = = = = =\n";
    this_thread::sleep_for(chrono::seconds(4));
    if (!pin_flag) {
        cout << "[Error] connection error..." << endl;
    }
    else if (!conf_flag) {
        cout << "Check you glitch configuration again..." << endl;
    }
    else {
        cout << "hspace{7hi3_1s_4_pieC3_0f_c4ke}" << endl;
    }
}

int main() {
    string targetChip;

    cout << "Enter the target chip model name (9-character): ";
    cin >> targetChip;

    for (char& c : targetChip) {
        c = toupper(c);
    }
    
    ChipInfo* chipInfo = findChipInfo(targetChip);

    if (chipInfo == nullptr) {
        cout << "Hmm... not found." << endl;
        return 0;
    }

    int choice;
    while (true) {
        showMenu();
        
        cin >> choice;
        if (choice < 1 || choice > 5) {
            cout << "Invalid choice.." << endl;
            exit(0);
        }

        switch (choice) {
        case 1:
            showChipWhisperer();
            break;
        case 2:
            connectPin();
            break;
        case 3:
            configureGlitching();
            break;
        case 4:
            checkFlag();
            start();
            return 0;
        case 5:
            cout << "Exit." << endl;
            return 0;
        default:
            cout << "Invalid choice. Try again." << endl;
        }

    }

    return 0;
}
