//
// Created by Abduqodir on 02.04.2015.
//

#include "settings.h"
/*
settings::param::param(const settings::param & other) {
    this->value = other.value;
    this->name = other.name;
    this->parent = other.parent;
}
*/
settings::param::param(const string & name, std::string value, const settings *parent) {
    this->value = value;
    this->name = name;
    this->parent = const_cast<settings* const>(parent);
}

settings::param::operator std::string() const {
    return string(value);
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
settings::param & settings::param::operator=(char const * value) {
    return this->operator=(std::string(value));
}
settings::param & settings::param::operator=(std::string const & value) {
    //cout << "string: " << value << endl;
    this->value = value;
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator=(int value) {
    this->value = to_string(value);
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator=(bool value) {
    //cout << "bool: " << value << endl;
    this->value = to_string(value);
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator=(double value) {
    this->value = to_string(value);
    parent->set(this->name, this->value);
    return *this;
}

settings::param & settings::param::operator+=(std::string const & value) {
    this->value += value;
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator+=(int value) {
    this->value = to_string(stoi(this->value) + value);
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator+=(double value) {
    this->value = to_string(stod(this->value) + value);
    parent->set(this->name, this->value);
    return *this;
}

settings::param & settings::param::operator-=(int value) {
    this->value = to_string(stoi(this->value) - value);
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator-=(double value) {
    this->value = to_string(stod(this->value) - value);
    parent->set(this->name, this->value);
    return *this;
}

settings::param & settings::param::operator*=(int value) {
    this->value = to_string(stoi(this->value) * value);
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator*=(double value) {
    this->value = to_string(stod(this->value) * value);
    parent->set(this->name, this->value);
    return *this;
}

settings::param & settings::param::operator/=(int value) {
    this->value = to_string(stoi(this->value) / value);
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator/=(double value) {
    this->value = to_string(stod(this->value) / value);
    parent->set(this->name, this->value);
    return *this;
}

settings::param & settings::param::operator|=(bool value) {
    this->value = (value ? "true" : this->value);
    parent->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator&=(bool value) {
    this->value = (value ? this->value : "false");
    parent->set(this->name, this->value);
    return *this;
}
bool settings::param::is_empty() const {
    return this->value.empty();
}

settings::settings(const string & filename) {
    if (!std::ifstream(filename).good()){
        std::ofstream(filename);
    }
    this->filename = filename;
    reload();
}
string const & settings::get(std::string const & name, std::string const & def) const {
    return (params.find(name) == params.end() ? def : params.find(name)->second);
}
void settings::set(std::string const &name, std::string const &value) {
    this->params[name] = value;
    this->updateFile();
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
const settings::param settings::operator[](std::string const &name) const {
    return param(name, params.find(name)->second, this);
}
settings::param settings::operator[](std::string const & name) {
    return param(name, params.find(name)->second, this);
}
void settings::updateFile() {
    std::ofstream output(filename);
    for (auto item : params) {
        output << item.first << endl;
        output << item.second << endl;
    }
    output.close();
}