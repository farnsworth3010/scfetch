#include <iostream>
#include <fstream>
#include <unistd.h>  // to readlink ()
#include <libgen.h> // to basename()
using namespace std;
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

struct
{
    string black = "\033[30m";
    string cyan = "\033[36m";
    string boldcyan = "\033[1m\033[36m";
    string reset = "\033[0m";
} clrs;

void print_logo(string distro)
{
    string file;
    if (distro == "Arch Linux")
    {
        file = "arch.txt";
    }
    ifstream logo("../logos/" + file);
    if (logo.is_open())
    {
        string str;
        while (getline(logo, str))
        {
            cout << str << endl;
        }
    }
}

string get_distro()
{
    string distro;
    ifstream in("/etc/os-release");
    if (in.is_open())
    {
        string str;
        while (getline(in, str))
        {
            if (str.find("NAME=") != -1)
            {
                int quote_left = str.find('"') + 1;
                int quote_right = str.rfind('"');
                distro = str.substr(quote_left, quote_right - quote_left);
                break;
            }
        }
    }
    return distro;
}

string get_host()
{
    string host;
    ifstream in("/etc/hostname");
    if (in.is_open())
    {
        string str;
        getline(in, str);
        return str;
    }
    return host;
}

string get_kernel_version()
{
    string kernel;
    ifstream in("/proc/version");
    if (in.is_open())
    {
        string str;
        getline(in, str);
        int start = str.find("version") + 8;
        int end = str.find("(") - 1;
        kernel = str.substr(start, end - start);
        in.close();
    }
    return kernel;
}

void print_info(ifstream &logo, string label, string info)
{
    string logo_str;
    if (getline(logo, logo_str))
    {
        cout << clrs.boldcyan << logo_str << "   " << label << clrs.reset << info << endl;
    }
}

string get_uptime()
{
    string uptime;
    ifstream in("/proc/uptime");
    if (in.is_open())
    {
        string str;
        getline(in, str);
        int end = str.find(" ") - 1;
        int uptime_sec = stoi(str.substr(0, end));
        int hours = uptime_sec / 3600;
        int mins = (uptime_sec % 3600) / 60;
        uptime = to_string(hours) + " hours, " + to_string(mins) + " mins";
        in.close();
    }
    return uptime;
}

string get_packages(string distro)
{
    string packages;
    string flatpak = "";
    string package_manager;
    if (distro == "Arch Linux")
    {
        system("pacman -Q | wc -l >> temp.txt");
        package_manager = "pacman";
    }
    ifstream in("./temp.txt");
    if (in.is_open())
    {
        string str;
        getline(in, str);
        packages = str;
        in.close();
    }
    remove("temp.txt");
    if (system("flatpak --version >> /dev/null") == 0) {
        system("flatpak list | wc -l >> temp.txt");
        ifstream in("./temp.txt");
        if (in.is_open())
        {
            string str;
            getline(in, str);
            flatpak += ", ";
            flatpak += str;
            flatpak += " (flatpak)";
            in.close();
        }

        remove("temp.txt");
    }
    return packages + " (" + package_manager + ")" + flatpak;
}

ifstream get_logo(string distro) {
    string path = "default.txt";
    if (distro == "Arch Linux") {
        path = "arch.txt";
    }
    ifstream logo(string(getenv("HOME")) + "/.config/scfetch/logos/" + path);
    return logo;
}

int main()
{
    string distro = get_distro();
    ifstream logo = get_logo(distro);
    string accent_color = clrs.boldcyan;
    string host = string(getenv("USER")) + "@" + get_host();
    print_info(logo, host, "");
    print_info(logo, "", string(host.length(), '-'));
    print_info(logo, "OS: ", get_distro());
    print_info(logo, "Hostname: ", get_host());
    print_info(logo, "Kernel: ", get_kernel_version());
    print_info(logo, "Uptime: ", get_uptime());
    print_info(logo, "Packages: ", get_packages(distro));
    string logo_str;
    while (getline(logo, logo_str))
    {
        cout << accent_color << logo_str << clrs.reset << endl;
    }
    return 0;
}
