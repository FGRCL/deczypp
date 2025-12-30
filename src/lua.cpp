#include "lua.hpp"
#include <format>
#include <lua5.4/lua.hpp>
#include <stdexcept>
#include <string>
#include <variant>

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

lua::Selector::Selector(LuaKey key, lua_State &luaState, Selector *parent)
    : key(key), parent(parent), luaState(luaState) {}

lua::Selector lua::Selector::operator[](LuaKey key) {
  return Selector(key, luaState, this);
};

std::string lua::Selector::getFullPath() {
  if (parent == nullptr) {
    return std::visit(
        overloaded{[](int key) { return std::format("{}", key); },
                   [](const char *key) { return std::format("{}", key); }},
        key);
  } else {
    return std::visit(
        overloaded{[this](int key) {
                     return std::format("{}.{}", parent->getFullPath(), key);
                   },
                   [this](const char *key) {
                     return std::format("{}.{}", parent->getFullPath(), key);
                   }},
        key);
  }
}

void lua::Selector::push() {
  if (parent == nullptr) {
    std::visit(
        overloaded{
            [](int key) {
              throw std::runtime_error(std::format(
                  "The first selector key cannot be an integers. Got {}.",
                  key));
            },
            [this](const char *key) { lua_getglobal(&luaState, key); }},
        key);
  } else {
    parent->push();
    std::visit(
        overloaded{[this](int key) { lua_pushinteger(&luaState, key); },
                   [this](const char *key) { lua_pushstring(&luaState, key); }},
        key);
    lua_gettable(&luaState, -2);
  }
}

void lua::Selector::pop() {
  lua_pop(&luaState, 1);
  if (parent != nullptr) {
    parent->pop();
  }
}

lua::LuaProgramResult::LuaProgramResult(lua_State &luaState)
    : luaState(luaState) {};

lua::Selector lua::LuaProgramResult::operator[](LuaKey key) {
  return Selector(key, luaState, nullptr);
}

lua::LuaProgram::LuaProgram() : luaState(*luaL_newstate()) {}

lua::LuaProgramResult lua::LuaProgram::execute(std::string programFile) {
  lua_rawlen(&luaState, -1);
  if (luaL_loadfile(&luaState, programFile.c_str()) ||
      lua_pcall(&luaState, 0, 0, 0)) {
    throw std::runtime_error("Failed to run lua program.");
  }
  return LuaProgramResult(luaState);
}

bool lua::convert(lua::Selector &selector, bool *) {
  if (!lua_isboolean(&selector.luaState, -1)) {
    throw std::runtime_error(std::format("Could not convert value at key [{}] "
                                         "to a boolean. It is not a boolean.",
                                         selector.getFullPath()));
  }
  bool result = lua_toboolean(&selector.luaState, -1);
  return result;
}

double lua::convert(lua::Selector &selector, double *) {
  int isnum;
  int result = lua_tonumberx(&selector.luaState, -1, &isnum);
  if (!isnum) {
    throw std::runtime_error(
        std::format("Could not convert value at key [{}] to a double. It is "
                    "not a double (lua number).",
                    selector.getFullPath()));
  }
  return result;
}

int lua::convert(lua::Selector &selector, int *) {
  int isnum;
  int result = lua_tointegerx(&selector.luaState, -1, &isnum);
  if (!isnum) {
    throw std::runtime_error(std::format("Could not convert value at key [{}] "
                                         "to an integer. It is not an integer.",
                                         selector.getFullPath()));
  }
  return result;
}

std::string lua::convert(lua::Selector &selector, std::string *) {
  if (!lua_isstring(&selector.luaState, -1)) {
    throw std::runtime_error(std::format(
        "Could not convert value at key [{}] to a string. It is not a string.",
        selector.getFullPath()));
  }
  std::string result = lua_tostring(&selector.luaState, -1);
  return result;
}
