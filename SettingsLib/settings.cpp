//
// Created by Abduqodir on 02.04.2015.
//

#include "settings.h"

/*settings::param::param(const param & other) {
    this->value = other.value;
    this->name = other.name;
}*/
settings::param::param(string const & name, string const & value) {
    this->value = value;
    this->name = name;
}

settings::param::operator std::string() const {
    return value;
}
settings::param::operator bool() const {
    return (value == "true");
}
settings::param::operator int() const {
    return stoi(value);
}
settings::param::operator double() const {
    return stod(value);
}

settings::param & settings::param::operator=(std::string const & value) {
    this->value = value;
    return *this;
}
settings::param & settings::param::operator=(int value) {
    this->value = to_string(value);
    return *this;
}
settings::param & settings::param::operator=(bool value) {
    this->value = to_string(value);
    return *this;
}
settings::param & settings::param::operator=(double value) {
    this->value = to_string(value);
    return *this;
}

settings::param & settings::param::operator+=(std::string const & value) {
    this->value += value;
    return *this;
}
settings::param & settings::param::operator+=(int value) {
    this->value = to_string(stoi(this->value) + value);
    return *this;
}
settings::param & settings::param::operator+=(double value) {
    this->value = to_string(stod(this->value) + value);
    return *this;
}

settings::param & settings::param::operator-=(int value) {
    this->value = to_string(stoi(this->value) - value);
    return *this;
}
settings::param & settings::param::operator-=(double value) {
    this->value = to_string(stod(this->value) - value);
    return *this;
}

settings::param & settings::param::operator*=(int value) {
    this->value = to_string(stoi(this->value) * value);
    return *this;
}
settings::param & settings::param::operator*=(double value) {
    this->value = to_string(stod(this->value) * value);
    return *this;
}

settings::param & settings::param::operator/=(int value) {
    this->value = to_string(stoi(this->value) / value);
    return *this;
}
settings::param & settings::param::operator/=(double value) {
    this->value = to_string(stod(this->value) / value);
    return *this;
}

settings::param & settings::param::operator|=(bool value) {
    this->value = (value ? "true" : this->value);
    return *this;
}
settings::param & settings::param::operator&=(bool value) {
    this->value = (value ? this->value : "false");
    return *this;
}
bool settings::param::is_empty() const {
    return this->value.empty();
}

settings::settings(const string & filename) {
    this->filename = filename;
    reload();
}
string const & settings::get(std::string const & name, std::string const & def) const {
    if(params.find(name) == params.end()) {
        return def;
    }
    else {
        return params.find(name)->second;
    }
}
void settings::set(std::string const &name, std::string const &value) {
    this->params[name] = value;
    this->update();
}
void settings::reset() {
    std::ofstream(filename, std::ofstream::trunc);
    params.clear();
}
void settings::reload() {
    params.clear();
    std::ifstream input(filename);
    string name, value;
    while (getline(input, name)) {
        getline(input, value);
        params[name] = value;
    }
    input.close();
}

const settings::param settings::operator[](std::string const & name) const {
    return *(new param(name, params.at(name)));
}
settings::param settings::operator[](std::string const & name) {
    return *(new param(name, params.at(name)));
}
void settings::update() {
    std::ofstream output(filename);
    for (auto item : params) {
        output << item.first << endl;
        output << item.second << endl;
    }
    output.close();
}