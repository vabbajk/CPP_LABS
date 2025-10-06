#ifndef SELF_MADE_STRING_H
#define SELF_MADE_STRING_H

#include <iostream>
#include <string>

class String {
private:
  char *data;
  int length;

  static int str_length(const char *s);
  static void str_copy(char *dest, const char *src, int dest_size);
  static bool str_equal(const char *s1, const char *s2);

public:
  String();
  explicit String(const char *s);
  String(const String &other);
  ~String();

  String operator()(int start, int end) const;
  String &operator=(const char *str);
  String &operator=(const String &other);

  const char &operator[](int index) const;

  int get_length() const;
  void clear();
  bool is_empty() const;

  friend bool operator!(const String &s) {
    return s.length == 0 || s.data == nullptr;
  }

  friend std::ostream &operator<<(std::ostream &out, const String &s) {
    if (s.data) {
      out << (s.data);
    } else {
      out << "Строка пуста!";
    }
    return out;
  }

  friend std::istream &operator>>(std::istream &in, String &s) {
    std::string temp;
    std::getline(in, temp);

    delete[] s.data;
    s.length = static_cast<int>(temp.size());
    s.data = new char[s.length + 1];

    for (int i = 0; i < s.length; i++)
      s.data[i] = temp[i];
    s.data[s.length] = '\0';

    return in;
  }

  friend bool operator==(const String &s1, const String &s2) {
    return str_equal(s1.data, s2.data);
  }
};

#endif
