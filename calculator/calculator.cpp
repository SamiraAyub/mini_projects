#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>

using namespace std;

const double EPSILON = 1e-12;

double readNumber(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);

        stringstream input(line);
        double value;
        char extra;

        if (input >> value && !(input >> extra) && isfinite(value)) {
            return value;
        }

        cout << "Invalid number. Please try again (for example, 12 or 3.5).\n";
    }
}

int readInt(const string& prompt, int minValue, int maxValue) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);

        stringstream input(line);
        int value;
        char extra;

        if (input >> value && !(input >> extra)
            && value >= minValue && value <= maxValue) {
            return value;
        }

        cout << "Please enter a whole number from "
             << minValue << " to " << maxValue << ".\n";
    }
}

vector<double> readNumbers(int minimumCount) {
    int count = readInt("How many numbers? ",
                        minimumCount, 1000);

    vector<double> numbers;
    numbers.reserve(count);

    for (int i = 0; i < count; ++i) {
        numbers.push_back(readNumber(
            "Enter number " + to_string(i + 1) + ": "));
    }

    return numbers;
}

void showResult(const string& label, double result,
                vector<string>& history) {
    cout << "\n" << label << " = " << result << "\n\n";

    ostringstream entry;
    entry << label << " = " << setprecision(12) << result;
    history.push_back(entry.str());
}

void showMenu() {
    cout << "====================================\n";
    cout << "          CLI CALCULATOR\n";
    cout << "====================================\n";
    cout << " 1. Addition       (2 or more numbers)\n";
    cout << " 2. Subtraction    (2 or more numbers)\n";
    cout << " 3. Multiplication (2 or more numbers)\n";
    cout << " 4. Division       (2 or more numbers)\n";
    cout << " 5. Power          (base ^ exponent)\n";
    cout << " 6. Modulus        (remainder)\n";
    cout << " 7. Square root\n";
    cout << " 8. Percentage     (X percent of Y)\n";
    cout << " 9. Average        (2 or more numbers)\n";
    cout << "10. Absolute value\n";
    cout << "11. View history\n";
    cout << " 0. Exit\n";
    cout << "------------------------------------\n";
}

int main() {
    vector<string> history;

    while (true) {
        showMenu();
        int choice = readInt("Choose an option: ", 0, 11);

        if (choice == 0) {
            cout << "\nThanks for using the calculator.\n";
            break;
        }

        if (choice == 11) {
            cout << "\n========== CALCULATION HISTORY ==========\n";
            if (history.empty()) {
                cout << "No calculations yet.\n";
            } else {
                for (size_t i = 0; i < history.size(); ++i) {
                    cout << i + 1 << ". " << history[i] << '\n';
                }
            }
            cout << '\n';
            continue;
        }

        double result = 0.0;
        string label;

        switch (choice) {
            case 1: {
                vector<double> nums = readNumbers(2);
                result = 0.0;
                for (double n : nums) result += n;
                label = "Addition";
                break;
            }
            case 2: {
                vector<double> nums = readNumbers(2);
                result = nums[0];
                for (size_t i = 1; i < nums.size(); ++i) {
                    result -= nums[i];
                }
                label = "Subtraction";
                break;
            }
            case 3: {
                vector<double> nums = readNumbers(2);
                result = 1.0;
                for (double n : nums) result *= n;
                label = "Multiplication";
                break;
            }
            case 4: {
                vector<double> nums = readNumbers(2);
                result = nums[0];
                bool divisionError = false;

                for (size_t i = 1; i < nums.size(); ++i) {
                    if (fabs(nums[i]) < EPSILON) {
                        cout << "\nError: Cannot divide by zero. "
                             << "Calculation cancelled.\n\n";
                        divisionError = true;
                        break;
                    }
                    result /= nums[i];
                }

                if (divisionError) continue;
                label = "Division";
                break;
            }
            case 5: {
                double base = readNumber("Enter the base: ");
                double exponent = readNumber("Enter the exponent: ");
                result = pow(base, exponent);

                if (!isfinite(result)) {
                    cout << "\nError: This power operation has no "
                         << "finite real result.\n\n";
                    continue;
                }

                label = "Power (" + to_string(base) + " ^ "
                      + to_string(exponent) + ")";
                break;
            }
            case 6: {
                double a = readNumber("Enter the first number: ");
                double b = readNumber("Enter the second number: ");

                if (fabs(b) < EPSILON) {
                    cout << "\nError: Modulus by zero is undefined.\n\n";
                    continue;
                }

                result = fmod(a, b);
                label = "Modulus (" + to_string(a) + " % "
                      + to_string(b) + ")";
                break;
            }
            case 7: {
                double n = readNumber("Enter a number: ");

                if (n < 0) {
                    cout << "\nError: Square root of a negative number "
                         << "is not a real number.\n\n";
                    continue;
                }

                result = sqrt(n);
                label = "Square root of " + to_string(n);
                break;
            }
            case 8: {
                double percent = readNumber("Enter the percentage: ");
                double amount = readNumber("Enter the total amount: ");
                result = (percent / 100.0) * amount;
                label = to_string(percent) + "% of " + to_string(amount);
                break;
            }
            case 9: {
                vector<double> nums = readNumbers(2);
                double sum = 0.0;
                for (double n : nums) sum += n;
                result = sum / nums.size();
                label = "Average";
                break;
            }
            case 10: {
                double n = readNumber("Enter a number: ");
                result = fabs(n);
                label = "Absolute value of " + to_string(n);
                break;
            }
            default:
                // readInt already limits the menu choice to a valid range.
                cout << "Invalid choice.\n";
                continue;
        }

        if (!isfinite(result)) {
            cout << "\nError: The result is too large or undefined.\n\n";
            continue;
        }

        showResult(label, result, history);
    }

    return 0;
}
