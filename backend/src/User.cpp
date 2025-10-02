#include "User.h"
#include <cpprest/json.h>

using namespace web;

User::User() : id(0), name(""), email(""), createdAt(""), updatedAt("") {}

User::User(int id, const std::string& name, const std::string& email)
    : id(id), name(name), email(email), createdAt(""), updatedAt("") {}

json::value User::toJson() const {
    json::value result;
    result["id"] = json::value::number(id);
    result["name"] = json::value::string(utility::conversions::to_string_t(name));
    result["email"] = json::value::string(utility::conversions::to_string_t(email));
    
    if (!createdAt.empty()) {
        result["created_at"] = json::value::string(utility::conversions::to_string_t(createdAt));
    }
    
    if (!updatedAt.empty()) {
        result["updated_at"] = json::value::string(utility::conversions::to_string_t(updatedAt));
    }
    
    return result;
}

User User::fromJson(const json::value& json) {
    User user;
    
    if (json.has_field("id")) {
        user.id = json.at("id").as_integer();
    }
    
    if (json.has_field("name")) {
        user.name = utility::conversions::to_utf8string(json.at("name").as_string());
    }
    
    if (json.has_field("email")) {
        user.email = utility::conversions::to_utf8string(json.at("email").as_string());
    }
    
    return user;
}
