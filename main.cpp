#include <iostream>
#include <fstream>
#include <unistd.h> // to readlink ()
#include <libgen.h> // to basename()

// to get terminal width
#include <sys/ioctl.h>
#include <stdio.h>

using namespace std;
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */

struct
{
    string black = "\033[30m";
    string cyan = "\033[36m";
    string boldcyan = "\033[1m\033[36m";
    string reset = "\033[0m";
    string white = "\033[37m";
    string boldwhite = "\033[1m\033[37m";
    string boldgreen = "\033[1m\033[32m";
    string red = "\033[31m";
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

void print_info(ifstream &logo, string label, string info, string accent_color, int width)
{
    string logo_str;
    if (getline(logo, logo_str))
    {
        cout << accent_color << logo_str << "   " << label << clrs.reset << info << endl;
    }
    else
    {
        string spacer = string(width, ' ');
        cout << spacer << accent_color << "   " << label << clrs.reset << info << endl;
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
    if (system("pacman --version >> /dev/null 2> /dev/null") == 0) // arch based
    {
        system("pacman -Q | wc -l >> temp.txt");
        package_manager = "pacman";
    }
    else if (system("apt 2> /dev/null") == 0) { // debian based
        if (system("dpkg --list 2> /dev/null | wc --lines >> temp.txt ") == 0) {
            package_manager = "apt";
        }
    }
    else
    {
        package_manager = "unknown";
    }
    if (package_manager != "unknown")
    {
        ifstream in("./temp.txt");
        if (in.is_open())
        {
            string str;
            getline(in, str);
            packages = str;
            in.close();
        }
        remove("temp.txt");
    }
    if (system("flatpak --version >> /dev/null") == 0)
    {
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

ifstream get_logo(string distro)
{
    string path = "default.txt";
    if (distro == "Arch Linux")
    {
        path = "arch.txt";
    }
    if (distro == "Manjaro Linux")
    {
        path = "manjaro.txt";
    }
    ifstream logo(string(getenv("HOME")) + "/.config/scfetch/logos/" + path);
    return logo;
}

string get_resolution()
{
    string resolution;
    if (system("xrandr >> ./temp.txt") == 0)
    {
        ifstream in("./temp.txt");
        if (in.is_open())
        {
            string str;
            getline(in, str);
            int start = str.find("current") + 8;
            int end = str.find(",", start);
            resolution = str.substr(start, end - start);
            in.close();
        }
        remove("temp.txt");
    }
    return resolution;
}

string get_de()
{
    string de = getenv("XDG_CURRENT_DESKTOP");
    if (de == "GNOME")
    {
        if (system("gnome-shell --version >> ./temp.txt") == 0)
        {
            ifstream in("./temp.txt");
            if (in.is_open())
            {
                string str;
                getline(in, str);
                int start = str.rfind(" ");
                de += str.substr(start, str.length());
                in.close();
            }
            remove("temp.txt");
        }
    }
    return de;
}
string get_wm()
{
    string de = getenv("XDG_CURRENT_DESKTOP");
    string wm;
    if (de == "GNOME")
    {
        wm = "Mutter";
    }
    return wm;
}
string get_terminal()
{
    string terminal;
    if (system("basename \"$(cat \"/proc/$PPID/comm\")\" >> temp.txt") == 0)
    {
        ifstream in("./temp.txt");
        if (in.is_open())
        {
            getline(in, terminal);
            in.close();
        }
        remove("temp.txt");
    }
    return terminal;
}
string get_cpu()
{
    string cpu;
    ifstream in("/proc/cpuinfo");
    if (in.is_open())
    {
        string str;
        while (getline(in, str))
        {
            if (str.find("model name") != -1)
            {
                int start = str.find(":") + 2;
                cpu = str.substr(start, str.length());
                break;
            }
        }
    }
    return cpu;
}
string get_accent_color(string distro)
{
    if (distro == "Arch Linux")
    {
        return clrs.boldcyan;
    }
    else if (distro == "Ubuntu")
    {
        return clrs.red;
    }
    else if (distro == "Manjaro Linux")
    {
        return clrs.boldgreen;
    }
    else
    {
        return clrs.white;
    }
}
int get_logo_width(ifstream &logo)
{
    string str;
    getline(logo, str);
    logo.clear();
    logo.seekg(0);
    return str.length();
}

int main(int argc, char **argv)
{
    string distro = get_distro();
    if (argc > 1)
    {
        distro = argv[1];
    }
    // struct winsize w;
    // ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);

    ifstream logo = get_logo(distro);
    int width = get_logo_width(logo);
    string accent_color = get_accent_color(distro);
    string host = string(getenv("USER")) + "@" + get_host();
    print_info(logo, host, "", accent_color, width);
    print_info(logo, "", string(host.length(), '-'), accent_color, width);
    print_info(logo, "OS: ", get_distro(), accent_color, width);
    print_info(logo, "Hostname: ", get_host(), accent_color, width);
    print_info(logo, "Kernel: ", get_kernel_version(), accent_color, width);
    print_info(logo, "Uptime: ", get_uptime(), accent_color, width);
    print_info(logo, "Packages: ", get_packages(distro), accent_color, width);
    print_info(logo, "Shell: ", string(getenv("SHELL")), accent_color, width);
    print_info(logo, "Resolution: ", get_resolution(), accent_color, width);
    print_info(logo, "DE: ", get_de(), accent_color, width);
    print_info(logo, "WM: ", get_wm(), accent_color, width);
    print_info(logo, "Terminal: ", get_terminal(), accent_color, width);
    print_info(logo, "CPU: ", get_cpu(), accent_color, width);
    string logo_str;
    while (getline(logo, logo_str))
    {
        cout << accent_color << logo_str << clrs.reset << endl;
    }
    return 0;
}

// xrandr
