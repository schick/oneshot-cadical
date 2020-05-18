
#include <cassert>
#include <thread>

#include "params.h"

/**
 * Taken from Hordesat:ParameterProcessor.h by Tomas Balyo.
 */
void Parameters::init(int argc, char** argv) {
    setDefaults();
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (arg[0] != '-') {
            _filename = std::string(arg);
            continue;
        }
        char* eq = strchr(arg, '=');
        if (eq == nullptr) {
            _params[arg+1];
        } else {
            *eq = 0;
            char* left = arg+1;
            char* right = eq+1;
            _params[left] = right;
        }
    }
}

void Parameters::setDefaults() {
    setParam("ext", "lin");
    setParam("limit", "10000");
    setParam("threads", to_string(thread::hardware_concurrency()).c_str());
}

void Parameters::printUsage() const {
    printf("Usage: oneshot-cadical [options] <formula>\n");
    printf("<formula>:      File path for formula in DIMACS format.\n");
    printf("Options:\n");
    printf("-random         Use a new random seed for every SolverThread.\n");
    printf("                Default: false\n");
    printf("-ext=<strategy> Choose the strategy used to calculate the threshold for extension.\n");
    printf("                Possible options: lin, quad or expo .\n");
    printf("                Default: lin\n");
    printf("-limit=<#conf>  Set the number of conflicts per iteration.\n");
    printf("                Default: 10000\n");
    printf("-threads=<#>    Set the number of threads.\n");
    printf("                Default: hardware dependent\n");
}

string Parameters::getFilename() const {
  return _filename;
}

void Parameters::printParams() const {
    std::string out = "";
    for (const auto& it : _params) {
        if (it.second.empty()) {
            out += it.first + ", ";
        } else {
            out += it.first + "=" + it.second + ", ";
        }
    }
    out = out.substr(0, out.size()-2);
    printf("Program options: %s\n", out.c_str());
}

void Parameters::setParam(const char* name) {
    _params[name];
}

void Parameters::setParam(const char* name, const char* value) {
    _params[name] = value;
}

bool Parameters::isSet(const string& name) const {
    return _params.find(name) != _params.end();
}

string Parameters::getParam(const string& name, const string& defaultValue) const {
    if (isSet(name)) {
        return _params.at(name);
    } else {
        return defaultValue;
    }
}

string Parameters::getParam(const string& name) const {
    return getParam(name, "ndef");
}

int Parameters::getIntParam(const string& name, int defaultValue) const {
    if (isSet(name)) {
        return atoi(_params.at(name).c_str());
    } else {
        return defaultValue;
    }
}

int Parameters::getIntParam(const string& name) const {
    assert(isSet(name));
    return atoi(_params.at(name).c_str());
}

float Parameters::getFloatParam(const string& name, float defaultValue) const {
    if (isSet(name)) {
        return atof(_params.at(name).c_str());
    } else {
        return defaultValue;
    }
}

float Parameters::getFloatParam(const string& name) const {
    assert(isSet(name));
    return atof(_params.at(name).c_str());
}
