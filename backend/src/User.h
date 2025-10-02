#ifndef USER_H
#define USER_H

#include <string>
#include <cpprest/json.h>

class User {
private:
    int id;
    std::string name;
    std::string email;
    std::string createdAt;
    std::string updatedAt;

public:
    User();
    User(int id, const std::string& name, const std::string& email);
    
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getCreatedAt() const { return createdAt; }
    std::string getUpdatedAt() const { return updatedAt; }
    
    // Setters
    void setId(int id) { this->id = id; }
    void setName(const std::string& name) { this->name = name; }
    void setEmail(const std::string& email) { this->email = email; }
    void setCreatedAt(const std::string& createdAt) { this->createdAt = createdAt; }
    void setUpdatedAt(const std::string& updatedAt) { this->updatedAt = updatedAt; }
    
    // JSON conversion
    web::json::value toJson() const;
    static User fromJson(const web::json::value& json);
};

#endif // USER_H
